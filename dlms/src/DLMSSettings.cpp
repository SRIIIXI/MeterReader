#include "DLMSSettings.h"

DLMSSettings::DLMSSettings(bool isServer) :
    m_Server(isServer),
    m_PlcSettings(this)
{
    m_UseCustomChallenge = false;
    m_BlockIndex = 1;
    m_Connected = DLMS_CONNECTION_STATE_NONE;
    m_DlmsVersionNumber = DLMS_VERSION;
    ResetFrameSequence();
    m_InvokeID = 1;
    m_LongInvokeID = 1;
    m_Priority = DLMS_PRIORITY_HIGH;
    m_ServiceClass = DLMS_SERVICE_CLASS_UN_CONFIRMED;
    m_ClientAddress = 0;
    m_ServerAddress = 0;
    m_PushClientAddress = 0;
    m_InterfaceType = DLMS_INTERFACE_TYPE_HDLC;
    m_Authentication = DLMS_AUTHENTICATION_NONE;
    m_MaxServerPDUSize = m_MaxReceivePDUSize = 0xFFFF;
    m_Cipher = NULL;
    m_SourceSystemTitle.Clear();
    m_Index = 0;
    m_Count = 0;
    m_ProposedConformance = m_NegotiatedConformance = (DLMS_CONFORMANCE)0;
    m_ProtocolVersion = NULL;
    m_QualityOfService = 0;
    m_UserId = 0;
    m_UseUtc2NormalTime = false;
    m_DateTimeSkips = DATETIME_SKIPS_NONE;
    m_BlockNumberAck = 0;
    m_GbtWindowSize = 1;
    m_AutoIncreaseInvokeID = false;
    m_ExpectedInvocationCounter = 0;
    m_ExpectedSecurityPolicy = 0xFF;
    m_ExpectedSecuritySuite = 0xFF;
}

//Destructor.
DLMSSettings::~DLMSSettings()
{
    m_Objects.Free();
    m_AllocatedObjects.Free();
}

ByteBuffer& DLMSSettings::GetCtoSChallenge()
{
    return m_CtoSChallenge;
}


void DLMSSettings::SetCtoSChallenge(ByteBuffer& value)
{
    m_CtoSChallenge = value;
}

ByteBuffer& DLMSSettings::GetStoCChallenge()
{
    return m_StoCChallenge;
}

void DLMSSettings::SetStoCChallenge(ByteBuffer& value)
{
    m_StoCChallenge = value;
}

DLMS_AUTHENTICATION DLMSSettings::GetAuthentication()
{
    return m_Authentication;
}

void DLMSSettings::SetAuthentication(DLMS_AUTHENTICATION value)
{
    m_Authentication = value;
}

ByteBuffer& DLMSSettings::GetPassword()
{
    return m_Password;
}

void DLMSSettings::SetPassword(ByteBuffer& value)
{
    m_Password = value;
}

unsigned char DLMSSettings::GetDlmsVersionNumber()
{
    return m_DlmsVersionNumber;
}

void DLMSSettings::SetDlmsVersionNumber(unsigned char value)
{
    m_DlmsVersionNumber = value;
}

void DLMSSettings::ResetFrameSequence()
{
    if (m_Server)
    {
        m_SenderFrame = SERVER_START_SENDER_FRAME_SEQUENCE;
        m_ReceiverFrame = SERVER_START_RECEIVER_FRAME_SEQUENCE;
    }
    else
    {
        m_SenderFrame = CLIENT_START_SENDER_FRAME_SEQUENCE;
        m_ReceiverFrame = CLIENT_START_RCEIVER_FRAME_SEQUENCE;
    }
}

// Increase receiver sequence.
//
// @param value
//            Frame value.
// Increased receiver frame sequence.
static unsigned char IncreaseReceiverSequence(unsigned char value)
{
    return ((value + 0x20) | 0x10 | (value & 0xE));
}

// Increase sender sequence.
//
// @param value
//            Frame value.
// Increased sender frame sequence.
static unsigned char IncreaseSendSequence(unsigned char value)
{
    return (unsigned char)((value & 0xF0) | ((value + 0x2) & 0xE));
}

