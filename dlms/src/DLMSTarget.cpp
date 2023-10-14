#include "DLMSTarget.h"

DLMSTarget::DLMSTarget() : m_Target(NULL), m_AttributeIndex(0), m_DataIndex(0)
{
};

DLMSTarget::~DLMSTarget()
{
    Clear();
};

void DLMSTarget::Clear()
{
    m_Target = NULL;
    m_AttributeIndex = 0;
    m_DataIndex = 0;
}

DLMSObject* DLMSTarget::GetTarget()
{
    return m_Target;
}

void DLMSTarget::SetTarget(DLMSObject* value)
{
    m_Target = value;
}

unsigned char DLMSTarget::GetAttributeIndex()
{
    return m_AttributeIndex;
}

void DLMSTarget::SetAttributeIndex(unsigned char value)
{
    m_AttributeIndex = value;
}

unsigned char DLMSTarget::GetDataIndex()
{
    return m_DataIndex;
}

void DLMSTarget::SetDataIndex(unsigned char value)
{
    m_DataIndex = value;
}

DLMSVariant& DLMSTarget::GetValue() {
    return m_Value;
}

void DLMSTarget::SetValue(DLMSVariant& value) {
    m_Value = value;
}

std::string DLMSTarget::ToString()
{
    if (m_Target == NULL)
    {
        return "";
    }
    std::string ln;
    std::string str = Helpers::IntToString(m_Target->GetObjectType());
    str.append(" ");
    m_Target->GetLogicalName(ln);
    str.append(ln);
    str.append(" ");
    str.append(Helpers::IntToString(m_AttributeIndex));
    str.append(" ");
    str.append(Helpers::IntToString(m_DataIndex));
    str.append(" ");
    str.append(m_Value.ToString());
    return str;
}
