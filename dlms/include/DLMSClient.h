#ifndef DLMS_CLIENT_H
#define DLMS_CLIENT_H

#include "DLMSProfileGeneric.h"
#include "DateTime.h"
#include "DLMSAccessItem.h"
#include "DLMSConverter.h"
#include "DLMSSettings.h"
#include "ReplyData.h"

class DLMSClient
{
protected:
    friend class DLMSSchedule;
    DLMSSettings m_Settings;
    char m_ManufacturerId[3];
    ByteBuffer m_CustomerPIN;
private:
    short m_ChallengeSize;
    // If protected release is used release is including a ciphered xDLMS Initiate request.
    bool m_UseProtectedRelease;
    bool m_IsAuthenticationRequired;
    // SN referencing
    int ParseSNObjects(
        ByteBuffer& buff,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    //Parse SN object item.
    int ParseSNObjectItem(DLMSVariant& value, bool ignoreInactiveObjects);
    //Parse LN object item.
    int ParseLNObjectItem(DLMSVariant& value, bool ignoreInactiveObjects);

    /**
    * Parse LN objects.
    *
    * buff
    *            Byte stream where objects are parsed.
    * onlyKnownObjects
    *            Only known objects are parsed.
    * @return Collection of COSEM objects.
    */
    int ParseLNObjects(
        ByteBuffer& buff,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    /**
    * Generates a read message.
    *
    * name : Short or Logical Name.
    * objectType : COSEM object type.
    * attributeOrdinal : Attribute index of the object.
    * data : Read data parameter.
    * reply : Generated read message(s).
    * Returns error status.
    */
    int Read(
        DLMSVariant& name,
        DLMS_OBJECT_TYPE objectType,
        int attributeOrdinal,
        ByteBuffer* data,
        std::vector<ByteBuffer>& reply);

public:
    /////////////////////////////////////////////////////////////////////////////
    //Constructor
    /////////////////////////////////////////////////////////////////////////////
    DLMSClient(
        bool UseLogicalNameReferencing = true,
        int ClientAddress = 16,
        int ServerAddress = 1,
        //Authentication type.
        DLMS_AUTHENTICATION authentication = DLMS_AUTHENTICATION_NONE,
        //Password if authentication is used.
        const char* password = NULL,
        DLMS_INTERFACE_TYPE intefaceType = DLMS_INTERFACE_TYPE_HDLC,
        const char* systemtitle = NULL,
        const char* customerpin = NULL);

    /////////////////////////////////////////////////////////////////////////////
    //Destructor.
    /////////////////////////////////////////////////////////////////////////////
    ~DLMSClient();

    bool GetUseLogicalNameReferencing();

    DLMS_INTERFACE_TYPE GetInterfaceType();

    /**
     * @return Used Priority.
     */
    DLMS_PRIORITY GetPriority();

    /**
     * @param value
     *            Used Priority.
     */
    void SetPriority(DLMS_PRIORITY value);

    /**
     * @return Used service class.
     */
    DLMS_SERVICE_CLASS GetServiceClass();

    /**
     * @param value
     *            Used service class.
     */
    void SetServiceClass(DLMS_SERVICE_CLASS value);


    /**
       * @return Invoke ID.
    */
    unsigned char GetInvokeID();

    /**
     * @param value
     *            Invoke ID.
     */
    void SetInvokeID(unsigned char value);

    /**
     * @return Auto increase Invoke ID.
     */
    bool GetAutoIncreaseInvokeID();

    /**
     * @param value
     *            Auto increase Invoke ID.
     */
    void SetAutoIncreaseInvokeID(bool value);

    // Gets used authentication.
    DLMS_AUTHENTICATION GetAuthentication();

    //Sets Used authentication.
    void SetAuthentication(DLMS_AUTHENTICATION value);

    // Gets client address.
    unsigned long GetClientAddress();

    // Sets client address.
    void SetClientAddress(unsigned long value);

    // Server address.
    unsigned long GetServerAddress();

    // Server address.
    void SetServerAddress(unsigned long value);

    // Maximum client PDU size.
    unsigned short GetMaxPduSize();

    // If protected release is used release is including a ciphered xDLMS Initiate request.
    bool GetUseProtectedRelease();

    // If protected release is used release is including a ciphered xDLMS Initiate request.
    void SetUseProtectedRelease(bool value);

    /////////////////////////////////////////////////////////////////////////////
    // Standard says that Time zone is from normal time to UTC in minutes.
    // If meter is configured to use UTC time (UTC to normal time) set this to true.
    bool GetUseUtc2NormalTime();
    void SetUseUtc2NormalTime(bool value);

    /////////////////////////////////////////////////////////////////////////
    // Expected Invocation(Frame) counter value.
    // Expected Invocation counter is not check if value is zero.
    uint64_t GetExpectedInvocationCounter();
    void SetExpectedInvocationCounter(uint64_t value);

    /////////////////////////////////////////////////////////////////////////
    // Skip selected date time fields.
    DATETIME_SKIPS GetDateTimeSkips();
    void SetDateTimeSkips(DATETIME_SKIPS value);

    /////////////////////////////////////////////////////////////////////////////
    //User id is the identifier of the user.
    unsigned char GetUserID();
    void SetUserID(unsigned char value);

    /////////////////////////////////////////////////////////////////////////////
    //Quality of service.
    unsigned char GetQualityOfService();
    void SetQualityOfService(unsigned char value);

    /////////////////////////////////////////////////////////////////////////////
    //  Source system title.
    // Meter returns system title when ciphered connection is made or GMAC authentication is used.
    ByteBuffer& GetSourceSystemTitle();

    ////////////////////////////////////////////////////////////////////////////
    // Customer PIN
    void SetCustomerPIN(unsigned char* pin, int len);
    ByteBuffer GetCustomerPIN();

    /////////////////////////////////////////////////////////////////////////////
    // Maximum client PDU size.
    int SetMaxReceivePDUSize(unsigned short value);
    unsigned short GetMaxReceivePDUSize();

    /////////////////////////////////////////////////////////////////////////////
    // General Block transfer window size.
    int SetGbtWindowSize(unsigned char value);
    unsigned char GetGbtWindowSize();

    //HDLC connection settings. GetLimits is obsolete. Use GetHdlcSettings instead.
    DLMSLimits& GetLimits();

    /////////////////////////////////////////////////////////////////////////////
    //HDLC connection settings.
    HdlcSettings& GetHdlcSettings();

    /////////////////////////////////////////////////////////////////////////////
    //PLC connection settings.
    PlcSettings& GetPlcSettings();

    /////////////////////////////////////////////////////////////////////////////
    // Collection of the objects.
    DLMSObjectCollection& GetObjects();

    /////////////////////////////////////////////////////////////////////////////
    // Returns SNRMRequest query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // packets: Packets to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int SNRMRequest(std::vector<ByteBuffer>& packets);

    /////////////////////////////////////////////////////////////////////////////
    // Parses UAResponse.
    /////////////////////////////////////////////////////////////////////////////
    // data: Byte array containing the data from the meter.
    // Returns: 0 if succeed. Otherwise error number.
    int ParseUAResponse(
        ByteBuffer& data);

    /////////////////////////////////////////////////////////////////////////////
    // Returns AARQRequest query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // Packets: Packets to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int AARQRequest(std::vector<ByteBuffer>& packets);

    /**
    * Parses the AARE response. Parse method will update the following data:
    * <ul>
    * <li>DLMSVersion</li>
    * <li>MaxReceivePDUSize</li>
    * <li>UseLogicalNameReferencing</li>
    * <li>LNSettings or SNSettings</li>
    * </ul>
    * LNSettings or SNSettings will be updated, depending on the referencing,
    * Logical name or Short name.
    *
    * reply
    *            Received data.
    * @see GXDLMSClient#aarqRequest
    * @see GXDLMSClient#GetUseLogicalNameReferencing
    * @see GXDLMSClient#GetLNSettings
    * @see GXDLMSClient#GetSNSettings
    */
    int ParseAAREResponse(ByteBuffer& data);

    /**
    * @return Is authentication Required.
    */
    bool IsAuthenticationRequired();

    /**
     * @return Get challenge request if HLS authentication is used.
     */
    int GetReplytoEDMIStoC(std::vector<ByteBuffer>& packets, std::map<std::string, ByteBuffer> &debuginfo, bool debug = false);
    int GetApplicationAssociationRequest(std::vector<ByteBuffer>& packets);
    void EnableCustomChallenge(bool fl);
    /**
     * Parse server's challenge if HLS authentication is used.
     *
     * @param reply
     *            Received reply from the server.
     */
    int ValidateEDMIAutheticationResponse(ByteBuffer& reply);
    int ParseApplicationAssociationResponse(ByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Returns ReceiverReady query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // reply: Reply data.
    // Data: Data to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int ReceiverReady(ReplyData& reply, ByteBuffer& Data);

    /////////////////////////////////////////////////////////////////////////////
    // Returns ReceiverReady query as byte array.
    /////////////////////////////////////////////////////////////////////////////
    // Type: type of the next requested packet.
    // Data: Data to send.
    // Returns: 0 if succeed. Otherwise error number.
    /////////////////////////////////////////////////////////////////////////////
    int ReceiverReady(DLMS_DATA_REQUEST_TYPES Type, ByteBuffer& Data);

    /////////////////////////////////////////////////////////////////////////////
    // Changes byte array received from the meter to given type.
    /////////////////////////////////////////////////////////////////////////////
    // value: Byte array received from the meter.
    // type: Wanted type.
    // useUtc: Standard says that Time zone is from normal time to UTC in minutes.
    //         If meter is configured to use UTC time (UTC to normal time)
    //         set this to true.
    // returns Value changed by type.
    static int ChangeType(ByteBuffer& value, DLMS_DATA_TYPE type, bool useUtc, DLMSVariant& newValue);

    /////////////////////////////////////////////////////////////////////////////
    // Changes byte array received from the meter to given type.
    /////////////////////////////////////////////////////////////////////////////
    // value Byte array received from the meter.
    // type Wanted type.
    // returns Value changed by type.
    static int ChangeType(ByteBuffer& value, DLMS_DATA_TYPE type, DLMSVariant& newValue);

    /////////////////////////////////////////////////////////////////////////////
    // Changes byte array received from the meter to given type.
    /////////////////////////////////////////////////////////////////////////////
    // value Byte array received from the meter.
    // type Wanted type.
    // returns Value changed by type.
    static int ChangeType(DLMSVariant& value, DLMS_DATA_TYPE type, DLMSVariant& newValue);

    /**
    * Parses the COSEM objects of the received data.
    *
    * objects : Read objects.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    */
    int ParseObjects(std::vector<DLMSVariant>& objects, bool onlyKnownObjects);

    /**
    * Parses the COSEM objects of the received data.
    *
    * objects : Read objects.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    * ignoreInactiveObjects : Inactivity objects are ignored.
    */
    int ParseObjects(
        std::vector<DLMSVariant>& objects,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    /**
    * Parses the COSEM objects of the received data.
    *
    * data : Received data, from the device, as byte array.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    */
    int ParseObjects(
        ByteBuffer& data,
        bool onlyKnownObjects);

    /**
    * Parses the COSEM objects of the received data.
    *
    * data : Received data, from the device, as byte array.
    * objects : Collection of COSEM objects.
    * onlyKnownObjects : Only known objects are parsed.
    * ignoreInactiveObjects : Inactivity objects are ignored.
    */
    int ParseObjects(
        ByteBuffer& data,
        bool onlyKnownObjects,
        bool ignoreInactiveObjects);

    /*
    * Get Value from byte array received from the meter.
    */
    int UpdateValue(
        DLMSObject& target,
        int attributeIndex,
        DLMSVariant& value);

    /**
        * Get Value from byte array received from the meter.
        *
        * data
        *            Byte array received from the meter.
        * @return Received data.
        */
    int GetValue(
        ByteBuffer& data,
        DLMSVariant& value);

    /**
    * Update list of values.
    *
    * list : List of read objects and atributes.
    * values :  Received values.
    */
    int UpdateValues(
        std::vector< std::pair<DLMSObject*, unsigned char> >& list,
        std::vector<DLMSVariant>& values);

    /**
    * Generates a release request.
    *
    * reply : Generated release message(s).
    * Returns error status.
    */
    int ReleaseRequest(
        std::vector<ByteBuffer>& packets);

    /**
    * Generates a disconnect request.
    *
    * reply : Generated disconnect message(s).
    * Returns error status.
    */
    int DisconnectRequest(
        std::vector<ByteBuffer>& packets);

    /**
    * Removes the HDLC frame from the packet, and returns COSEM data only.
    *
    * @param reply
    *            The received data from the device.
    * @param data
    *            The exported reply information.
    * @return Is frame complete.
    */
    int GetData(
        ByteBuffer& reply,
        ReplyData& data);

    /**
    * Removes the HDLC frame from the packet, and returns COSEM data only.
    *
    * @param reply
    *            The received data from the device.
    * @param data
    *            The exported reply information.
    * @param notify
    *            Information from the notify message.
    * @return Is frame complete.
    */
    int GetData(
        ByteBuffer& reply,
        ReplyData& data,
        ReplyData& notify);

    /**
    * Reads the Association view from the device. This method is used to get
    * all objects in the device.
    *
    * reply : Generated read message(s).
    * Returns error status.
    */
    int GetObjectsRequest(
        std::vector<ByteBuffer>& reply);

    /**
    Generates the keep alive message.

    Keepalive message is needed only HDLC framing.
    For keepalive we are reading logical name for Association object.
    This is done because all the meters can't handle HDLC keep alive message.
    */
    int GetKeepAlive(
        std::vector<ByteBuffer>& reply);


    /**
    * Generates a read message.
    *
    * name : Short or Logical Name.
    * objectType : COSEM object type.
    * attributeOrdinal : Attribute index of the object.
    * reply : Generated read message(s).
    * Returns error status.
    */
    int Read(
        DLMSVariant& name,
        DLMS_OBJECT_TYPE objectType,
        int attributeOrdinal,
        std::vector<ByteBuffer>& reply);

    /**
    * Generates a read message.
    *
    * @param pObject
    *            COSEM object to write.
    * @param attributeOrdinal
    *            Attribute index of the object.
    * @param reply
    *            Generated read message(s).
    * Returns error status.
    */
    int Read(
        DLMSObject* pObject,
        int attributeOrdinal,
        std::vector<ByteBuffer>& reply);

    /**
    * Read list of COSEM objects.
    *
    * @param list
    *            DLMS objects to read.
    * @return Read request as byte array.
    */
    int ReadList(
        std::vector<std::pair<DLMSObject*, unsigned char> >& list,
        std::vector<ByteBuffer>& reply);

    /**
    * Write list of COSEM objects.
    *
    * @param list
    *            DLMS objects to read.
    * @return Write request as byte array.
    */
    int WriteList(
        std::vector<std::pair<DLMSObject*, unsigned char> >& list,
        std::vector<ByteBuffer>& reply);

    /**
    * Generates a write message.
    *
    * @param name
    *            Short or Logical Name.
    * @param objectType
    *            Object type.
    * @param index
    *            Attribute index where data is write.
    * @param value
    *            Data to Write.
    * @param reply
    *             Generated write message(s).
    * Returns error status.
    */
    int Write(
        DLMSVariant& name,
        DLMS_OBJECT_TYPE objectType,
        int index,
        DLMSVariant& data,
        std::vector<ByteBuffer>& reply);

    /**
     * Generates a write message.
     *
     * @param pObject
     *            COSEM object to write.
     * @param index
     *            Attribute index where data is write.
     * @param data
     *            Data to Write.
     * @param reply
     *            Generated write message(s).
     * Returns error status.
     */
    int Write(
        DLMSObject* pObject,
        int index,
        DLMSVariant& data,
        std::vector<ByteBuffer>& reply);

    /**
    * Generates a write message.
    *
    * @param pObject
    *            COSEM object to write.
    * @param index
    *            Attribute index where data is write.
    * @param reply
    *            Generated write message(s).
    * Returns error status.
    */
    int Write(
        DLMSObject* pObject,
        int index,
        std::vector<ByteBuffer>& reply);

    /**
    * Generate Method (Action) request.
    *
    * @param item
    *            Method object short name or Logical Name.
    * @param index
    *            Method index.
    * @param data
    *            Method data.
    * @param type
    *            Data type.
    * @return DLMS action message.
    */
    int Method(
        DLMSObject* item,
        int index,
        DLMSVariant& data,
        std::vector<ByteBuffer>& reply);


    /**
    * Generate Method (Action) request.
    *
    * @param item
    *            Method object short name or Logical Name.
    * @param index
    *            Method index.
    * @param data
    *            Method data.
    * @param type
    *            Data type.
    * @return DLMS action message.
    */
    int Method(
        DLMSObject* item,
        int index,
        DLMSVariant& data,
        DLMS_DATA_TYPE dataType,
        std::vector<ByteBuffer>& reply);

    /**
   * Generate Method (Action) request..
   *
   * @param name
   *            Method object short name or Logical Name.
   * @param objectType
   *            Object type.
   * @param methodIndex
   *            Method index.
   * @param value
   *            Method data.
   * @param dataType
   *            Data type.
   * @return DLMS action message.
   */
    int Method(
        DLMSVariant name,
        DLMS_OBJECT_TYPE objectType,
        int methodIndex,
        DLMSVariant& data,
        std::vector<ByteBuffer>& reply);

    /**
    * Generate Method (Action) request..
    *
    * @param name
    *            Method object short name or Logical Name.
    * @param objectType
    *            Object type.
    * @param methodIndex
    *            Method index.
    * @param value
    *            Method data.
    * @param dataType
    *            Data type.
    * @return DLMS action message.
    */
    int Method(
        DLMSVariant name,
        DLMS_OBJECT_TYPE objectType,
        int methodIndex,
        DLMSVariant& data,
        DLMS_DATA_TYPE dataType,
        std::vector<ByteBuffer>& reply);

    /**
    * Read rows by entry.
    *
    * @param pg
    *            Profile generic object to read.
    * @param index
    *            Zero bases start index.
    * @param count
    *            Rows count to read.
    * @return Read message as byte array.
    */
    int ReadRowsByEntry(
        DLMSProfileGeneric* pg,
        int index,
        int count,
        std::vector<ByteBuffer>& reply);


    /**
    * Read rows by entry.
    *
    * @param pg
    *            Profile generic object to read.
    * @param index
    *            Zero bases start index.
    * @param count
    *            Rows count to read.
    * @return Read message as byte array.
    */
    int ReadRowsByEntry(
        DLMSProfileGeneric* pg,
        int index,
        int count,
        std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& columns,
        std::vector<ByteBuffer>& reply);


    /**
    * Read rows by range. Use this method to read Profile Generic table between
    * dates.
    *
    * @param pg
    *            Profile generic object to read.
    * @param start
    *            Start time.
    * @param end
    *            End time.
    * @return Generated read message.
    */
    int ReadRowsByRange(
        DLMSProfileGeneric* pObject,
        DateTime& start,
        DateTime& end,
        std::vector<ByteBuffer>& reply);

    /**
     * Read rows by range. Use this method to read Profile Generic table between
     * dates.
     *
     * @param pg
     *            Profile generic object to read.
     * @param start
     *            Start time.
     * @param end
     *            End time.
     * @return Generated read message.
     */
    int ReadRowsByRange(
        DLMSProfileGeneric* pg,
        struct tm* start,
        struct tm* end,
        std::vector<ByteBuffer>& reply);

    /**
    * Read rows by range. Use this method to read Profile Generic table between
    * dates.
    *
    * @param pg
    *            Profile generic object to read.
    * @param start
    *            Start time.
    * @param end
    *            End time.
    * @return Generated read message.
    */
    int ReadRowsByRange(
        DLMSProfileGeneric* pg,
        struct tm* start,
        struct tm* end,
        std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& columns,
        std::vector<ByteBuffer>& reply);


    /**
    *  Client will know what functionality server offers.
    *
    * @return Functionality.
    */
    DLMS_CONFORMANCE GetNegotiatedConformance();

    /**
    * Negotiated functionality for the server. Client can set this if meter report error value.
    *
    * @param value
    *            Proposed functionality.
    */
    void SetNegotiatedConformance(DLMS_CONFORMANCE value);

    /**
    * Proposed functionality for the server.
    * @return Proposed functionality.
    */
    DLMS_CONFORMANCE GetProposedConformance();

    /**
    * Proposed functionality for the server.
    *
    * @param value
    *            Proposed functionality.
    */
    void SetProposedConformance(DLMS_CONFORMANCE value);

    /**
     * Converts meter serial number to server address. Default formula is used.
     * All meters do not use standard formula or support serial number
     * addressing at all.
     *
     * serialNumber: Meter serial number.
     * logicalAddress: Meter logical address.
     * formula: Formula used to convert serial number to server address.
     *            Set to NULL if standard formula is used.
     * @return Server address.
     */
    static int GetServerAddressFromSerialNumber(
        unsigned long serialNumber,
        unsigned short logicalAddress,
        const char* formula = NULL);

    /**
     * Convert physical address and logical address to server address.
     *
     * @param logicalAddress
     *            Server logical address.
     * @param physicalAddress
     *            Server physical address.
     * @param addressSize
     *            Address size in bytes.
     * @return Server address.
     */
    static int  GetServerAddress(unsigned long logicalAddress,
        unsigned long physicalAddress,
        unsigned char addressSize = 0);

    /**
    * @return Protocol version.
    */
    char* GetProtocolVersion();

    /**
    * @param value
    *            Protocol version.
    */
    void SetProtocolVersion(char* value);

    //Convert object type enum value to string.
    static const std::string ObjectTypeToString(DLMS_OBJECT_TYPE type)
    {
        return DLMSConverter::ToString(type);
    }

    //Parse push objects.
    int ParsePushObjects(
        std::vector<DLMSVariant>& data,
        std::vector<std::pair<DLMSObject*, unsigned char> >& items);

    /*
    Set custom challenge.
    */
    void SetCtoSChallenge(ByteBuffer& value);

    /*
    Get custom challenge.
    */
    ByteBuffer& GetCtoSChallenge();


    // Generates a access service message.
    // time: Send time. Set to NULL is not used.
    // packets: Access request as a byte array.
    int AccessRequest(struct tm* time, std::vector<DLMSAccessItem>& list, std::vector<ByteBuffer>& packets);

    /// Parse access response.
    /// list: Collection of access items.
    /// data: Received data from the meter.
    int ParseAccessResponse(std::vector<DLMSAccessItem>& list, ByteBuffer& data);

    // Manufacturer ID.
    // 
    // Manufacturer ID(FLAG ID) is used for manucaturer depending functionality.
    char* GetManufacturerId();
    void SetManufacturerId(char value[3]);

    // Encrypt Landis+Gyr High level password.
    // password: User password.
    // seed: Seed received from the meter.
    // Returns occurred error.
    int EncryptLandisGyrHighLevelAuthentication(ByteBuffer& password, ByteBuffer& seed, ByteBuffer& crypted);

    ByteBuffer GetStoCChallenge() { return m_Settings.GetStoCChallenge(); }

    int DecryptAesKeyWrapping(ByteBuffer& data, ByteBuffer& kek, ByteBuffer& reply);

    void UnwrapKeys(const unsigned char* random_octet, const int random_octet_len, const unsigned char* wrapped_key, const int wrapped_key_len, ByteBuffer &reply);
};

#endif
