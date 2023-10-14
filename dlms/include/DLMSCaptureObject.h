#ifndef DLMS_CAPTURE_OBJECT_H
#define DLMS_CAPTURE_OBJECT_H

class DLMSCaptureObject
{
private:
    int m_AttributeIndex;
    int m_DataIndex;

public:
    /*
     * Constructor.
     */
    DLMSCaptureObject();
    /*
     * Constructor.
     */
    DLMSCaptureObject(int attributeIndex, int dataIndex);

    /**
     Index of DLMS object in the profile generic table
    */
    int GetAttributeIndex();
    void SetAttributeIndex(int value);

    /**
     Data index of DLMS object in the profile generic table.
    */
    int GetDataIndex();
    void SetDataIndex(int value);
};
#endif
