#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSEmergencyProfile.h"
#include <sstream>

int DLMSEmergencyProfile::GetID()
{
    return m_ID;
}
void DLMSEmergencyProfile::SetID(int value)
{
    m_ID = value;
}
DateTime& DLMSEmergencyProfile::GetActivationTime()
{
    return m_ActivationTime;
}
void DLMSEmergencyProfile::SetActivationTime(DateTime value)
{
    m_ActivationTime = value;
}
int DLMSEmergencyProfile::GetDuration()
{
    return m_Duration;
}
void DLMSEmergencyProfile::SetDuration(int value)
{
    m_Duration = value;
}

std::string DLMSEmergencyProfile::ToString()
{
    std::stringstream sb;
    sb << m_ID;
    sb << " ";
    sb << m_ActivationTime.ToString().c_str();
    sb << " ";
    sb << m_Duration;
    return sb.str();
}
