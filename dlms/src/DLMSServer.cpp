#include "ServerReply.h"
#include "DLMSServer.h"
#include "DLMS.h"
#include "DLMSObjectFactory.h"
#include "DLMSProfileGeneric.h"
#include "DLMSAssociationShortName.h"
#include "DLMSAssociationLogicalName.h"
#include "DLMSClient.h"
#include "APDU.h"
#include "Secure.h"
#include "DLMSValueEventCollection.h"
#include "DLMSLNCommandHandler.h"
#include "DLMSSNCommandHandler.h"

DLMSServer::DLMSServer(bool logicalNameReferencing,
    DLMS_INTERFACE_TYPE type) : m_Transaction(NULL), m_Settings(true)
{
#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
    m_Hdlc = NULL;
#endif //DLMS_IGNORE_IEC_HDLC_SETUP
#ifndef DLMS_IGNORE_TCP_UDP_SETUP
    m_Wrapper = NULL;
#endif //DLMS_IGNORE_TCP_UDP_SETUP
    m_DataReceived = 0;
    m_Settings.SetUseLogicalNameReferencing(logicalNameReferencing);
    m_Settings.SetInterfaceType(type);
    if (GetUseLogicalNameReferencing())
    {
        SetConformance((DLMS_CONFORMANCE)(DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION |
            DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE |
            DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ |
            DLMS_CONFORMANCE_SET | DLMS_CONFORMANCE_SELECTIVE_ACCESS |
            DLMS_CONFORMANCE_ACTION | DLMS_CONFORMANCE_MULTIPLE_REFERENCES |
            DLMS_CONFORMANCE_GET | DLMS_CONFORMANCE_GENERAL_PROTECTION |
            DLMS_CONFORMANCE_ACCESS));
    }
    else
    {
        SetConformance((DLMS_CONFORMANCE)(DLMS_CONFORMANCE_INFORMATION_REPORT |
            DLMS_CONFORMANCE_READ | DLMS_CONFORMANCE_UN_CONFIRMED_WRITE |
            DLMS_CONFORMANCE_WRITE | DLMS_CONFORMANCE_PARAMETERIZED_ACCESS |
            DLMS_CONFORMANCE_MULTIPLE_REFERENCES |
            DLMS_CONFORMANCE_GENERAL_PROTECTION));
    }
    Reset();
}

#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
DLMSServer::DLMSServer(
    DLMSAssociationLogicalName* ln,
    DLMSIecHdlcSetup* hdlc) :
    DLMSServer(true, DLMS_INTERFACE_TYPE_HDLC)
{
    m_Settings.GetObjects().push_back(ln);
    m_Settings.GetObjects().push_back(hdlc);
    m_Hdlc = hdlc;
}

DLMSServer::DLMSServer(
    DLMSAssociationLogicalName* ln,
    DLMSTcpUdpSetup* wrapper) :
    DLMSServer(true, DLMS_INTERFACE_TYPE_WRAPPER)
{
    m_Settings.GetObjects().push_back(ln);
    m_Settings.GetObjects().push_back(wrapper);
    m_Wrapper = wrapper;
}
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME

#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
DLMSServer::DLMSServer(
    DLMSAssociationShortName* sn,
    DLMSIecHdlcSetup* hdlc) :
    DLMSServer(false, DLMS_INTERFACE_TYPE_HDLC)
{
    m_Settings.GetObjects().push_back(sn);
    m_Settings.GetObjects().push_back(hdlc);
    m_Hdlc = hdlc;
}

DLMSServer::DLMSServer(
    DLMSAssociationShortName* sn,
    DLMSTcpUdpSetup* wrapper) :
    DLMSServer(false, DLMS_INTERFACE_TYPE_WRAPPER)
{
    m_Settings.GetObjects().push_back(sn);
    m_Settings.GetObjects().push_back(wrapper);
    m_Wrapper = wrapper;
}
#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME

DLMSServer::~DLMSServer()
{

}

unsigned long DLMSServer::GetPushClientAddress()
{
    return m_Settings.GetPushClientAddress();
}

void DLMSServer::SetPushClientAddress(unsigned long value)
{
    m_Settings.SetPushClientAddress(value);
}

#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
DLMSIecHdlcSetup* DLMSServer::GetHdlc()
{
    return m_Hdlc;
}

void DLMSServer::SetHdlc(DLMSIecHdlcSetup* value)
{
    m_Hdlc = value;
}
#endif //DLMS_IGNORE_IEC_HDLC_SETUP

#ifndef DLMS_IGNORE_TCP_UDP_SETUP

DLMSTcpUdpSetup* DLMSServer::GetWrapper()
{
    return m_Wrapper;
}

void DLMSServer::SetWrapper(DLMSTcpUdpSetup* value)
{
    m_Wrapper = value;
}
#endif //DLMS_IGNORE_TCP_UDP_SETUP

DLMSObjectCollection& DLMSServer::GetItems()
{
    return m_Settings.GetObjects();
}

void DLMSServer::SetCipher(Cipher* value)
{
    m_Settings.SetCipher(value);
}

DLMSSettings& DLMSServer::GetSettings()
{
    return m_Settings;
}

ByteBuffer& DLMSServer::GetStoCChallenge()
{
    return m_Settings.GetStoCChallenge();
}

void DLMSServer::SetStoCChallenge(ByteBuffer& value)
{
    m_Settings.SetUseCustomChallenge(value.GetSize() != 0);
    m_Settings.SetStoCChallenge(value);
}

DLMS_INTERFACE_TYPE DLMSServer::GetInterfaceType()
{
    return m_Settings.GetInterfaceType();
}

void DLMSServer::SetStartingPacketIndex(int value)
{
    m_Settings.SetBlockIndex(value);
}

