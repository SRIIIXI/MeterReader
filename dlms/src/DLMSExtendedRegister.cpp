#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSExtendedRegister.h"

#ifndef DLMS_IGNORE_EXTENDED_REGISTER
bool DLMSExtendedRegister::IsRead(int index)
{
    if (index == 3)
    {
        return m_Unit != 0;
    }
    return DLMSObject::IsRead(index);
}
//Constructor.
DLMSExtendedRegister::DLMSExtendedRegister() :
    DLMSExtendedRegister("", 0)
{
}

//SN Constructor.
DLMSExtendedRegister::DLMSExtendedRegister(std::string ln, unsigned short sn) :
    DLMSRegister(DLMS_OBJECT_TYPE_EXTENDED_REGISTER, ln, sn)
{
}

//LN Constructor.
DLMSExtendedRegister::DLMSExtendedRegister(std::string ln) :
    DLMSExtendedRegister(ln, 0)
{
}

// Get value of COSEM Data object.
DLMSVariant& DLMSExtendedRegister::GetValue()
{
    return DLMSRegister::GetValue();
}

// Set value of COSEM Data object.
void DLMSExtendedRegister::SetValue(DLMSVariant& value)
{
    DLMSRegister::SetValue(value);
}

/**
 Status of COSEM Extended Register object.
*/
DLMSVariant& DLMSExtendedRegister::GetStatus()
{
    return m_Status;
}
void DLMSExtendedRegister::SetStatus(DLMSVariant& value)
{
    m_Status = value;
}

/**
 Capture time of COSEM Extended Register object.
*/
DateTime& DLMSExtendedRegister::GetCaptureTime()
{
    return m_CaptureTime;
}

void DLMSExtendedRegister::SetCaptureTime(DateTime& value)
{
    m_CaptureTime = value;
}

int DLMSExtendedRegister::GetUIDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_DATETIME;
    }
    else
    {
        return DLMSRegister::GetUIDataType(index, type);
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns amount of attributes.
int DLMSExtendedRegister::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int DLMSExtendedRegister::GetMethodCount()
{
    return 1;
}

void DLMSExtendedRegister::GetValues(std::vector<std::string>& values)
{
    DLMSRegister::GetValues(values);
    values.push_back(m_Status.ToString());
    values.push_back(m_CaptureTime.ToString());
}

void DLMSExtendedRegister::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    DLMSRegister::GetAttributeIndexToRead(all, attributes);
    //Status
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //CaptureTime
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
}

int DLMSExtendedRegister::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index < 4)
    {
        return DLMSRegister::GetDataType(index, type);
    }
    if (index == 4)
    {
        return DLMSObject::GetDataType(index, type);
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}


int DLMSExtendedRegister::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_Value.Clear();
        m_CaptureTime = DateTime(DateTime::Now());
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

int DLMSExtendedRegister::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() < 4)
    {
        return DLMSRegister::GetValue(settings, e);
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(m_Status);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(m_CaptureTime);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int DLMSExtendedRegister::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() < 4)
    {
        return DLMSRegister::SetValue(settings, e);
    }
    else if (e.GetIndex() == 4)
    {
        m_Status = e.GetValue();
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            DLMSVariant tmp;
            DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
            m_CaptureTime = tmp.dateTime;
        }
        else
        {
            m_CaptureTime = e.GetValue().dateTime;
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_EXTENDED_REGISTER
