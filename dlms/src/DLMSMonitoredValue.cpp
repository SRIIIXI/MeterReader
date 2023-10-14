#include "DLMSMonitoredValue.h"
#include "DLMSConverter.h"

DLMSMonitoredValue::DLMSMonitoredValue()
{
    m_ObjectType = DLMS_OBJECT_TYPE_NONE;
    m_AttributeIndex = 0;
}

void DLMSMonitoredValue::Update(DLMSObject* pObj, int attributeIndex)
{
    m_ObjectType = pObj->GetObjectType();
    pObj->GetLogicalName(m_LogicalName);
    m_AttributeIndex = attributeIndex;
}

DLMS_OBJECT_TYPE DLMSMonitoredValue::GetObjectType()
{
    return m_ObjectType;
}
void DLMSMonitoredValue::SetObjectType(DLMS_OBJECT_TYPE value)
{
    m_ObjectType = value;
}

std::string DLMSMonitoredValue::GetLogicalName()
{
    return m_LogicalName;
}
void DLMSMonitoredValue::SetLogicalName(std::string value)
{
    m_LogicalName = value;
}

int DLMSMonitoredValue::GetAttributeIndex()
{
    return m_AttributeIndex;
}
void DLMSMonitoredValue::SetAttributeIndex(int value)
{
    m_AttributeIndex = value;
}

std::string DLMSMonitoredValue::ToString()
{
    if (m_ObjectType == DLMS_OBJECT_TYPE_NONE)
    {
        return "";
    }
    std::stringstream sb;
    sb << DLMSConverter::ToString(m_ObjectType);
    sb << " ";
    sb << m_LogicalName.c_str();
    sb << " ";
    sb << m_AttributeIndex;
    return sb.str();

}
