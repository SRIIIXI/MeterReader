#include "DLMSObject.h"
#include "Helpers.h"

//SN Constructor.
DLMSObject::DLMSObject(DLMS_OBJECT_TYPE type, std::string& ln, unsigned short sn)
{
    Initialize(sn, type, 0, NULL);
    Helpers::SetLogicalName(ln.c_str(), m_LN);
}

//LN Constructor.
DLMSObject::DLMSObject(DLMS_OBJECT_TYPE type, std::string& ln)
{
    Initialize(0, type, 0, NULL);
    Helpers::SetLogicalName(ln.c_str(), m_LN);
}

DLMSObject::DLMSObject()
{
    Initialize(0, DLMS_OBJECT_TYPE_NONE, 0, NULL);
}

DLMSObject::DLMSObject(short sn, unsigned short class_id, unsigned char version, ByteBuffer& ln)
{
    Initialize(sn, class_id, version, &ln);
}

DLMSObject::DLMSObject(DLMS_OBJECT_TYPE type)
{
    Initialize(0, type, 0, NULL);
}

int DLMSObject::GetLogicalName(DLMSObject* target, DLMSVariant& value)
{
    if (target == NULL)
    {
        unsigned char ln[6] = {0};
        value.Add(ln, 6);
    }
    else
    {
        value.Add(target->m_LN, 6);
    }
    value.vt = DLMS_DATA_TYPE_OCTET_STRING;
    return DLMS_ERROR_CODE_OK;
}

int DLMSObject::SetLogicalName(DLMSObject * target, DLMSVariant& value)
{
    if (value.vt == DLMS_DATA_TYPE_STRING)
    {
        return Helpers::SetLogicalName(value.strVal.c_str(), target->m_LN);
    }
    if (value.vt != DLMS_DATA_TYPE_OCTET_STRING || value.GetSize() != 6)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    memcpy(target->m_LN, value.byteArr, 6);
    return DLMS_ERROR_CODE_OK;
}

int DLMSObject::SetLogicalName(DLMSObject* target, std::string& value)
{
    return Helpers::SetLogicalName(value.c_str(), target->m_LN);
}

void DLMSObject::Initialize(short sn, unsigned short class_id, unsigned char version, ByteBuffer* ln)
{
    m_SN = sn;
    m_ObjectType = (DLMS_OBJECT_TYPE)class_id;
    m_Version = version;
    if (ln == NULL)
    {
        memset(m_LN, 0, 6);
    }
    else
    {
        int cnt = ln->GetSize();
        if (cnt == 6)
        {
            ln->Get(m_LN, 6);
        }
        else
        {
            assert(false);
        }
    }
    //Attribute 1 is always Logical name.
    m_Attributes.push_back(DLMSAttribute(1, DLMS_DATA_TYPE_OCTET_STRING, DLMS_DATA_TYPE_OCTET_STRING));
}

DLMSObject::~DLMSObject(void)
{
    m_Attributes.clear();
    m_MethodAttributes.clear();
}

DLMSVariant DLMSObject::GetName()
{
    if (m_SN != 0)
    {
        return DLMSVariant(m_SN);
    }
    DLMSVariant ln;
    Helpers::GetLogicalName(m_LN, ln.strVal);
    ln.vt = DLMS_DATA_TYPE_STRING;
    return ln;
}

int DLMSObject::SetName(DLMSVariant& value)
{
    if (value.vt == DLMS_DATA_TYPE_UINT16)
    {
        m_SN = value.uiVal;
        return DLMS_ERROR_CODE_OK;
    }
    if (value.vt == DLMS_DATA_TYPE_STRING)
    {
        Helpers::SetLogicalName(value.strVal.c_str(), m_LN);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

DLMS_OBJECT_TYPE DLMSObject::GetObjectType()
{
    return m_ObjectType;
}

int DLMSObject::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index < 1)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    for (std::vector<DLMSAttribute>::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            type = (*it).GetDataType();
            return DLMS_ERROR_CODE_OK;
        }
    }
    type = DLMS_DATA_TYPE_NONE;
    return DLMS_ERROR_CODE_OK;
}

