#ifndef DLMS_IEC_HDLC_SETUP_H
#define DLMS_IEC_HDLC_SETUP_H

#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
#include "DLMSObject.h"

class DLMSIecHdlcSetup : public DLMSObject
{
    int m_InactivityTimeout;
    int m_DeviceAddress;
    int m_MaximumInfoLengthTransmit;
    DLMS_BAUD_RATE m_CommunicationSpeed;
    int m_WindowSizeTransmit;
    int m_WindowSizeReceive;
    int m_InterCharachterTimeout;
    int m_MaximumInfoLengthReceive;

public:
    //Constructor.
    DLMSIecHdlcSetup();

    //SN Constructor.
    DLMSIecHdlcSetup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSIecHdlcSetup(std::string ln);

    DLMS_BAUD_RATE GetCommunicationSpeed();

    void SetCommunicationSpeed(DLMS_BAUD_RATE value);

    int GetWindowSizeTransmit();
    void SetWindowSizeTransmit(int value);

    int GetWindowSizeReceive();
    void SetWindowSizeReceive(int value);
    int GetMaximumInfoLengthTransmit();
    void SetMaximumInfoLengthTransmit(int value);
    int GetMaximumInfoLengthReceive();
    void SetMaximumInfoLengthReceive(int value);

    int GetInterCharachterTimeout();
    void SetInterCharachterTimeout(int value);


    int GetInactivityTimeout();
    void SetInactivityTimeout(int value);

    int GetDeviceAddress();
    void SetDeviceAddress(int value);

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
