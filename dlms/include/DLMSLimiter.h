#ifndef DLMS_LIMITER_H
#define DLMS_LIMITER_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_LIMITER

#include "DLMSObject.h"
#include "DLMSEmergencyProfile.h"
#include "DLMSActionItem.h"

class DLMSLimiter : public DLMSObject
{
    DLMSObject* m_MonitoredValue;
    int m_MonitoredAttributeIndex;
    DLMSVariant m_ThresholdActive;
    DLMSVariant m_ThresholdNormal;
    DLMSVariant m_ThresholdEmergency;
    long m_MinOverThresholdDuration;
    long m_MinUnderThresholdDuration;
    DLMSEmergencyProfile m_EmergencyProfile;
    std::vector<int> m_EmergencyProfileGroupIDs;
    bool m_EmergencyProfileActive;
    DLMSActionItem m_ActionOverThreshold;
    DLMSActionItem m_ActionUnderThreshold;
public:
    //Constructor.
    DLMSLimiter();

    //SN Constructor.
    DLMSLimiter(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSLimiter(std::string ln);


    /**
     Defines an attribute of an object to be monitored.
    */
    DLMSObject* GetMonitoredValue();
    void SetMonitoredValue(DLMSObject* value);

    /**
     Provides the active threshold value to which the attribute monitored is compared.
    */
    DLMSVariant& GetThresholdActive();
    void SetThresholdActive(DLMSVariant& value);

    /**
     Provides the threshold value to which the attribute monitored
     is compared when in normal operation.
    */
    DLMSVariant& GetThresholdNormal();
    void SetThresholdNormal(DLMSVariant& value);

    /**
     Provides the threshold value to which the attribute monitored
     is compared when an emergency profile is active.
    */
    DLMSVariant& GetThresholdEmergency();
    void SetThresholdEmergency(DLMSVariant& value);

    /**
     Defines minimal over threshold duration in seconds required
     to execute the over threshold action.
    */
    long GetMinOverThresholdDuration();
    void SetMinOverThresholdDuration(long value);

    /**
     Defines minimal under threshold duration in seconds required to
     execute the under threshold action.
    */
    long GetMinUnderThresholdDuration();
    void SetMinUnderThresholdDuration(long value);

    DLMSEmergencyProfile& GetEmergencyProfile();
    void SetEmergencyProfile(DLMSEmergencyProfile value);

    std::vector<int>& GetEmergencyProfileGroupIDs();
    void SetEmergencyProfileGroupIDs(std::vector<int> value);

    /**
     Is Emergency Profile active.
    */
    bool GetEmergencyProfileActive();
    void SetEmergencyProfileActive(bool value);

    /**
     Defines the scripts to be executed when the monitored value
     crosses the threshold for minimal duration time.
    */
    DLMSActionItem& GetActionOverThreshold();
    void SetActionOverThreshold(DLMSActionItem value);

    /**
     Defines the scripts to be executed when the monitored value
     crosses the threshold for minimal duration time.
    */
    DLMSActionItem& GetActionUnderThreshold();
    void SetActionUnderThreshold(DLMSActionItem value);

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