bool DLMSSettings::CheckFrame(unsigned char frame)
{
    //If notify
    if (frame == 0x13)
    {
        return true;
    }
    // If U frame.
    if ((frame & 0x3) == 3)
    {
        if (frame == 0x73 || frame == 0x93)
        {
            ResetFrameSequence();
        }
        return true;
    }
    // If S -frame
    if ((frame & 0x3) == 1)
    {
        m_ReceiverFrame = IncreaseReceiverSequence(m_ReceiverFrame);
        return true;
    }

    //Handle I-frame.
    unsigned char expected;
    if ((m_SenderFrame & 0x1) == 0)
    {
        expected = IncreaseReceiverSequence(IncreaseSendSequence(m_ReceiverFrame));
        if (frame == expected)
        {
            m_ReceiverFrame = frame;
            return true;
        }
        if (frame == (expected & ~0x10))
        {
            m_ReceiverFrame = IncreaseSendSequence(m_ReceiverFrame);
            return true;
        }
    }
    //If answer for RR.
    else
    {
        expected = IncreaseSendSequence(m_ReceiverFrame);
        if (frame == expected)
        {
            m_ReceiverFrame = frame;
            return true;
        }
    }
    return false;
}

unsigned char DLMSSettings::GetNextSend(unsigned char first)
{
    if (first)
    {
        m_SenderFrame = IncreaseReceiverSequence(IncreaseSendSequence(m_SenderFrame));
    }
    else
    {
        m_SenderFrame = IncreaseSendSequence(m_SenderFrame);
    }
    return m_SenderFrame;
}

unsigned char DLMSSettings::GetReceiverReady()
{
    m_SenderFrame = IncreaseReceiverSequence((unsigned char)(m_SenderFrame | 1));
    return (unsigned char)(m_SenderFrame & 0xF1);
}

unsigned char DLMSSettings::GetKeepAlive()
{
    m_SenderFrame = (unsigned char)(m_SenderFrame | 1);
    return (unsigned char)(m_SenderFrame & 0xF1);
}

unsigned long DLMSSettings::GetBlockIndex()
{
    return m_BlockIndex;
}

void DLMSSettings::SetBlockIndex(unsigned long value)
{
    m_BlockIndex = value;
}

void DLMSSettings::ResetBlockIndex()
{
    m_BlockIndex = 1;
    m_BlockNumberAck = 0;
}

void DLMSSettings::IncreaseBlockIndex()
{
    m_BlockIndex += 1;
}

bool DLMSSettings::IsServer()
{
    return m_Server;
}

DLMSLimits& DLMSSettings::GetHdlcSettings()
{
    return m_HdlcSettings;
}

PlcSettings& DLMSSettings::GetPlcSettings()
{
    return m_PlcSettings;
}

DLMS_INTERFACE_TYPE DLMSSettings::GetInterfaceType()
{
    return m_InterfaceType;
}

void DLMSSettings::SetInterfaceType(DLMS_INTERFACE_TYPE value)
{
    m_InterfaceType = value;
}

unsigned long DLMSSettings::GetClientAddress()
{
    return m_ClientAddress;
}

void DLMSSettings::SetClientAddress(unsigned long value)
{
    m_ClientAddress = value;
}

unsigned long DLMSSettings::GetPushClientAddress()
{
    return m_PushClientAddress;
}

void DLMSSettings::SetPushClientAddress(unsigned long value)
{
    m_PushClientAddress = value;
}

unsigned long DLMSSettings::GetServerAddress()
{
    return m_ServerAddress;
}

// Server address.
void DLMSSettings::SetServerAddress(unsigned long value)
{
    m_ServerAddress = value;
}

// DLMS version number.
unsigned char DLMSSettings::GetDLMSVersion()
{
    return m_DlmsVersionNumber;
}

// DLMS version number.
void DLMSSettings::SetDLMSVersion(unsigned char value)
{
    m_DlmsVersionNumber = value;
}

// Maximum PDU size.
unsigned short DLMSSettings::GetMaxPduSize()
{
    return m_MaxReceivePDUSize;
}

int DLMSSettings::SetMaxReceivePDUSize(unsigned short value)
{
    if (value < 64 && !m_Server)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_MaxReceivePDUSize = value;
    return 0;
}

