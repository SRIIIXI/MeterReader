#ifndef DLMS_SN_COMMAND_HANDLER_H
#define DLMS_SN_COMMAND_HANDLER_H

#include "ByteBuffer.h"
#include "DLMSTranslatorStructure.h"
#include "DLMSSettings.h"
#include "DLMSValueEventCollection.h"
#include "SNInfo.h"

class DLMSSNCommandHandler
{
    /**
* Find Short Name object.
*
* @param sn
*/
    static int FindSNObject(DLMSServer* server, int sn, SNInfo& i);

    /**
* Get data for Read command.
*
* @param settings
*            DLMS settings.
* @param list
*            received objects.
* @param data
*            Data as byte array.
* @param type
*            Response type.
*/
    static int GetReadData(
        DLMSSettings& settings,
        std::vector<DLMSValueEventArg*>& list,
        ByteBuffer& data,
        DLMS_SINGLE_READ_RESPONSE& type);

    /**
    * Handle read Block in blocks.
    *
    * @param data
    *            Received data.
    */
    static int HandleReadBlockNumberAccess(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml);

    static int HandleReadDataBlockAccess(
        DLMSSettings& settings,
        DLMSServer* server,
        DLMS_COMMAND command,
        ByteBuffer& data,
        int cnt,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    static int ReturnSNError(
        DLMSSettings& settings,
        DLMSServer* server,
        DLMS_COMMAND cmd,
        DLMS_ERROR_CODE error,
        ByteBuffer* replyData);

    static int HandleRead(
        DLMSSettings& settings,
        DLMSServer* server,
        DLMS_VARIABLE_ACCESS_SPECIFICATION type,
        ByteBuffer& data,
        DLMSValueEventCollection& list,
        std::vector<DLMSValueEventArg*>& reads,
        std::vector<DLMSValueEventArg*>& actions,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml);
public:
    /**
  * Handle read request.
  */
    static int HandleReadRequest(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);

    /**
    * Handle write request.
    *
    * @param reply
    *            Received data from the client.
    * @return Reply.
    */
    static  int HandleWriteRequest(
        DLMSSettings& settings,
        DLMSServer* server,
        ByteBuffer& data,
        ByteBuffer* replyData,
        DLMSTranslatorStructure* xml,
        unsigned char cipheredCommand);


    // Handle Information Report.
    static int HandleInformationReport(
        DLMSSettings& settings,
        ReplyData& reply);
};
#endif
