#include "DLMSMacAddressSetup.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_MAC_ADDRESS_SETUP
DLMSMacAddressSetup::DLMSMacAddressSetup() :
    DLMSMacAddressSetup("0.0.25.2.0.255", 0)
{
}

DLMSMacAddressSetup::DLMSMacAddressSetup(std::string ln) :
    DLMSMacAddressSetup(ln, 0)
{
}

DLMSMacAddressSetup::DLMSMacAddressSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP, ln, sn)
{

}

/**
 Value of COSEM Data object.
*/
std::string DLMSMacAddressSetup::GetMacAddress()
{
    return m_MacAddress;
}
void DLMSMacAddressSetup::SetMacAddress(std::string value)
{
    m_MacAddress = value;
}

// Returns amount of attributes.
int DLMSMacAddressSetup::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSMacAddressSetup::GetMethodCount()
{
    return 0;
}

void DLMSMacAddressSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_MacAddress);
}

void DLMSMacAddressSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //MacAddress
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSMacAddressSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}


// Returns value of given attribute.
int DLMSMacAddressSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        std::string add = GetMacAddress();
        Helpers::Replace(add, ":", ".");
        ByteBuffer data;
        Helpers::HexToBytes(add, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSMacAddressSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        std::string add = Helpers::BytesToHex(e.GetValue().byteArr, e.GetValue().GetSize());
        Helpers::Replace(add, " ", ":");
        SetMacAddress(add);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
#endif //DLMS_IGNORE_MAC_ADDRESS_SETUP
