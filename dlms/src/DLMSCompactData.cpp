#include "DLMSCompactData.h"
#include "DLMSObjectFactory.h"
#include "DLMSServer.h"

#ifndef DLMS_IGNORE_COMPACT_DATA
//Constructor.
DLMSCompactData::DLMSCompactData() :
    DLMSCompactData("", 0)
{
}

//SN Constructor.
DLMSCompactData::DLMSCompactData(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_COMPACT_DATA, ln, sn)
{

}

//LN Constructor.
DLMSCompactData::DLMSCompactData(std::string ln) :
    DLMSCompactData(ln, 0)
{

}

DLMSCompactData::~DLMSCompactData()
{
    for (std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
    {
        delete it->second;
    }
    m_CaptureObjects.clear();
}

ByteBuffer& DLMSCompactData::GetBuffer() {
    return m_Buffer;
}

void DLMSCompactData::SetBuffer(ByteBuffer& value) {
    m_Buffer = value;
}

std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& DLMSCompactData::GetCaptureObjects() {
    return m_CaptureObjects;
}

unsigned char DLMSCompactData::GetTemplateId() {
    return m_TemplateId;
}

void DLMSCompactData::SetTemplateId(unsigned char value) {
    m_TemplateId = value;
}

ByteBuffer& DLMSCompactData::GetTemplateDescription() {
    return m_TemplateDescription;
}

void DLMSCompactData::SetTemplateDescription(ByteBuffer& value) {
    m_TemplateDescription = value;
}

DLMS_CAPTURE_METHOD DLMSCompactData::GetCaptureMethod() {
    return m_CaptureMethod;
}

void DLMSCompactData::SetCaptureMethod(DLMS_CAPTURE_METHOD value) {
    m_CaptureMethod = value;
}

// Returns amount of attributes.
int DLMSCompactData::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int DLMSCompactData::GetMethodCount()
{
    return 2;
}

void DLMSCompactData::GetValues(std::vector<std::string>& values)
{
    std::stringstream sb;
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_Buffer.ToString());
    //Clear str.
    sb.str(std::string());
    sb << '[';
    bool empty = true;
    for (std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->first->GetName().ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_TemplateId));
    values.push_back(m_TemplateDescription.ToString());
    values.push_back(Helpers::IntToString(m_CaptureMethod));
}

void DLMSCompactData::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // Buffer
    if (all || CanRead(2)) {
        attributes.push_back(2);
    }
    // CaptureObjects
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // TemplateId
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // TemplateDescription
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // CaptureMethod
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
}

int DLMSCompactData::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 3:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 4:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 5:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 6:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}


int DLMSCompactData::GetCaptureObjects(DLMSSettings& settings, DLMSValueEventArg& e)
{
    ByteBuffer data;
    unsigned long cnt = (unsigned long)m_CaptureObjects.size();
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    //Add count
    Helpers::SetObjectCount(cnt, data);
    std::string ln;
    int ret;
    DLMSVariant tmp, ai, di;
    for (std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(4); //Count
        tmp = it->first->GetObjectType();
        //ClassID
        if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, tmp)) != 0)
        {
            return ret;
        }
        tmp.Clear();
        if ((ret = GetLogicalName((*it).first, tmp)) != 0)
        {
            return ret;
        }
        ai = (*it).second->GetAttributeIndex();
        di = (*it).second->GetDataIndex();
        //LN
        if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0 ||
            //Attribute Index
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_INT8, ai)) != 0 ||
            //Data Index
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, di)) != 0)
        {
            return ret;
        }
    }
    e.SetValue(data);
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSCompactData::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    switch (e.GetIndex())
    {
    case 1:
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    break;
    case 2:
        e.SetValue(m_Buffer);
        break;
    case 3:
        return GetCaptureObjects(settings, e);
    case 4:
        e.SetValue(m_TemplateId);
        break;
    case 5:
        e.SetValue(m_TemplateDescription);
        break;
    case 6:
        e.SetValue(m_CaptureMethod);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSCompactData::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    switch (e.GetIndex()) {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
        m_Buffer.Clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            m_Buffer.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        }
        else if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_Buffer.AddString(e.GetValue().strVal);
        }
        break;
    case 3:
        for (std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >::iterator it = m_CaptureObjects.begin(); it != m_CaptureObjects.end(); ++it)
        {
            delete it->second;
        }
        m_CaptureObjects.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                if ((*it).Arr.size() != 4)
                {
                    //Invalid structure format.
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)(*it).Arr[0].ToInteger();
                std::string ln;
                Helpers::GetLogicalName((*it).Arr[1].byteArr, ln);
                DLMSObject* pObj = settings.GetObjects().FindByLN(type, ln);
                if (pObj == NULL)
                {
                    continue;
                    pObj = DLMSObjectFactory::CreateObject(type, ln);
                    if (pObj != NULL)
                    {
                        settings.AddAllocateObject(pObj);
                    }
                }
                DLMSCaptureObject* pCO = new DLMSCaptureObject(it->Arr[2].ToInteger(), it->Arr[3].ToInteger());
                m_CaptureObjects.push_back(std::pair<DLMSObject*, DLMSCaptureObject*>(pObj, pCO));
            }
        }
        break;
    case 4:
        m_TemplateId = e.GetValue().ToInteger();
        break;
    case 5:
        m_TemplateDescription.Clear();
        m_TemplateDescription.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        break;
    case 6:
        m_CaptureMethod = (DLMS_CAPTURE_METHOD)e.GetValue().ToInteger();
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_COMPACT_DATA
