#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSCaptureObject.h"

// Constructor.
DLMSCaptureObject::DLMSCaptureObject()
{

}

// Constructor.
DLMSCaptureObject::DLMSCaptureObject(int attributeIndex, int dataIndex)
{
    m_AttributeIndex = attributeIndex;
    m_DataIndex = dataIndex;
}

// Index of DLMS object in the profile generic table
int DLMSCaptureObject::GetAttributeIndex()
{
    return m_AttributeIndex;
}
void DLMSCaptureObject::SetAttributeIndex(int value)
{
    m_AttributeIndex = value;
}

// Data index of DLMS object in the profile generic table.
int DLMSCaptureObject::GetDataIndex()
{
    return m_DataIndex;
}
void DLMSCaptureObject::SetDataIndex(int value)
{
    m_DataIndex = value;
}
