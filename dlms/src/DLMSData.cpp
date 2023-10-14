#include "DLMSData.h"

#ifndef DLMS_IGNORE_DATA

//Constructor.
DLMSData::DLMSData() :
    DLMSData("", 0)
{
}

//SN Constructor.
DLMSData::DLMSData(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_DATA, ln, sn)
{

}

//LN Constructor.
DLMSData::DLMSData(std::string ln) :
    DLMSData(ln, 0)
{

}

// Get value of COSEM Data object.
DLMSVariant&DLMSData::GetValue()
{
    return m_Value;
}

// Set value of COSEM Data object.
void DLMSData::SetValue(DLMSVariant& value)
{
    m_Value = value;
}

// Returns amount of attributes.
int DLMSData::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSData::GetMethodCount()
{
    return 0;
}

void DLMSData::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_Value.ToString());
}

void DLMSData::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Value
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSData::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        return DLMSObject::GetDataType(index, type);
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int DLMSData::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        e.SetValue(m_Value);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSData::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        SetValue(e.GetValue());
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_DATA
