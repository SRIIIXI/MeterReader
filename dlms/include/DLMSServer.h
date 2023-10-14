#ifndef DLMS_SERVER_H
#define DLMS_SERVER_H

#include <vector>
#include "DLMSLongTransaction.h"
#include "ReplyData.h"
#include "DLMSSettings.h"
#include "SNInfo.h"
#include "DLMSSNParameters.h"
#include "DLMSLNParameters.h"
#include "DLMSConnectionEventArgs.h"
#include "DLMSHdlcSetup.h"
#include "DLMSTcpUdpSetup.h"
#include "DLMSAssociationLogicalName.h"
#include "DLMSAssociationShortName.h"
//#include "DLMSPushSetup.h"

class DLMSProfileGeneric;
class ServerReply;

class DLMSServer
{
    friend class DLMSProfileGeneric;
    friend class DLMSValueEventArg;
    friend class DLMSAssociationLogicalName;
    friend class DLMSAssociationShortName;
    friend class DLMSLNCommandHandler;
    friend class DLMSSNCommandHandler;
private:
    long m_DataReceived;
#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
    DLMSIecHdlcSetup* m_Hdlc;
#endif //DLMS_IGNORE_IEC_HDLC_SETUP
#ifndef DLMS_IGNORE_TCP_UDP_SETUP
    DLMSTcpUdpSetup* m_Wrapper;
#endif //DLMS_IGNORE_TCP_UDP_SETUP
    ReplyData m_Info;
    /**
     * Received data.
     */
    ByteBuffer m_ReceivedData;

    /**
     * Reply data.
     */
    ByteBuffer m_ReplyData;

    /**
     * Long get or read transaction information.
     */
    DLMSLongTransaction* m_Transaction;

    /**
     * Is server initialized.
     */
    bool m_Initialized;

    /**
    * Parse SNRM Request. If server do not accept client empty byte array is
    * returned.
    *
    * @return Returns returned UA packet.
    */
    int HandleSnrmRequest(DLMSSettings& settings, ByteBuffer& data, ByteBuffer& reply);

    /**
    * Reset settings when connection is made or close.
    *
    * @param connected
    *            Is co3nnected.
    */
    void Reset(bool connected);

    int ReportError(
        DLMS_COMMAND command,
        DLMS_ERROR_CODE error,
        ByteBuffer& reply);

    /**
    * Generate confirmed service error.
    *
    * @param service
    *            Confirmed service error.
    * @param type
    *            Service error.
    * @param code
    *            code
    * @return
    */
    void GenerateConfirmedServiceError(
        DLMS_CONFIRMED_SERVICE_ERROR service,
        DLMS_SERVICE_ERROR type,
        unsigned char code, ByteBuffer& data);

    /**
    * Handle received command.
    */
    int HandleCommand(
        DLMS_COMMAND cmd,
        ByteBuffer& data,
        ServerReply& sr,
        unsigned char cipheredCommand);

    /**
    * Parse AARQ request that client send and returns AARE request.
    *
    * @return Reply to the client.
    */
    int HandleAarqRequest(
        ByteBuffer& data,
        DLMSConnectionEventArgs& connectionInfo);

    /**
    * Count how many rows can fit to one PDU.
    *
    * @param pg
    *            Read profile generic.
    * @return Rows to fit one PDU.
    */
    unsigned short GetRowsToPdu(
        DLMSProfileGeneric* pg);

    /**
    * Update short names.
    *
    * @param force
    *            Force update.
    */
    int UpdateShortNames(bool force);

    /**
    * Handles release request.
    *
    * @param data
    *            Received data.
    * @param connectionInfo
    *            Connection info.
    */
    int HandleReleaseRequest(
        ByteBuffer& data);

    int AddData(
        DLMSObject* obj,
        unsigned char index,
        ByteBuffer& buff);

    /**
    * Handles GBT.
    *
    * @param data
    *            Received data.
    * @param connectionInfo
    *            Connection info.
    */
    int HandleGeneralBlockTransfer(
        ServerReply& sr,
        ByteBuffer& data,
        unsigned char cipheredCommand);

protected:
    /**
     * Server Settings.
     */
    DLMSSettings m_Settings;

    /**
     * @param value
     *            Cipher interface that is used to cipher PDU.
     */
    void SetCipher(Cipher* value);

    /**
    * @return Get settings.
    */
    DLMSSettings& GetSettings();

    /**
        * Check is data sent to this server.
        *
        * @param serverAddress
        *            Server address.
        * @param clientAddress
        *            Client address.
        * @return True, if data is sent to this server.
        */
    virtual bool IsTarget(
        unsigned long int serverAddress,
        unsigned long clientAddress) = 0;

    /**
     * Check whether the authentication and password are correct.
     *
     * @param authentication
     *            Authentication level.
     * @param password
     *            Password.
     * @return Source diagnostic.
     */
    virtual DLMS_SOURCE_DIAGNOSTIC ValidateAuthentication(
        DLMS_AUTHENTICATION authentication,
        ByteBuffer& password) = 0;

