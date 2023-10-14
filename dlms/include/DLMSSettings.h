#ifndef DLMS_SETTINGS_H
#define DLMS_SETTINGS_H

#include "Enums.h"
#include "ByteBuffer.h"
#include "DLMSLimits.h"
#include "PlcSettings.h"
#include "DLMSObjectCollection.h"
#include "Cipher.h"

// Server sender frame sequence starting number.
const unsigned char SERVER_START_SENDER_FRAME_SEQUENCE = 0x1E;

// Server receiver frame sequence starting number.
const unsigned char SERVER_START_RECEIVER_FRAME_SEQUENCE = 0xFE;

// Client sender frame sequence starting number.
const unsigned char CLIENT_START_SENDER_FRAME_SEQUENCE = 0xFE;

// Client receiver frame sequence starting number.
const unsigned char CLIENT_START_RCEIVER_FRAME_SEQUENCE = 0xE;
// DLMS version number.
const unsigned char DLMS_VERSION = 6;

const unsigned short MAX_RECEIVE_PDU_SIZE = 0xFFFF;

// This class includes DLMS communication Settings.
class DLMSSettings
{
    // List of server or client objects that are dynamically allocated.
    DLMSObjectCollection m_AllocatedObjects;
    //Is connection made for the server.
    DLMS_CONNECTION_STATE m_Connected;

    // Is custom challenges used. If custom challenge is used new challenge is
    // not generated if it is Set. This is for debugging purposes.
    bool m_UseCustomChallenge;

    // Client to server challenge.
    ByteBuffer m_CtoSChallenge;

    // Server to Client challenge.
    ByteBuffer m_StoCChallenge;

    ByteBuffer m_SourceSystemTitle;

    // Pre-established system title.
    ByteBuffer m_PreEstablishedSystemTitle;

    // Invoke ID.
    unsigned char m_InvokeID;
    bool m_AutoIncreaseInvokeID;

    //Long Invoke ID.
    int m_LongInvokeID;

    // Priority.
    DLMS_PRIORITY m_Priority;

    // Service class.
    DLMS_SERVICE_CLASS m_ServiceClass;

    // Client address.
    unsigned long m_ClientAddress;
    // Client address.
    unsigned long m_PushClientAddress;

    // Server address.
    unsigned long m_ServerAddress;

    // Is Logical Name referencing used.
    bool m_UseLogicalNameReferencing;

    // Interface type.
    DLMS_INTERFACE_TYPE m_InterfaceType;

    // User authentication.
    DLMS_AUTHENTICATION m_Authentication;

    // User password.
    ByteBuffer m_Password;

    /**
     * Key Encrypting Key, also known as Master key.
     */
    ByteBuffer m_Kek;

    /**
     * Long data count.
     */
    unsigned short m_Count;

    /**
     * Long data index.
     */
    unsigned short m_Index;

    // DLMS version number.
    unsigned char m_DlmsVersionNumber;

    // Maximum client PDU size.
    unsigned short m_MaxReceivePDUSize;

    // Maximum server PDU size.
    unsigned short m_MaxServerPDUSize;

    // HDLC sender frame sequence number.
    unsigned char m_SenderFrame;

    // HDLC receiver block sequence number.
    unsigned char m_ReceiverFrame;

    // Is this server or client.
    bool m_Server;

    // HDLC connection settings.
    DLMSLimits m_HdlcSettings;
    // PLC connection settings.
    PlcSettings m_PlcSettings;

    // Block packet index.
    unsigned long m_BlockIndex;

    // List of server or client objects.
    DLMSObjectCollection m_Objects;

    /**
     * Cipher interface that is used to cipher PDU.
     */
    Cipher* m_Cipher;

    /*
   * Block number acknowledged in GBT.
   */
    int m_BlockNumberAck;

    /*
     * GBT window size.
     */
    unsigned char m_GbtWindowSize;

    /**
    * Proposed conformance block. Client asks this funtionality.
    */
    DLMS_CONFORMANCE m_ProposedConformance;

    /**
    * Server tells what functionality is available and client will know it.
    */
    DLMS_CONFORMANCE m_NegotiatedConformance;

    /**
    *  Protocol version.
    */
    char* m_ProtocolVersion;

    unsigned char m_UserId;

    unsigned char m_QualityOfService;

    /////////////////////////////////////////////////////////////////////////
    // DLMS Standard says that Time zone is from normal time to UTC in minutes.
    // If meter is configured to use UTC time (UTC to normal time) set this to true.
    bool m_UseUtc2NormalTime;

    /////////////////////////////////////////////////////////////////////////
    // Expected Invocation(Frame) counter value.
    // Expected Invocation counter is not check if value is zero.
    uint64_t m_ExpectedInvocationCounter;

