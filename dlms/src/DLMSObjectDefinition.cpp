#include "DLMSObjectDefinition.h"
#include <sstream>
#include "DLMSConverter.h"

DLMSObjectDefinition::DLMSObjectDefinition()
{
    m_ObjectType = DLMS_OBJECT_TYPE_NONE;
}

/*
 * Constructor
 */
DLMSObjectDefinition::DLMSObjectDefinition(DLMS_OBJECT_TYPE classId, std::string logicalName)
{
    m_ObjectType = classId;
    m_LogicalName = logicalName;
}

std::string DLMSObjectDefinition::ToString()
{
    std::stringstream sb;
    sb << DLMSConverter::ToString(m_ObjectType);
    sb << " ";
    sb << m_LogicalName.c_str();
    return sb.str();
}

DLMS_OBJECT_TYPE DLMSObjectDefinition::GetObjectType()
{
    return m_ObjectType;
}

void DLMSObjectDefinition::SetObjectType(DLMS_OBJECT_TYPE value)
{
    m_ObjectType = value;
}

std::string DLMSObjectDefinition::GetLogicalName()
{
    return m_LogicalName;
}
void DLMSObjectDefinition::SetLogicalName(std::string value)
{
    m_LogicalName = value;
}
