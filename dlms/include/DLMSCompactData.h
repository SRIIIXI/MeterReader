#ifndef DLMS_COMPACT_DATA_H
#define DLMS_COMPACT_DATA_H

#ifndef DLMS_IGNORE_COMPACT_DATA
#include "DLMSObject.h"
#include "DLMSCaptureObject.h"

typedef enum
{
    /**
     * Data is captured with Capture-method.
     */
    DLMS_CAPTURE_METHOD_INVOKE,
    /**
     * Data is captured upon reading.
     */
    DLMS_CAPTURE_METHOD_IMPLICIT
}DLMS_CAPTURE_METHOD;

class DLMSCompactData : public DLMSObject
{
    /*
    * Compact buffer
    */
    ByteBuffer m_Buffer;

    /*
     * Capture objects.
     */
    std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> > m_CaptureObjects;

    /*
     * Template ID.
     */
    unsigned char m_TemplateId;

    /*
     * Template description.
     */
    ByteBuffer m_TemplateDescription;

    /*
     * Capture method.
     */
    DLMS_CAPTURE_METHOD m_CaptureMethod;

    int GetCaptureObjects(DLMSSettings& settings, DLMSValueEventArg& e);
public:
    //Constructor.
    DLMSCompactData();

    //SN Constructor.
    DLMSCompactData(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSCompactData(std::string ln);

    ~DLMSCompactData();

    /*
     * Returns Compact buffer
     */
    ByteBuffer& GetBuffer();

    /*
     * value: Compact buffer
     */
    void SetBuffer(ByteBuffer& value);

    /*
     * Capture objects.
     */
    std::vector<std::pair<DLMSObject*, DLMSCaptureObject*> >& GetCaptureObjects();

    /*
     * Returns Template ID.
     */
    unsigned char GetTemplateId();

    /*
     * value: Template ID.
     */
    void SetTemplateId(unsigned char value);

    /*
     * Returns template description.
     */
    ByteBuffer& GetTemplateDescription();

    /*
     * value: Template description.
     */
    void SetTemplateDescription(ByteBuffer& value);

    /*
     * Returns Capture method.
     */
    DLMS_CAPTURE_METHOD GetCaptureMethod();

    /*
     * value: Capture method.
     */
    void SetCaptureMethod(DLMS_CAPTURE_METHOD value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
