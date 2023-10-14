#include "DLMSUtilityTables.h"

#ifndef DLMS_IGNORE_UTILITY_TABLES
//Constructor.
DLMSUtilityTables::DLMSUtilityTables() :
    DLMSUtilityTables("0.0.65.0.0.255", 0)
{
}

//SN Constructor.
DLMSUtilityTables::DLMSUtilityTables(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_UTILITY_TABLES, ln, sn)
{

}

//LN Constructor.
DLMSUtilityTables::DLMSUtilityTables(std::string ln) :
    DLMSUtilityTables(ln, 0)
{
    m_TableId = 0;
}

unsigned short DLMSUtilityTables::GetTableId()
{
    return m_TableId;
}

void DLMSUtilityTables::SetTableId(unsigned short value)
{
    m_TableId = value;
}

ByteBuffer& DLMSUtilityTables::GetBuffer()
{
    return m_Buffer;
}

void DLMSUtilityTables::SetBuffer(ByteBuffer& value)
{
    m_Buffer = value;
}

// Returns amount of attributes.
int DLMSUtilityTables::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int DLMSUtilityTables::GetMethodCount()
{
    return 0;
}

void DLMSUtilityTables::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_TableId));
    values.push_back(Helpers::IntToString(m_TableId));
    values.push_back(Helpers::IntToString(m_Buffer.GetSize()));
    values.push_back(m_Buffer.ToHexString());
}

void DLMSUtilityTables::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Table ID
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //Length
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //Buffer
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
}

int DLMSUtilityTables::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    case 3:
        type = DLMS_DATA_TYPE_UINT32;
        break;
    case 4:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSUtilityTables::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(m_TableId);
        break;
    case 3:
        e.SetValue(m_Buffer.GetSize());
        break;
    case 4:
        e.SetValue(m_Buffer);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSUtilityTables::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex()) {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_TableId = e.GetValue().ToInteger();
        // Skip len.
        break;
    case 3:
        // Skip len.
        break;
    case 4:
        m_Buffer.Clear();
        m_Buffer.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}
#endif //DLMS_IGNORE_UTILITY_TABLES
