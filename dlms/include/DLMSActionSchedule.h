#ifndef DLMS_ACTION_SCHEDULE_H
#define DLMS_ACTION_SCHEDULE_H

#ifndef DLMS_IGNORE_ACTION_SCHEDULE
#include "DLMSObject.h"

class DLMSActionSchedule : public DLMSObject
{
    std::string m_ExecutedScriptLogicalName;
    int m_ExecutedScriptSelector;
    DLMS_SINGLE_ACTION_SCHEDULE_TYPE m_Type;
    std::vector<DateTime> m_ExecutionTime;

public:

    /**
     Constructor.
    */
    DLMSActionSchedule();

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    DLMSActionSchedule(std::string ln);

    /**
     Constructor.

     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    DLMSActionSchedule(std::string ln, unsigned short sn);

    std::string& GetExecutedScriptLogicalName();
    void SetExecutedScriptLogicalName(std::string value);

    int GetExecutedScriptSelector();
    void SetExecutedScriptSelector(int value);

    DLMS_SINGLE_ACTION_SCHEDULE_TYPE GetType();
    void SetType(DLMS_SINGLE_ACTION_SCHEDULE_TYPE value);

    std::vector<DateTime>& GetExecutionTime();
    void SetExecutionTime(std::vector<DateTime>& value);

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
