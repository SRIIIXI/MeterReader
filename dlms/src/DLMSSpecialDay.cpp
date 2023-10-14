#include "DLMSSpecialDay.h"
#include <sstream>

int DLMSSpecialDay::GetIndex()
{
    return m_Index;
}
void DLMSSpecialDay::SetIndex(int value)
{
    m_Index = value;
}

Date& DLMSSpecialDay::GetDate()
{
    return m_Date;
}

void DLMSSpecialDay::SetDate(Date& value)
{
    m_Date = value;
}

void DLMSSpecialDay::SetDate(DateTime& value)
{
    m_Date = value;
}

int DLMSSpecialDay::GetDayId()
{
    return m_DayId;
}
void DLMSSpecialDay::SetDayId(int value)
{
    m_DayId = value;
}

std::string DLMSSpecialDay::ToString()
{
    std::stringstream sb;
    sb << m_Index;
    sb << " ";
    sb << m_Date.ToString().c_str();
    sb << " ";
    sb << m_DayId;
    return sb.str();
}
