#ifndef DLMS_NTP_SETUP_H
#define DLMS_NTP_SETUP_H

#include "Ignore.h"
#include "DLMSObject.h"
#ifndef DLMS_IGNORE_NTP_SETUP

class DLMSNtpSetup : public DLMSObject
{
    /**
    * Is NTP time synchronisation active.
    */
    bool m_Activated;
    /**
     * NTP server address.
     */
    std::string m_ServerAddress;
    /**
     * UDP port related to this protocol.
     */
    uint16_t m_Port;
    /**
     * Authentication method.
     */
    DLMS_NTP_AUTHENTICATION_METHOD m_Authentication;
    /**
     * Symmetric keys for authentication.
     */
    std::map<uint32_t, ByteBuffer> m_Keys;
    /**
     * Client key (NTP server public key).
     */
    ByteBuffer m_ClientKey;

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

public:
    //Constructor.
    DLMSNtpSetup();

    //SN Constructor.
    DLMSNtpSetup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSNtpSetup(std::string ln);

    /**
     * @return Is NTP time synchronisation active.
     */
    bool GetActivated();

    /**
     * @param value
     *            Is NTP time synchronisation active.
     */
    void SetActivated(bool value);

    /**
     * @return NTP server address.
     */
    std::string& GetServerAddress();

    /**
     * @param value
     *            NTP server address.
     */
    void SetServerAddress(std::string& value);

    /**
     * @return UDP port related to this protocol.
     */
    uint16_t GetPort();

    /**
     * @param value
     *            UDP port related to this protocol.
     */
    void SetPort(uint16_t value);

    /**
     * @return Authentication method.
     */
    DLMS_NTP_AUTHENTICATION_METHOD GetAuthentication();

    /**
     * @param value
     *            Authentication method.
     */
    void SetAuthentication(DLMS_NTP_AUTHENTICATION_METHOD value);

    /**
     * @return Symmetric keys for authentication.
     */
    std::map<uint32_t, ByteBuffer>& GetKeys();

    /**
     * @return Client key (NTP server public key).
     */
    ByteBuffer& GetClientKey();

    /**
     * @param value
     *            Client key (NTP server public key).
     */
    void SetClientKey(ByteBuffer& value);

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

    //Synchronizes the time of the DLMS server with the NTP server.
    int Synchronize(DLMSClient* client, std::vector<ByteBuffer>& reply);

    int AddAuthenticationKey(DLMSClient* client, uint32_t id, ByteBuffer& key, std::vector<ByteBuffer>& reply);

    //Remove symmetric authentication key.
    int DeleteAuthenticationKey(DLMSClient* client, uint32_t id, std::vector<ByteBuffer>& reply);

};
#endif
#endif
