#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSGPRSSetup.h"

#ifndef DLMS_IGNORE_GPRS_SETUP
//Constructor.
DLMSGPRSSetup::DLMSGPRSSetup() :
    DLMSGPRSSetup("", 0)
{
}

//SN Constructor.
DLMSGPRSSetup::DLMSGPRSSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_GPRS_SETUP, ln, sn)
{
    m_PINCode = 0;
}

//LN Constructor.
DLMSGPRSSetup::DLMSGPRSSetup(std::string ln) :
    DLMSGPRSSetup(ln, 0)
{
}

std::string& DLMSGPRSSetup::GetAPN()
{
    return m_APN;
}
void DLMSGPRSSetup::SetAPN(std::string& value)
{
    m_APN = value;
}

long DLMSGPRSSetup::GetPINCode()
{
    return m_PINCode;
}
void DLMSGPRSSetup::SetPINCode(long value)
{
    m_PINCode = value;
}

DLMSQualityOfService& DLMSGPRSSetup::GetDefaultQualityOfService()
{
    return m_DefaultQualityOfService;
}

DLMSQualityOfService& DLMSGPRSSetup::GetRequestedQualityOfService()
{
    return m_RequestedQualityOfService;
}

// Returns amount of attributes.
int DLMSGPRSSetup::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int DLMSGPRSSetup::GetMethodCount()
{
    return 0;
}

void DLMSGPRSSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_APN);
    values.push_back(DLMSVariant(m_PINCode).ToString());
    values.push_back(m_DefaultQualityOfService.ToString() + " " + m_RequestedQualityOfService.ToString());
}

void DLMSGPRSSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //APN
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //PINCode
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //DefaultQualityOfService + RequestedQualityOfService
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int DLMSGPRSSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_UINT16;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int DLMSGPRSSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        ByteBuffer bb;
        bb.AddString(m_APN);
        e.SetValue(bb);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(m_PINCode);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(5);
        int ret;
        DLMSVariant precedence = m_DefaultQualityOfService.GetPrecedence();
        DLMSVariant delay = m_DefaultQualityOfService.GetDelay();
        DLMSVariant reliability = m_DefaultQualityOfService.GetReliability();
        DLMSVariant peak = m_DefaultQualityOfService.GetPeakThroughput();
        DLMSVariant mean = m_DefaultQualityOfService.GetMeanThroughput();
        if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, precedence)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, delay)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, reliability)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, peak)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, mean)) != 0)
        {
            return ret;
        }
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(5);
        precedence = m_RequestedQualityOfService.GetPrecedence();
        delay = m_RequestedQualityOfService.GetDelay();
        reliability = m_RequestedQualityOfService.GetReliability();
        peak = m_RequestedQualityOfService.GetPeakThroughput();
        mean = m_RequestedQualityOfService.GetMeanThroughput();

        if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, precedence)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, delay)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, reliability)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, peak)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, mean)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSGPRSSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_APN = e.GetValue().ToString();
        }
        else
        {
            m_APN.clear();
            m_APN.append(e.GetValue().byteArr, e.GetValue().byteArr + e.GetValue().GetSize());
        }
    }
    else if (e.GetIndex() == 3)
    {
        m_PINCode = e.GetValue().uiVal;
    }
    else if (e.GetIndex() == 4)
    {
        m_DefaultQualityOfService.SetPrecedence(e.GetValue().Arr[0].Arr[0].iVal);
        m_DefaultQualityOfService.SetDelay(e.GetValue().Arr[0].Arr[1].iVal);
        m_DefaultQualityOfService.SetReliability(e.GetValue().Arr[0].Arr[2].iVal);
        m_DefaultQualityOfService.SetPeakThroughput(e.GetValue().Arr[0].Arr[3].iVal);
        m_DefaultQualityOfService.SetMeanThroughput(e.GetValue().Arr[0].Arr[4].iVal);

        m_RequestedQualityOfService.SetPrecedence(e.GetValue().Arr[1].Arr[0].iVal);
        m_RequestedQualityOfService.SetDelay(e.GetValue().Arr[1].Arr[1].iVal);
        m_RequestedQualityOfService.SetReliability(e.GetValue().Arr[1].Arr[2].iVal);
        m_RequestedQualityOfService.SetPeakThroughput(e.GetValue().Arr[1].Arr[3].iVal);
        m_RequestedQualityOfService.SetMeanThroughput(e.GetValue().Arr[1].Arr[4].iVal);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_GPRS_SETUP
