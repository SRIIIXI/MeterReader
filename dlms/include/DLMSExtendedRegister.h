#ifndef DLMS_EXTENDED_REGISTER_H
#define DLMS_EXTENDED_REGISTER_H

#include <math.h>
#include "DLMSRegister.h"
#ifndef DLMS_IGNORE_EXTENDED_REGISTER

class DLMSExtendedRegister : public DLMSRegister
{
private:
    DateTime m_CaptureTime;
    DLMSVariant m_Status;

protected:
    bool IsRead(int index);
public:
    //Constructor.
    DLMSExtendedRegister(void);

    //SN Constructor.
    DLMSExtendedRegister(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSExtendedRegister(std::string ln);

    // Get value of COSEM Data object.
    DLMSVariant& GetValue();

    // Set value of COSEM Data object.
    void SetValue(DLMSVariant& value);

    /**
     Status of COSEM Extended Register object.
    */
    DLMSVariant& GetStatus();

    void SetStatus(DLMSVariant& value);

    /**
     Capture time of COSEM Extended Register object.
    */
    DateTime& GetCaptureTime();

    void SetCaptureTime(DateTime& value);

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

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    int GetUIDataType(int index, DLMS_DATA_TYPE& type);
};
#endif
#endif
