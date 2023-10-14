#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSClock.h"
#include "DLMSConverter.h"

#ifndef DLMS_IGNORE_CLOCK
DLMSClock::DLMSClock() :
    DLMSClock("0.0.1.0.0.255", 0)
{
}

DLMSClock::DLMSClock(std::string ln) :
    DLMSClock(ln, 0)
{
}

DLMSClock::DLMSClock(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_CLOCK, ln, sn)
{
    m_Deviation = m_ClockBase = DLMS_CLOCK_BASE_NONE;
    m_Enabled = false;
    m_TimeZone = 0;
    m_Status = DLMS_CLOCK_STATUS_OK;
}

/**
 Time of COSEM Clock object.
*/
DateTime& DLMSClock::GetTime()
{
    return m_Time;
}
void DLMSClock::SetTime(DateTime& value)
{
    m_Time = value;
}

/**
 TimeZone of COSEM Clock object.
*/
short DLMSClock::GetTimeZone()
{
    return m_TimeZone;
}
void DLMSClock::SetTimeZone(short value)
{
    m_TimeZone = value;
}

/**
 Status of COSEM Clock object.
*/
DLMS_CLOCK_STATUS DLMSClock::GetStatus()
{
    return m_Status;
}
void DLMSClock::SetStatus(DLMS_CLOCK_STATUS value)
{
    m_Status = value;
}

DateTime& DLMSClock::GetBegin()
{
    return m_Begin;
}

void DLMSClock::SetBegin(DateTime& value)
{
    m_Begin = value;
}

DateTime& DLMSClock::GetEnd()
{
    return m_End;
}
void DLMSClock::SetEnd(DateTime& value)
{
    m_End = value;
}

char DLMSClock::GetDeviation()
{
    return m_Deviation;
}
void DLMSClock::SetDeviation(char value)
{
    m_Deviation = value;
}

bool DLMSClock::GetEnabled()
{
    return m_Enabled;
}
void DLMSClock::SetEnabled(bool value)
{
    m_Enabled = value;
}

/**
 Clock base of COSEM Clock object.
*/
DLMS_CLOCK_BASE DLMSClock::GetClockBase()
{
    return m_ClockBase;
}
void DLMSClock::SetClockBase(DLMS_CLOCK_BASE value)
{
    m_ClockBase = value;
}

// Returns amount of attributes.
int DLMSClock::GetAttributeCount()
{
    return 9;
}

// Returns amount of methods.
int DLMSClock::GetMethodCount()
{
    return 6;
}

void DLMSClock::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string tmp;
    GetLogicalName(tmp);
    values.push_back(tmp);
    values.push_back(m_Time.ToString());
    values.push_back(DLMSVariant(m_TimeZone).ToString());
    tmp.append(DLMSConverter::ToString(m_Status));
    values.push_back(tmp);
    values.push_back(m_Begin.ToString());
    values.push_back(m_End.ToString());
    values.push_back(DLMSVariant(m_Deviation).ToString());
    values.push_back(DLMSVariant(m_Enabled).ToString());
    values.push_back(DLMSConverter::ToString(m_ClockBase));
}

void DLMSClock::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Time
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //TimeZone
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //Status
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //Begin
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
    //End
    if (all || !IsRead(6))
    {
        attributes.push_back(6);
    }
    //Deviation
    if (all || !IsRead(7))
    {
        attributes.push_back(7);
    }
    //Enabled
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
    //ClockBase
    if (all || !IsRead(9))
    {
        attributes.push_back(9);
    }
}

int DLMSClock::GetUIDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 2 || index == 5 || index == 6)
    {
        type = DLMS_DATA_TYPE_DATETIME;
    }
    else
    {
        return DLMSObject::GetUIDataType(index, type);
    }
    return DLMS_ERROR_CODE_OK;
}

int DLMSClock::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_INT16;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_INT8;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_BOOLEAN;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

/*
 * Returns value of given attribute.
 */
int DLMSClock::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(GetTime());
        return 0;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(GetTimeZone());
        return 0;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue((unsigned char)GetStatus());
        return 0;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(GetBegin());
        return 0;
    }
    if (e.GetIndex() == 6)
    {
        e.SetValue(GetEnd());
        return 0;
    }
    if (e.GetIndex() == 7)
    {
        e.SetValue(GetDeviation());
        return 0;
    }
    if (e.GetIndex() == 8)
    {
        e.SetValue(GetEnabled());
        return 0;
    }
    if (e.GetIndex() == 9)
    {
        e.SetValue(GetClockBase());
        return 0;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

/*
 * Set value of given attribute.
 */
int DLMSClock::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            DLMSVariant tmp;
            DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
            SetTime(tmp.dateTime);
        }
        else
        {
            SetTime(e.GetValue().dateTime);
        }
    }
    else if (e.GetIndex() == 3)
    {
        SetTimeZone(e.GetValue().ToInteger());
    }
    else if (e.GetIndex() == 4)
    {
        SetStatus((DLMS_CLOCK_STATUS)e.GetValue().ToInteger());
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            DLMSVariant tmp;
            DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
            SetBegin(tmp.dateTime);
        }
        else
        {
            SetBegin(e.GetValue().dateTime);
        }
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            DLMSVariant tmp;
            DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
            SetEnd(tmp.dateTime);
        }
        else
        {
            SetEnd(e.GetValue().dateTime);
        }
    }
    else if (e.GetIndex() == 7)
    {
        SetDeviation(e.GetValue().ToInteger());
    }
    else if (e.GetIndex() == 8)
    {
        SetEnabled(e.GetValue().boolVal);
    }
    else if (e.GetIndex() == 9)
    {
        SetClockBase((DLMS_CLOCK_BASE)e.GetValue().ToInteger());
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_CLOCK