int DLMSServer::GetInvokeID()
{
    return m_Settings.GetInvokeID();
}

DLMSLimits& DLMSServer::GetLimits()
{
    return m_Settings.GetHdlcSettings();
}

HdlcSettings& DLMSServer::GetHdlcSettings()
{
    return m_Settings.GetHdlcSettings();
}

unsigned short DLMSServer::GetMaxReceivePDUSize()
{
    return m_Settings.GetMaxServerPDUSize();
}

void DLMSServer::SetMaxReceivePDUSize(unsigned short value)
{
    m_Settings.SetMaxServerPDUSize(value);
}

bool DLMSServer::GetUseLogicalNameReferencing()
{
    return m_Settings.GetUseLogicalNameReferencing();
}

/**
 * @param value
 *            Is Logical Name referencing used.
 */
void DLMSServer::SetUseLogicalNameReferencing(bool value)
{
    m_Settings.SetUseLogicalNameReferencing(value);
}

int DLMSServer::Initialize()
{
    DLMSObject* associationObject = NULL;
    m_Initialized = true;
    std::string ln;
    for (DLMSObjectCollection::iterator it = m_Settings.GetObjects().begin(); it != m_Settings.GetObjects().end(); ++it)
    {
        (*it)->GetLogicalName(ln);
        if (ln.size() == 0)
        {
            //Invalid Logical Name.
            return DLMS_ERROR_CODE_INVALID_LOGICAL_NAME;
        }
        if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            DLMSProfileGeneric* pg = (DLMSProfileGeneric*)(*it);
            if (pg->GetProfileEntries() < 1)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            if (pg->GetCapturePeriod() > 0)
            {
                //TODO: Start thread. new GXProfileGenericUpdater(this, pg).start();
            }
        }
#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
        else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME
            && !m_Settings.GetUseLogicalNameReferencing())
        {
            DLMSObjectCollection& list = ((DLMSAssociationShortName*)*it)->GetObjectList();
            if (list.size() == 0)
            {
                list.insert(list.end(), GetItems().begin(), GetItems().end());
            }
            associationObject = *it;
        }
#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME
#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
        else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME
            && m_Settings.GetUseLogicalNameReferencing())
        {
            DLMSObjectCollection& list = ((DLMSAssociationLogicalName*)*it)->GetObjectList();
            if (list.size() == 0)
            {
                list.insert(list.end(), GetItems().begin(), GetItems().end());
            }
            associationObject = *it;
            ((DLMSAssociationLogicalName*)*it)->GetXDLMSContextInfo().SetMaxReceivePduSize(m_Settings.GetMaxServerPDUSize());
            ((DLMSAssociationLogicalName*)*it)->GetXDLMSContextInfo().SetMaxSendPduSize(m_Settings.GetMaxServerPDUSize());
            ((DLMSAssociationLogicalName*)*it)->GetXDLMSContextInfo().SetConformance(m_Settings.GetProposedConformance());
        }
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
    }
    if (associationObject == NULL)
    {
        if (GetUseLogicalNameReferencing())
        {
#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
            DLMSAssociationLogicalName* it2 = (DLMSAssociationLogicalName*)DLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME);
            DLMSObjectCollection& list = it2->GetObjectList();
            GetItems().push_back(it2);
            list.insert(list.end(), GetItems().begin(), GetItems().end());
            ((DLMSAssociationLogicalName*)it2)->GetXDLMSContextInfo().SetMaxReceivePduSize(m_Settings.GetMaxServerPDUSize());
            ((DLMSAssociationLogicalName*)it2)->GetXDLMSContextInfo().SetMaxSendPduSize(m_Settings.GetMaxServerPDUSize());
            ((DLMSAssociationLogicalName*)it2)->GetXDLMSContextInfo().SetConformance(m_Settings.GetProposedConformance());
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
        }
        else
        {
#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
            DLMSAssociationShortName* it2 = (DLMSAssociationShortName*)DLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME);
            DLMSObjectCollection& list = it2->GetObjectList();
            GetItems().push_back(it2);
            list.insert(list.end(), GetItems().begin(), GetItems().end());
#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME
        }
    }
    // Arrange items by Short Name.

    if (!m_Settings.GetUseLogicalNameReferencing())
    {
        UpdateShortNames(false);
    }
    return 0;
}


int DLMSServer::UpdateShortNames()
{
    return UpdateShortNames(true);
}

int DLMSServer::UpdateShortNames(bool force)
{
    int ret;
    short sn = 0xA0;
    unsigned char offset, count;
    for (DLMSObjectCollection::iterator it = m_Settings.GetObjects().begin();
        it != m_Settings.GetObjects().end(); ++it)
    {
        if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME ||
            (*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME)
        {
            continue;
        }
        // Generate Short Name if not given.
        if (force || (*it)->GetShortName() == 0)
        {
            (*it)->SetShortName(sn);
            // Add method index addresses.
            if ((ret = DLMS::GetActionInfo((*it)->GetObjectType(), offset, count)) != 0)
            {
                return ret;
            }
            if (count != 0)
            {
                sn += offset + (8 * count);
            }
            else
            {
                // If there are no methods.
                // Add attribute index addresses.
                sn += 8 * (*it)->GetAttributeCount();
            }
        }
        else
        {
            sn = (*it)->GetShortName();
        }
    }
    return 0;
}

