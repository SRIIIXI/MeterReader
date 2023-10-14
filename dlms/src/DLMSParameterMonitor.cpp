#include "DLMSParameterMonitor.h"

#ifndef DLMS_IGNORE_PARAMETER_MONITOR
#include "DLMSObjectFactory.h"
#include "DLMSSettings.h"

//Constructor.
DLMSParameterMonitor::DLMSParameterMonitor() :
    DLMSParameterMonitor("0.0.16.2.0.255", 0)
{
}

//SN Constructor.
DLMSParameterMonitor::DLMSParameterMonitor(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_PARAMETER_MONITOR, ln, sn)
{

}

//LN Constructor.
DLMSParameterMonitor::DLMSParameterMonitor(std::string ln) :
    DLMSParameterMonitor(ln, 0)
{

}

DLMSParameterMonitor::~DLMSParameterMonitor()
{
    for (std::vector<DLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        delete (*it);
    }
    m_Parameters.clear();
}

DLMSTarget& DLMSParameterMonitor::GetChangedParameter()
{
    return m_ChangedParameter;
}
void DLMSParameterMonitor::SetChangedParameter(DLMSTarget& value)
{
    m_ChangedParameter = value;
}

/**
 * Capture time.
 */
DateTime& DLMSParameterMonitor::GetCaptureTime()
{
    return m_CaptureTime;
}

void DLMSParameterMonitor::SetCaptureTime(DateTime& value)
{
    m_CaptureTime = value;
}

/**
 * Changed Parameter
 */
std::vector<DLMSTarget*>& DLMSParameterMonitor::GetParameters()
{
    return m_Parameters;
}
void DLMSParameterMonitor::SetParameters(std::vector<DLMSTarget*>& value)
{
    m_Parameters = value;
}

// Returns amount of attributes.
int DLMSParameterMonitor::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int DLMSParameterMonitor::GetMethodCount()
{
    return 2;
}

void DLMSParameterMonitor::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string tmp;
    GetLogicalName(tmp);
    values.push_back(tmp);
    values.push_back(m_ChangedParameter.ToString());
    values.push_back(m_CaptureTime.ToString());
    tmp = "";
    for (std::vector<DLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        tmp += (*it)->ToString();
    }
    values.push_back(tmp);
}

void DLMSParameterMonitor::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // ChangedParameter
    if (all || CanRead(2)) {
        attributes.push_back(2);
    }
    // CaptureTime
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // Parameters
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
}

int DLMSParameterMonitor::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 3:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 4:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSParameterMonitor::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    DLMSVariant tmp;
    ByteBuffer data;
    switch (e.GetIndex())
    {
    case 1:
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        break;
    case 2:
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(4);
        if (m_ChangedParameter.GetTarget() == NULL)
        {
            unsigned char ln[6] = { 0 };
            tmp = 0;
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, tmp);
            tmp.Clear();
            tmp.Add(ln, 6);
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            tmp = 1;
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_INT8, tmp);
            tmp.Clear();
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_NONE, tmp);
        }
        else
        {
            tmp = m_ChangedParameter.GetTarget()->GetObjectType();
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, tmp);
            tmp.Clear();
            if ((ret = GetLogicalName(m_ChangedParameter.GetTarget(), tmp)) != 0)
            {
                return ret;
            }
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            tmp = m_ChangedParameter.GetAttributeIndex();
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_INT8, tmp)) != 0)
            {
                return ret;
            }
            tmp = m_ChangedParameter.GetValue();
            if ((ret = Helpers::SetData(&settings, data, tmp.vt, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    break;
    case 3:
        e.SetValue(m_CaptureTime);
        break;
    case 4:
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        data.SetUInt8((unsigned char)m_Parameters.size());
        for (std::vector<DLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            tmp = (*it)->GetTarget()->GetObjectType();
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, tmp);
            tmp.Clear();
            if ((ret = GetLogicalName((*it)->GetTarget(), tmp)) != 0)
            {
                return ret;
            }
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            tmp = (*it)->GetAttributeIndex();
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_INT8, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSParameterMonitor::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    ByteBuffer bb;
    DLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
    {
        if (e.GetValue().Arr.size() != 4)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        m_ChangedParameter.Clear();
        DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)e.GetValue().Arr[0].ToInteger();
        unsigned char* ln = e.GetValue().Arr[1].byteArr;
        m_ChangedParameter.SetTarget(settings.GetObjects().FindByLN(type, ln));
        if (m_ChangedParameter.GetTarget() == NULL)
        {
            DLMSObject* tmp = DLMSObjectFactory::CreateObject(type);
            settings.AddAllocateObject(tmp);
            m_ChangedParameter.SetTarget(tmp);
            DLMSObject::SetLogicalName(m_ChangedParameter.GetTarget(), e.GetValue().Arr[1]);
        }
        m_ChangedParameter.SetAttributeIndex(e.GetValue().Arr[2].ToInteger());
        m_ChangedParameter.SetValue(e.GetValue().Arr[3]);
        break;
    }
    case 3:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            e.GetValue().GetBytes(bb);
            DataInfo info;
            info.SetType(DLMS_DATA_TYPE_DATETIME);
            if ((ret = Helpers::GetData(&settings, bb, info, tmp)) != 0)
            {
                return ret;
            }
            m_CaptureTime = tmp.dateTime;
        }
        else
        {
            m_CaptureTime = e.GetValue().dateTime;
        }
        break;
    case 4:
    {
        for (std::vector<DLMSTarget*>::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
        {
            delete (*it);
        }
        m_Parameters.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            if (it->Arr.size() != 3)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)it->Arr[0].ToInteger();
            unsigned char* ln = it->Arr[1].byteArr;
            DLMSTarget* pObj = new DLMSTarget();
            pObj->SetTarget(settings.GetObjects().FindByLN(type, ln));
            if (pObj->GetTarget() == NULL)
            {
                DLMSObject* tmp = DLMSObjectFactory::CreateObject(type);
                settings.AddAllocateObject(tmp);
                pObj->SetTarget(tmp);
                DLMSObject::SetLogicalName(pObj->GetTarget(), it->Arr[1]);
            }
            pObj->SetAttributeIndex(it->Arr[2].ToInteger());
            m_Parameters.push_back(pObj);
        }
        break;
    }
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_PARAMETER_MONITOR
