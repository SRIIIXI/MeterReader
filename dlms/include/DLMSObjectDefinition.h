#ifndef DLMS_OBJECT_DEFINITION_H
#define DLMS_OBJECT_DEFINITION_H

#include <string>
#include "Enums.h"

class DLMSObjectDefinition
{
private:
    DLMS_OBJECT_TYPE m_ObjectType;
    std::string m_LogicalName;

public:
    DLMS_OBJECT_TYPE GetObjectType();
    void SetObjectType(DLMS_OBJECT_TYPE value);

    std::string GetLogicalName();
    void SetLogicalName(std::string value);

    /*
     * Constructor
     */
    DLMSObjectDefinition();

    /*
     * Constructor
     */
    DLMSObjectDefinition(DLMS_OBJECT_TYPE classId, std::string logicalName);

    std::string ToString();
};
#endif
