#ifndef DLMS_REGISTER_MONITOR_H
#define DLMS_REGISTER_MONITOR_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_REGISTER_MONITOR
#include "DLMSMonitoredValue.h"
#include "DLMSActionSet.h"

class DLMSRegisterMonitor : public DLMSObject
{
    std::vector<DLMSActionSet*> m_Actions;
    DLMSMonitoredValue m_MonitoredValue;
    std::vector<DLMSVariant> m_Thresholds;

public:
    /**
     Constructor.
    */
    DLMSRegisterMonitor();

    /**
     destructor.
    */
    virtual ~DLMSRegisterMonitor();

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    DLMSRegisterMonitor(std::string ln);

    /**
     Constructor.

     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    DLMSRegisterMonitor(std::string ln, unsigned short sn);

    std::vector<DLMSVariant>& GetThresholds();

    void SetThresholds(std::vector<DLMSVariant>& value);


    DLMSMonitoredValue& GetMonitoredValue();

    void SetMonitoredValue(DLMSMonitoredValue& value);

    std::vector<DLMSActionSet*>& GetActions();

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
