#ifndef DLMS_COMMUNICATION_PORT_PROTECTION_H
#define DLMS_COMMUNICATION_PORT_PROTECTION_H

#include "DLMSObject.h"
#ifndef DLMS_IGNORE_COMMUNICATION_PORT_PROTECTION

class DLMSCommunicationPortProtection : public DLMSObject
{
    /**
     * Controls the protection mode.
     */
    DLMS_PROTECTION_MODE m_ProtectionMode;
    /**
     * Number of allowed failed communication attempts before port is disabled.
     */
    uint16_t m_AllowedFailedAttempts;
    /**
     * The lockout time.
     */
    uint32_t m_InitialLockoutTime;
    /**
     * Holds a factor that controls how the lockout time is increased with each
     * failed attempt.
     */
    unsigned char m_SteepnessFactor;
    /**
     * The lockout time.
     */
    uint32_t m_MaxLockoutTime;
    /**
     * The communication port being protected
     */
    DLMSObject* m_Port;
    /**
     * Current protection status.
     */
    DLMS_PROTECTION_STATUS m_ProtectionStatus;
    /**
     * Failed attempts.
     */
    uint32_t m_FailedAttempts;
    /**
     * Total failed attempts.
     */
    uint32_t m_CumulativeFailedAttempts;
public:
    //Constructor.
    DLMSCommunicationPortProtection();

    //SN Constructor.
    DLMSCommunicationPortProtection(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSCommunicationPortProtection(std::string ln);

    /**
    * Returns Controls the protection mode.
    */
    DLMS_PROTECTION_MODE GetProtectionMode();

    /**
     * value: Controls the protection mode.
     */
    void SetProtectionMode(DLMS_PROTECTION_MODE value);

    /**
     * Returns Number of allowed failed communication attempts before port is
     *         disabled.
     */
    uint16_t GetAllowedFailedAttempts();

    /**
     * value: Number of allowed failed communication attempts before port is disabled.
     */
    void SetAllowedFailedAttempts(uint16_t value);

    /**
     * Returns The lockout time.
     */
    uint32_t GetInitialLockoutTime();

    /**
     * value: The lockout time.
     */
    void SetInitialLockoutTime(uint32_t value);

    /**
     * Returns Holds a factor that controls how the lockout time is increased
     *         with each failed attempt.
     */
    unsigned char GetSteepnessFactor();

    /**
     * value: Holds a factor that controls how the lockout time is increased
     *            with each failed attempt.
     */
    void SetSteepnessFactor(unsigned char value);

    /**
     * Returns The lockout time.
     */
    uint32_t GetMaxLockoutTime();

    /**
     * value: The lockout time.
     */
    void SetMaxLockoutTime(uint32_t value);

    /**
     * Returns The communication port being protected
     */
    DLMSObject* GetPort();

    /**
     * value: The communication port being protected
     */
    void SetPort(DLMSObject* value);

    /**
     * Returns Current protection status.
     */
    DLMS_PROTECTION_STATUS GetProtectionStatus();

    /**
     * value: Current protection status.
     */
    void SetProtectionStatus(DLMS_PROTECTION_STATUS value);

    /**
     * Returns Failed attempts.
     */
    uint32_t GetFailedAttempts();

    /**
     * value: Failed attempts.
     */
    void SetFailedAttempts(uint32_t value);

    /**
     * Returns Total failed attempts.
     */
    uint32_t GetCumulativeFailedAttempts();

    /**
     * value: Total failed attempts.
     */
    void SetCumulativeFailedAttempts(uint32_t value);

    /*
     * Resets FailedAttempts and current lockout time to zero. Protection status
     * is set to unlocked.
     *
     * client: DLMS client.
     * Returns Action bytes.
     */
    int Reset(
        DLMSClient* client,
        std::vector<ByteBuffer>& reply);

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
