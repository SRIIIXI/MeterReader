#ifndef DLMS_IEC_TWISTEDPAIR_SETUP_H
#define DLMS_IEC_TWISTEDPAIR_SETUP_H

#include "DLMSObject.h"
#ifndef DLMS_IGNORE_IEC_TWISTED_PAIR_SETUP

class DLMSIecTwistedPairSetup : public DLMSObject
{
    /*
    * Working mode.
    */
    DLMS_IEC_TWISTED_PAIR_SETUP_MODE m_Mode;

    /*
     * Communication speed.
     */
    DLMS_BAUD_RATE m_Speed;

    /*
     * List of Primary Station Addresses.
     */
    std::vector<unsigned char> m_PrimaryAddresses;

    /*
     * List of the TAB(i) for which the real equipment has been programmed in
     * the case of forgotten station call.
     */
    std::vector<char> m_Tabis;

public:
    //Constructor.
    DLMSIecTwistedPairSetup();

    //SN Constructor.
    DLMSIecTwistedPairSetup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSIecTwistedPairSetup(std::string ln);

    /**
    * Working mode.
    */
    DLMS_IEC_TWISTED_PAIR_SETUP_MODE GetMode();
    void SetMode(DLMS_IEC_TWISTED_PAIR_SETUP_MODE value);

    /**
     * Communication speed.
     */
    DLMS_BAUD_RATE GetSpeed();
    void SetSpeed(DLMS_BAUD_RATE value);

    /*
     * list of Primary Station Addresses.
     */
    std::vector<unsigned char>& GetPrimaryAddresses();

    /*
     * list of the TABis.
     */
    std::vector<char>& GetTabis();

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
