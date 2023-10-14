#include "DLMSScriptAction.h"
#include <sstream>

/**
 Defines which action to be applied to the referenced object.
*/
SCRIPT_ACTION_TYPE DLMSScriptAction::GetType()
{
    return m_Type;
}
void DLMSScriptAction::SetType(SCRIPT_ACTION_TYPE value)
{
    m_Type = value;
}

/**
 Executed object type.
*/
DLMS_OBJECT_TYPE DLMSScriptAction::GetObjectType()
{
    return m_ObjectType;
}
void DLMSScriptAction::SetObjectType(DLMS_OBJECT_TYPE value)
{
    m_ObjectType = value;
}

/**
 Logical name of executed object.
*/
std::string DLMSScriptAction::GetLogicalName()
{
    return m_LogicalName;
}
void DLMSScriptAction::SetLogicalName(std::string value)
{
    m_LogicalName = value;
}

/**
 defines which attribute of the selected object is affected; or
 * which specific method is to be executed.
*/
int DLMSScriptAction::GetIndex()
{
    return m_Index;
}
void DLMSScriptAction::SetIndex(int value)
{
    m_Index = value;
}

/**
 Parameter is service spesific.
*/
DLMSVariant DLMSScriptAction::GetParameter()
{
    return m_Parameter;
}
void DLMSScriptAction::SetParameter(DLMSVariant value)
{
    m_Parameter = value;
}

std::string DLMSScriptAction::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    sb << " ";
    sb << m_ObjectType;
    sb << " ";
    sb << m_LogicalName.c_str();
    sb << " ";
    sb << m_Index;
    sb << " ";
    sb << m_Parameter.ToString().c_str();
    return sb.str();
}