    /////////////////////////////////////////////////////////////////////////
    // Expected security policy.
    // If Expected security policy is set client can't connect with other security policies.
    unsigned char m_ExpectedSecurityPolicy;

    /////////////////////////////////////////////////////////////////////////
    // Expected security suite.
    // If Expected security suite is set client can't connect with other security suite.
    unsigned char m_ExpectedSecuritySuite;

    DATETIME_SKIPS m_DateTimeSkips;

    /**
     * Last executed command.
     */
    unsigned char m_Command;

    /**
     * Last executed command type
     */
    unsigned char m_CommandType;
public:
    // Constructor.
    DLMSSettings(bool isServer);

    //Destructor.
    ~DLMSSettings();

    // Client to Server challenge.
    ByteBuffer& GetCtoSChallenge();

    // Client to Server challenge.
    void SetCtoSChallenge(ByteBuffer& value);

    // Get server to Client challenge.
    ByteBuffer& GetStoCChallenge();

    // Set server to Client challenge.
    void SetStoCChallenge(ByteBuffer& value);

    // Gets used authentication.
    DLMS_AUTHENTICATION GetAuthentication();

    //Sets Used authentication.
    void SetAuthentication(DLMS_AUTHENTICATION value);

    //Gets password.
    ByteBuffer& GetPassword();

    // Sets password.
    void SetPassword(ByteBuffer& value);

    // Used DLMS version number.
    unsigned char GetDlmsVersionNumber();

    // Used DLMS version number.
    void SetDlmsVersionNumber(unsigned char value);

    // Reset frame sequence.
    void ResetFrameSequence();

    bool CheckFrame(unsigned char frame);

    // Generates I-frame.
    unsigned char GetNextSend(unsigned char first);

    // Generates Receiver Ready S-frame.
    unsigned char GetReceiverReady();

    // Generates Keep Alive S-frame.
    unsigned char GetKeepAlive();

    // Gets current block index.
    unsigned long GetBlockIndex();

    // Sets current block index.
    void SetBlockIndex(unsigned long value);

    // Resets block index to default value.
    void ResetBlockIndex();

    // Increases block index.
    void IncreaseBlockIndex();

    //Is acting as server or client.
    bool IsServer();

    //HDLC connection settings.
    DLMSLimits& GetHdlcSettings();

    //PLC connection settings.
    PlcSettings& GetPlcSettings();

    // Used interface.
    DLMS_INTERFACE_TYPE GetInterfaceType();

    // Used interface.
    void SetInterfaceType(DLMS_INTERFACE_TYPE value);

    // Gets client address.
    unsigned long GetClientAddress();

    // Sets client address.
    void SetClientAddress(unsigned long value);

    // Gets push client address.
    unsigned long GetPushClientAddress();

    // Sets push client address.
    void SetPushClientAddress(unsigned long value);

    // Server address.
    unsigned long GetServerAddress();

    // Server address.
    void SetServerAddress(unsigned long value);

    // DLMS version number.
    unsigned char GetDLMSVersion();

    // DLMS version number.
    void SetDLMSVersion(unsigned char value);

    // Maximum PDU size.
    unsigned short GetMaxPduSize();

    // Maximum PDU size.
    int SetMaxReceivePDUSize(unsigned short value);

    // Maximum server PDU size.
    unsigned short GetMaxServerPDUSize();

    // Maximum server PDU size.
    int SetMaxServerPDUSize(unsigned short value);

    // Is Logical Name Referencing used.
    bool GetUseLogicalNameReferencing();

    // Is Logical Name Referencing used.
    void SetUseLogicalNameReferencing(bool value);

    // Used priority.
    DLMS_PRIORITY GetPriority();

    // Used priority.
    void SetPriority(DLMS_PRIORITY value);

    // Used service class.
    DLMS_SERVICE_CLASS GetServiceClass();

    // Used service class.
    void SetServiceClass(DLMS_SERVICE_CLASS value);

    // Invoke ID.
    unsigned char GetInvokeID();

    // Invoke ID.
    void SetInvokeID(unsigned char value);


    // Update invoke ID.
    void UpdateInvokeId(unsigned char value);

    /**
       * @return Invoke ID.
       */
    unsigned long GetLongInvokeID();

    /**
     * @param value
     *            Invoke ID.
     */
    int SetLongInvokeID(unsigned long value);

    // Collection of the objects.
    DLMSObjectCollection& GetObjects();

    // Get Is custom challenges used.
    bool GetUseCustomChallenge();

    // Set is custom challenges used.
    void SetUseCustomChallenge(bool value);

    //Get is connection made for the server.
    DLMS_CONNECTION_STATE GetConnected();

    //Set is connection made for the server.
    void SetConnected(DLMS_CONNECTION_STATE value);

