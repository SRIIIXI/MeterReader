#include <sstream>
#include "DLMSActionItem.h"

//Constructor.
DLMSActionItem::DLMSActionItem()
{
    m_ScriptSelector = 0;
}

std::string& DLMSActionItem::GetLogicalName()
{
    return m_LogicalName;
}
void DLMSActionItem::SetLogicalName(std::string& value)
{
    m_LogicalName = value;
}

int DLMSActionItem::GetScriptSelector()
{
    return m_ScriptSelector;
}
void DLMSActionItem::SetScriptSelector(int value)
{
    m_ScriptSelector = value;
}

std::string DLMSActionItem::ToString()
{
    std::stringstream sb;
    sb << m_LogicalName.c_str();
    sb << " ";
    sb << m_ScriptSelector;
    return sb.str();
}
