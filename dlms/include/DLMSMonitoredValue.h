#ifndef DLMS_MONITORED_VALUE_H
#define DLMS_MONITORED_VALUE_H

#include "Ignore.h"
#include "DLMSObject.h"

class DLMSMonitoredValue
{
    DLMS_OBJECT_TYPE m_ObjectType;
    std::string m_LogicalName;
    int m_AttributeIndex;
public:

    DLMSMonitoredValue();
    void Update(DLMSObject* pObj, int attributeIndex);

    DLMS_OBJECT_TYPE GetObjectType();
    void SetObjectType(DLMS_OBJECT_TYPE value);

    std::string GetLogicalName();
    void SetLogicalName(std::string value);

    int GetAttributeIndex();
    void SetAttributeIndex(int value);

    std::string ToString();
};

#endif
