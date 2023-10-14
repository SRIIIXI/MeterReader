#include "DLMSSFSKActiveInitiator.h"

#ifndef DLMS_IGNORE_SFSK_ACTIVE_INITIATOR
//Constructor.
DLMSSFSKActiveInitiator::DLMSSFSKActiveInitiator() :
    DLMSSFSKActiveInitiator("0.0.26.1.0.255", 0)
{
}

//SN Constructor.
DLMSSFSKActiveInitiator::DLMSSFSKActiveInitiator(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SFSK_ACTIVE_INITIATOR, ln, sn)
{

}

//LN Constructor.
DLMSSFSKActiveInitiator::DLMSSFSKActiveInitiator(std::string ln) :
    DLMSSFSKActiveInitiator(ln, 0)
{

}

ByteBuffer& DLMSSFSKActiveInitiator::GetSystemTitle()
{
    return m_SystemTitle;
}

void DLMSSFSKActiveInitiator::SetSystemTitle(ByteBuffer& value)
{
    m_SystemTitle = value;
}

uint16_t DLMSSFSKActiveInitiator::GetMacAddress()
{
    return m_MacAddress;
}

void DLMSSFSKActiveInitiator::SetMacAddress(uint16_t value)
{
    m_MacAddress = value;
}

unsigned char DLMSSFSKActiveInitiator::GetLSapSelector()
{
    return m_LSapSelector;
}

void DLMSSFSKActiveInitiator::SetLSapSelector(unsigned char value)
{
    m_LSapSelector = value;
}

// Returns amount of attributes.
int DLMSSFSKActiveInitiator::GetAttributeCount()
{
    return 1;
}

// Returns amount of methods.
int DLMSSFSKActiveInitiator::GetMethodCount()
{
    return 1;
}

void DLMSSFSKActiveInitiator::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << m_SystemTitle.ToHexString(false);
    sb << ", ";
    sb << Helpers::IntToString(m_MacAddress);
    sb << ", ";
    sb << Helpers::IntToString(m_LSapSelector);
    values.push_back(sb.str());
}

void DLMSSFSKActiveInitiator::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // Active Initiator
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSSFSKActiveInitiator::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSSFSKActiveInitiator::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    if (e.GetIndex() == 1)
    {
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) == 0)
        {
            e.SetValue(tmp);
        }
    }
    else if (e.GetIndex() == 2)
    {
        ByteBuffer bb;
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
            (ret = bb.SetUInt8(3)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_OCTET_STRING, m_SystemTitle)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT16, m_MacAddress)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT8, m_LSapSelector)) == 0)
        {
            e.SetValue(bb);
        }
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSSFSKActiveInitiator::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            m_SystemTitle.Clear();
            e.GetValue().Arr[0].GetBytes(m_SystemTitle);
            m_MacAddress = e.GetValue().Arr[1].ToInteger();
            m_LSapSelector = e.GetValue().Arr[2].ToInteger();
        }
        else
        {
            m_SystemTitle.Clear();
            m_MacAddress = 0;
            m_LSapSelector = 0;
        }
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
#endif //DLMS_IGNORE_SFSK_ACTIVE_INITIATOR