void DLMSServer::Reset(bool connected)
{
    if (m_Transaction != NULL)
    {
        delete m_Transaction;
        m_Transaction = NULL;
    }
    m_Settings.SetProtocolVersion(NULL);
    m_Settings.SetCount(0);
    m_Settings.SetIndex(0);
    m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
    m_ReceivedData.Clear();
    m_ReplyData.Clear();
    if (!connected)
    {
        m_Info.Clear();
        m_Settings.SetServerAddress(0);
        m_Settings.SetClientAddress(0);
    }

    m_Settings.SetAuthentication(DLMS_AUTHENTICATION_NONE);
    if (m_Settings.GetCipher() != NULL)
    {
        if (!connected)
        {
            m_Settings.GetCipher()->Reset();
        }
        else
        {
            m_Settings.GetCipher()->SetSecurity(DLMS_SECURITY_NONE);
        }
    }
}

void DLMSServer::Reset()
{
    Reset(false);
}

/**
    * Parse AARQ request that client send and returns AARE request.
    *
    * @return Reply to the client.
    */
int DLMSServer::HandleAarqRequest(
    ByteBuffer& data,
    DLMSConnectionEventArgs& connectionInfo)
{
    int ret;
    ByteBuffer error;
    DLMS_ASSOCIATION_RESULT result;
    DLMS_SOURCE_DIAGNOSTIC diagnostic;
    m_Settings.GetCtoSChallenge().Clear();
    if (!m_Settings.GetUseCustomChallenge())
    {
        m_Settings.GetStoCChallenge().Clear();
    }
    if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        Reset(true);
    }
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    ret = APDU::ParsePDU(m_Settings, m_Settings.GetCipher(), data, result, diagnostic, NULL);
#else
    ret = CGXAPDU::ParsePDU(m_Settings, m_Settings.GetCipher(), data, result, diagnostic);
#endif //DLMS_IGNORE_XML_TRANSLATOR

    if (ret == DLMS_ERROR_CODE_INVOCATION_COUNTER_TOO_SMALL ||
        ret == DLMS_ERROR_CODE_INVALID_DECIPHERING_ERROR ||
        ret == DLMS_ERROR_CODE_INVALID_SECURITY_SUITE)
    {
        if (DLMS::UseHdlc(m_Settings.GetInterfaceType()))
        {
            m_ReplyData.Set(LLC_REPLY_BYTES, 3);
        }
        m_ReplyData.SetUInt8(DLMS_COMMAND_EXCEPTION_RESPONSE);
        m_ReplyData.SetUInt8(DLMS_EXCEPTION_STATE_ERROR_SERVICE_UNKNOWN);
        if (ret == DLMS_ERROR_CODE_INVOCATION_COUNTER_TOO_SMALL)
        {
            m_ReplyData.SetUInt8(DLMS_EXCEPTION_SERVICE_ERROR_INVOCATION_COUNTER_ERROR);
            m_ReplyData.SetUInt32((uint32_t)m_Settings.GetExpectedInvocationCounter());
        }
        else
        {
            m_ReplyData.SetUInt8(DLMS_EXCEPTION_SERVICE_ERROR_DECIPHERING_ERROR);
        }
        return 0;
    }
    if (ret != 0)
    {
        return ret;
    }
    if (result == DLMS_ASSOCIATION_RESULT_ACCEPTED)
    {
        if (m_Settings.GetDLMSVersion() != 6)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
            error.SetUInt8(0xE);
            error.SetUInt8(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR);
            error.SetUInt8(DLMS_SERVICE_ERROR_INITIATE);
            error.SetUInt8(DLMS_INITIATE_DLMS_VERSION_TOO_LOW);
            m_Settings.SetDLMSVersion(6);
        }
        else if (m_Settings.GetMaxPduSize() < 64)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
            error.SetUInt8(0xE);
            error.SetUInt8(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR);
            error.SetUInt8(DLMS_SERVICE_ERROR_INITIATE);
            error.SetUInt8(DLMS_INITIATE_PDU_SIZE_TOOSHORT);
            m_Settings.SetMaxReceivePDUSize(64);
        }
        else if (m_Settings.GetNegotiatedConformance() == 0)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN;
            error.SetUInt8(0xE);
            error.SetUInt8(DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR);
            error.SetUInt8(DLMS_SERVICE_ERROR_INITIATE);
            error.SetUInt8(DLMS_INITIATE_INCOMPATIBLE_CONFORMANCE);
        }
        else if (diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED;
            InvalidConnection(connectionInfo);
        }
        else
        {
            diagnostic = ValidateAuthentication(m_Settings.GetAuthentication(), m_Settings.GetPassword());
            if (diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
            {
                result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
                InvalidConnection(connectionInfo);
            }
            else if (m_Settings.GetAuthentication() > DLMS_AUTHENTICATION_LOW)
            {
                // If High authentication is used.
                result = DLMS_ASSOCIATION_RESULT_ACCEPTED;
                diagnostic = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED;
            }
            else
            {
                Connected(connectionInfo);
                m_Settings.SetConnected((DLMS_CONNECTION_STATE)(m_Settings.GetConnected() | DLMS_CONNECTION_STATE_DLMS));
            }
        }
    }
    if (m_Settings.GetAuthentication() > DLMS_AUTHENTICATION_LOW)
    {
        // If High authentication is used.
        if (!m_Settings.GetUseCustomChallenge())
        {
            ByteBuffer challenge;
            if ((ret = Secure::GenerateChallenge(m_Settings.GetAuthentication(), challenge)) != 0)
            {
                return ret;
            }
            m_Settings.SetStoCChallenge(challenge);
        }
        if (m_Settings.GetUseLogicalNameReferencing())
        {
#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
            unsigned char l[] = { 0,0,40,0,0,255 };
            DLMSAssociationLogicalName* ln = (DLMSAssociationLogicalName*)m_Settings.GetObjects().FindByLN(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, l);
            if (ln != NULL)
            {
                if (m_Settings.GetCipher() == NULL || m_Settings.GetCipher()->GetSecurity() == DLMS_SECURITY_NONE)
                {
                    ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME);
                }
                else
                {
                    ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING);
                }
                ln->GetAuthenticationMechanismName().SetMechanismId(m_Settings.GetAuthentication());
                ln->SetAssociationStatus(DLMS_ASSOCIATION_STATUS_ASSOCIATION_PENDING);
            }
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
        }
    }
    else
    {
#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
        unsigned char l[] = { 0,0,40,0,0,255 };
        DLMSAssociationLogicalName* ln = (DLMSAssociationLogicalName*)m_Settings.GetObjects().FindByLN(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, l);
        if (ln != NULL)
        {
            if (m_Settings.GetCipher() == NULL || m_Settings.GetCipher()->GetSecurity() == DLMS_SECURITY_NONE)
            {
                ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME);
            }
            else
            {
                ln->GetApplicationContextName().SetContextId(DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING);
            }
            ln->GetAuthenticationMechanismName().SetMechanismId(m_Settings.GetAuthentication());
            ln->SetAssociationStatus(DLMS_ASSOCIATION_STATUS_ASSOCIATED);
        }
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
    }
    if (DLMS::UseHdlc(m_Settings.GetInterfaceType()))
    {
        m_ReplyData.Set(LLC_REPLY_BYTES, 3);
    }
    // Generate AARE packet.
    return APDU::GenerateAARE(m_Settings, m_ReplyData, result, diagnostic, m_Settings.GetCipher(), &error, NULL);
}

