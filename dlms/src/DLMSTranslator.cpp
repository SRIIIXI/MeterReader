#include "DLMSTranslator.h"
#ifndef DLMS_IGNORE_XML_TRANSLATOR
#include "DLMSSettings.h"
#include "ReplyData.h"
#include "APDU.h"
#include "Cipher.h"
#include "DLMS.h"
#include "DLMSLNCommandHandler.h"
#include "DLMSSNCommandHandler.h"

DLMSTranslator::DLMSTranslator(DLMS_TRANSLATOR_OUTPUT_TYPE type)
{
    m_Security = DLMS_SECURITY_NONE;
    m_OutputType = type;
    GetTags(type, m_Tags, m_TagsByName);
    m_SAck = m_RAck = 0;
    sending = false;
    m_SSendSequence = 0;
    m_SReceiveSequence = 0;
    m_RSendSequence = 0;
    m_RReceiveSequence = 0;
    m_Comments = false;
    m_PduOnly = false;
    m_CompletePdu = false;
    m_Hex = false;
    m_ShowStringAsHex = false;
    m_OmitXmlDeclaration = false;
    m_OmitXmlNameSpace = false;
    m_MultipleFrames = false;
    if (type == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        m_Hex = true;
    }
}


bool DLMSTranslator::GetComments()
{
    return m_Comments;
}

void DLMSTranslator::SetComments(bool value)
{
    m_Comments = value;
}

bool DLMSTranslator::GetPduOnly()
{
    return m_PduOnly;
}

void DLMSTranslator::SetPduOnly(bool value)
{
    m_PduOnly = value;
}

bool DLMSTranslator::GetCompletePdu()
{
    return m_CompletePdu;
}

void DLMSTranslator::SetCompletePdu(bool value)
{
    m_CompletePdu = value;
}

bool DLMSTranslator::GetHex()
{
    return m_Hex;
}

void DLMSTranslator::SetHex(bool value)
{
    m_Hex = value;
}

bool DLMSTranslator::GetShowStringAsHex()
{
    return m_ShowStringAsHex;
}

void DLMSTranslator::SetShowStringAsHex(bool value)
{
    m_ShowStringAsHex = value;
}

bool DLMSTranslator::GetOmitXmlDeclaration()
{
    return m_OmitXmlDeclaration;
}

void DLMSTranslator::SetOmitXmlDeclaration(bool value)
{
    m_OmitXmlDeclaration = value;
}

bool DLMSTranslator::GetOmitXmlNameSpace()
{
    return m_OmitXmlNameSpace;
}

void DLMSTranslator::SetOmitXmlNameSpace(bool value)
{
    m_OmitXmlNameSpace = value;
}

DLMS_SECURITY DLMSTranslator::GetSecurity()
{
    return m_Security;
}

void DLMSTranslator::SetSecurity(DLMS_SECURITY value)
{
    m_Security = value;
}

ByteBuffer& DLMSTranslator::GetSystemTitle()
{
    return m_SystemTitle;
}

void DLMSTranslator::SetSystemTitle(ByteBuffer& value)
{
    m_SystemTitle.Clear();
    m_SystemTitle.Set(value.GetData(), value.GetSize() - value.GetPosition());
}

ByteBuffer& DLMSTranslator::GetBlockCipherKey()
{
    return m_BlockCipherKey;
}

void DLMSTranslator::SetBlockCipherKey(ByteBuffer& value)
{
    m_BlockCipherKey.Clear();
    m_BlockCipherKey.Set(value.GetData(), value.GetSize() - value.GetPosition());
}

ByteBuffer& DLMSTranslator::GetAuthenticationKey()
{
    return m_AuthenticationKey;
}

void DLMSTranslator::SetAuthenticationKey(ByteBuffer& value)
{
    m_AuthenticationKey.Clear();
    m_AuthenticationKey.Set(value.GetData(), value.GetSize() - value.GetPosition());
}

unsigned long DLMSTranslator::GetFrameCounter()
{
    return m_FrameCounter;
}

void DLMSTranslator::SetFrameCounter(unsigned long value)
{
    m_FrameCounter = value;
}

