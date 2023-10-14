#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSTcpUdpSetup.h"

#ifndef DLMS_IGNORE_TCP_UDP_SETUP
/**
 Constructor.
*/
DLMSTcpUdpSetup::DLMSTcpUdpSetup() :
    DLMSTcpUdpSetup("0.0.25.0.0.255", 0)
{
}

/**
 Constructor.

 @param ln Logical Name of the object.
*/
DLMSTcpUdpSetup::DLMSTcpUdpSetup(std::string ln) :
    DLMSTcpUdpSetup(ln, 0)
{
}

/**
 Constructor.

 @param ln Logical Name of the object.
 @param sn Short Name of the object.
*/
DLMSTcpUdpSetup::DLMSTcpUdpSetup(std::string ln, short sn) : DLMSObject(DLMS_OBJECT_TYPE_TCP_UDP_SETUP, ln, sn)
{
    m_Port = 4059;
    m_IPReference = "127.0.0.1";
    m_MaximumSimultaneousConnections = 1;
    m_InactivityTimeout = 180;
    m_MaximumSegmentSize = 576;
}

int DLMSTcpUdpSetup::GetPort()
{
    return m_Port;
}

void DLMSTcpUdpSetup::SetPort(int value)
{
    m_Port = value;
}

std::string DLMSTcpUdpSetup::GetIPReference()
{
    return m_IPReference;
}
void DLMSTcpUdpSetup::SetIPReference(std::string value)
{
    m_IPReference = value;
}

int DLMSTcpUdpSetup::GetMaximumSegmentSize()
{
    return m_MaximumSegmentSize;
}
void DLMSTcpUdpSetup::SetMaximumSegmentSize(int value)
{
    m_MaximumSegmentSize = value;
}

int DLMSTcpUdpSetup::GetMaximumSimultaneousConnections()
{
    return m_MaximumSimultaneousConnections;
}
void DLMSTcpUdpSetup::SetMaximumSimultaneousConnections(int value)
{
    m_MaximumSimultaneousConnections = value;
}

int DLMSTcpUdpSetup::GetInactivityTimeout()
{
    return m_InactivityTimeout;
}
void DLMSTcpUdpSetup::SetInactivityTimeout(int value)
{
    m_InactivityTimeout = value;
}

// Returns amount of attributes.
int DLMSTcpUdpSetup::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int DLMSTcpUdpSetup::GetMethodCount()
{
    return 0;
}

void DLMSTcpUdpSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_Port).ToString());
    values.push_back(m_IPReference);
    values.push_back(DLMSVariant(m_MaximumSegmentSize).ToString());
    values.push_back(DLMSVariant(m_MaximumSimultaneousConnections).ToString());
    values.push_back(DLMSVariant(m_InactivityTimeout).ToString());
}

void DLMSTcpUdpSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Port
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //IPReference
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //MaximumSegmentSize
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //MaximumSimultaneousConnections
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //InactivityTimeout
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
}

int DLMSTcpUdpSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 6)
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
int DLMSTcpUdpSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        DLMSVariant tmp = GetPort();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        DLMSVariant tmp;
        Helpers::SetLogicalName(m_IPReference.c_str(), tmp);
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        DLMSVariant tmp = GetMaximumSegmentSize();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        DLMSVariant tmp = GetMaximumSimultaneousConnections();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        DLMSVariant tmp = GetInactivityTimeout();
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSTcpUdpSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        SetPort(e.GetValue().ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetIPReference("");
        }
        else
        {
            if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                Helpers::GetLogicalName(e.GetValue().byteArr, m_IPReference);
            }
            else
            {
                SetIPReference(e.GetValue().ToString());
            }
        }
    }
    else if (e.GetIndex() == 4)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetMaximumSegmentSize(576);
        }
        else
        {
            SetMaximumSegmentSize(e.GetValue().ToInteger());
        }
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetMaximumSimultaneousConnections(1);
        }
        else
        {
            SetMaximumSimultaneousConnections(e.GetValue().ToInteger());
        }
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            SetInactivityTimeout(180);
        }
        else
        {
            SetInactivityTimeout(e.GetValue().ToInteger());
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_TCP_UDP_SETUP
