#ifndef DLMS_TCP_UDP_SETUP_H
#define DLMS_TCP_UDP_SETUP_H

#include "Ignore.h"
#include "DLMSObject.h"
#ifndef DLMS_IGNORE_TCP_UDP_SETUP

class DLMSTcpUdpSetup : public DLMSObject
{
    int m_Port;
    std::string m_IPReference;
    int m_MaximumSimultaneousConnections;
    int m_InactivityTimeout;
    int m_MaximumSegmentSize;

public:
    /**
     Constructor.
    */
    DLMSTcpUdpSetup();

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    DLMSTcpUdpSetup(std::string ln);


    /**
     Constructor.

     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    DLMSTcpUdpSetup(std::string ln, short sn);

    int GetPort();

    void SetPort(int value);

    std::string GetIPReference();

    void SetIPReference(std::string value);

    int GetMaximumSegmentSize();

    void SetMaximumSegmentSize(int value);

    int GetMaximumSimultaneousConnections();

    void SetMaximumSimultaneousConnections(int value);

    int GetInactivityTimeout();

    void SetInactivityTimeout(int value);

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