    /**
    * Cipher interface that is used to cipher PDU.
    */
    Cipher* GetCipher();

    /**
    * Cipher interface that is used to cipher PDU.
    */
    void SetCipher(Cipher* value);

    /**
     * @return Source system title.
     */
    ByteBuffer& GetSourceSystemTitle();

    /**
     * @param value
     *            Source system title.
     */
    int SetSourceSystemTitle(ByteBuffer& value);

    /**
     * @return Pre-established system title.
     */
    ByteBuffer& GetPreEstablishedSystemTitle();

    /**
     * value: Pre-established system title..
     */
    int SetPreEstablishedSystemTitle(ByteBuffer& value);

    /**
     * @return Key Encrypting Key, also known as Master key.
     */
    ByteBuffer& GetKek();

    /**
     * @param value
     *            Key Encrypting Key, also known as Master key.
     */
    void SetKek(ByteBuffer& value);

    /**
     * @return Long data count.
     */
    unsigned short GetCount();

    /**
     * @param count
     *            Long data count.
     */
    void SetCount(unsigned short value);

    /**
     * @return Long data index.
     */
    unsigned short GetIndex();

    /**
     * @param index
     *            Long data index
     */
    void SetIndex(unsigned short value);

    /**
    * Server will tell what functionality is available. Client will know what functionality server offers.
    * @return Available functionality.
    */
    DLMS_CONFORMANCE GetNegotiatedConformance();

    /**
    * Server will tell what functionality is available. Client will know what functionality server offers from this value.
    *
    * @param value
    *            Available functionality.
    */
    void SetNegotiatedConformance(DLMS_CONFORMANCE value);

    /**
    * Client proposes this functionality. Server checks what it can offer from Conformance and updates it to proposed conformance.
    * @return Functionality.
    */
    DLMS_CONFORMANCE GetProposedConformance();

    /**
    * Client proposes this functionality. Server checks what it can offer from Conformance and updates it to proposed conformance.
    *
    * @param value
    *            Functionality.
    */
    void SetProposedConformance(DLMS_CONFORMANCE value);

    /////////////////////////////////////////////////////////////////////////
    // Protocol version.
    char* GetProtocolVersion();
    void SetProtocolVersion(const char* value);

    /////////////////////////////////////////////////////////////////////////
    // DLMS Standard says that Time zone is from normal time to UTC in minutes.
    // If meter is configured to use UTC time (UTC to normal time) set this to true.
    bool GetUseUtc2NormalTime();
    void SetUseUtc2NormalTime(bool value);

    /////////////////////////////////////////////////////////////////////////
    // Expected Invocation(Frame) counter value.
    // Expected Invocation counter is not check if value is zero.
    uint64_t GetExpectedInvocationCounter();
    void SetExpectedInvocationCounter(uint64_t uint32_t);

    /////////////////////////////////////////////////////////////////////////
    // Expected security policy.
    // If Expected security policy is set client can't connect with other security policies.
    unsigned char GetExpectedSecurityPolicy();
    void SetExpectedSecurityPolicy(unsigned char value);

    /////////////////////////////////////////////////////////////////////////
    // Expected security suite.
    // If Expected security suite is set client can't connect with other security suite.
    //Default value is -1.
    void SetExpectedSecuritySuite(unsigned char value);
    unsigned char GetExpectedSecuritySuite();

    /////////////////////////////////////////////////////////////////////////
    // Skip selected date time fields.
    DATETIME_SKIPS GetDateTimeSkips();
    void SetDateTimeSkips(DATETIME_SKIPS value);

    // User ID.
    unsigned char GetUserID();

    // User ID.
    void SetUserID(unsigned char value);

    // Quality of service.
    unsigned char GetQualityOfService();

    // Quality of service.
    void SetQualityOfService(unsigned char value);

    //Auto increase Invoke ID.
    bool GetAutoIncreaseInvokeID();

    //Auto increase Invoke ID.
    void SetAutoIncreaseInvokeID(bool value);

    /*
    * Block number acknowledged in GBT.
    */
    int GetBlockNumberAck();

    /**
     * Block number acknowledged in GBT.
     */
    void SetBlockNumberAck(int value);

    /*
    * GBT window size.
    */
    unsigned char GetGbtWindowSize();

    /*
     * GBT window size.
    */
    void SetGbtWindowSize(unsigned char value);

    /**
     * the command
     */
    unsigned char GetCommand();

    /**
     * the command to set
     */
    void SetCommand(unsigned char value);

    /**
     * Command Type
     */
    unsigned char GetCommandType();

    /**
     * Command Type
     */
    void SetCommandType(unsigned char value);

    //Add dynamically allocated objects here so they are released.
    //This is done if object is not in the association view, but might be ex. capture objects.
    void AddAllocateObject(DLMSObject* pObj);
};

#endif
