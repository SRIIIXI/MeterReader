#ifndef DLMS_DATA_H
#define DLMS_DATA_H

#include "DLMSObject.h"
#ifndef DLMS_IGNORE_DATA

class DLMSData : public DLMSObject
{
    DLMSVariant m_Value;
public:
    //Constructor.
    DLMSData();

    //SN Constructor.
    DLMSData(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSData(std::string ln);

    // Get value of COSEM Data object.
    DLMSVariant& GetValue();

    // Set value of COSEM Data object.
    void SetValue(DLMSVariant& value);

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
