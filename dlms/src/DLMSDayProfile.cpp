#include "DLMSDayProfile.h"
#include <sstream>

/**
 Constructor.
*/
DLMSDayProfile::DLMSDayProfile()
{
    m_DayId = 0;
}

/**
 Constructor.
*/
DLMSDayProfile::DLMSDayProfile(short dayId, std::vector<DLMSDayProfileAction*>& schedules)
{
    SetDayId(dayId);
    SetDaySchedules(schedules);
}

DLMSDayProfile::~DLMSDayProfile()
{
    for (std::vector<DLMSDayProfileAction*>::iterator it = m_DaySchedules.begin(); it != m_DaySchedules.end(); ++it)
    {
        delete *it;
    }
    m_DaySchedules.clear();
}

/**
 User defined identifier, identifying the currentday_profile.
*/
short DLMSDayProfile::GetDayId()
{
    return m_DayId;
}
void DLMSDayProfile::SetDayId(short value)
{
    m_DayId = value;
}

std::vector<DLMSDayProfileAction*>& DLMSDayProfile::GetDaySchedules()
{
    return m_DaySchedules;
}
void DLMSDayProfile::SetDaySchedules(std::vector<DLMSDayProfileAction*>& value)
{
    m_DaySchedules = value;
}

std::string DLMSDayProfile::ToString()
{
    std::stringstream sb;
    sb << m_DayId;
    for (std::vector<DLMSDayProfileAction*>::iterator it = m_DaySchedules.begin(); it != m_DaySchedules.end(); ++it)
    {
        sb << " ";
        sb << (*it)->ToString().c_str();
    }
    return sb.str();

}
