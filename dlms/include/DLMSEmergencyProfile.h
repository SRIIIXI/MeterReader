#ifndef DLMS_EMERGENCY_PROFILE_H
#define DLMS_EMERGENCY_PROFILE_H
#include "DateTime.h"

class DLMSEmergencyProfile
{
    int m_ID;
    DateTime m_ActivationTime;
    int m_Duration;
public:

    int GetID();
    void SetID(int value);

    DateTime& GetActivationTime();
    void SetActivationTime(DateTime value);

    int GetDuration();
    void SetDuration(int value);

    std::string ToString();
};

#endif
