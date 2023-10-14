#ifndef DLMS_IP4_SETUP_H
#define DLMS_IP4_SETUP_H

#ifndef DLMS_IGNORE_IP4_SETUP
#include "DLMSObject.h"
#include "DLMSIp4SetupIpOption.h"

class DLMSIp4Setup : public DLMSObject
{
    std::string m_DataLinkLayerReference;
    unsigned int m_IPAddress;
    std::vector<unsigned int> m_MulticastIPAddress;
    std::vector<DLMSIp4SetupIpOption> m_IPOptions;
    unsigned int m_SubnetMask;
    unsigned int m_GatewayIPAddress;
    bool m_UseDHCP;
    unsigned int m_PrimaryDNSAddress;
    unsigned int m_SecondaryDNSAddress;

    DLMSVariant m_Value;
public:
    //Constructor.
    DLMSIp4Setup();

    //SN Constructor.
    DLMSIp4Setup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSIp4Setup(std::string ln);

    std::string& GetDataLinkLayerReference();
    void SetDataLinkLayerReference(std::string value);

    unsigned int GetIPAddress();
    void SetIPAddress(unsigned int value);

    void GetIPAddress(std::string& value);
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    void SetIPAddress(std::string& value);
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

    std::vector<unsigned int>& GetMulticastIPAddress();

    std::vector<DLMSIp4SetupIpOption>& GetIPOptions();

    void SetIPOptions(std::vector<DLMSIp4SetupIpOption>& value);

    unsigned int GetSubnetMask();
    void SetSubnetMask(unsigned int value);
    void GetSubnetMask(std::string& value);
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    void SetSubnetMask(std::string& value);
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

    unsigned int GetGatewayIPAddress();
    void SetGatewayIPAddress(unsigned int value);
    void GetGatewayIPAddress(std::string& value);
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    void SetGatewayIPAddress(std::string& value);
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

    bool GetUseDHCP();
    void SetUseDHCP(bool value);

    unsigned int GetPrimaryDNSAddress();
    void SetPrimaryDNSAddress(unsigned int value);
    void GetPrimaryDNSAddress(std::string& value);
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    void SetPrimaryDNSAddress(std::string& value);
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

    unsigned int GetSecondaryDNSAddress();
    void SetSecondaryDNSAddress(unsigned int value);
    void GetSecondaryDNSAddress(std::string& value);
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    void SetSecondaryDNSAddress(std::string& value);
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

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
