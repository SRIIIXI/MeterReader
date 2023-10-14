#ifndef DLMS_IP6_SETUP_H
#define DLMS_IP6_SETUP_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_IP6_SETUP
#include "DLMSObject.h"
#include "NeighborDiscoverySetup.h"

#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <ws2tcpip.h>
#else //Linux includes.
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define IN6_ADDR struct in6_addr
#endif

// Enumerated Address config modes.
typedef enum DLMS_IP6_ADDRESS_CONFIG_MODE{
    // Auto Configuration.
    DLMS_IP6_ADDRESS_CONFIG_MODE_AUTO,
    // DHCP v6.
    DLMS_IP6_ADDRESS_CONFIG_MODE_DHCP_V6,
    // Manual
    DLMS_IP6_ADDRESS_CONFIG_MODE_MANUAL,
    // Neighbour Discovery.
    DLMS_IP6_ADDRESS_CONFIG_MODE_NEIGHBOUR_DISCOVERY
}DLMS_IP6_ADDRESS_CONFIG_MODE;

class DLMSIp6Setup : public DLMSObject
{
    friend class XmlWriter;
    std::string m_DataLinkLayerReference;
    DLMS_IP6_ADDRESS_CONFIG_MODE m_AddressConfigMode;
    std::vector<IN6_ADDR> m_UnicastIPAddress;
    std::vector<IN6_ADDR> m_MulticastIPAddress;
    std::vector<IN6_ADDR> m_GatewayIPAddress;
    IN6_ADDR m_PrimaryDNSAddress;
    IN6_ADDR m_SecondaryDNSAddress;
    unsigned char m_TrafficClass;
    std::vector<NeighborDiscoverySetup*> m_NeighborDiscoverySetup;
public:
    //Constructor.
    DLMSIp6Setup();

    //SN Constructor.
    DLMSIp6Setup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSIp6Setup(std::string ln);

    //Destructor.
    ~DLMSIp6Setup();

    std::string& GetDataLinkLayerReference() {
        return m_DataLinkLayerReference;
    }

    void SetDataLinkLayerReference(std::string& value)
    {
        m_DataLinkLayerReference = value;
    }

    DLMS_IP6_ADDRESS_CONFIG_MODE GetAddressConfigMode()
    {
        return m_AddressConfigMode;
    }

    void SetAddressConfigMode(DLMS_IP6_ADDRESS_CONFIG_MODE value)
    {
        m_AddressConfigMode = value;
    }

    std::vector<IN6_ADDR>& GetUnicastIPAddress()
    {
        return m_UnicastIPAddress;
    }

    void SetUnicastIPAddress(std::vector<IN6_ADDR>& value)
    {
        m_UnicastIPAddress = value;
    }

    std::vector<IN6_ADDR>& GetMulticastIPAddress()
    {
        return m_MulticastIPAddress;
    }

    void setMulticastIPAddress(std::vector<IN6_ADDR>& value)
    {
        m_MulticastIPAddress = value;
    }

    std::vector<IN6_ADDR>& GetGatewayIPAddress()
    {
        return m_GatewayIPAddress;
    }

    void SetGatewayIPAddress(std::vector<IN6_ADDR>& value)
    {
        m_GatewayIPAddress = value;
    }

    IN6_ADDR& GetPrimaryDNSAddress()
    {
        return m_PrimaryDNSAddress;
    }

    void SetPrimaryDNSAddress(IN6_ADDR& value)
    {
        m_PrimaryDNSAddress = value;
    }

    IN6_ADDR& GetSecondaryDNSAddress()
    {
        return m_SecondaryDNSAddress;
    }

    void m_SetSecondaryDNSAddress(IN6_ADDR& value)
    {
        m_SecondaryDNSAddress = value;
    }

    unsigned char GetTrafficClass() {
        return m_TrafficClass;
    }

    void m_SetTrafficClass(unsigned char value) {
        m_TrafficClass = value;
    }

    std::vector<NeighborDiscoverySetup*>& GetNeighborDiscoverySetup() {
        return m_NeighborDiscoverySetup;
    }

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
