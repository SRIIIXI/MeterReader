#ifndef DLMS_ACTIVITY_CALENDAR_H
#define DLMS_ACTIVITY_CALENDAR_H

#ifndef DLMS_IGNORE_ACTIVITY_CALENDAR

#include "DLMSObject.h"
#include "DLMSSeasonProfile.h"
#include "DLMSWeekProfile.h"
#include "DLMSDayProfile.h"

class DLMSActivityCalendar : public DLMSObject
{
    std::string m_CalendarNameActive;
    std::string m_CalendarNamePassive;
    std::vector<DLMSSeasonProfile*> m_SeasonProfileActive;
    std::vector<DLMSWeekProfile*> m_WeekProfileTableActive;
    std::vector<DLMSDayProfile*> m_DayProfileTableActive;
    std::vector<DLMSSeasonProfile*> m_SeasonProfilePassive;
    std::vector<DLMSDayProfile*> m_DayProfileTablePassive;
    std::vector<DLMSWeekProfile*> m_WeekProfileTablePassive;
    DateTime m_Time;
public:
    //Constructor.
    DLMSActivityCalendar();

    //SN Constructor.
    DLMSActivityCalendar(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSActivityCalendar(std::string ln);

    //Destructor.
    ~DLMSActivityCalendar();

    std::string GetCalendarNameActive();
    void SetCalendarNameActive(std::string value);

    std::vector<DLMSSeasonProfile*>& GetSeasonProfileActive();
    void SetSeasonProfileActive(std::vector<DLMSSeasonProfile*>& value);

    std::vector<DLMSWeekProfile*>& GetWeekProfileTableActive();
    void SetWeekProfileTableActive(std::vector<DLMSWeekProfile*>& value);

    std::vector<DLMSDayProfile*>& GetDayProfileTableActive();
    void SetDayProfileTableActive(std::vector<DLMSDayProfile*>& value);

    std::string GetCalendarNamePassive();
    void SetCalendarNamePassive(std::string value);

    std::vector<DLMSSeasonProfile*>& GetSeasonProfilePassive();
    void SetSeasonProfilePassive(std::vector<DLMSSeasonProfile*>& value);

    std::vector<DLMSWeekProfile*>& GetWeekProfileTablePassive();
    void SetWeekProfileTablePassive(std::vector<DLMSWeekProfile*>& value);

    std::vector<DLMSDayProfile*>& GetDayProfileTablePassive();
    void SetDayProfileTablePassive(std::vector<DLMSDayProfile*>& value);

    DateTime& GetTime();
    void SetTime(DateTime& value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif

#endif
