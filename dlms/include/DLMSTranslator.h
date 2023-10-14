#ifndef DLMS_TRANSLATOR_H
#define DLMS_TRANSLATOR_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_XML_TRANSLATOR
#include <algorithm>
#include "Enums.h"
#include "ByteBuffer.h"
#include "TranslatorSimpleTags.h"
#include "TranslatorStandardTags.h"
#include "DLMSTranslatorStructure.h"
#include "DLMSSettings.h"


class DLMSTranslator
{
    DLMS_TRANSLATOR_OUTPUT_TYPE m_OutputType;
    unsigned short m_SAck, m_RAck;
    bool sending;
    unsigned char m_SSendSequence;
    unsigned char m_SReceiveSequence;
    unsigned char m_RSendSequence;
    unsigned char m_RReceiveSequence;
    bool m_Comments;
    std::map<unsigned long, std::string> m_Tags;
    std::map<std::string, unsigned long> m_TagsByName;
    bool m_PduOnly;
    bool m_CompletePdu;
    bool m_Hex;
    bool m_ShowStringAsHex;
    bool m_OmitXmlDeclaration;
    bool m_OmitXmlNameSpace;
    // Sending data in multiple frames.
    bool m_MultipleFrames;

    // If only PDUs are shown and PDU is received on parts.
    ByteBuffer m_PduFrames;

    int PduToXml(ByteBuffer& value, bool omitDeclaration, bool omitNameSpace, std::string& output);

    int PduToXml(DLMSTranslatorStructure* xml, ByteBuffer& value, bool omitDeclaration, bool omitNameSpace, bool allowUnknownCommand, std::string& output);
    void GetCiphering(DLMSSettings& settings, bool force);
public:
    // Are comments added.
    bool GetComments();
    // Are comments added.
    void SetComments(bool value);

    // Is only PDU shown when data is parsed with MessageToXml.
    bool GetPduOnly();
    // Is only PDU shown when data is parsed with MessageToXml.
    void SetPduOnly(bool value);

    // Is only complete PDU parsed and shown.
    bool GetCompletePdu();
    // Is only complete PDU parsed and shown.
    void SetCompletePdu(bool value);

    // Are numeric values shown as hex.
    bool GetHex();
    // Are numeric values shown as hex.
    void SetHex(bool value);

    // Is string serialized as hex.
    bool GetShowStringAsHex();
    // Is string serialized as hex.
    void SetShowStringAsHex(bool value);

    // Is XML declaration skipped.
    bool GetOmitXmlDeclaration();
    // Is XML declaration skipped.
    void SetOmitXmlDeclaration(bool value);

    // Is XML declaration skipped.
    bool GetOmitXmlNameSpace();
    // Is XML declaration skipped.
    void SetOmitXmlNameSpace(bool value);

    DLMS_SECURITY m_Security;
    /**
    * System title.
    */
    ByteBuffer m_SystemTitle;

    /**
    * Server System title.
    */
    ByteBuffer m_ServerSystemTitle;

    /**
    * Dedicated key.
    */
    ByteBuffer m_DedicatedKey;

    /**
    * Is data encrypted.
    */
    bool m_Encrypt;

    /**
    *  Block cipher key.
    */
    ByteBuffer m_BlockCipherKey;
    /**
    *  Block Authentication key.
    */
    ByteBuffer m_AuthenticationKey;

    /**
     * Frame counter. AKA Invocation counter.
     */
    unsigned long m_FrameCounter;

    /*
    * Get all tags.
    * type: Output type.
    * list: List of tags by ID.
    * tagsByName: List of tags by name.
    */
    static void GetTags(DLMS_TRANSLATOR_OUTPUT_TYPE type,
        std::map<unsigned long, std::string>& list,
        std::map<std::string, unsigned long>& tagsByName)
    {
        if (type == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
        {
            CTranslatorSimpleTags::GetGeneralTags(type, list);
            CTranslatorSimpleTags::GetSnTags(type, list);
            CTranslatorSimpleTags::GetLnTags(type, list);
            CTranslatorSimpleTags::GetGloTags(type, list);
            CTranslatorSimpleTags::GetDedTags(type, list);
            CTranslatorSimpleTags::GetTranslatorTags(type, list);
            CTranslatorSimpleTags::GetDataTypeTags(list);
        }
        else
        {
            CTranslatorStandardTags::GetGeneralTags(type, list);
            CTranslatorStandardTags::GetSnTags(type, list);
            CTranslatorStandardTags::GetLnTags(type, list);
            CTranslatorStandardTags::GetGloTags(type, list);
            CTranslatorStandardTags::GetDedTags(type, list);
            CTranslatorStandardTags::GetTranslatorTags(type, list);
            CTranslatorStandardTags::GetDataTypeTags(list);
        }
        // Simple is not case sensitive.
        bool lowercase = type == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML;
        for (std::map<unsigned long, std::string>::iterator it = list.begin(); it != list.end(); ++it)
        {
            std::string str = it->second;
            if (lowercase)
            {
                std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            }
            if (tagsByName.find(str) == tagsByName.end())
            {
                tagsByName[str] = it->first;
            }
        }
    }

public:
    // Constructor.
    // Translator output type.
    DLMSTranslator(DLMS_TRANSLATOR_OUTPUT_TYPE type);

    /**
   * @return Used security.
   */
    DLMS_SECURITY GetSecurity();

    /**
    * @param value
    *            Used security.
    */
    void SetSecurity(DLMS_SECURITY value);

    /**
     * @return System title.
     */
    ByteBuffer& GetSystemTitle();

    /**
    *  @param value System title.
    */
    void SetSystemTitle(ByteBuffer& value);


    /*
     * Returns System title.
     */
    ByteBuffer& GetServerSystemTitle();

    /*
    *  value: System title.
    */
    void SetServerSystemTitle(ByteBuffer& value);

    /*
     * Returns dedicated key.
    */
    ByteBuffer& GetDedicatedKey();

    /*
    *  value: Dedicated ky.
    */
    void SetDedicatedKey(ByteBuffer& value);


    /**
     * @return Block cipher key.
     */
    ByteBuffer& GetBlockCipherKey();

    /**
    *  @param value Block cipher key.
    */
    void SetBlockCipherKey(ByteBuffer& value);

    /**
     * @return Authentication key.
     */
    ByteBuffer& GetAuthenticationKey();

    /**
     * @param value
     *            Authentication key.
     */
    void SetAuthenticationKey(ByteBuffer& value);

    /**
     * @return Frame counter. Invocation counter.
     */
    unsigned long GetFrameCounter();

    void SetFrameCounter(unsigned long value);

    // Convert bytes to xml.
    // value: Bytes to convert.
    // Returns Converted xml.
    int PduToXml(ByteBuffer& value, std::string& output)
    {
        return PduToXml(value, m_OmitXmlDeclaration, m_OmitXmlNameSpace, output);
    }

    static DLMS_ERROR_CODE ValueOfErrorCode(
        DLMS_TRANSLATOR_OUTPUT_TYPE type,
        std::string& value);

    static int ErrorCodeToString(
        DLMS_TRANSLATOR_OUTPUT_TYPE type,
        DLMS_ERROR_CODE value,
        std::string& result);

    // Convert data bytes to XML.
    // <param name="data">Data to parse.</param>
    // <param name="xml">Generated xml.</param>
    // <returns>Error code.</returns>
    int DataToXml(ByteBuffer& data, std::string& xml);

};
#endif
#endif
