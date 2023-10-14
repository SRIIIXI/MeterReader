#include "DLMSScheduleEntry.h"

unsigned short DLMSScheduleEntry::GetIndex()
{
    return m_Index;
}

void DLMSScheduleEntry::SetIndex(unsigned short value)
{
    m_Index = value;
}

bool DLMSScheduleEntry::GetEnable()
{
    return m_Enable;
}

void DLMSScheduleEntry::SetEnable(bool value)
{
    m_Enable = value;
}

std::string DLMSScheduleEntry::GetLogicalName()
{
    return m_LogicalName;
}

void DLMSScheduleEntry::SetLogicalName(std::string value)
{
    m_LogicalName = value;
}

unsigned short DLMSScheduleEntry::GetScriptSelector()
{
    return m_ScriptSelector;
}


void DLMSScheduleEntry::SetScriptSelector(unsigned short value)
{
    m_ScriptSelector = value;
}

Time& DLMSScheduleEntry::GetSwitchTime()
{
    return m_SwitchTime;
}

void DLMSScheduleEntry::SetSwitchTime(Time& value)
{
    m_SwitchTime = value;
}

unsigned short DLMSScheduleEntry::GetValidityWindow()
{
    return m_ValidityWindow;
}

void DLMSScheduleEntry::SetValidityWindow(unsigned short value)
{
    m_ValidityWindow = value;
}

DLMS_WEEKDAYS DLMSScheduleEntry::GetExecWeekdays()
{
    return m_ExecWeekdays;
}

void DLMSScheduleEntry::SetExecWeekdays(DLMS_WEEKDAYS value)
{
    m_ExecWeekdays = value;
}

std::string& DLMSScheduleEntry::GetExecSpecDays()
{
    return m_ExecSpecDays;
}

void DLMSScheduleEntry::SetExecSpecDays(std::string& value)
{
    m_ExecSpecDays = value;
}

Date& DLMSScheduleEntry::GetBeginDate()
{
    return m_BeginDate;
}

void DLMSScheduleEntry::SetBeginDate(Date& value)
{
    m_BeginDate = value;
}

Date& DLMSScheduleEntry::GetEndDate()
{
    return m_EndDate;
}

void DLMSScheduleEntry::SetEndDate(Date& value)
{
    m_EndDate = value;
}