/**
 * Parse SNRM Request. If server do not accept client empty byte array is
 * returned.
 *
 * @return Returns returned UA packet.
 */
int DLMSServer::HandleSnrmRequest(
    DLMSSettings& settings,
    ByteBuffer& data,
    ByteBuffer& reply)
{
    int ret;
    Reset(true);
    if ((ret = DLMS::ParseSnrmUaResponse(data, &m_Settings.GetHdlcSettings())) != 0)
    {
        return ret;
    }
    reply.SetUInt8(0x81); // FromatID
    reply.SetUInt8(0x80); // GroupID
    reply.SetUInt8(0); // Length

#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
    if (m_Hdlc != NULL)
    {
        //If client wants send larger HDLC frames what meter accepts.
        if (m_Settings.GetHdlcSettings().GetMaxInfoTX() > m_Hdlc->GetMaximumInfoLengthReceive())
        {
            m_Settings.GetHdlcSettings().SetMaxInfoTX(m_Hdlc->GetMaximumInfoLengthReceive());
        }
        //If client wants receive larger HDLC frames what meter accepts.
        if (m_Settings.GetHdlcSettings().GetMaxInfoRX() > m_Hdlc->GetMaximumInfoLengthTransmit())
        {
            m_Settings.GetHdlcSettings().SetMaxInfoRX(m_Hdlc->GetMaximumInfoLengthTransmit());
        }
        //If client asks higher window size what meter accepts.
        if (m_Settings.GetHdlcSettings().GetWindowSizeTX() > m_Hdlc->GetWindowSizeReceive())
        {
            m_Settings.GetHdlcSettings().SetWindowSizeTX(m_Hdlc->GetWindowSizeReceive());
        }
        //If client asks higher window size what meter accepts.
        if (m_Settings.GetHdlcSettings().GetWindowSizeRX() > m_Hdlc->GetWindowSizeTransmit())
        {
            m_Settings.GetHdlcSettings().SetWindowSizeRX(m_Hdlc->GetWindowSizeTransmit());
        }
    }
#endif //DLMS_IGNORE_IEC_HDLC_SETUP

    reply.SetUInt8(HDLC_INFO_MAX_INFO_TX);
    DLMS::AppendHdlcParameter(reply, GetLimits().GetMaxInfoTX());

    reply.SetUInt8(HDLC_INFO_MAX_INFO_RX);
    DLMS::AppendHdlcParameter(reply, GetLimits().GetMaxInfoRX());

    reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
    reply.SetUInt8(4);
    reply.SetUInt32(m_Settings.GetHdlcSettings().GetWindowSizeTX());

    reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
    reply.SetUInt8(4);
    reply.SetUInt32(m_Settings.GetHdlcSettings().GetWindowSizeRX());
    int len = reply.GetSize() - 3;
    reply.SetUInt8(2, len); // Length
    return ret;
}

/**
 * Generates disconnect request.
 *
 * @return Disconnect request.
 */
int GenerateDisconnectRequest(DLMSSettings& settings, ByteBuffer& reply)
{
    if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        reply.SetUInt8(0x63);
        reply.SetUInt8(0x0);
    }
    else
    {
        reply.SetUInt8(0x81); // FromatID
        reply.SetUInt8(0x80); // GroupID
        reply.SetUInt8(0); // Length

        reply.SetUInt8(HDLC_INFO_MAX_INFO_TX);
        DLMS::AppendHdlcParameter(reply, settings.GetHdlcSettings().GetMaxInfoTX());

        reply.SetUInt8(HDLC_INFO_MAX_INFO_RX);
        DLMS::AppendHdlcParameter(reply, settings.GetHdlcSettings().GetMaxInfoRX());

        reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
        reply.SetUInt8(4);
        reply.SetUInt32(settings.GetHdlcSettings().GetWindowSizeTX());

        reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
        reply.SetUInt8(4);
        reply.SetUInt32(settings.GetHdlcSettings().GetWindowSizeRX());
        int len = reply.GetSize() - 3;
        reply.SetUInt8(2, len); // Length.
    }
    return 0;
}