int DLMSObject::SetDataType(int index, DLMS_DATA_TYPE type)
{
    for (AttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            (*it).SetDataType(type);
            return DLMS_ERROR_CODE_OK;
        }
    }
    DLMSAttribute att(index);
    att.SetDataType(type);
    m_Attributes.push_back(att);
    return DLMS_ERROR_CODE_OK;
}

DLMS_ACCESS_MODE DLMSObject::GetAccess(int index)
{
    //LN is read only.
    if (index == 1)
    {
        return DLMS_ACCESS_MODE_READ;
    }
    for (AttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            return (*it).GetAccess();
        }
    }
    return DLMS_ACCESS_MODE_READ_WRITE;
}

// Set attribute access.
void DLMSObject::SetAccess(int index, DLMS_ACCESS_MODE access)
{
    for (AttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            (*it).SetAccess(access);
            return;
        }
    }
    DLMSAttribute att(index);
    att.SetAccess(access);
    m_Attributes.push_back(att);
}

DLMS_METHOD_ACCESS_MODE DLMSObject::GetMethodAccess(int index)
{
    for (AttributeCollection::iterator it = m_MethodAttributes.begin(); it != m_MethodAttributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            return (*it).GetMethodAccess();
        }
    }
    return DLMS_METHOD_ACCESS_MODE_ACCESS;
}

void DLMSObject::SetMethodAccess(int index, DLMS_METHOD_ACCESS_MODE access)
{
    for (AttributeCollection::iterator it = m_MethodAttributes.begin(); it != m_MethodAttributes.end(); ++it)
    {
        if ((*it).GetIndex() == index)
        {
            (*it).SetMethodAccess(access);
            return;
        }
    }
    DLMSAttribute att(index);
    att.SetMethodAccess(access);
    m_MethodAttributes.push_back(att);
}

int DLMSObject::GetUIDataType(int index, DLMS_DATA_TYPE& type)
{
    for (AttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if (it->GetIndex() == index)
        {
            type = it->GetUIDataType();
            return DLMS_ERROR_CODE_OK;
        }
    }
    type = DLMS_DATA_TYPE_NONE;
    return DLMS_ERROR_CODE_OK;
}

void DLMSObject::SetUIDataType(int index, DLMS_DATA_TYPE type)
{
    for (AttributeCollection::iterator it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
        if (it->GetIndex() == index)
        {
            return it->SetUIDataType(type);
        }
    }
    DLMSAttribute att(index);
    att.SetUIDataType(type);
    m_Attributes.push_back(att);
}

unsigned short DLMSObject::GetShortName()
{
    return m_SN;
}

void DLMSObject::SetShortName(unsigned short value)
{
    m_SN = value;
}

void DLMSObject::GetLogicalName(std::string& ln)
{
    Helpers::GetLogicalName(m_LN, ln);
}

void DLMSObject::SetVersion(unsigned short value)
{
    m_Version = value;
}

unsigned short DLMSObject::GetVersion()
{
    return m_Version;
}

AttributeCollection& DLMSObject::GetAttributes()
{
    return m_Attributes;
}

AttributeCollection& DLMSObject::GetMethodAttributes()
{
    return m_MethodAttributes;
}

//Get Object's Logical Name.
std::string& DLMSObject::GetDescription()
{
    return m_Description;
}

//Set Object's Logical Name.
void DLMSObject::SetDescription(std::string& value)
{
    m_Description = value;
}

bool DLMSObject::IsRead(int index)
{
    if (!CanRead(index))
    {
        return true;
    }
    return m_ReadTimes.find(index) != m_ReadTimes.end();
}

bool DLMSObject::CanRead(int index)
{
    return (GetAccess(index) & DLMS_ACCESS_MODE_READ) != 0;
}
