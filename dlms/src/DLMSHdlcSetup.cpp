#include "DLMSHdlcSetup.h"
#include "DLMSConverter.h"

#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
//Constructor.
DLMSIecHdlcSetup::DLMSIecHdlcSetup() :
    DLMSIecHdlcSetup("0.0.22.0.0.255", 0)
{
}

//SN Constructor.
DLMSIecHdlcSetup::DLMSIecHdlcSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_IEC_HDLC_SETUP, ln, sn)
{
    m_CommunicationSpeed = DLMS_BAUD_RATE_9600;
    m_WindowSizeTransmit = m_WindowSizeReceive = 1;
    m_MaximumInfoLengthTransmit = m_MaximumInfoLengthReceive = 128;
    m_InactivityTimeout = 120;
    m_DeviceAddress = 0;
    m_InterCharachterTimeout = 30;
    m_Version = 1;
}

//LN Constructor.
DLMSIecHdlcSetup::DLMSIecHdlcSetup(std::string ln) :
    DLMSIecHdlcSetup(ln, 0)
{
}

DLMS_BAUD_RATE DLMSIecHdlcSetup::GetCommunicationSpeed()
{
    return m_CommunicationSpeed;
}
void DLMSIecHdlcSetup::SetCommunicationSpeed(DLMS_BAUD_RATE value)
{
    m_CommunicationSpeed = value;
}

int DLMSIecHdlcSetup::GetWindowSizeTransmit()
{
    return m_WindowSizeTransmit;
}
void DLMSIecHdlcSetup::SetWindowSizeTransmit(int value)
{
    m_WindowSizeTransmit = value;
}

int DLMSIecHdlcSetup::GetWindowSizeReceive()
{
    return m_WindowSizeReceive;
}
void DLMSIecHdlcSetup::SetWindowSizeReceive(int value)
{
    m_WindowSizeReceive = value;
}

int DLMSIecHdlcSetup::GetMaximumInfoLengthTransmit()
{
    return m_MaximumInfoLengthTransmit;
}
void DLMSIecHdlcSetup::SetMaximumInfoLengthTransmit(int value)
{
    m_MaximumInfoLengthTransmit = value;
}

int DLMSIecHdlcSetup::GetMaximumInfoLengthReceive()
{
    return m_MaximumInfoLengthReceive;
}
void DLMSIecHdlcSetup::SetMaximumInfoLengthReceive(int value)
{
    m_MaximumInfoLengthReceive = value;
}

int DLMSIecHdlcSetup::GetInterCharachterTimeout()
{
    return m_InterCharachterTimeout;
}
void DLMSIecHdlcSetup::SetInterCharachterTimeout(int value)
{
    m_InterCharachterTimeout = value;
}

int DLMSIecHdlcSetup::GetInactivityTimeout()
{
    return m_InactivityTimeout;
}
void DLMSIecHdlcSetup::SetInactivityTimeout(int value)
{
    m_InactivityTimeout = value;
}

int DLMSIecHdlcSetup::GetDeviceAddress()
{
    return m_DeviceAddress;
}
void DLMSIecHdlcSetup::SetDeviceAddress(int value)
{
    m_DeviceAddress = value;
}

// Returns amount of attributes.
int DLMSIecHdlcSetup::GetAttributeCount()
{
    return 9;
}

// Returns amount of methods.
int DLMSIecHdlcSetup::GetMethodCount()
{
    return 0;
}

void DLMSIecHdlcSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSConverter::ToString(m_CommunicationSpeed));
    values.push_back(DLMSVariant(m_WindowSizeTransmit).ToString());
    values.push_back(DLMSVariant(m_WindowSizeReceive).ToString());
    values.push_back(DLMSVariant(m_MaximumInfoLengthTransmit).ToString());
    values.push_back(DLMSVariant(m_MaximumInfoLengthReceive).ToString());
    values.push_back(DLMSVariant(m_InterCharachterTimeout).ToString());
    values.push_back(DLMSVariant(m_InactivityTimeout).ToString());
    values.push_back(DLMSVariant(m_DeviceAddress).ToString());
}

void DLMSIecHdlcSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //CommunicationSpeed
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //WindowSizeTransmit
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //WindowSizeReceive
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //MaximumInfoLengthTransmit
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //MaximumInfoLengthReceive
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    //InterCharachterTimeout
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
    //InactivityTimeout
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
    //DeviceAddress
    if (all || !IsRead(9))
    {
        attributes.push_back(9);
    }
}

int DLMSIecHdlcSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSIecHdlcSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
    else if (e.GetIndex() == 2)
    {
        e.SetValue(m_CommunicationSpeed);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_WindowSizeTransmit);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_WindowSizeReceive);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_MaximumInfoLengthTransmit);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue(m_MaximumInfoLengthReceive);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(m_InterCharachterTimeout);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(m_InactivityTimeout);
    }
    else if (e.GetIndex() == 9)
    {
        e.SetValue(m_DeviceAddress);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSIecHdlcSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_CommunicationSpeed = (DLMS_BAUD_RATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_WindowSizeTransmit = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_WindowSizeReceive = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_MaximumInfoLengthTransmit = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        m_MaximumInfoLengthReceive = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_InterCharachterTimeout = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_InactivityTimeout = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 9)
    {
        m_DeviceAddress = e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_IEC_HDLC_SETUP