int DLMSServer::ReportError(DLMS_COMMAND command, DLMS_ERROR_CODE error, ByteBuffer& reply)
{
    int ret;
    DLMS_COMMAND cmd;
    ByteBuffer data;
    switch (command)
    {
    case DLMS_COMMAND_READ_REQUEST:
        cmd = DLMS_COMMAND_READ_RESPONSE;
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
        cmd = DLMS_COMMAND_WRITE_RESPONSE;
        break;
    case DLMS_COMMAND_GET_REQUEST:
        cmd = DLMS_COMMAND_GET_RESPONSE;
        break;
    case DLMS_COMMAND_SET_REQUEST:
        cmd = DLMS_COMMAND_SET_RESPONSE;
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        cmd = DLMS_COMMAND_METHOD_RESPONSE;
        break;
    default:
        // Return HW error and close connection.
        cmd = DLMS_COMMAND_NONE;
        break;
    }

    if (m_Settings.GetUseLogicalNameReferencing())
    {
        DLMSLNParameters p(&m_Settings, 0, cmd, 1, NULL, NULL, error, m_Info.GetCipheredCommand());
        ret = DLMS::GetLNPdu(p, data);
    }
    else
    {
        ByteBuffer bb;
        bb.SetUInt8(error);
        DLMSSNParameters p(&m_Settings, cmd, 1, 1, NULL, &bb);
        ret = DLMS::GetSNPdu(p, data);
    }
    if (ret == 0)
    {
        if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
        {
            ret = DLMS::GetWrapperFrame(m_Settings, command, data, reply);
        }
        else
        {
            ret = DLMS::GetHdlcFrame(m_Settings, 0, &data, reply);
        }
    }
    return ret;
}

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
void DLMSServer::GenerateConfirmedServiceError(
    DLMS_CONFIRMED_SERVICE_ERROR service,
    DLMS_SERVICE_ERROR type,
    unsigned char code, ByteBuffer& data)
{
    data.SetUInt8(DLMS_COMMAND_CONFIRMED_SERVICE_ERROR);
    data.SetUInt8(service);
    data.SetUInt8(type);
    data.SetUInt8(code);
}

unsigned short DLMSServer::GetRowsToPdu(DLMSProfileGeneric* pg)
{
    DLMS_DATA_TYPE dt;
    int rowsize = 0;
    // Count how many rows we can fit to one PDU.
    for (std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >::iterator it = pg->GetCaptureObjects().begin();
        it != pg->GetCaptureObjects().end(); ++it)
    {
        it->first->GetDataType(it->second->GetAttributeIndex(), dt);
        if (dt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            it->first->GetUIDataType(it->second->GetAttributeIndex(), dt);
            if (dt == DLMS_DATA_TYPE_DATETIME)
            {
                rowsize += Helpers::GetDataTypeSize(DLMS_DATA_TYPE_DATETIME);
            }
            else if (dt == DLMS_DATA_TYPE_DATE)
            {
                rowsize += Helpers::GetDataTypeSize(DLMS_DATA_TYPE_DATE);
            }
            else if (dt == DLMS_DATA_TYPE_TIME)
            {
                rowsize += Helpers::GetDataTypeSize(DLMS_DATA_TYPE_TIME);
            }
        }
        else if (dt == DLMS_DATA_TYPE_NONE)
        {
            rowsize += 2;
        }
        else
        {
            rowsize += Helpers::GetDataTypeSize(dt);
        }
    }
    if (rowsize != 0)
    {
        return m_Settings.GetMaxPduSize() / rowsize;
    }
    return 0;
}

/**
* Handles release request.
*
* @param data
*            Received data.
* @param connectionInfo
*            Connection info.
*/
int DLMSServer::HandleReleaseRequest(ByteBuffer& data)
{
    if (DLMS::UseHdlc(m_Settings.GetInterfaceType()))
    {
        m_ReplyData.Set(LLC_REPLY_BYTES, 3);
    }
    ByteBuffer tmp;
    int ret = APDU::GetUserInformation(m_Settings, m_Settings.GetCipher(), tmp);
    if (ret != 0)
    {
        return ret;
    }
    m_ReplyData.SetUInt8(0x63);
    //Len.
    m_ReplyData.SetUInt8((unsigned char)(tmp.GetSize() + 3));
    m_ReplyData.SetUInt8(0x80);
    m_ReplyData.SetUInt8(0x01);
    m_ReplyData.SetUInt8(0x00);
    m_ReplyData.SetUInt8(0xBE);
    m_ReplyData.SetUInt8((unsigned char)(tmp.GetSize() + 1));
    m_ReplyData.SetUInt8(4);
    m_ReplyData.SetUInt8((unsigned char)tmp.GetSize());
    m_ReplyData.Set(&tmp, 0, tmp.GetSize());
    return 0;
}

