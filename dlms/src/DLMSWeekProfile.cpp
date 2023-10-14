#include "DLMSWeekProfile.h"
#include <sstream>

/**
 Constructor.
*/
DLMSWeekProfile::DLMSWeekProfile()
{
    m_Monday = m_Tuesday = m_Wednesday = m_Thursday = m_Friday = m_Saturday = m_Sunday = 0;
}

/**
 Constructor.
*/
DLMSWeekProfile::DLMSWeekProfile(std::string name, int monday, int tuesday, int wednesday, int thursday, int friday, int saturday, int sunday)
{
    SetName(name);
    SetMonday(monday);
    SetTuesday(tuesday);
    SetWednesday(wednesday);
    SetThursday(thursday);
    SetFriday(friday);
    SetSaturday(saturday);
    SetSunday(sunday);
}

void DLMSWeekProfile::GetName(ByteBuffer& value)
{
    value.Clear();
    value.Set(&m_Name, 0, m_Name.GetSize());
}

void DLMSWeekProfile::SetName(ByteBuffer& value)
{
    m_Name = value;
}

std::string DLMSWeekProfile::GetName()
{
    return m_Name.ToString();
}
void DLMSWeekProfile::SetName(std::string value)
{
    m_Name.AddString(value.c_str());
}

int DLMSWeekProfile::GetMonday()
{
    return m_Monday;
}
void DLMSWeekProfile::SetMonday(int value)
{
    m_Monday = value;
}

int DLMSWeekProfile::GetTuesday()
{
    return m_Tuesday;
}
void DLMSWeekProfile::SetTuesday(int value)
{
    m_Tuesday = value;
}

int DLMSWeekProfile::GetWednesday()
{
    return m_Wednesday;
}
void DLMSWeekProfile::SetWednesday(int value)
{
    m_Wednesday = value;
}

int DLMSWeekProfile::GetThursday()
{
    return m_Thursday;
}
void DLMSWeekProfile::SetThursday(int value)
{
    m_Thursday = value;
}

int DLMSWeekProfile::GetFriday()
{
    return m_Friday;
}
void DLMSWeekProfile::SetFriday(int value)
{
    m_Friday = value;
}

int DLMSWeekProfile::GetSaturday()
{
    return m_Saturday;
}
void DLMSWeekProfile::SetSaturday(int value)
{
    m_Saturday = value;
}

int DLMSWeekProfile::GetSunday()
{
    return m_Sunday;
}
void DLMSWeekProfile::SetSunday(int value)
{
    m_Sunday = value;
}

std::string DLMSWeekProfile::ToString()
{
    std::stringstream sb;
    if (m_Name.IsAsciiString())
    {
        sb << m_Name.ToString();
    }
    else
    {
        sb << m_Name.ToHexString();
    }
    return sb.str();
}
