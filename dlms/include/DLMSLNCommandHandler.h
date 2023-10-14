#ifndef DLMS_LN_COMMAND_HANDLER_H
#define DLMS_LN_COMMAND_HANDLER_H

#include "ByteBuffer.h"
#include "DLMSLNParameters.h"
#include "DLMSTranslatorStructure.h"
#include "DLMSSettings.h"
#include "DLMSConnectionEventArgs.h"
#include "ReplyData.h"

class DLMSLNCommandHandler
{
    /**
 * Handle get request normal command.
 *
 * @param data
 *            Received data.
 */
    static int GetRequestNormal(
        DLMSSettings& settings,
        unsigned char invokeID,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    /**
     * Handle get request with list command.
     *
     * @param data
     *            Received data.
     */
    static int GetRequestWithList(
        DLMSSettings& settings,
        unsigned char invokeID,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    static int HanleSetRequestWithDataBlock(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        DLMSLNParameters& p,
        DLMSTranslatorStructure* xml);

    static int HandleSetRequestNormal(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        short type,
        DLMSLNParameters& p,
        DLMSTranslatorStructure* xml);

    static int HanleSetRequestWithList(
        DLMSSettings& settings,
        unsigned char invoke,
        DLMSServer* server,
        ByteBuffer& data,
        DLMSLNParameters& p,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

public:
    /**
    * Handle get request next data block command.
    *
    * @param data
    *            Received data.
    */
    static int GetRequestNextDataBlock(
        DLMSSettings& settings,
        unsigned char invokeID,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        bool streaming,
        unsigned char cipheredCommand);

    /**
    * Handle Get request.
    *
    * @return Reply to the client.
    */
    static int HandleGetRequest(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    /**
    * Handle Set request.
    *
    * @return Reply to the client.
    */
    static int HandleSetRequest(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    static int MethodRequestNormal(
        DLMSSettings& settings,
        uint8_t invokeId,
        DLMSServer* server,
        ByteBuffer& data,
        DLMSConnectionEventArgs* connectionInfo,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    static int MethodRequestNextBlock(
        DLMSSettings& settings,
        uint8_t invokeId,
        DLMSServer* server,
        ByteBuffer& data,
        DLMSConnectionEventArgs* connectionInfo,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        bool streaming,
        unsigned char cipheredCommand);

    /**
  * Handle action request.
  *
  * @param reply
  *            Received data from the client.
  * @return Reply.
  */
    static int HandleMethodRequest(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSConnectionEventArgs* connectionInfo,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    //Handle Access request.
    static int HandleAccessRequest(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    // Handle Event Notification.
    static int HandleEventNotification(
        DLMSSettings& settings,
        ReplyData& reply);

};
#endif