    /**
     * Find object.
     *
     * @param objectType
     *            Object type.
     * @param sn
     *            Short Name. In Logical name referencing this is not used.
     * @param ln
     *            Logical Name. In Short Name referencing this is not used.
     * @return Found object or NULL if object is not found.
     */
    virtual DLMSObject* FindObject(
        DLMS_OBJECT_TYPE objectType,
        int sn,
        std::string& ln) = 0;

    /**
     * Read selected item(s).
     *
     * @param args
     *            Handled read requests.
     */
    virtual void PreRead(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
     * Write selected item(s).
     *
     * @param args
     *            Handled write requests.
     */
    virtual void PreWrite(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
     * Accepted connection is made for the server. All initialization is done
     * here.
     */
    virtual void Connected(DLMSConnectionEventArgs& connectionInfo) = 0;

    /**
     * Client has try to made invalid connection. Password is incorrect.
     *
     * @param connectionInfo
     *            Connection information.
     */
    virtual void InvalidConnection(DLMSConnectionEventArgs& connectionInfo) = 0;

    /**
     * Server has close the connection. All clean up is made here.
     */
    virtual void Disconnected(DLMSConnectionEventArgs& connectionInfo) = 0;

    /**
    * Get attribute access mode.
    *
    * @param arg
    *            Value event argument.
    * @return Access mode.
    * @throws Exception
    *             Server handler occurred exceptions.
    */
    virtual DLMS_ACCESS_MODE GetAttributeAccess(DLMSValueEventArg* arg) = 0;

    /**
    * Get method access mode.
    *
    * @param arg
    *            Value event argument.
    * @return Method access mode.
    * @throws Exception
    *             Server handler occurred exceptions.
    */
    virtual DLMS_METHOD_ACCESS_MODE GetMethodAccess(DLMSValueEventArg* arg) = 0;

    /**
     * Action is occurred.
     *
     * @param args
     *            Handled action requests.
     */
    virtual void PreAction(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
    * Read selected item(s).
    *
    * @param args
    *            Handled read requests.
    */
    virtual void PostRead(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
    * Write selected item(s).
    *
    * @param args
    *            Handled write requests.
    */
    virtual void PostWrite(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
    * Action is occurred.
    *
    * @param args
    *            Handled action requests.
    */
    virtual void PostAction(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
    * Get selected value(s). This is called when example profile generic
    * request current value.
    *
    * @param args
    *            Value event arguments.
    */
    virtual void PreGet(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
    * Get selected value(s). This is called when example profile generic
    * request current value.
    *
    * @param args
    *            Value event arguments.
    */
    virtual void PostGet(
        std::vector<DLMSValueEventArg*>& args) = 0;

    /**
    * Update short names.
    */
    int UpdateShortNames();

    int HandleRequest(ServerReply& sr);
public:
#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
    /**
    * @return HDLC settings.
    */
    DLMSIecHdlcSetup* GetHdlc();

    /**
    * @param value
    *            HDLC settings.
    */
    void SetHdlc(DLMSIecHdlcSetup* value);
#endif //DLMS_IGNORE_IEC_HDLC_SETUP
#ifndef DLMS_IGNORE_TCP_UDP_SETUP
    /**
    * @return Wrapper settings.
    */
    DLMSTcpUdpSetup* GetWrapper();

    /**
    * @param value
    *            Wrapper settings.
    */
    void SetWrapper(DLMSTcpUdpSetup* value);
#endif //DLMS_IGNORE_TCP_UDP_SETUP

    /////////////////////////////////////////////////////////////////////////////
    // Standard says that Time zone is from normal time to UTC in minutes.
    // If meter is configured to use UTC time (UTC to normal time) set this to true.
    bool GetUseUtc2NormalTime();
    void SetUseUtc2NormalTime(bool value);

    /////////////////////////////////////////////////////////////////////////
    // Client Invocation (Frame) counter value.
    // Client  Invocation counter is not check if value is zero.
    uint64_t GetExpectedInvocationCounter();
    void SetExpectedInvocationCounter(uint64_t value);

    /////////////////////////////////////////////////////////////////////////
    // Expected security policy.
    // If Expected security policy is set client can't connect with other security policies.
    unsigned char GetExpectedSecurityPolicy();
    void SetExpectedSecurityPolicy(unsigned char value);

    /////////////////////////////////////////////////////////////////////////
    // Expected security suite.
    // If Expected security suite is set client can't connect with other security suite.
    //Default value is 0xFF;
    unsigned char GetExpectedSecuritySuite();
    void SetExpectedSecuritySuite(unsigned char value);

    /////////////////////////////////////////////////////////////////////////
    // Skip selected date time fields.
    DATETIME_SKIPS GetDateTimeSkips();
    void SetDateTimeSkips(DATETIME_SKIPS value);


    /**
     * @return Server to client challenge.
     */
    ByteBuffer& GetStoCChallenge();

    /**
     * Server to Client custom challenge. This is for debugging purposes. Reset
     * custom challenge settings StoCChallenge to NULL.
     *
     * @param value
     *            Server to Client challenge.
     */
    void SetStoCChallenge(
        ByteBuffer& value);

    /**
     * @return Interface type.
     */
    DLMS_INTERFACE_TYPE GetInterfaceType();

    /**
     * Set starting packet index. Default is One based, but some meters use Zero
     * based value. Usually this is not used.
     *
     * @param value
     *            Zero based starting index.
     */
    void SetStartingPacketIndex(int value);

    /**
     * @return Invoke ID.
     */
    int GetInvokeID();

    /**
     * Constructor.
     *
     * @param logicalNameReferencing
     *            Is logical name referencing used.
     * @param type
     *            Interface type.
     */
    DLMSServer(
        bool logicalNameReferencing,
        DLMS_INTERFACE_TYPE type);

#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param hdlc
    *            HDLC settings.
    */
    DLMSServer(
        DLMSAssociationLogicalName* ln, DLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    DLMSServer(
        DLMSAssociationLogicalName* ln, DLMSTcpUdpSetup* wrapper);

#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME

#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param hdlc
    *            HDLC settings.
    */
    DLMSServer(
        DLMSAssociationShortName* sn, DLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    DLMSServer(
        DLMSAssociationShortName* sn, DLMSTcpUdpSetup* wrapper);

#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME

    /**
    * Destructor.
    */
    ~DLMSServer();

    //Server is using push client address when sending push messages. Client address is used if PushAddress is zero.
    unsigned long GetPushClientAddress();
    void SetPushClientAddress(unsigned long value);


    /**
     * @return List of objects that meter supports.
     */
    DLMSObjectCollection& GetItems();

    //HDLC connection settings. GetLimits is obsolete. Use GetHdlcSettings instead.
    DLMSLimits& GetLimits();
    //HDLC connection settings.
    HdlcSettings& GetHdlcSettings();

    /**
     * Retrieves the maximum size of received PDU. PDU size tells maximum size
     * of PDU packet. Value can be from 0 to 0xFFFF. By default the value is
     * 0xFFFF.
     *
     * @return Maximum size of received PDU.
     */
    unsigned short GetMaxReceivePDUSize();

    /**
     * @param value
     *            Maximum size of received PDU.
     */
    void SetMaxReceivePDUSize(
        unsigned short value);

    /**
     * Determines, whether Logical, or Short name, referencing is used.
     * Referencing depends on the device to communicate with. Normally, a device
     * supports only either Logical or Short name referencing. The referencing
     * is defined by the device manufacturer. If the referencing is wrong, the
     * SNMR message will fail.
     *
     * @see #getMaxReceivePDUSize
     * @return Is logical name referencing used.
     */
    bool GetUseLogicalNameReferencing();

    /**
     * @param value
     *            Is Logical Name referencing used.
     */
    void SetUseLogicalNameReferencing(
        bool value);

    /**
     * Initialize server. This must call after server objects are set.
     */
    int Initialize();

    /**
     * Reset after connection is closed.
     */
    void Reset();

    /**
     * Handles client request.
     *
     * @param data
     *            Received data from the client.
     * @return Response to the request. Response is NULL if request packet is
     *         not complete.
     */
    int HandleRequest(
        ByteBuffer& data,
        ByteBuffer& reply);

    /**
    * Handles client request.
    *
    * @param data
    *            Received data from the client.
    * @return Response to the request. Response is NULL if request packet is
    *         not complete.
    */
    int HandleRequest(
        DLMSConnectionEventArgs& connectionInfo,
        ByteBuffer& data,
        ByteBuffer& reply);

    /**
     * Handles client request.
     *
     * @param data
     *            Received data from the client.
     * @return Response to the request. Response is NULL if request packet is
     *         not complete.
     */
    int HandleRequest(
        unsigned char* data,
        unsigned short size,
        ByteBuffer& reply);

    /**
     * Handles client request.
     *
     * @param data
     *            Received data from the client.
     * @return Response to the request. Response is NULL if request packet is
     *         not complete.
     */
    int HandleRequest(
        DLMSConnectionEventArgs& connectionInfo,
        unsigned char* data,
        unsigned short size,
        ByteBuffer& reply);


    /**
    * Server will tell what functionality is available for the client.
    * @return Available functionality.
    */
    DLMS_CONFORMANCE GetConformance();

    /**
    * Server will tell what functionality is available for the client.
    *
    * @param value
    *            Available functionality.
    */
    void SetConformance(DLMS_CONFORMANCE value);

    int GenerateDataNotificationMessages(
        struct tm* time,
        ByteBuffer& data,
        std::vector<ByteBuffer>& reply);

    int GenerateDataNotificationMessages(
        struct tm* date,
        std::vector<std::pair<DLMSObject*, unsigned char> >& objects,
        std::vector<ByteBuffer>& reply);

#ifndef DLMS_IGNORE_PUSH_SETUP
    int GeneratePushSetupMessages(
        struct tm* date,
        DLMSPushSetup* push,
        std::vector<ByteBuffer>& reply);
#endif
};
#endif