void DLMSTranslator::GetCiphering(DLMSSettings& settings, bool force)
{
    if (force || m_Security != DLMS_SECURITY_NONE)
    {
        Cipher* c = settings.GetCipher();
        c->SetSecurity(m_Security);
        c->SetSystemTitle(m_SystemTitle);
        c->SetBlockCipherKey(m_BlockCipherKey);
        c->SetAuthenticationKey(m_AuthenticationKey);
        c->SetFrameCounter(m_FrameCounter);
        c->SetDedicatedKey(m_DedicatedKey);
        settings.SetSourceSystemTitle(m_ServerSystemTitle);
    }
    else
    {
        settings.SetCipher(NULL);
    }
}

int DLMSTranslator::PduToXml(ByteBuffer& value, bool omitDeclaration, bool omitNameSpace, std::string& output)
{
    DLMSTranslatorStructure xml(m_OutputType, m_OmitXmlNameSpace, m_Hex, m_ShowStringAsHex, m_Comments, m_Tags);
    return PduToXml(&xml, value, omitDeclaration, m_OmitXmlNameSpace, true, output);
}

int GetUa(ByteBuffer& data, DLMSTranslatorStructure* xml)
{
    unsigned char ch, id, len;
    unsigned short ui;
    unsigned long ul;
    int ret;
    // Skip FromatID
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Skip Group ID.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Skip Group len
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    DLMSVariant value;
    while (data.GetPosition() < data.GetSize())
    {
        if ((ret = data.GetUInt8(&id)) != 0)
        {
            return ret;
        }
        if ((ret = data.GetUInt8(&len)) != 0)
        {
            return ret;
        }
        switch (len)
        {
        case 1:
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            value = ch;
            break;
        case 2:
            if ((ret = data.GetUInt16(&ui)) != 0)
            {
                return ret;
            }
            value = ui;
            break;
        case 4:
            if ((ret = data.GetUInt32(&ul)) != 0)
            {
                return ret;
            }
            value = ul;
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        // RX / TX are delivered from the partner's point of view =>
        // reversed to ours
        switch (id)
        {
        case HDLC_INFO_MAX_INFO_TX:
            xml->AppendLine("<MaxInfoTX Value=\"" + value.ToString() + "\" />");
            break;
        case HDLC_INFO_MAX_INFO_RX:
            xml->AppendLine("<MaxInfoRX Value=\"" + value.ToString() + "\" />");
            break;
        case HDLC_INFO_WINDOW_SIZE_TX:
            xml->AppendLine("<WindowSizeTX Value=\"" + value.ToString() + "\" />");
            break;
        case HDLC_INFO_WINDOW_SIZE_RX:
            xml->AppendLine("<WindowSizeRX Value=\"" + value.ToString() + "\" />");
            break;
        default:
            ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
            break;
        }
    }
    return ret;
}

bool IsCiphered(unsigned char cmd)
{
    switch (cmd)
    {
    case DLMS_COMMAND_GLO_READ_REQUEST:
    case DLMS_COMMAND_GLO_WRITE_REQUEST:
    case DLMS_COMMAND_GLO_GET_REQUEST:
    case DLMS_COMMAND_GLO_SET_REQUEST:
    case DLMS_COMMAND_GLO_READ_RESPONSE:
    case DLMS_COMMAND_GLO_WRITE_RESPONSE:
    case DLMS_COMMAND_GLO_GET_RESPONSE:
    case DLMS_COMMAND_GLO_SET_RESPONSE:
    case DLMS_COMMAND_GLO_METHOD_REQUEST:
    case DLMS_COMMAND_GLO_METHOD_RESPONSE:
    case DLMS_COMMAND_DED_GET_REQUEST:
    case DLMS_COMMAND_DED_SET_REQUEST:
    case DLMS_COMMAND_DED_READ_RESPONSE:
    case DLMS_COMMAND_DED_GET_RESPONSE:
    case DLMS_COMMAND_DED_SET_RESPONSE:
    case DLMS_COMMAND_DED_METHOD_REQUEST:
    case DLMS_COMMAND_DED_METHOD_RESPONSE:
    case DLMS_COMMAND_GENERAL_GLO_CIPHERING:
    case DLMS_COMMAND_GENERAL_DED_CIPHERING:
    case DLMS_COMMAND_GENERAL_CIPHERING:
        return true;
    default:
        return false;
    }
}

int DLMSTranslator::PduToXml(DLMSTranslatorStructure* xml, ByteBuffer& value, bool omitDeclaration, bool omitNameSpace, bool allowUnknownCommand, std::string& output)
{
    DLMS_ASSOCIATION_RESULT result;
    DLMS_SOURCE_DIAGNOSTIC diagnostic;
    int ret;
    std::string str;
    ReplyData data;
    unsigned char cmd, ch;
    unsigned long len;
    DLMSSettings settings(true);
    output.clear();
    if ((ret = value.GetUInt8(&cmd)) != 0)
    {
        return ret;
    }
    GetCiphering(settings, IsCiphered(cmd));
    ByteBuffer tmp;
    switch (cmd)
    {
    case DLMS_COMMAND_AARQ:
        value.SetPosition(0);
        APDU::ParsePDU(settings, settings.GetCipher(), value, result, diagnostic, xml);
        break;
    case DLMS_COMMAND_INITIATE_REQUEST:
        value.SetPosition(0);
        APDU::ParseInitiate(true, settings, settings.GetCipher(), value, xml);
        break;
    case DLMS_COMMAND_INITIATE_RESPONSE:
    {
        value.SetPosition(0);
        DLMSSettings s(false);
        GetCiphering(s, false);
        APDU::ParseInitiate(true, s, s.GetCipher(), value,
            xml);
    }
    break;
    case 0x81://Ua
        value.SetPosition(0);
        GetUa(value, xml);
        break;
    case DLMS_COMMAND_AARE:
    {
        value.SetPosition(0);
        DLMSSettings s(false);
        GetCiphering(s, false);
        APDU::ParsePDU(s, s.GetCipher(), value, result, diagnostic, xml);
    }
    break;
    case DLMS_COMMAND_GET_REQUEST:
        DLMSLNCommandHandler::HandleGetRequest(settings, NULL, value, NULL, xml, DLMS_COMMAND_NONE);
        break;
    case DLMS_COMMAND_SET_REQUEST:
        DLMSLNCommandHandler::HandleSetRequest(settings, NULL, value, NULL, xml, DLMS_COMMAND_NONE);
        break;
    case DLMS_COMMAND_READ_REQUEST:
        DLMSSNCommandHandler::HandleReadRequest(settings, NULL, value, NULL, xml, DLMS_COMMAND_NONE);
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        DLMSLNCommandHandler::HandleMethodRequest(settings, NULL, value, NULL, NULL, xml, DLMS_COMMAND_NONE);
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
        DLMSSNCommandHandler::HandleWriteRequest(settings, NULL, value, NULL, xml, DLMS_COMMAND_NONE);
        break;
    case DLMS_COMMAND_ACCESS_REQUEST:
        DLMSLNCommandHandler::HandleAccessRequest(settings, NULL, value, NULL, xml, DLMS_COMMAND_NONE);
        break;
    case DLMS_COMMAND_DATA_NOTIFICATION:
        data.SetXml(xml);
        data.SetData(value);
        value.SetPosition(0);
        DLMS::GetPdu(settings, data);
        break;
    case DLMS_COMMAND_INFORMATION_REPORT:
        data.SetXml(xml);
        data.SetData(value);
        ret = DLMSSNCommandHandler::HandleInformationReport(settings, data);
        break;
    case DLMS_COMMAND_EVENT_NOTIFICATION:
        data.SetXml(xml);
        data.SetData(value);
        ret = DLMSLNCommandHandler::HandleEventNotification(settings, data);
        break;
    case DLMS_COMMAND_READ_RESPONSE:
    case DLMS_COMMAND_WRITE_RESPONSE:
    case DLMS_COMMAND_GET_RESPONSE:
    case DLMS_COMMAND_SET_RESPONSE:
    case DLMS_COMMAND_METHOD_RESPONSE:
    case DLMS_COMMAND_ACCESS_RESPONSE:
    case DLMS_COMMAND_GENERAL_BLOCK_TRANSFER:
        data.SetXml(xml);
        data.SetData(value);
        value.SetPosition(0);
        DLMS::GetPdu(settings, data);
        break;
    case DLMS_COMMAND_GENERAL_CIPHERING:
        data.SetXml(xml);
        data.SetData(value);
        value.SetPosition(0);
        DLMS::GetPdu(settings, data);
        break;
    case DLMS_COMMAND_RELEASE_REQUEST:
        xml->AppendStartTag(cmd);
        value.GetUInt8(&ch);
        // Len.
        if (value.Available() != 0)
        {
            // BerType
            if ((ret = value.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            // Len.
            if ((ret = value.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if ((ret = value.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                str = CTranslatorSimpleTags::ReleaseRequestReasonToString((DLMS_RELEASE_REQUEST_REASON)ch);
            }
            else
            {
                str = CTranslatorStandardTags::ReleaseRequestReasonToString((DLMS_RELEASE_REQUEST_REASON)ch);
            }
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_REASON, "Value", str);
            if (value.Available() != 0)
            {
                DLMS_ASSOCIATION_RESULT result;
                DLMS_SOURCE_DIAGNOSTIC diagnostic;
                ret = APDU::ParsePDU2(settings, settings.GetCipher(), value, result, diagnostic, xml);
            }
        }
        xml->AppendEndTag(cmd);
        break;
    case DLMS_COMMAND_RELEASE_RESPONSE:
        xml->AppendStartTag(cmd);
        if ((ret = value.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch != 0)
        {
            //BerType
            if ((ret = value.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            //Len.
            if ((ret = value.GetUInt8(&ch)) != 0)
            {
                return ret;
            }

            if ((ret = value.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if (xml->GetOutputType() == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
            {
                str = CTranslatorSimpleTags::ReleaseResponseReasonToString((DLMS_RELEASE_RESPONSE_REASON)ch);
            }
            else
            {
                str = CTranslatorStandardTags::ReleaseResponseReasonToString((DLMS_RELEASE_RESPONSE_REASON)ch);
            }
            xml->AppendLine(DLMS_TRANSLATOR_TAGS_REASON, "Value", str);
            if (value.Available() != 0)
            {
                DLMS_ASSOCIATION_RESULT result;
                DLMS_SOURCE_DIAGNOSTIC diagnostic;
                ret = APDU::ParsePDU2(settings, settings.GetCipher(), value, result, diagnostic, xml);
            }
        }
        xml->AppendEndTag(cmd);
        break;
    case DLMS_COMMAND_GLO_READ_REQUEST:
    case DLMS_COMMAND_GLO_WRITE_REQUEST:
    case DLMS_COMMAND_GLO_GET_REQUEST:
    case DLMS_COMMAND_GLO_SET_REQUEST:
    case DLMS_COMMAND_GLO_READ_RESPONSE:
    case DLMS_COMMAND_GLO_WRITE_RESPONSE:
    case DLMS_COMMAND_GLO_GET_RESPONSE:
    case DLMS_COMMAND_GLO_SET_RESPONSE:
    case DLMS_COMMAND_GLO_METHOD_REQUEST:
    case DLMS_COMMAND_GLO_METHOD_RESPONSE:
    case DLMS_COMMAND_DED_GET_REQUEST:
    case DLMS_COMMAND_DED_SET_REQUEST:
    case DLMS_COMMAND_DED_GET_RESPONSE:
    case DLMS_COMMAND_DED_SET_RESPONSE:
    case DLMS_COMMAND_DED_METHOD_REQUEST:
    case DLMS_COMMAND_DED_METHOD_RESPONSE:
    case DLMS_COMMAND_GLO_CONFIRMED_SERVICE_ERROR:
    case DLMS_COMMAND_DED_CONFIRMED_SERVICE_ERROR:
        if (settings.GetCipher() != NULL && m_Comments)
        {
            int originalPosition = value.GetPosition();
            int len2 = xml->GetXmlLength();
            value.SetPosition(value.GetPosition() - 1);
            DLMS_COMMAND c = (DLMS_COMMAND)cmd;
            ByteBuffer* st;
            if (c == DLMS_COMMAND_GLO_READ_REQUEST || c == DLMS_COMMAND_GLO_WRITE_REQUEST || c == DLMS_COMMAND_GLO_GET_REQUEST || c == DLMS_COMMAND_GLO_SET_REQUEST || c == DLMS_COMMAND_GLO_METHOD_REQUEST ||
                c == DLMS_COMMAND_DED_GET_REQUEST || c == DLMS_COMMAND_DED_SET_REQUEST || c == DLMS_COMMAND_DED_METHOD_REQUEST)
            {
                st = &settings.GetCipher()->GetSystemTitle();
            }
            else
            {
                st = &settings.GetSourceSystemTitle();
            }
            if (st->GetSize() != 0)
            {
                DLMS_SECURITY security = DLMS_SECURITY_NONE;
                DLMS_SECURITY_SUITE suite;
                uint64_t InvocationCounter;
                if ((ret = settings.GetCipher()->Decrypt(*st, value,
                    settings.GetCipher()->GetBlockCipherKey(),
                    security, suite, InvocationCounter)) != 0)
                {
                    // It's OK if this fails. Ciphering settings are not correct.
                    xml->SetXmlLength(len2);
                }
                else
                {
                    xml->StartComment("Decrypt data: " + value.ToString());
                    PduToXml(xml, value, omitDeclaration, omitNameSpace, false, output);
                    xml->EndComment();
                }
            }
            value.SetPosition(originalPosition);
        }

        unsigned long cnt;
        if ((ret = Helpers::GetObjectCount(value, cnt)) != 0)
        {
            return ret;
        }
        if (cnt != value.GetSize() - value.GetPosition())
        {
            std::string tmp = "Invalid length: ";
            tmp.append(Helpers::IntToString(cnt));
            tmp.append(". It should be: ");
            tmp.append(Helpers::IntToString(value.Available()));
            xml->AppendComment(tmp);
        }
        str = value.ToHexString(value.GetPosition(), value.Available(), false);
        xml->AppendLine(cmd, "", str);
        break;
    case DLMS_COMMAND_GENERAL_GLO_CIPHERING:
        if (settings.GetCipher() != NULL && m_Comments)
        {
            DLMS_SECURITY security;
            DLMS_SECURITY_SUITE suite;
            uint64_t invocationCounter;
            int len2 = xml->GetXmlLength();
            int originalPosition = value.GetPosition();
            value.SetPosition(value.GetPosition() - 1);
            //Check is this client msg.
            ByteBuffer& st = settings.GetCipher()->GetSystemTitle();
            if (st.GetSize() != 0)
            {
                data.GetData().SetPosition(data.GetData().GetPosition() - 1);
                if (settings.GetCipher()->Decrypt(settings.GetSourceSystemTitle(), data.GetData(),
                    settings.GetCipher()->GetBlockCipherKey(),
                    security, suite, invocationCounter) == 0)
                {
                    xml->StartComment("Decrypt data: " + data.GetData().ToHexString());
                    ByteBuffer& bb = data.GetData();
                    ret = PduToXml(xml, bb, omitDeclaration, omitNameSpace, false, output);
                    xml->EndComment();
                }
            }
            if (ret != 0)
            {
                xml->SetXmlLength(len2);
                value.SetPosition(originalPosition);
                value.SetPosition(value.GetPosition() - 1);
                //Check is this server msg.
                st = settings.GetSourceSystemTitle();
                if (st.GetSize() != 0)
                {
                    if (settings.GetCipher()->Decrypt(settings.GetSourceSystemTitle(), data.GetData(),
                        settings.GetCipher()->GetBlockCipherKey(),
                        security, suite, invocationCounter) == 0)
                    {
                        xml->StartComment("Decrypt data: " + data.GetData().ToHexString());
                        ret = PduToXml(xml, data.GetData(), omitDeclaration, omitNameSpace, false, output);
                        xml->EndComment();
                    }
                    else
                    {
                        // It's OK if this fails. Ciphering settings are not correct.
                        xml->SetXmlLength(len2);
                    }
                }
            }
            value.SetPosition(originalPosition);
        }
        if ((ret = Helpers::GetObjectCount(value, len)) != 0)
        {
            return ret;
        }
        tmp.Clear();
        tmp.Set(&value, value.GetPosition(), len);
        xml->AppendStartTag(DLMS_COMMAND_GENERAL_GLO_CIPHERING);
        str = tmp.ToHexString(0, len, false);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_SYSTEM_TITLE, "", str);
        if ((ret = Helpers::GetObjectCount(value, len)) != 0)
        {
            return ret;
        }
        tmp.Clear();
        tmp.Set(&value, value.GetPosition(), len);
        str = tmp.ToHexString(false);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_CIPHERED_SERVICE, "", str);
        xml->AppendEndTag(DLMS_COMMAND_GENERAL_GLO_CIPHERING);
        break;
    case DLMS_COMMAND_CONFIRMED_SERVICE_ERROR:
        data.SetXml(xml);
        data.SetData(value);
        ret = DLMS::HandleConfirmedServiceError(data);
        break;
    case DLMS_COMMAND_GATEWAY_REQUEST:
    case DLMS_COMMAND_GATEWAY_RESPONSE:
    {
        data.SetXml(xml);
        data.SetData(value);
        //Get Network ID.
        unsigned char id;
        if ((ret = value.GetUInt8(&id)) != 0)
        {
            return ret;
        }
        //Get Physical device address.
        if ((ret = Helpers::GetObjectCount(value, len)) != 0)
        {
            return ret;
        }
        tmp.Clear();
        tmp.Set(&value, value.GetPosition(), len);
        xml->AppendStartTag((DLMS_COMMAND)cmd);
        str = Helpers::IntToString(id);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_NETWORK_ID, NULL, str);
        str = tmp.ToHexString(0, len, false);
        xml->AppendLine(DLMS_TRANSLATOR_TAGS_PHYSICAL_DEVICE_ADDRESS, NULL, str);
        ByteBuffer bb(value);
        PduToXml(xml, bb, omitDeclaration, omitNameSpace, allowUnknownCommand, output);
        xml->AppendEndTag(cmd);
    }
    break;
    case DLMS_COMMAND_EXCEPTION_RESPONSE:
        data.SetXml(xml);
        data.SetData(value);
        DLMS::HandleExceptionResponse(data);
        break;
    default:
        if (!allowUnknownCommand)
        {
            return DLMS_ERROR_CODE_INVALID_COMMAND;
        }
        str = "<Data=\"";
        value.SetPosition(value.GetPosition() - 1);
        str.append(value.ToHexString(value.GetPosition(), value.Available(), false));
        str.append("\" />");
        xml->AppendLine(str);
        break;
    }
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
    {
        std::ostringstream sb;
        if (!omitDeclaration)
        {
            sb << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
        }
        if (!omitNameSpace)
        {
            if (cmd != DLMS_COMMAND_AARE && cmd != DLMS_COMMAND_AARQ)
            {
                sb << "<x:xDLMS-APDU xmlns:x=\"http://www.dlms.com/COSEMpdu\">\r\n";
            }
            else
            {
                sb << "<x:aCSE-APDU xmlns:x=\"http://www.dlms.com/COSEMpdu\">\r\n";
            }
        }
        sb << xml->ToString();
        //    sb << "\r\n";
        if (!omitNameSpace)
        {
            if (cmd != DLMS_COMMAND_AARE && cmd != DLMS_COMMAND_AARQ)
            {
                sb << "</x:xDLMS-APDU>\r\n";
            }
            else
            {
                sb << "</x:aCSE-APDU>\r\n";
            }
        }
        output = sb.str();
        return 0;
    }
    output = xml->ToString();
    return 0;
}

DLMS_ERROR_CODE DLMSTranslator::ValueOfErrorCode(DLMS_TRANSLATOR_OUTPUT_TYPE type, std::string& value)
{
    if (type == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
    {
        return CTranslatorStandardTags::ValueOfErrorCode(value);
    }
    else
    {
        return CTranslatorSimpleTags::ValueOfErrorCode(value);
    }
}

int DLMSTranslator::ErrorCodeToString(DLMS_TRANSLATOR_OUTPUT_TYPE type, DLMS_ERROR_CODE value, std::string& result)
{
    if (type == DLMS_TRANSLATOR_OUTPUT_TYPE_STANDARD_XML)
    {
        return CTranslatorStandardTags::ErrorCodeToString(value, result);
    }
    else
    {
        return CTranslatorSimpleTags::ErrorCodeToString(value, result);
    }
}

int DLMSTranslator::DataToXml(ByteBuffer& data, std::string& xml)
{
    int ret;
    DLMSVariant value;
    DataInfo di;
    DLMSSettings settings(false);
    DLMSTranslatorStructure tmp(m_OutputType, m_OmitXmlNameSpace, m_Hex, m_ShowStringAsHex, m_Comments, m_Tags);
    di.SetXml(&tmp);
    ret = Helpers::GetData(&settings, data, di, value);
    xml = di.GetXml()->ToString();
    return ret;
}

ByteBuffer& DLMSTranslator::GetServerSystemTitle()
{
    return m_ServerSystemTitle;
}

void DLMSTranslator::SetServerSystemTitle(ByteBuffer& value)
{
    m_ServerSystemTitle.Clear();
    m_ServerSystemTitle.Set(value.GetData(), value.GetSize() - value.GetPosition());
}

ByteBuffer& DLMSTranslator::GetDedicatedKey()
{
    return m_DedicatedKey;
}

void DLMSTranslator::SetDedicatedKey(ByteBuffer& value)
{
    m_DedicatedKey.Clear();
    m_DedicatedKey.Set(value.GetData(), value.GetSize() - value.GetPosition());
}
#endif //DLMS_IGNORE_XML_TRANSLATOR
