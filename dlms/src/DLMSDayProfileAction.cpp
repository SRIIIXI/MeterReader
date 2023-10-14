#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSDayProfileAction.h"
#include <sstream>

/**
 Constructor.
*/
DLMSDayProfileAction::DLMSDayProfileAction()
{
    m_ScriptSelector = 0;
}

/**
 Constructor.
*/
DLMSDayProfileAction::DLMSDayProfileAction(Time& startTime, std::string scriptLogicalName, int scriptSelector)
{
    SetStartTime(startTime);
    SetScriptLogicalName(scriptLogicalName);
    SetScriptSelector(scriptSelector);
}

/**
 Defines the time when the script is to be executed.
*/
Time& DLMSDayProfileAction::GetStartTime()
{
    return m_StartTime;
}
void DLMSDayProfileAction::SetStartTime(Time& value)
{
    m_StartTime = value;
}

/**
 Defines the logical name of the "Script table" object;
*/
std::string& DLMSDayProfileAction::GetScriptLogicalName()
{
    return m_ScriptLogicalName;
}
void DLMSDayProfileAction::SetScriptLogicalName(std::string& value)
{
    m_ScriptLogicalName = value;
}

/**
 Defines the script_identifier of the script to be executed.
*/
int DLMSDayProfileAction::GetScriptSelector()
{
    return m_ScriptSelector;
}
void DLMSDayProfileAction::SetScriptSelector(int value)
{
    m_ScriptSelector = value;
}

std::string DLMSDayProfileAction::ToString()
{
    std::stringstream sb;
    sb << m_StartTime.ToString().c_str();
    sb << " ";
    sb << m_ScriptLogicalName.c_str();
    return sb.str();

}