int DLMSServer::HandleGeneralBlockTransfer(
    ServerReply& sr,
    ByteBuffer& data,
    unsigned char cipheredCommand)
{
    unsigned char bc;
    unsigned short blockNumber = 0, blockNumberAck = 0;
    if (!sr.IsStreaming())
    {
        unsigned long len;
        //BlockControl
        data.GetUInt8(&bc);
        //Block number.
        data.GetUInt16(&blockNumber);
        //Block number acknowledged.
        data.GetUInt16(&blockNumberAck);
        Helpers::GetObjectCount(data, len);
        if (len > data.Available())
        {
            GenerateConfirmedServiceError(
                DLMS_CONFIRMED_SERVICE_ERROR_INITIATE_ERROR,
                DLMS_SERVICE_ERROR_SERVICE,
                DLMS_SERVICE_UNSUPPORTED, m_ReplyData);
        }
    }
    if (m_Transaction != NULL)
    {
        if (m_Transaction->GetCommand() == DLMS_COMMAND_GET_REQUEST || m_Transaction->GetCommand() == DLMS_COMMAND_METHOD_REQUEST)
        {
            // Get request for next data block
            if (sr.GetCount() == 0)
            {
                m_Settings.SetBlockNumberAck(m_Settings.GetBlockNumberAck() + 1);
                sr.SetCount((bc & 0x3F));
            }
            DLMSLNCommandHandler::GetRequestNextDataBlock(m_Settings, 0,
                this, data, &m_ReplyData, NULL, true, cipheredCommand);
            if (sr.GetCount() != 0)
            {
                sr.SetCount(sr.GetCount() - 1);
            }
            if (m_Transaction == NULL)
            {
                sr.SetCount(0);
            }
            //Save server GBT window size to settings because sr is lost.
            if (m_Settings.IsServer())
            {
                m_Settings.SetCount(sr.GetCount());
            }
        }
        else
        {
            m_Transaction->GetData().Set(&data);
            // Send ACK.
            unsigned char igonoreAck = (bc & 0x40) != 0 &&
                (blockNumberAck * m_Settings.GetGbtWindowSize()) + 1 > blockNumber;
            int windowSize = m_Settings.GetGbtWindowSize();
            int bn = m_Settings.GetBlockIndex();
            if ((bc & 0x80) != 0)
            {
                HandleCommand(m_Transaction->GetCommand(),
                    m_Transaction->GetData(), sr, cipheredCommand);
                delete m_Transaction;
                m_Transaction = NULL;
                igonoreAck = false;
                windowSize = 1;
            }
            if (igonoreAck)
            {
                return false;
            }
            m_ReplyData.SetUInt8(DLMS_COMMAND_GENERAL_BLOCK_TRANSFER);
            m_ReplyData.SetUInt8((0x80 | windowSize));
            m_Settings.SetBlockIndex(m_Settings.GetBlockIndex() + 1);
            m_ReplyData.SetUInt16(bn);
            m_ReplyData.SetUInt16(blockNumber);
            m_ReplyData.SetUInt8(0);
        }
    }
    else
    {
        data.GetUInt8(&bc);
        m_Transaction = new DLMSLongTransaction((DLMS_COMMAND)bc, data);
        m_ReplyData.SetUInt8(DLMS_COMMAND_GENERAL_BLOCK_TRANSFER);
        m_ReplyData.SetUInt8((0x80 | m_Settings.GetGbtWindowSize()));
        m_ReplyData.SetUInt16(blockNumber);
        ++blockNumberAck;
        m_ReplyData.SetUInt16(blockNumberAck);
        m_ReplyData.SetUInt8(0);
    }
    return 0;
}

