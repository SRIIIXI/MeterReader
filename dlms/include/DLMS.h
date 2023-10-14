#ifndef DLMS_H
#define DLMS_H

#include "DLMSLimits.h"
#include "DLMSObject.h"
#include "DLMSObjectCollection.h"
#include "ByteBuffer.h"
#include "DLMSSettings.h"
#include "DLMSVariant.h"
#include "Cipher.h"
#include "ReplyData.h"
#include "DLMSLNParameters.h"
#include "DLMSSNParameters.h"

class DLMS
{
private:
    friend class DLMSClient;
    friend class DLMSServer;

    static int AppendMultipleSNBlocks(
        DLMSSNParameters& p,
        ByteBuffer& reply);

    static unsigned short CountFCS16(ByteBuffer& buff, int index, int count);
    static uint32_t CountFCS24(unsigned char* buff, int index, int count);

    /////////////////////////////////////////////////////////////////////////////
    // Get adress as GXDLMSVariant.
    /////////////////////////////////////////////////////////////////////////////
    static int GetAddress(long value, unsigned long& address, int& size);

    /////////////////////////////////////////////////////////////////////////////
    // Get address as an byte array.
    /////////////////////////////////////////////////////////////////////////////
    static int GetAddressBytes(unsigned long value, ByteBuffer& bytes);

    /////////////////////////////////////////////////////////////////////////////
    // Returns true if HDLC us used.
    /////////////////////////////////////////////////////////////////////////////
    // type : Interface type.
    // Returns True, if HDLC is used.
    /////////////////////////////////////////////////////////////////////////////
    static bool UseHdlc(DLMS_INTERFACE_TYPE type);

    /////////////////////////////////////////////////////////////////////////////
    // Returns true if executed command is reply.
    /////////////////////////////////////////////////////////////////////////////
    // cmd : Executed command.
    // Returns Is command reply.
    /////////////////////////////////////////////////////////////////////////////
    static bool IsReplyMessage(DLMS_COMMAND cmd);

    /////////////////////////////////////////////////////////////////////////////
    // Check LLC bytes.
    /////////////////////////////////////////////////////////////////////////////
    // server : Is server.
    // data : Received data.
    /////////////////////////////////////////////////////////////////////////////
    static void GetLLCBytes(bool server, ByteBuffer& data);

    /////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////
    static int CheckWrapperAddress(
        DLMSSettings& settings,
        ByteBuffer& buff,
        ReplyData* notify);

