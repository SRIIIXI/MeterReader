#ifndef DLMS_MAC_ADDRESS_SETUP_H
#define DLMS_MAC_ADDRESS_SETUP_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_MAC_ADDRESS_SETUP
#include "DLMSObject.h"

class DLMSMacAddressSetup : public DLMSObject
{
    std::string m_MacAddress;
public:
    /**
     Constructor.
    */
    DLMSMacAddressSetup();
    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    DLMSMacAddressSetup(std::string ln);

    /**
     Constructor.

     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    DLMSMacAddressSetup(std::string ln, unsigned short sn);

    /**
     Value of COSEM Data object.
    */
    std::string GetMacAddress();
    void SetMacAddress(std::string value);

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
