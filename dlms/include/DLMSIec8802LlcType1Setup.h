#ifndef DLMS_IEC_8802_LLC_TYPE1_SETUP_H
#define DLMS_IEC_8802_LLC_TYPE1_SETUP_H

#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE1_SETUP
#include "DLMSObject.h"

class DLMSIec8802LlcType1Setup : public DLMSObject
{
    /**
    * Maximum number of octets in a UI PDU.
    */
    uint16_t m_MaximumOctetsUiPdu;

public:
    //Constructor.
    DLMSIec8802LlcType1Setup();

    //SN Constructor.
    DLMSIec8802LlcType1Setup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSIec8802LlcType1Setup(std::string ln);

    /**
    * Maximum number of octets in a UI PDU.
    */
    uint16_t GetMaximumOctetsUiPdu();
    void SetMaximumOctetsUiPdu(uint16_t value);

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