    /////////////////////////////////////////////////////////////////////////////
    // Get value from data.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settings.
    // reply : Received data.
    /////////////////////////////////////////////////////////////////////////////
    static int GetValueFromData(
        DLMSSettings& settings,
        ReplyData& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Get data from HDLC or wrapper frame.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data that includes HDLC frame.
    // info : Reply data.
    // hdlc : Is HDLC framing used.
    /////////////////////////////////////////////////////////////////////////////
    static void GetDataFromFrame(
        ByteBuffer& reply,
        ReplyData& info,
        bool hdlc);

    /////////////////////////////////////////////////////////////////////////////
    // Get data from Wireless M-Bus frame.
    /////////////////////////////////////////////////////////////////////////////
    /// <param name="settings">DLMS settings.</param>
    /// <param name="buff">Received data.</param>
    /// <param name="data">Reply information.</param>
    static int GetMBusData(
        DLMSSettings& settings,
        ByteBuffer& buff,
        ReplyData& data);

    /////////////////////////////////////////////////////////////////////////////
    // Get data from S-FSK PLC frame.
    /////////////////////////////////////////////////////////////////////////////
    // settings: DLMS settings.
    // buff: Received data.
    // data: Reply information.
    static int GetPlcData(
        DLMSSettings& settings,
        ByteBuffer& buff,
        ReplyData& data);

    /////////////////////////////////////////////////////////////////////////////
    // Get data from S-FSK PLC Hdlc frame.
    /////////////////////////////////////////////////////////////////////////////
    // settings: DLMS settings.
    // buff: Received data.
    // data: Reply information.
    static int GetPlcHdlcData(
        DLMSSettings& settings,
        ByteBuffer& buff,
        ReplyData& data,
        unsigned char* frame);

    /**
    * Handle read response data block result.
    *
    * @param settings
    *            DLMS settings.
    * @param reply
    *            Received reply.
    * @param index
    *            Starting index.
    */
    static int ReadResponseDataBlockResult(
        DLMSSettings& settings,
        ReplyData& reply,
        int index);

    static int HandleGloDedRequest(
        DLMSSettings& settings,
        ReplyData& data);

    static int HandleGloDedResponse(
        DLMSSettings& settings,
        ReplyData& data, int index);

    static int HandleGeneralCiphering(
        DLMSSettings& settings,
        ReplyData& data);


    static int HandleGetResponseWithList(
        DLMSSettings& settings,
        ReplyData& reply);

public:

    /////////////////////////////////////////////////////////////////////////////
    // Check client and server addresses. Reserved for internal use.
    /////////////////////////////////////////////////////////////////////////////
    static int CheckInit(DLMSSettings& settings);

    /**
     * Get all Logical name messages. Client uses this to generate messages.
     *
     * @param p
     *            LN settings.
     * @param reply
     *            Generated messages.
     * @return    Status code.
     */
    static int GetLnMessages(
        DLMSLNParameters& p,
        std::vector<ByteBuffer>& reply);

    /**
    * Get all Short Name messages. Client uses this to generate messages.
    *
    * @param p
    *            DLMS SN parameters.
    * @param reply
    *            Generated messages.
    * @return    Status code.
    */
    static int GetSnMessages(
        DLMSSNParameters& p,
        std::vector<ByteBuffer>& reply);

    /**
    * Handle General block transfer message.
    *
    * @param settings
    *            DLMS settings.
    * @param data
    *            received data.
    */
    static int HandleGbt(DLMSSettings& settings, ReplyData& data);

    /**
     * Split DLMS PDU to wrapper frames.
     *
     * @param settings
     *            DLMS settings.
     * @param data
     *            Wrapped data.
     * @return Wrapper frames.
    */
    static int GetWrapperFrame(
        DLMSSettings& settings,
        DLMS_COMMAND command,
        ByteBuffer& data,
        ByteBuffer& reply);

    /**
    * Get next logical name PDU.
    *
    * @param p
    *            LN parameters.
    * @param reply
    *            Generated message.
    */
    static int GetLNPdu(
        DLMSLNParameters& p,
        ByteBuffer& reply);

    /**
    * @param p
    * @param reply
    */
    static int GetSNPdu(
        DLMSSNParameters& p,
        ByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Generates an acknowledgment message, with which the server is informed to
    // send next packets.
    /////////////////////////////////////////////////////////////////////////////
    // type : Frame type
    // Returns : Acknowledgment message as unsigned char array.
    /////////////////////////////////////////////////////////////////////////////
    static int ReceiverReady(
        DLMSSettings& settings,
        DLMS_DATA_REQUEST_TYPES type,
        Cipher* cipher,
        ByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Generates an acknowledgment message, with which the server is informed to
    // send next packets.
    /////////////////////////////////////////////////////////////////////////////
    // type : Frame type
    // Returns : Acknowledgment message as unsigned char array.
    /////////////////////////////////////////////////////////////////////////////
    static int ReceiverReady(
        DLMSSettings& settings,
        ReplyData& data,
        Cipher* cipher,
        ByteBuffer& reply);

    /**
    * Get HDLC frame for data.
    *
    * settings: DLMS settings.
    * frame: Frame ID. If zero new is generated.
    * data: Data to add.
    * reply: HDLC frame.
    */
    static int GetHdlcFrame(
        DLMSSettings& settings,
        unsigned char frame,
        ByteBuffer* data,
        ByteBuffer& reply);

    static int GetHdlcData(
        bool server,
        DLMSSettings& settings,
        ByteBuffer& reply,
        ReplyData& data,
        unsigned char& frame,
        ReplyData* notify);


    /////////////////////////////////////////////////////////////////////////////
    // Get MAC LLC frame for data.
    /////////////////////////////////////////////////////////////////////////////
    // settings: DLMS settings.
    // frame: HDLC frame sequence number.
    // creditFields: Credit fields.
    // data: Data to add.
    // reply: MAC frame.
    static int GetMacFrame(
        DLMSSettings& settings,
        unsigned char frame,
        unsigned char creditFields,
        ByteBuffer* data,
        ByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Get MAC LLC frame for data.
    /////////////////////////////////////////////////////////////////////////////
    // settings: DLMS settings.
    // data: Data to add.
    // reply: MAC frame.
    static int GetPlcFrame(
        DLMSSettings& settings,
        unsigned char creditFields,
        ByteBuffer* data,
        ByteBuffer& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Get MAC HDLC frame for data.
    /////////////////////////////////////////////////////////////////////////////
    // settings: DLMS settings.
    // frame: HDLC frame.
    // creditFields: Credit fields.
    // data: Data to add.
    // reply: MAC frame.
    static int GetMacHdlcFrame(
        DLMSSettings& settings,
        unsigned char frame,
        unsigned char creditFields,
        ByteBuffer* data,
        ByteBuffer& reply);

    /**
     * Get HDLC address from byte array.
     *
     * @param buff
     *            byte array.
     * @return HDLC address.
     */
    static int GetHDLCAddress(
        ByteBuffer& buff,
        unsigned long& address);

    /**
     * Check that client and server address match.
     *
     * @param server
     *            Is server.
     * @param settings
     *            DLMS settings.
     * @param reply
     *            Received data.
     * @param index
     *            Position.
     * @return True, if client and server address match.
     */
    static int CheckHdlcAddress(
        bool server,
        DLMSSettings& settings,
        ByteBuffer& reply,
        int index,
        unsigned long& source,
        unsigned long& target);

    /////////////////////////////////////////////////////////////////////////////
    // Get data from TCP/IP frame.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settigns.
    // buff : Received data.
    // data : Reply information.
    /////////////////////////////////////////////////////////////////////////////
    static int GetTcpData(
        DLMSSettings& settings,
        ByteBuffer& buff,
        ReplyData& data,
        ReplyData* notify);

    /////////////////////////////////////////////////////////////////////////////
    // Handle read response and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // data : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleReadResponse(
        DLMSSettings& settings,
        ReplyData& data,
        int index);

    static int AddInvokeId(
        DLMSTranslatorStructure* xml, 
        DLMS_COMMAND command, 
        uint8_t type, 
        uint32_t invokeId);

    /////////////////////////////////////////////////////////////////////////////
    // Handle method response and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // data : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleMethodResponse(
        DLMSSettings& settings,
        ReplyData& data,
        unsigned long index);

    /////////////////////////////////////////////////////////////////////////////
    // Handle push and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandlePush(ReplyData& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Handle set response and update error status.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleSetResponse(
        DLMSSettings& settings,
        ReplyData& data);

    static int HandleAccessResponse(
        DLMSSettings& settings,
        ReplyData& reply);

    /**
    * Handle data notification get data from block and/or update error status.
    *
    * @param settings
    *            DLMS settings.
    * @param reply
    *            Received data from the client.
    */
    static int HandleDataNotification(
        DLMSSettings& settings,
        ReplyData& reply);

    /////////////////////////////////////////////////////////////////////////////
    // Handle write response and update error status.
    /////////////////////////////////////////////////////////////////////////////
    // reply : Received data from the client.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleWriteResponse(
        ReplyData& data);

    static int HandleGetResponseNormal(
        DLMSSettings& settings,
        ReplyData& reply,
        bool& empty);

    static int HandleGetResponseNextDataBlock(
        DLMSSettings& settings,
        ReplyData& reply,
        int index);

    /////////////////////////////////////////////////////////////////////////////
    // Handle get response and get data from block and/or update error status.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settings.
    // reply : Received data from the client.
    // index : Block index number.
    /////////////////////////////////////////////////////////////////////////////
    static int HandleGetResponse(
        DLMSSettings& settings,
        ReplyData& reply,
        int index);

    /////////////////////////////////////////////////////////////////////////////
    // Get PDU from the packet.
    /////////////////////////////////////////////////////////////////////////////
    // settings : DLMS settings.
    // data : received data.
    // cipher : Cipher interface.
    /////////////////////////////////////////////////////////////////////////////
    static int GetPdu(
        DLMSSettings& settings,
        ReplyData& data);

    /////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////
    static int GetData(
        DLMSSettings& settings,
        ByteBuffer& reply,
        ReplyData& data,
        ReplyData* notify);

    /////////////////////////////////////////////////////////////////////////////
    // Get action info.
    /////////////////////////////////////////////////////////////////////////////
    // objectType : Object type.
    // value : Starting address.
    // count : Attribute count.
    /////////////////////////////////////////////////////////////////////////////
    static int GetActionInfo(
        DLMS_OBJECT_TYPE objectType,
        unsigned char& value,
        unsigned char& count);


    static int AppendData(
        DLMSSettings* settings,
        DLMSObject* obj,
        unsigned char index,
        ByteBuffer& bb,
        DLMSVariant& value);

    static int ParseSnrmUaResponse(
        ByteBuffer& data,
        HdlcSettings* limits);

    // Add HDLC parameter.
    static void AppendHdlcParameter(ByteBuffer& data, unsigned short value);

    static int HandleConfirmedServiceError(ReplyData& data);

    static int HandleExceptionResponse(ReplyData& data);
};
#endif