int DLMSServer::HandleCommand(
    DLMS_COMMAND cmd,
    ByteBuffer& data,
    ServerReply& sr,
    unsigned char cipheredCommand)
{
    int ret = 0;
    unsigned char frame = 0;
    if (DLMS::UseHdlc(m_Settings.GetInterfaceType()) && m_ReplyData.GetSize() != 0)
    {
        //Get next frame.
        frame = m_Settings.GetNextSend(false);
    }
    DLMSConnectionEventArgs& connectionInfo = sr.GetConnectionInfo();
    switch (cmd)
    {
    case DLMS_COMMAND_ACCESS_REQUEST:
        ret = DLMSLNCommandHandler::HandleAccessRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_SET_REQUEST:
        ret = DLMSLNCommandHandler::HandleSetRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
        ret = DLMSSNCommandHandler::HandleWriteRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_GET_REQUEST:
        if (data.GetSize() != 0)
        {
            ret = DLMSLNCommandHandler::HandleGetRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        }
        break;
    case DLMS_COMMAND_READ_REQUEST:
        ret = DLMSSNCommandHandler::HandleReadRequest(m_Settings, this, data, &m_ReplyData, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        ret = DLMSLNCommandHandler::HandleMethodRequest(m_Settings, this, data, &m_ReplyData, &connectionInfo, NULL, cipheredCommand);
        break;
    case DLMS_COMMAND_SNRM:
        ret = HandleSnrmRequest(m_Settings, data, m_ReplyData);
        m_Settings.SetConnected(DLMS_CONNECTION_STATE_HDLC);
        frame = DLMS_COMMAND_UA;
        break;
    case DLMS_COMMAND_AARQ:
        ret = HandleAarqRequest(data, connectionInfo);
        break;
    case DLMS_COMMAND_RELEASE_REQUEST:
        ret = HandleReleaseRequest(data);
        m_Settings.SetConnected((DLMS_CONNECTION_STATE)(m_Settings.GetConnected() & ~DLMS_CONNECTION_STATE_DLMS));
        Disconnected(connectionInfo);
        break;
    case DLMS_COMMAND_DISCONNECT_REQUEST:
        ret = GenerateDisconnectRequest(m_Settings, m_ReplyData);
        if (m_Settings.GetConnected() > DLMS_CONNECTION_STATE_NONE)
        {
            if (m_Settings.GetConnected() == DLMS_CONNECTION_STATE_DLMS)
            {
                Disconnected(connectionInfo);
            }
            m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
        }
        m_Settings.SetConnected(DLMS_CONNECTION_STATE_NONE);
        Disconnected(connectionInfo);
        frame = DLMS_COMMAND_UA;
        break;
    case DLMS_COMMAND_GENERAL_BLOCK_TRANSFER:
        ret = HandleGeneralBlockTransfer(sr, data, m_Info.GetCipheredCommand());
        break;
    case DLMS_COMMAND_DISCOVER_REQUEST:
    {
        DLMSPlcRegister reg;
        m_Settings.GetPlcSettings().ParseDiscoverRequest(data, reg);
        bool newMeter = m_Settings.GetPlcSettings().GetMacSourceAddress() == 0xFFE && m_Settings.GetPlcSettings().GetMacDestinationAddress() == 0xFFF;
        return m_Settings.GetPlcSettings().DiscoverReport(m_Settings.GetPlcSettings().GetSystemTitle(), newMeter, m_ReplyData);
    }
    case DLMS_COMMAND_REGISTER_REQUEST:
        m_Settings.GetPlcSettings().ParseRegisterRequest(data);
        return m_Settings.GetPlcSettings().DiscoverReport(m_Settings.GetPlcSettings().GetSystemTitle(), false, m_ReplyData);
    case DLMS_COMMAND_PING_REQUEST:
        break;
    case DLMS_COMMAND_NONE:
        // Client wants to get next block.
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_COMMAND;
        break;
    }
    if (ret == 0)
    {
        ByteBuffer& reply = sr.GetReply();
        if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
        {
            ret = DLMS::GetWrapperFrame(m_Settings, cmd, m_ReplyData, reply);
        }
        else
        {
            ret = DLMS::GetHdlcFrame(m_Settings, frame, &m_ReplyData, reply);
        }
    }
    if (cmd == DLMS_COMMAND_DISCONNECT_REQUEST ||
        (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER && cmd == DLMS_COMMAND_RELEASE_REQUEST))
    {
        Reset();
    }
    return ret;
}

int DLMSServer::HandleRequest(
    ByteBuffer& data,
    ByteBuffer& reply)
{
    return HandleRequest(
        data.GetData(),
        (unsigned short)(data.GetSize() - data.GetPosition()),
        reply);
}

int DLMSServer::HandleRequest(
    DLMSConnectionEventArgs& connectionInfo,
    ByteBuffer& data,
    ByteBuffer& reply)
{
    ServerReply sr(data);
    sr.SetConnectionInfo(connectionInfo);
    int ret = HandleRequest(sr);
    if (ret == 0)
    {
        reply.Set(&sr.GetReply());
    }
    return ret;
}

int DLMSServer::HandleRequest(
    unsigned char* buff,
    unsigned short size,
    ByteBuffer& reply)
{
    DLMSConnectionEventArgs connectionInfo;
    return HandleRequest(connectionInfo, buff, size, reply);
}

int DLMSServer::HandleRequest(
    DLMSConnectionEventArgs& connectionInfo,
    unsigned char* buff,
    unsigned short size,
    ByteBuffer& reply)
{
    ByteBuffer data;
    data.Set(buff, size);
    return HandleRequest(connectionInfo, data, reply);
}

int DLMSServer::HandleRequest(
    ServerReply& sr)
{
    int ret;
    if (!sr.IsStreaming() && sr.GetData().GetSize() == 0)
    {
        return 0;
    }
    if (!m_Initialized)
    {
        //Server not Initialized.
        return DLMS_ERROR_CODE_NOT_INITIALIZED;
    }
    m_ReceivedData.Set(&sr.GetData());
    ByteBuffer& reply = sr.GetReply();
    bool first = m_Settings.GetServerAddress() == 0
        && m_Settings.GetClientAddress() == 0;
    if ((ret = DLMS::GetData(m_Settings, m_ReceivedData, m_Info, NULL)) != 0)
    {
        //If all data is not received yet.
        if (ret == DLMS_ERROR_CODE_FALSE)
        {
            ret = 0;
        }
        else
        {
            ret = DLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_UNACCEPTABLE_FRAME, NULL, reply);
            m_ReceivedData.SetSize(0);
        }
        return ret;
    }
    // If all data is not received yet.
    if (!m_Info.IsComplete())
    {
        return 0;
    }
    m_ReceivedData.Clear();
    if (m_Info.GetCommand() == DLMS_COMMAND_DISCONNECT_REQUEST && m_Settings.GetConnected() == DLMS_CONNECTION_STATE_NONE)
    {
        ret = DLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_DISCONNECT_MODE, NULL, reply);
        m_Info.Clear();
        return ret;
    }

    if (first || m_Info.GetCommand() == DLMS_COMMAND_SNRM ||
        (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER && m_Info.GetCommand() == DLMS_COMMAND_AARQ))
    {
        // Check is data send to this server.
        if (!IsTarget(m_Settings.GetServerAddress(), m_Settings.GetClientAddress()))
        {
            m_Info.Clear();
            return 0;
        }
    }
    // If client want next frame.
    if ((m_Info.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) == DLMS_DATA_REQUEST_TYPES_FRAME)
    {
        m_DataReceived = (long)time(NULL);
        return DLMS::GetHdlcFrame(m_Settings, m_Settings.GetReceiverReady(), &m_ReplyData, reply);
    }
    // Update command if m_Transaction and next frame is asked.
    if (m_Info.GetCommand() == DLMS_COMMAND_NONE)
    {
        if (m_Transaction != NULL)
        {
            m_Info.SetCommand(m_Transaction->GetCommand());
        }
        else if (m_ReplyData.GetSize() == 0)
        {
            m_DataReceived = (long)time(NULL);
            return DLMS::GetHdlcFrame(m_Settings, m_Settings.GetReceiverReady(), NULL, reply);
        }
    }
    // Check inactivity time out.
#ifndef DLMS_IGNORE_IEC_HDLC_SETUP
    if (m_Hdlc != NULL && m_Hdlc->GetInactivityTimeout() != 0)
    {
        if (m_Info.GetCommand() != DLMS_COMMAND_SNRM)
        {
            long elapsed = (long)(time(NULL) - m_DataReceived);
            // If inactivity time out is elapsed.
            if (elapsed >= m_Hdlc->GetInactivityTimeout())
            {
                Reset();
                m_DataReceived = 0;
                return 0;
            }
        }
    }
