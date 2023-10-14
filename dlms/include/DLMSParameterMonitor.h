#ifndef PARAMETER_MONITOR_H
#define PARAMETER_MONITOR_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_PARAMETER_MONITOR

#include "DLMSObject.h"
#include "DLMSTarget.h"

class DLMSParameterMonitor : public DLMSObject
{
    /**
    * Changed parameter.
    */
    DLMSTarget m_ChangedParameter;
    /**
     * Capture time.
     */
    DateTime m_CaptureTime;
    /**
     * Changed Parameter
     */
    std::vector<DLMSTarget*> m_Parameters;

public:
    //Constructor.
    DLMSParameterMonitor();

    //SN Constructor.
    DLMSParameterMonitor(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSParameterMonitor(std::string ln);

    //Destructor.
    ~DLMSParameterMonitor();

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

    /**
   * Changed parameter.
   */
    DLMSTarget& GetChangedParameter();
    void SetChangedParameter(DLMSTarget& value);

    /**
     * Capture time.
     */
    DateTime& GetCaptureTime();
    void SetCaptureTime(DateTime& value);

    /**
     * Changed Parameter
     */
    std::vector<DLMSTarget*>& GetParameters();
    void SetParameters(std::vector<DLMSTarget*>& value);
};
#endif
#endif
