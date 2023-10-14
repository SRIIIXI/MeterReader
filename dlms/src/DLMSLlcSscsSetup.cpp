#include "DLMSLlcSscsSetup.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_LLC_SSCS_SETUP
//Constructor.
DLMSLlcSscsSetup::DLMSLlcSscsSetup() :
    DLMSLlcSscsSetup("0.0.28.0.0.255", 0)
{
}

//SN Constructor.
DLMSLlcSscsSetup::DLMSLlcSscsSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_LLC_SSCS_SETUP, ln, sn)
{
    m_BaseNodeAddress = m_ServiceNodeAddress = 0;
}

//LN Constructor.
DLMSLlcSscsSetup::DLMSLlcSscsSetup(std::string ln) :
    DLMSLlcSscsSetup(ln, 0)
{

}

unsigned short DLMSLlcSscsSetup::GetServiceNodeAddress()
{
    return m_ServiceNodeAddress;
}


void DLMSLlcSscsSetup::SetServiceNodeAddress(unsigned short value)
{
    m_ServiceNodeAddress = value;
}

unsigned short DLMSLlcSscsSetup::GetBaseNodeAddress()
{
    return m_BaseNodeAddress;
}

void DLMSLlcSscsSetup::SetBaseNodeAddress(unsigned short value)
{
    m_BaseNodeAddress = value;
}


int DLMSLlcSscsSetup::Reset(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int DLMSLlcSscsSetup::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_ServiceNodeAddress = m_BaseNodeAddress = 0;
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns amount of attributes.
int DLMSLlcSscsSetup::GetAttributeCount()
{
    return 3;
}

// Returns amount of methods.
int DLMSLlcSscsSetup::GetMethodCount()
{
    return 1;
}

void DLMSLlcSscsSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_ServiceNodeAddress).ToString());
    values.push_back(DLMSVariant(m_BaseNodeAddress).ToString());
}

void DLMSLlcSscsSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // ServiceNodeAddress
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // BaseNodeAddress
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
}

int DLMSLlcSscsSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
    case 3:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int DLMSLlcSscsSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 2)
    {
        e.SetValue(m_ServiceNodeAddress);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_BaseNodeAddress);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSLlcSscsSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ServiceNodeAddress = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_BaseNodeAddress = e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

#endif //DLMS_IGNORE_LLC_SSCS_SETUP
