#include "DLMSLNParameters.h"

DLMSLNParameters::DLMSLNParameters(
    DLMSSettings* settings,
    unsigned long invokeId,
    DLMS_COMMAND command,
    int commandType,
    ByteBuffer* attributeDescriptor,
    ByteBuffer* data,
    int status,
    unsigned char cipheredCommand)
{
    m_Settings = settings;
    m_BlockIndex = settings->GetBlockIndex();
    m_BlockNumberAck = settings->GetBlockNumberAck();
    m_Command = command;
    m_CipheredCommand = cipheredCommand;
    m_RequestType = commandType;
    m_AttributeDescriptor = attributeDescriptor;
    m_Data = data;
    m_Time = NULL;
    m_Status = status;
    m_MultipleBlocks = settings->GetCount() != settings->GetIndex();
    m_LastBlock = settings->GetCount() == settings->GetIndex();
    m_InvokeId = invokeId;
    if (settings != NULL) {
        settings->SetCommand(command);
        if (command == DLMS_COMMAND_GET_REQUEST && commandType != DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK)
        {
            settings->SetCommandType(commandType);
        }
    }
    m_BlockNumber = 0;
    m_Streaming = false;
    m_WindowSize = 1;
}

DLMSSettings* DLMSLNParameters::GetSettings()
{
    return m_Settings;
}

DLMS_COMMAND DLMSLNParameters::GetCommand()
{
    return m_Command;
}

void DLMSLNParameters::SetCommand(DLMS_COMMAND value)
{
    m_Command = value;
}

int DLMSLNParameters::GetRequestType()
{
    return m_RequestType;
}

void DLMSLNParameters::SetRequestType(int value)
{
    m_RequestType = value;
}

ByteBuffer* DLMSLNParameters::GetAttributeDescriptor()
{
    return m_AttributeDescriptor;
}

ByteBuffer* DLMSLNParameters::GetData()
{
    return m_Data;
}

struct tm* DLMSLNParameters::GetTime()
{
    return m_Time;
}

void DLMSLNParameters::SetTime(struct tm* value)
{
    m_Time = value;
}

int DLMSLNParameters::GetStatus()
{
    return m_Status;
}

void DLMSLNParameters::SetStatus(int value)
{
    m_Status = value;
}

bool DLMSLNParameters::IsMultipleBlocks()
{
    return m_MultipleBlocks;
}

void DLMSLNParameters::SetMultipleBlocks(bool value)
{
    m_MultipleBlocks = value;
}

bool DLMSLNParameters::IsLastBlock()
{
    return m_LastBlock;
}

void DLMSLNParameters::SetLastBlock(bool value)
{
    m_LastBlock = value;
}

unsigned long DLMSLNParameters::GetBlockIndex()
{
    return m_BlockIndex;
}

void DLMSLNParameters::SetBlockIndex(unsigned long value)
{
    m_BlockIndex = value;
}

unsigned long DLMSLNParameters::GetInvokeId() {
    return m_InvokeId;
}

void DLMSLNParameters::SetInvokeId(unsigned long value) {
    m_InvokeId = value;
}
