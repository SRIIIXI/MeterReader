#ifndef DLMS_GSM_CELL_INFO_H
#define DLMS_GSM_CELL_INFO_H

#include <string>

class DLMSGSMCellInfo
{
private:
    /*
    * Four byte cell ID.
    */
    unsigned int m_CellId;

    /*
    * Two byte location area code (LAC).
    */
    unsigned short m_LocationId;
    /*
    * Signal quality.
    */
    unsigned char m_SignalQuality;
    /*
    * Bit Error Rate.
    */
    unsigned char m_Ber;

    /*
    * Mobile Country Code.
    */
    unsigned short m_MobileCountryCode;

    /**
     * Mobile Network Code.
     */
    unsigned short m_MobileNetworkCode;

    /**
     * Absolute radio frequency channel number.
     */
    unsigned long m_ChannelNumber;


public:
    //Constructor.
    DLMSGSMCellInfo();
    /**
    * @return Two byte cell ID.
    */
    unsigned int GetCellId();

    /*
    * @param value
    *            Four byte cell ID.
    */
    void SetCellId(unsigned int value);

    /*
    * @return Two byte location area code (LAC).
    */
    unsigned short GetLocationId();

    /*
    * @param value
    *            Two byte location area code (LAC).
    */
    void SetLocationId(unsigned short value);

    /*
    * @return Signal quality.
    */
    unsigned char GetSignalQuality();

    /*
    * @param value
    *            Signal quality.
    */
    void SetSignalQuality(unsigned char value);

    /*
    * @return Bit Error Rate.
    */
    unsigned char GetBer();

    /**
    * @param value
    *            Bit Error Rate.
    */
    void SetBer(unsigned char value);

    /*
    * Mobile Country Code.
    */
    unsigned short GetMobileCountryCode();

    /*
    * Mobile Country Code.
    */
    void SetMobileCountryCode(unsigned short value);

    /**
     * Mobile Network Code.
     */
    unsigned short GetMobileNetworkCode();

    /**
     * Mobile Network Code.
     */
    void SetMobileNetworkCode(unsigned short value);

    /**
     * Absolute radio frequency channel number.
     */
    unsigned long GetChannelNumber();

    /**
     * Absolute radio frequency channel number.
     */
    void SetChannelNumber(unsigned long value);

    std::string ToString();
};

#endif
