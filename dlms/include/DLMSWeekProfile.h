#ifndef DLMS_WEEK_PROFILE_H
#define DLMS_WEEK_PROFILE_H

#include <string>
#include "ByteBuffer.h"

class DLMSWeekProfile
{
    ByteBuffer m_Name;
    int m_Monday;
    int m_Tuesday;
    int m_Wednesday;
    int m_Thursday;
    int m_Friday;
    int m_Saturday;
    int m_Sunday;
public:
    /**
     Constructor.
    */
    DLMSWeekProfile();

    /**
     Constructor.
    */
    DLMSWeekProfile(std::string name, int monday, int tuesday, int wednesday, int thursday, int friday, int saturday, int sunday);

    std::string GetName();

    void SetName(std::string value);

    void GetName(ByteBuffer& value);

    void SetName(ByteBuffer& value);

    int GetMonday();

    void SetMonday(int value);

    int GetTuesday();

    void SetTuesday(int value);

    int GetWednesday();

    void SetWednesday(int value);

    int GetThursday();

    void SetThursday(int value);

    int GetFriday();

    void SetFriday(int value);

    int GetSaturday();

    void SetSaturday(int value);

    int GetSunday();

    void SetSunday(int value);

    std::string ToString();
};

#endif
