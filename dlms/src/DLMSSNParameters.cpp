#include "DLMSSNParameters.h"

DLMSSNParameters::DLMSSNParameters(DLMSSettings* settings,
    DLMS_COMMAND command,
    int count,
    int commandType,
    ByteBuffer* attributeDescriptor,
    ByteBuffer* data)
{
    m_Settings = settings;
    m_BlockIndex = (unsigned short)settings->GetBlockIndex();
    m_Command = command;
    m_Count = count;
    m_RequestType = commandType;
    m_AttributeDescriptor = attributeDescriptor;
    m_Data = data;
    m_MultipleBlocks = false;
    m_Time = 0;
    if (settings != NULL) 
    {
        settings->SetCommand(command);
    }
}

DLMSSettings* DLMSSNParameters::GetSettings()
{
    return m_Settings;
}

DLMS_COMMAND DLMSSNParameters::GetCommand()
{
    return m_Command;
}

int DLMSSNParameters::GetRequestType()
{
    return m_RequestType;
}

void DLMSSNParameters::SetRequestType(int value)
{
    m_RequestType = value;
}

ByteBuffer* DLMSSNParameters::GetAttributeDescriptor()
{
    return m_AttributeDescriptor;
}

ByteBuffer* DLMSSNParameters::GetData()
{
    return m_Data;
}

struct tm* DLMSSNParameters::GetTime()
{
    return m_Time;
}

void DLMSSNParameters::SetTime(struct tm* value)
{
    m_Time = value;
}

bool DLMSSNParameters::IsMultipleBlocks()
{
    return m_MultipleBlocks;
}

void DLMSSNParameters::SetMultipleBlocks(bool value)
{
    m_MultipleBlocks = value;
}

unsigned short DLMSSNParameters::GetBlockIndex()
{
    return m_BlockIndex;
}

void DLMSSNParameters::SetBlockIndex(unsigned short value)
{
    m_BlockIndex = value;
}

int DLMSSNParameters::GetCount()
{
    return m_Count;
}
