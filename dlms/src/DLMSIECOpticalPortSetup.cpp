#include "DLMSIECOpticalPortSetup.h"
#include "DLMSClient.h"
#include "DLMSConverter.h"

#ifndef DLMS_IGNORE_IEC_LOCAL_PORT_SETUP
//Constructor.
DLMSIECLocalPortSetup::DLMSIECLocalPortSetup() :
    DLMSIECLocalPortSetup("0.0.20.0.0.255", 0)
{
}

//SN Constructor.
DLMSIECLocalPortSetup::DLMSIECLocalPortSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP, ln, sn)
{
    m_DefaultMode = DLMS_OPTICAL_PROTOCOL_MODE_DEFAULT;
    m_DefaultBaudrate = DLMS_BAUD_RATE_300;
    m_ProposedBaudrate = DLMS_BAUD_RATE_9600;
    m_Version = 1;
}

//LN Constructor.
DLMSIECLocalPortSetup::DLMSIECLocalPortSetup(std::string ln) :
    DLMSIECLocalPortSetup(ln, 0)
{
}

DLMS_OPTICAL_PROTOCOL_MODE DLMSIECLocalPortSetup::GetDefaultMode()
{
    return m_DefaultMode;
}
void DLMSIECLocalPortSetup::SetDefaultMode(DLMS_OPTICAL_PROTOCOL_MODE value)
{
    m_DefaultMode = value;
}

DLMS_BAUD_RATE DLMSIECLocalPortSetup::GetDefaultBaudrate()
{
    return m_DefaultBaudrate;
}
void DLMSIECLocalPortSetup::SetDefaultBaudrate(DLMS_BAUD_RATE value)
{
    m_DefaultBaudrate = value;
}

DLMS_BAUD_RATE DLMSIECLocalPortSetup::GetProposedBaudrate()
{
    return m_ProposedBaudrate;
}
void DLMSIECLocalPortSetup::SetProposedBaudrate(DLMS_BAUD_RATE value)
{
    m_ProposedBaudrate = value;
}

DLMS_LOCAL_PORT_RESPONSE_TIME DLMSIECLocalPortSetup::GetResponseTime()
{
    return m_ResponseTime;
}
void DLMSIECLocalPortSetup::SetResponseTime(DLMS_LOCAL_PORT_RESPONSE_TIME value)
{
    m_ResponseTime = value;
}

std::string DLMSIECLocalPortSetup::GetDeviceAddress()
{
    return m_DeviceAddress;
}
void DLMSIECLocalPortSetup::SetDeviceAddress(std::string value)
{
    m_DeviceAddress = value;
}

std::string DLMSIECLocalPortSetup::GetPassword1()
{
    return m_Password1;
}
void DLMSIECLocalPortSetup::SetPassword1(std::string value)
{
    m_Password1 = value;
}

std::string DLMSIECLocalPortSetup::GetPassword2()
{
    return m_Password2;
}
void DLMSIECLocalPortSetup::SetPassword2(std::string value)
{
    m_Password2 = value;
}

std::string DLMSIECLocalPortSetup::GetPassword5()
{
    return m_Password5;
}
void DLMSIECLocalPortSetup::SetPassword5(std::string value)
{
    m_Password5 = value;
}

// Returns amount of attributes.
int DLMSIECLocalPortSetup::GetAttributeCount()
{
    return 9;
}

// Returns amount of methods.
int DLMSIECLocalPortSetup::GetMethodCount()
{
    return 0;
}

void DLMSIECLocalPortSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_DefaultMode).ToString());
    values.push_back(DLMSConverter::ToString(m_DefaultBaudrate));
    values.push_back(DLMSConverter::ToString(m_ProposedBaudrate));
    values.push_back(DLMSVariant(m_ResponseTime).ToString());
    values.push_back(m_DeviceAddress);
    values.push_back(m_Password1);
    values.push_back(m_Password2);
    values.push_back(m_Password5);
}

void DLMSIECLocalPortSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //DefaultMode
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //DefaultBaudrate
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //ProposedBaudrate
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //ResponseTime
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //DeviceAddress
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    //Password1
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
    //Password2
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
    //Password5
    if (all || !IsRead(9))
    {
        attributes.push_back(9);
    }
}

int DLMSIECLocalPortSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 9)
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
int DLMSIECLocalPortSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(GetDefaultMode());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(GetDefaultBaudrate());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(GetProposedBaudrate());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(GetResponseTime());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.GetValue().Add(&m_DeviceAddress[0], (unsigned long)m_DeviceAddress.size());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.GetValue().Add(&m_Password1[0], (unsigned long)m_Password1.size());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.GetValue().Add(&m_Password2[0], (unsigned long)m_Password2.size());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        e.GetValue().Add(&m_Password5[0], (int)m_Password5.size());
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSIECLocalPortSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        SetDefaultMode((DLMS_OPTICAL_PROTOCOL_MODE)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        SetDefaultBaudrate((DLMS_BAUD_RATE)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        SetProposedBaudrate((DLMS_BAUD_RATE)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 5)
    {
        SetResponseTime((DLMS_LOCAL_PORT_RESPONSE_TIME)e.GetValue().lVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 6)
    {
        DLMSVariant tmp;
        DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetDeviceAddress(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 7)
    {
        DLMSVariant tmp;
        DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetPassword1(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 8)
    {
        DLMSVariant tmp;
        DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetPassword2(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 9)
    {
        DLMSVariant tmp;
        DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetPassword5(tmp.strVal);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

#endif //DLMS_IGNORE_IEC_LOCAL_PORT_SETUP
