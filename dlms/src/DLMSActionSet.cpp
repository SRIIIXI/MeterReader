#include "DLMSActionSet.h"
#include <sstream>

DLMSActionSet::DLMSActionSet()
{
}

DLMSActionItem& DLMSActionSet::GetActionUp()
{
    return m_ActionUp;
}

DLMSActionItem& DLMSActionSet::GetActionDown()
{
    return m_ActionDown;
}

std::string DLMSActionSet::ToString()
{
    std::stringstream sb;
    sb << m_ActionUp.ToString().c_str();
    sb << " ";
    sb << m_ActionDown.ToString().c_str();
    return sb.str();
}