// Maximum server PDU size.
unsigned short DLMSSettings::GetMaxServerPDUSize()
{
    return m_MaxServerPDUSize;
}

int DLMSSettings::SetMaxServerPDUSize(unsigned short value)
{
    if (value < 64)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_MaxServerPDUSize = value;
    return 0;
}


bool DLMSSettings::GetUseLogicalNameReferencing()
{
    return m_UseLogicalNameReferencing;
}

void DLMSSettings::SetUseLogicalNameReferencing(bool value)
{
    m_UseLogicalNameReferencing = value;
}

DLMS_PRIORITY DLMSSettings::GetPriority()
{
    return m_Priority;
}

void DLMSSettings::SetPriority(DLMS_PRIORITY value)
{
    m_Priority = value;
}

DLMS_SERVICE_CLASS DLMSSettings::GetServiceClass()
{
    return m_ServiceClass;
}

void DLMSSettings::SetServiceClass(DLMS_SERVICE_CLASS value)
{
    m_ServiceClass = value;
}

unsigned char DLMSSettings::GetInvokeID()
{
    return m_InvokeID;
}

void DLMSSettings::SetInvokeID(unsigned char value)
{
    m_InvokeID = value;
}

void DLMSSettings::UpdateInvokeId(unsigned char value) {
    if ((value & 0x80) != 0) {
        m_Priority = DLMS_PRIORITY_HIGH;
    }
    else {
        m_Priority = DLMS_PRIORITY_NORMAL;
    }
    if ((value & 0x40) != 0) {
        m_ServiceClass = DLMS_SERVICE_CLASS_CONFIRMED;
    }
    else
    {
        m_ServiceClass = DLMS_SERVICE_CLASS_UN_CONFIRMED;
    }
    m_InvokeID = (unsigned char)(value & 0xF);
}


