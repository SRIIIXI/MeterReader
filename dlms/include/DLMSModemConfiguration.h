#ifndef DLMS_MODEM_CONFIGURATION_H
#define DLMS_MODEM_CONFIGURATION_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_MODEM_CONFIGURATION
#include "DLMSObject.h"
#include "DLMSModemInitialisation.h"

class DLMSModemConfiguration : public DLMSObject
{
    std::vector<DLMSModemInitialisation> m_InitialisationStrings;
    std::vector< std::string > m_ModemProfile;
    DLMS_BAUD_RATE m_CommunicationSpeed;

public:

    /**
     Constructor.
    */
    DLMSModemConfiguration();
    /**
     Constructor.
     @param ln Logical Name of the object.
    */
    DLMSModemConfiguration(std::string ln);

    /**
     Constructor.
     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    DLMSModemConfiguration(std::string ln, unsigned short sn);

    DLMS_BAUD_RATE GetCommunicationSpeed();

    void SetCommunicationSpeed(DLMS_BAUD_RATE value);

    std::vector<DLMSModemInitialisation>& GetInitialisationStrings();
    void SetInitialisationStrings(std::vector<DLMSModemInitialisation>& value);

    std::vector< std::string >& GetModemProfile();
    void SetModemProfile(std::vector< std::string >& value);

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