#endif //DLMS_IGNORE_IEC_HDLC_SETUP
#ifndef DLMS_IGNORE_TCP_UDP_SETUP
    if (m_Wrapper != NULL && m_Wrapper->GetInactivityTimeout() != 0)
    {
        if (m_Info.GetCommand() != DLMS_COMMAND_AARQ)
        {
            long elapsed = (long)(time(NULL) - m_DataReceived);
            // If inactivity time out is elapsed.
            if (elapsed >= m_Wrapper->GetInactivityTimeout())
            {
                Reset();
                m_DataReceived = 0;
                return 0;
            }
        }
    }
#endif //DLMS_IGNORE_TCP_UDP_SETUP
    ret = HandleCommand(m_Info.GetCommand(), m_Info.GetData(), sr, m_Info.GetCipheredCommand());
    if (ret != 0)
    {
        ret = DLMS::GetHdlcFrame(m_Settings, DLMS_COMMAND_UNACCEPTABLE_FRAME, NULL, reply);
        m_ReceivedData.SetSize(0);
    }
    m_Info.Clear();
    m_DataReceived = (long)time(NULL);
    return ret;
}

DLMS_CONFORMANCE DLMSServer::GetConformance()
{
    return m_Settings.GetProposedConformance();
}

void DLMSServer::SetConformance(DLMS_CONFORMANCE value)
{
    m_Settings.SetProposedConformance(value);
}

int DLMSServer::AddData(
    DLMSObject* obj,
    unsigned char index,
    ByteBuffer& buff)
{
    int ret;
    DLMS_DATA_TYPE dt;
    DLMSValueEventArg e(NULL, obj, index);
    if ((ret = obj->GetValue(m_Settings, e)) != 0)
    {
        return ret;
    }
    if ((ret = obj->GetDataType(index, dt)) != 0)
    {
        return ret;
    }
    if (dt == DLMS_DATA_TYPE_ARRAY)
    {
        buff.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        return 0;
    }
    if (dt == DLMS_DATA_TYPE_NONE)
    {
        dt = e.GetValue().vt;
    }
    return Helpers::SetData(&m_Settings, buff, e.GetValue().vt, e.GetValue());
}

int DLMSServer::GenerateDataNotificationMessages(
    struct tm* time,
    ByteBuffer& data,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    if (GetUseLogicalNameReferencing())
    {
        DLMSLNParameters p(&m_Settings, 0, DLMS_COMMAND_DATA_NOTIFICATION, 0, NULL, &data, 0xff, DLMS_COMMAND_NONE);
        p.SetTime(time);
        ret = DLMS::GetLnMessages(p, reply);
    }
    else
    {
        DLMSSNParameters p(&m_Settings, DLMS_COMMAND_DATA_NOTIFICATION, 1, 0, &data, NULL);
        ret = DLMS::GetSnMessages(p, reply);
    }
    return ret;
}

int DLMSServer::GenerateDataNotificationMessages(
    struct tm* date,
    std::vector<std::pair<DLMSObject*, unsigned char> >& objects,
    std::vector<ByteBuffer>& reply)
{
    ByteBuffer buff;
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    Helpers::SetObjectCount((unsigned long)objects.size(), buff);
    for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = objects.begin(); it != objects.end(); ++it)
    {
        AddData(it->first, it->second, buff);
    }
    return GenerateDataNotificationMessages(date, buff, reply);
}

#ifndef DLMS_IGNORE_PUSH_SETUP
int DLMSServer::GeneratePushSetupMessages(
    struct tm* date,
    DLMSPushSetup* push,
    std::vector<ByteBuffer>& reply)
{
    if (push == NULL)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    ByteBuffer buff;
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    Helpers::SetObjectCount((unsigned long)push->GetPushObjectList().size(), buff);
    for (std::vector<std::pair<DLMSObject*, DLMSCaptureObject> >::iterator it = push->GetPushObjectList().begin(); it != push->GetPushObjectList().end(); ++it)
    {
        AddData(it->first, it->second.GetAttributeIndex(), buff);
    }
    return GenerateDataNotificationMessages(date, buff, reply);
}
#endif //DLMS_IGNORE_PUSH_SETUP

bool DLMSServer::GetUseUtc2NormalTime()
{
    return m_Settings.GetUseUtc2NormalTime();
}

void DLMSServer::SetUseUtc2NormalTime(bool value)
{
    m_Settings.SetUseUtc2NormalTime(value);
}

uint64_t DLMSServer::GetExpectedInvocationCounter()
{
    return m_Settings.GetExpectedInvocationCounter();
}

void DLMSServer::SetExpectedInvocationCounter(uint64_t value)
{
    m_Settings.SetExpectedInvocationCounter(value);
}

unsigned char DLMSServer::GetExpectedSecurityPolicy()
{
    return m_Settings.GetExpectedSecurityPolicy();
}

void DLMSServer::SetExpectedSecurityPolicy(unsigned char value)
{
    m_Settings.SetExpectedSecurityPolicy(value);
}

void DLMSServer::SetExpectedSecuritySuite(unsigned char value)
{
    m_Settings.SetExpectedSecuritySuite(value);
}

unsigned char DLMSServer::GetExpectedSecuritySuite()
{
    return m_Settings.GetExpectedSecuritySuite();
}

void DLMSServer::SetDateTimeSkips(DATETIME_SKIPS value)
{
    m_Settings.SetDateTimeSkips(value);
}

DATETIME_SKIPS DLMSServer::GetDateTimeSkips()
{
    return m_Settings.GetDateTimeSkips();
}
