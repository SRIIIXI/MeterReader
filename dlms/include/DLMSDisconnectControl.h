#ifndef DLMS_DISCONNECT_CONTROL_H
#define DLMS_DISCONNECT_CONTROL_H

#ifndef DLMS_IGNORE_DISCONNECT_CONTROL
#include "DLMSObject.h"

class DLMSDisconnectControl : public DLMSObject
{
    bool m_OutputState;
    DLMS_CONTROL_STATE m_ControlState;
    DLMS_CONTROL_MODE m_ControlMode;

public:
    //Constructor.
    DLMSDisconnectControl();

    //SN Constructor.
    DLMSDisconnectControl(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSDisconnectControl(std::string ln);

    /**
     Output state of COSEM Disconnect Control object.
    */
    bool GetOutputState();
    void SetOutputState(bool value);

    /**
     Control state of COSEM Disconnect Control object.
    */
    DLMS_CONTROL_STATE GetControlState();
    void SetControlState(DLMS_CONTROL_STATE value);

    /**
    Control mode of COSEM Disconnect Control object.
    */
    DLMS_CONTROL_MODE GetControlMode();
    void SetControlMode(DLMS_CONTROL_MODE value);

    // Forces the disconnect control object into 'disconnected' state
    // if remote disconnection is enabled(control mode > 0).
    int RemoteDisconnect(DLMSClient* client, std::vector<ByteBuffer>& reply);

    // Forces the disconnect control object into the 'ready_for_reconnection'
    // state if a direct remote reconnection is disabled(control_mode = 1, 3, 5, 6).
    // Forces the disconnect control object into the 'connected' state if
    // a direct remote reconnection is enabled(control_mode = 2, 4).
    int RemoteReconnect(DLMSClient* client, std::vector<ByteBuffer>& reply);


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
