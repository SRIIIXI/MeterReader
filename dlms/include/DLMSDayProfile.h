#ifndef DLMS_DAY_PROFILE_H
#define DLMS_DAY_PROFILE_H

#include <vector>
#include "DLMSDayProfileAction.h"

class DLMSDayProfile
{
    short m_DayId;
    std::vector<DLMSDayProfileAction*> m_DaySchedules;
public:
    /**
     Constructor.
    */
    DLMSDayProfile();

    /**
     Constructor.
    */
    DLMSDayProfile(short dayId, std::vector<DLMSDayProfileAction*>& schedules);

    //Destructor.
    ~DLMSDayProfile();


    /**
     User defined identifier, identifying the currentday_profile.
    */
    short GetDayId();
    void SetDayId(short value);

    std::vector<DLMSDayProfileAction*>& GetDaySchedules();
    void SetDaySchedules(std::vector<DLMSDayProfileAction*>& value);
    std::string ToString();
};

#endif
