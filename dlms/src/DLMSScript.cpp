#include "DLMSScript.h"

DLMSScript::DLMSScript()
{
}

DLMSScript::~DLMSScript()
{
    for (std::vector<DLMSScriptAction*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
    {
        delete *it;
    }
    m_Actions.clear();
}

int DLMSScript::GetID()
{
    return m_ID;
}

void DLMSScript::SetID(int value)
{
    m_ID = value;
}

std::vector<DLMSScriptAction*>& DLMSScript::GetActions()
{
    return m_Actions;
}
