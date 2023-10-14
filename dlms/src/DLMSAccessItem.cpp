#include "DLMSAccessItem.h"

DLMSObject* DLMSAccessItem::GetTarget()
{
    return m_Target;
}

void DLMSAccessItem::SetTarget(DLMSObject* value)
{
    m_Target = value;
}

DLMS_ACCESS_SERVICE_COMMAND_TYPE DLMSAccessItem::GetCommand()
{
    return m_Command;
}

void DLMSAccessItem::SetCommand(DLMS_ACCESS_SERVICE_COMMAND_TYPE value)
{
    m_Command = value;
}

unsigned char DLMSAccessItem::GetIndex()
{
    return m_Index;
}

/**
 * value: Attribute index.
 */
void DLMSAccessItem::SetIndex(unsigned char value)
{
    m_Index = value;
}

void DLMSAccessItem::SetError(DLMS_ERROR_CODE value)
{
    m_Error = value;
}

DLMS_ERROR_CODE DLMSAccessItem::GetError()
{
    return m_Error;
}

void DLMSAccessItem::SetValue(DLMSVariant& value)
{
    m_Value = value;
}

DLMSVariant& DLMSAccessItem::GetValue()
{
    return m_Value;
}

DLMSAccessItem::DLMSAccessItem()
{
    m_Error = DLMS_ERROR_CODE_OK;
}

DLMSAccessItem::DLMSAccessItem(DLMS_ACCESS_SERVICE_COMMAND_TYPE command,
    DLMSObject* target,
    unsigned char attributeIndex)
{
    m_Error = DLMS_ERROR_CODE_OK;
    m_Command = command;
    m_Target = target;
    m_Index = attributeIndex;
}
