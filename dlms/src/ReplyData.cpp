#include "ReplyData.h"

ReplyData::ReplyData(
    DLMS_DATA_REQUEST_TYPES more,
    DLMS_COMMAND cmd,
    ByteBuffer* buff,
    bool complete)
{
    Clear();
    m_DataType = DLMS_DATA_TYPE_NONE;
    m_MoreData = more;
    m_Command = cmd;
    m_CommandType = 0;
    m_Complete = complete;
    m_Time = NULL;
    m_pData = buff;
}

ReplyData::ReplyData()
{
    Clear();
}

DLMS_DATA_TYPE ReplyData::GetValueType()
{
    return m_DataType;
}

void ReplyData::SetValueType(DLMS_DATA_TYPE value)
{
    m_DataType = value;
}

DLMSVariant& ReplyData::GetValue()
{
    return m_DataValue;
}

void ReplyData::SetValue(DLMSVariant& value)
{
    m_DataValue = value;
}

unsigned long ReplyData::GetReadPosition()
{
    return m_ReadPosition;
}

void ReplyData::SetReadPosition(unsigned long value)
{
    m_ReadPosition = value;
}

int ReplyData::GetPacketLength()
{
    return m_PacketLength;
}

void ReplyData::SetPacketLength(int value)
{
    m_PacketLength = value;
}

void ReplyData::SetCommand(DLMS_COMMAND value)
{
    m_Command = value;
}

void ReplyData::SetData(ByteBuffer& value)
{
    m_pData = &value;
}

void ReplyData::SetComplete(bool value)
{
    m_Complete = value;
}

void ReplyData::SetTotalCount(int value)
{
    m_TotalCount = value;
}

void ReplyData::Clear()
{
    m_Data.Clear();
    m_pData = &m_Data;
    m_Time = NULL;
    m_MoreData = DLMS_DATA_REQUEST_TYPES_NONE;
    m_Command = DLMS_COMMAND_NONE;
    m_Complete = false;
    m_Peek = false;
    m_TotalCount = 0;
    m_DataValue.Clear();
    m_ReadPosition = 0;
    m_PacketLength = 0;
    m_DataType = DLMS_DATA_TYPE_NONE;
    m_CipherIndex = 0;
    m_pXml = NULL;
    m_InvokeId = 0;
    m_BlockNumber = 0;
    m_BlockNumberAck = 0;
    m_Streaming = false;
    m_GbtWindowSize = 0;
    m_ClientAddress = 0;
    m_ServerAddress = 0;
    m_CommandType = 0;
    m_CipheredCommand = 0;
}

bool ReplyData::IsMoreData()
{
    return m_MoreData != DLMS_DATA_REQUEST_TYPES_NONE;
}

DLMS_DATA_REQUEST_TYPES ReplyData::GetMoreData()
{
    return m_MoreData;
}

void ReplyData::SetMoreData(DLMS_DATA_REQUEST_TYPES value)
{
    m_MoreData = value;
}

DLMS_COMMAND ReplyData::GetCommand()
{
    return m_Command;
}

void ReplyData::SetCommandType(unsigned char value)
{
    m_CommandType = value;
}

unsigned char ReplyData::GetCommandType()
{
    return m_CommandType;
}

ByteBuffer& ReplyData::GetData()
{
    return *m_pData;
}

bool ReplyData::IsComplete()
{
    return m_Complete;
}

int ReplyData::GetTotalCount()
{
    return m_TotalCount;
}

int ReplyData::GetCount()
{
    if (m_DataValue.vt == DLMS_DATA_TYPE_ARRAY)
    {
        return (int)m_DataValue.Arr.size();
    }
    return 0;
}

bool ReplyData::GetPeek()
{
    return m_Peek;
}

void ReplyData::SetPeek(bool value)
{
    m_Peek = value;
}

unsigned short ReplyData::GetCipherIndex()
{
    return m_CipherIndex;
}

void ReplyData::SetCipherIndex(unsigned short value)
{
    m_CipherIndex = value;
}

struct tm* ReplyData::GetTime()
{
    return m_Time;
}

void ReplyData::SetTime(struct tm* value)
{
    if (value == NULL)
    {
        if (m_Time != NULL)
        {
            free(m_Time);
        }
        m_Time = value;
    }
    else
    {
        if (m_Time == NULL)
        {
            m_Time = (struct tm*)malloc(sizeof(struct tm));
        }
        memcpy(m_Time, value, sizeof(struct tm));
    }
}

#ifndef DLMS_IGNORE_XML_TRANSLATOR
DLMSTranslatorStructure* ReplyData::GetXml()
{
    return m_pXml;
}

void ReplyData::SetXml(DLMSTranslatorStructure* value)
{
    m_pXml = value;
}
#endif //DLMS_IGNORE_XML_TRANSLATOR

long ReplyData::GetInvokeId()
{
    return m_InvokeId;
}

void ReplyData::SetInvokeId(long value)
{
    m_InvokeId = value;
}

int ReplyData::GetBlockNumber()
{
    return m_BlockNumber;
}

void ReplyData::SetBlockNumber(int value)
{
    m_BlockNumber = value;
}

int ReplyData::GetBlockNumberAck()
{
    return m_BlockNumberAck;
}

void ReplyData::SetBlockNumberAck(int value)
{
    m_BlockNumberAck = value;
}

bool ReplyData::GetStreaming()
{
    return m_Streaming;
}

void ReplyData::SetStreaming(bool value)
{
    m_Streaming = value;
}

unsigned char ReplyData::GetGbtWindowSize() {
    return m_GbtWindowSize;
}

void ReplyData::SetGbtWindowSize(unsigned char value)
{
    m_GbtWindowSize = value;
}

bool ReplyData::IsStreaming()
{
    return (m_MoreData & DLMS_DATA_REQUEST_TYPES_FRAME) == 0 && m_Streaming && (m_BlockNumberAck * m_GbtWindowSize) + 1 > m_BlockNumber;
}

unsigned short ReplyData::GetClientAddress()
{
    return m_ClientAddress;
}

void ReplyData::SetClientAddress(unsigned short value)
{
    m_ClientAddress = value;
}

int ReplyData::GetServerAddress()
{
    return m_ServerAddress;
}

void ReplyData::SetServerAddress(int value)
{
    m_ServerAddress = value;
}

unsigned char ReplyData::GetCipheredCommand()
{
    return m_CipheredCommand;
}

void ReplyData::SetCipheredCommand(unsigned char value)
{
    m_CipheredCommand = value;
}
