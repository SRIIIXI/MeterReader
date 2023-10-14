#ifndef DLMS_SEASON_PROFILE_H
#define DLMS_SEASON_PROFILE_H

#include "DateTime.h"
#include "ByteBuffer.h"

class DLMSSeasonProfile
{
    ByteBuffer m_Name;
    DateTime m_Start;
    ByteBuffer m_WeekName;
public:
    /**
     Constructor.
    */
    DLMSSeasonProfile();

    /**
     Constructor.
    */
    DLMSSeasonProfile(std::string name, DateTime& start, std::string weekName);

    /**
     Constructor.
    */
    DLMSSeasonProfile(ByteBuffer& name, DateTime& start, ByteBuffer weekName);

    /**
     Name of season profile.
    */
    ByteBuffer& GetName();
    void SetName(ByteBuffer& value);

    /**
     Season Profile start time.
    */
    DateTime& GetStart();
    void SetStart(DateTime value);

    /**
     Week name of season profile.
    */
    ByteBuffer& GetWeekName();
    void SetWeekName(ByteBuffer& value);

    std::string ToString();
};

#endif
