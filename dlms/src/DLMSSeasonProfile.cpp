#include "DLMSSeasonProfile.h"
#include <sstream>

/**
 Constructor.
*/
DLMSSeasonProfile::DLMSSeasonProfile()
{

}

/**
 Constructor.
*/
DLMSSeasonProfile::DLMSSeasonProfile(std::string name, DateTime& start, std::string weekName)
{
    m_Name.AddString(name);
    SetStart(start);
    m_WeekName.AddString(weekName);
}

/**
 Constructor.
*/
DLMSSeasonProfile::DLMSSeasonProfile(ByteBuffer& name, DateTime& start, ByteBuffer weekName)
{
    m_Name.Set(&name);
    SetStart(start);
    m_WeekName.Set(&weekName);
}

/**
 Name of season profile.
*/
ByteBuffer& DLMSSeasonProfile::GetName()
{
    return m_Name;
}
void DLMSSeasonProfile::SetName(ByteBuffer& value)
{
    m_Name = value;
}

/**
 Season Profile start time.
*/
DateTime& DLMSSeasonProfile::GetStart()
{
    return m_Start;
}
void DLMSSeasonProfile::SetStart(DateTime value)
{
    m_Start = value;
}

/**
 Week name of season profile.
*/
ByteBuffer& DLMSSeasonProfile::GetWeekName()
{
    return m_WeekName;
}
void DLMSSeasonProfile::SetWeekName(ByteBuffer& value)
{
    m_WeekName = value;
}

std::string DLMSSeasonProfile::ToString()
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
    sb << " ";
    sb << m_Start.ToString().c_str();
    return sb.str();
}
