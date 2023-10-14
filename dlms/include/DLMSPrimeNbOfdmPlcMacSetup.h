#ifndef DLMS_PRIME_NBOFDM_PLCMAC_SETUP_H
#define DLMS_PRIME_NBOFDM_PLCMAC_SETUP_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_SETUP
#include "DLMSObject.h"

class DLMSPrimeNbOfdmPlcMacSetup : public DLMSObject
{
    /**
    * PIB attribute 0x0010.
    */
    unsigned char m_MacMinSwitchSearchTime;

    /**
     * PIB attribute 0x0011.
     */
    unsigned char m_MacMaxPromotionPdu;

    /**
     * PIB attribute 0x0012.
     */
    unsigned char m_MacPromotionPduTxPeriod;
    /**
     * PIB attribute 0x0013.
     */
    unsigned char m_MacBeaconsPerFrame;
    /**
     * PIB attribute 0x0014.
     */
    unsigned char m_MacScpMaxTxAttempts;
    /**
     * PIB attribute 0x0015.
     */
    unsigned char m_MacCtlReTxTimer;
    /**
     * PIB attribute 0x0018.
     */
    unsigned char m_MacMaxCtlReTx;
public:
    //Constructor.
    DLMSPrimeNbOfdmPlcMacSetup();

    //SN Constructor.
    DLMSPrimeNbOfdmPlcMacSetup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSPrimeNbOfdmPlcMacSetup(std::string ln);

    /*
     * Returns PIB attribute 0x0010.
     */
    unsigned char GetMacMinSwitchSearchTime();

    /*
     * value
     *            PIB attribute 0x0010.
     */
    void SetMacMinSwitchSearchTime(unsigned char value);

    /*
     * Returns PIB attribute 0x0011.
     */
    unsigned char GetMacMaxPromotionPdu();

    /*
     * value
     *            PIB attribute 0x0011.
     */
    void SetMacMaxPromotionPdu(unsigned char value);

    /*
     * Returns PIB attribute 0x0012.
     */
    unsigned char GetMacPromotionPduTxPeriod();

    /*
     * value
     *            PIB attribute 0x0012.
     */
    void SetMacPromotionPduTxPeriod(unsigned char value);

    /*
     * Returns PIB attribute 0x0013.
     */
    unsigned char GetMacBeaconsPerFrame();

    /*
     * value
     *            PIB attribute 0x0013.
     */
    void SetMacBeaconsPerFrame(unsigned char value);

    /*
     * Returns PIB attribute 0x0014.
     */
    unsigned char GetMacScpMaxTxAttempts();

    /*
     * value
     *            PIB attribute 0x0014.
     */
    void SetMacScpMaxTxAttempts(unsigned char value);

    /*
     * Returns PIB attribute 0x0015.
     */
    unsigned char GetMacCtlReTxTimer();

    /*
     * value
     *            PIB attribute 0x0015.
     */
    void SetMacCtlReTxTimer(unsigned char value);

    /*
     * Returns PIB attribute 0x0018.
     */
    unsigned char GetMacMaxCtlReTx();

    /*
     * value
     *            PIB attribute 0x0018.
     */
    void SetMacMaxCtlReTx(unsigned char value);

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
