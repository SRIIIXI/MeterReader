#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSActionSchedule.h"
#include <sstream>
#ifndef DLMS_IGNORE_ACTION_SCHEDULE

/**
 Constructor.
*/
DLMSActionSchedule::DLMSActionSchedule() :
    DLMSActionSchedule("0.0.15.0.0.255", 0)
{
}

DLMSActionSchedule::DLMSActionSchedule(std::string ln) :
    DLMSActionSchedule(ln, 0)
{
}

DLMSActionSchedule::DLMSActionSchedule(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_ACTION_SCHEDULE, ln, sn)
{
    m_ExecutedScriptSelector = 0;
    m_Type = DLMS_SINGLE_ACTION_SCHEDULE_TYPE1;
}

std::string& DLMSActionSchedule::GetExecutedScriptLogicalName()
{
    return m_ExecutedScriptLogicalName;
}
void DLMSActionSchedule::SetExecutedScriptLogicalName(std::string value)
{
    m_ExecutedScriptLogicalName = value;
}

int DLMSActionSchedule::GetExecutedScriptSelector()
{
    return m_ExecutedScriptSelector;
}
void DLMSActionSchedule::SetExecutedScriptSelector(int value)
{
    m_ExecutedScriptSelector = value;
}

DLMS_SINGLE_ACTION_SCHEDULE_TYPE DLMSActionSchedule::GetType()
{
    return m_Type;
}
void DLMSActionSchedule::SetType(DLMS_SINGLE_ACTION_SCHEDULE_TYPE value)
{
    m_Type = value;
}

std::vector<DateTime>& DLMSActionSchedule::GetExecutionTime()
{
    return m_ExecutionTime;
}
void DLMSActionSchedule::SetExecutionTime(std::vector<DateTime>& value)
{
    m_ExecutionTime = value;
}

// Returns amount of attributes.
int DLMSActionSchedule::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int DLMSActionSchedule::GetMethodCount()
{
    return 0;
}

void DLMSActionSchedule::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_ExecutedScriptLogicalName + " " + DLMSVariant(m_ExecutedScriptSelector).ToString());
    values.push_back(DLMSVariant(m_Type).ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DateTime>::iterator it = m_ExecutionTime.begin(); it != m_ExecutionTime.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void DLMSActionSchedule::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ExecutedScriptLogicalName is static and read only once.
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //Type is static and read only once.
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //ExecutionTime is static and read only once.
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int DLMSActionSchedule::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
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
int DLMSActionSchedule::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    ByteBuffer data;
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
        e.SetByteArray(true);
        int ret;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        DLMSVariant ln;
        Helpers::SetLogicalName(m_ExecutedScriptLogicalName.c_str(), ln);
        DLMSVariant ss = m_ExecutedScriptSelector;
        if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 ||
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, ss)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(GetType());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        int ret;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)GetExecutionTime().size(), data);
        DLMSVariant val1, val2;
        for (std::vector<DateTime>::iterator it = m_ExecutionTime.begin(); it != m_ExecutionTime.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            //Count
            data.SetUInt8(2);
            Time t(*it);
            Date d(*it);
            val1 = t;
            val2 = d;
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, val1)) != 0 ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, val2)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSActionSchedule::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        Helpers::GetLogicalName(e.GetValue().Arr[0].byteArr, m_ExecutedScriptLogicalName);
        SetExecutedScriptSelector(e.GetValue().Arr[1].ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        SetType((DLMS_SINGLE_ACTION_SCHEDULE_TYPE)e.GetValue().ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        int ret;
        m_ExecutionTime.clear();
        DLMSVariant time, date;
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin();
            it != e.GetValue().Arr.end(); ++it)
        {
            if ((ret = DLMSClient::ChangeType((*it).Arr[0], DLMS_DATA_TYPE_TIME, time)) != 0 ||
                (ret = DLMSClient::ChangeType((*it).Arr[1], DLMS_DATA_TYPE_DATE, date)) != 0)
            {
                return ret;
            }
            time.dateTime.SetSkip((DATETIME_SKIPS)(time.dateTime.GetSkip() & ~(DATETIME_SKIPS_YEAR | DATETIME_SKIPS_MONTH | DATETIME_SKIPS_DAY | DATETIME_SKIPS_DAYOFWEEK)));
            date.dateTime.SetSkip((DATETIME_SKIPS)(date.dateTime.GetSkip() & ~(DATETIME_SKIPS_HOUR | DATETIME_SKIPS_MINUTE | DATETIME_SKIPS_SECOND | DATETIME_SKIPS_MS)));
            struct tm val = time.dateTime.GetValue();
            struct tm val2 = date.dateTime.GetValue();
            val2.tm_hour = val.tm_hour;
            val2.tm_min = val.tm_min;
            val2.tm_sec = val.tm_sec;
            DateTime tmp(val2);
            tmp.SetSkip((DATETIME_SKIPS)(time.dateTime.GetSkip() | date.dateTime.GetSkip()));
            tmp.SetExtra((DATE_TIME_EXTRA_INFO)(time.dateTime.GetExtra() | date.dateTime.GetExtra()));
            m_ExecutionTime.push_back(tmp);
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
#endif //DLMS_IGNORE_ACTION_SCHEDULE
