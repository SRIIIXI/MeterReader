#ifndef DLMS_CLOCK_H
#define DLMS_CLOCK_H

#include "DLMSObject.h"

#ifndef DLMS_IGNORE_CLOCK

class DLMSClock : public DLMSObject
{
    DLMS_CLOCK_BASE m_ClockBase;
    char m_Deviation;
    bool m_Enabled;
    DateTime m_End;
    DLMS_CLOCK_STATUS m_Status;
    DateTime m_Begin;
    short m_TimeZone;
    DateTime m_Time;

public:
    /**
     Constructor.
    */
    DLMSClock();

    /**
     Constructor.
     @param ln Logical Name of the object.
    */
    DLMSClock(std::string ln);

    /**
     Constructor.
     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    DLMSClock(std::string ln, unsigned short sn);

    /**
     Time of COSEM Clock object.
    */
    DateTime& GetTime();

    void SetTime(DateTime& value);

    /**
     TimeZone of COSEM Clock object.
    */
    short GetTimeZone();
    void SetTimeZone(short value);

    /**
     Status of COSEM Clock object.
    */
    DLMS_CLOCK_STATUS GetStatus();
    void SetStatus(DLMS_CLOCK_STATUS value);

    DateTime& GetBegin();
    void SetBegin(DateTime& value);

    DateTime& GetEnd();
    void SetEnd(DateTime& value);

    char GetDeviation();
    void SetDeviation(char value);

    bool GetEnabled();
    void SetEnabled(bool value);

    /**
     Clock base of COSEM Clock object.
    */
    DLMS_CLOCK_BASE GetClockBase();
    void SetClockBase(DLMS_CLOCK_BASE value);

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

    int GetUIDataType(int index, DLMS_DATA_TYPE& type);
    int GetDataType(int index, DLMS_DATA_TYPE& type);

    /*
     * Returns value of given attribute.
     */
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    /*
     * Set value of given attribute.
     */
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