unsigned long DLMSSettings::GetLongInvokeID()
{
    return m_LongInvokeID;
}
int DLMSSettings::SetLongInvokeID(unsigned long value)
{
    if (value > 0xFFFFFF)
    {
        //Invalid InvokeID.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_LongInvokeID = value;
    return 0;
}
DLMSObjectCollection& DLMSSettings::GetObjects()
{
    return m_Objects;
}

bool DLMSSettings::GetUseCustomChallenge()
{
    return m_UseCustomChallenge;
}

void DLMSSettings::SetUseCustomChallenge(bool value)
{
    m_UseCustomChallenge = value;
}

DLMS_CONNECTION_STATE DLMSSettings::GetConnected()
{
    return m_Connected;
}

void DLMSSettings::SetConnected(DLMS_CONNECTION_STATE value)
{
    m_Connected = value;
}

Cipher* DLMSSettings::GetCipher()
{
    return m_Cipher;
}

void DLMSSettings::SetCipher(Cipher* value)
{
    m_Cipher = value;
}

ByteBuffer& DLMSSettings::GetSourceSystemTitle()
{
    return m_SourceSystemTitle;
}

int  DLMSSettings::SetSourceSystemTitle(ByteBuffer& value)
{
    if (value.GetSize() != 8)
    {
        //Invalid client system title.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_SourceSystemTitle = value;
    return DLMS_ERROR_CODE_OK;
}

ByteBuffer& DLMSSettings::GetPreEstablishedSystemTitle()
{
    return m_PreEstablishedSystemTitle;
}

int  DLMSSettings::SetPreEstablishedSystemTitle(ByteBuffer& value)
{
    if (value.GetSize() != 8 && value.GetSize() != 0)
    {
        //Invalid client system title.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    m_PreEstablishedSystemTitle = value;
    return DLMS_ERROR_CODE_OK;
}

/**
 * @return Key Encrypting Key, also known as Master key.
 */
ByteBuffer& DLMSSettings::GetKek()
{
    return m_Kek;
}

/**
 * @param value
 *            Key Encrypting Key, also known as Master key.
 */
void DLMSSettings::SetKek(ByteBuffer& value)
{
    m_Kek = value;
}

/**
 * @return Long data count.
 */
unsigned short DLMSSettings::GetCount()
{
    return m_Count;
}

/**
 * @param count
 *            Long data count.
 */
void DLMSSettings::SetCount(unsigned short value)
{
    m_Count = value;
}

/**
 * @return Long data index.
 */
unsigned short DLMSSettings::GetIndex()
{
    return m_Index;
}

/**
 * @param index
 *            Long data index
 */
void DLMSSettings::SetIndex(unsigned short value)
{
    m_Index = value;
}

DLMS_CONFORMANCE DLMSSettings::GetNegotiatedConformance()
{
    return m_NegotiatedConformance;
}

void DLMSSettings::SetNegotiatedConformance(DLMS_CONFORMANCE value)
{
    m_NegotiatedConformance = value;
}

DLMS_CONFORMANCE DLMSSettings::GetProposedConformance()
{
    return m_ProposedConformance;
}

void DLMSSettings::SetProposedConformance(DLMS_CONFORMANCE value)
{
    m_ProposedConformance = value;
}

char* DLMSSettings::GetProtocolVersion()
{
    return m_ProtocolVersion;
}

void DLMSSettings::SetProtocolVersion(const char* value)
{
    if (m_ProtocolVersion != NULL)
    {
        delete m_ProtocolVersion;
        m_ProtocolVersion = NULL;
    }
    if (value != NULL)
    {
        short len = (short)strlen(value);
        m_ProtocolVersion = new char[len + 1];
        memcpy(m_ProtocolVersion, value, len);
        m_ProtocolVersion[len] = 0;
    }
}

bool DLMSSettings::GetUseUtc2NormalTime()
{
    return m_UseUtc2NormalTime;
}

void DLMSSettings::SetUseUtc2NormalTime(bool value)
{
    m_UseUtc2NormalTime = value;
}

uint64_t DLMSSettings::GetExpectedInvocationCounter()
{
    return m_ExpectedInvocationCounter;
}

void DLMSSettings::SetExpectedInvocationCounter(uint64_t value)
{
    m_ExpectedInvocationCounter = value;
}

unsigned char DLMSSettings::GetExpectedSecurityPolicy()
{
    return m_ExpectedSecurityPolicy;
}

void DLMSSettings::SetExpectedSecurityPolicy(unsigned char value)
{
    m_ExpectedSecurityPolicy = value;
}

void DLMSSettings::SetExpectedSecuritySuite(unsigned char value)
{
    m_ExpectedSecuritySuite = value;
}

unsigned char DLMSSettings::GetExpectedSecuritySuite()
{
    return m_ExpectedSecuritySuite;
}

DATETIME_SKIPS DLMSSettings::GetDateTimeSkips()
{
    return m_DateTimeSkips;
}

void DLMSSettings::SetDateTimeSkips(DATETIME_SKIPS value)
{
    m_DateTimeSkips = value;
}


unsigned char DLMSSettings::GetUserID()
{
    return m_UserId;
}

void DLMSSettings::SetUserID(unsigned char value)
{
    m_UserId = value;
}

unsigned char DLMSSettings::GetQualityOfService()
{
    return m_QualityOfService;
}

void DLMSSettings::SetQualityOfService(unsigned char value)
{
    m_QualityOfService = value;
}

bool DLMSSettings::GetAutoIncreaseInvokeID()
{
    return m_AutoIncreaseInvokeID;
}

void DLMSSettings::SetAutoIncreaseInvokeID(bool value)
{
    m_AutoIncreaseInvokeID = value;
}

void DLMSSettings::AddAllocateObject(DLMSObject* pObj)
{
    m_AllocatedObjects.push_back(pObj);
}

int DLMSSettings::GetBlockNumberAck()
{
    return m_BlockNumberAck;
}

void DLMSSettings::SetBlockNumberAck(int value)
{
    m_BlockNumberAck = value;
}

unsigned char DLMSSettings::GetGbtWindowSize()
{
    return m_GbtWindowSize;
}

void DLMSSettings::SetGbtWindowSize(unsigned char value)
{
    m_GbtWindowSize = value;
}

unsigned char DLMSSettings::GetCommand()
{
    return m_Command;
}

void DLMSSettings::SetCommand(unsigned char value)
{
    m_Command = value;
}

unsigned char DLMSSettings::GetCommandType()
{
    return m_CommandType;
}

void DLMSSettings::SetCommandType(unsigned char value)
{
    m_CommandType = value;
}
