#ifndef APDU_H
#define APDU_H

#include <vector>
#include "Enums.h"
#include "DLMSSettings.h"
#include "DLMSTranslatorStructure.h"

/////////////////////////////////////////////////////////////////////////////
// The services to access the attributes and methods of COSEM objects are
// determined on DLMS/COSEM Application layer. The services are carried by
// Application Protocol Data Units (APDUs).
// <p />
// In DLMS/COSEM the meter is primarily a server, and the controlling system is
// a client. Also unsolicited (received without a request) messages are
// available.
/////////////////////////////////////////////////////////////////////////////
class APDU
{
    friend class DLMSTranslator;
private:
    /////////////////////////////////////////////////////////////////////////////
    // Constructor.
    /////////////////////////////////////////////////////////////////////////////
    APDU()
    {
    }
    static int ParsePDU2(
        DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer& buff,
        DLMS_ASSOCIATION_RESULT& result,
        DLMS_SOURCE_DIAGNOSTIC& diagnostic
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        , DLMSTranslatorStructure* xml
#endif //DLMS_IGNORE_XML_TRANSLATOR
    );

    static int Parse(bool initiateRequest,
        DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer& data,
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        DLMSTranslatorStructure* xml,
#endif //DLMS_IGNORE_XML_TRANSLATOR
        unsigned char tag);
public:

    /**
     * Parse User Information from PDU.
     */
    static int ParseUserInformation(
        DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer& data
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        , DLMSTranslatorStructure* xml
#endif //DLMS_IGNORE_XML_TRANSLATOR
    );

    /**
     * Generates Aarq.
     */
    static int GenerateAarq(
        DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer* encryptedData,
        ByteBuffer& data);

    /**
    * Generate user information.
    *
    * @param settings
    *            DLMS settings.
    * @param cipher
    * @param data
    *            Generated user information.
    */
    static int GenerateUserInformation(
        DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer* encryptedData,
        ByteBuffer& data);

    /**
     * Parse APDU.
     */
    static int ParsePDU(DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer& buff,
        DLMS_ASSOCIATION_RESULT &result,
        DLMS_SOURCE_DIAGNOSTIC& diagnostic
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        , DLMSTranslatorStructure* xml
#endif //DLMS_IGNORE_XML_TRANSLATOR
        );

    /**
     * Server generates AARE message.
     */
    static int GenerateAARE(
        DLMSSettings& settings,
        ByteBuffer& data,
        DLMS_ASSOCIATION_RESULT result,
        DLMS_SOURCE_DIAGNOSTIC diagnostic,
        Cipher* cipher,
        ByteBuffer *errorData,
        ByteBuffer *encryptedData);

    static int GetUserInformation(
        DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer& data);

    static int ParseInitiate(
        bool initiateRequest,
        DLMSSettings& settings,
        Cipher* cipher,
        ByteBuffer& data
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        , DLMSTranslatorStructure* xml
#endif //DLMS_IGNORE_XML_TRANSLATOR
        );
};

#endif
