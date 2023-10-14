#ifndef DLMS_IEC_8802_LLCTYPE2_SETUP_H
#define DLMS_IEC_8802_LLCTYPE2_SETUP_H

#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE2_SETUP
#include "DLMSObject.h"

class DLMSIec8802LlcType2Setup : public DLMSObject
{
    /**
     * Transmit Window Size K.
     */
    unsigned char m_TransmitWindowSizeK;
    /**
     * Transmit Window Size RW.
     */
    unsigned char m_TransmitWindowSizeRW;
    /**
     * Maximum octets in I Pdu N1.
     */
    uint16_t m_MaximumOctetsPdu;
    /**
     * Maximum number of transmissions, N2.
     */
    unsigned char m_MaximumNumberTransmissions;
    /**
     * Acknowledgement timer in seconds.
     */
    uint16_t m_AcknowledgementTimer;
    /**
     * P-bit timer in seconds.
     */
    uint16_t m_BitTimer;
    /**
     * Reject timer.
     */
    uint16_t m_RejectTimer;
    /**
     * Busy state timer.
     */
    uint16_t m_BusyStateTimer;
public:
    //Constructor.
    DLMSIec8802LlcType2Setup();

    //SN Constructor.
    DLMSIec8802LlcType2Setup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSIec8802LlcType2Setup(std::string ln);

    /**
    * Transmit Window Size K.
    */
    unsigned char GetTransmitWindowSizeK();
    void SetTransmitWindowSizeK(unsigned char value);

    /**
     * Transmit Window Size RW.
     */
    unsigned char GetTransmitWindowSizeRW();
    void SetTransmitWindowSizeRW(unsigned char value);

    /**
     * Maximum octets in I Pdu N1.
     */
    uint16_t GetMaximumOctetsPdu();
    void SetMaximumOctetsPdu(uint16_t value);

    /**
     * Maximum number of transmissions, N2.
     */
    unsigned char GetMaximumNumberTransmissions();
    void SetMaximumNumberTransmissions(unsigned char value);

    /**
     * Acknowledgement timer in seconds.
     */
    uint16_t GetAcknowledgementTimer();
    void SetAcknowledgementTimer(uint16_t value);

    /**
     * P-bit timer in seconds.
     */
    uint16_t GetBitTimer();
    void SetBitTimer(uint16_t value);

    /**
     * Reject timer.
     */
    uint16_t GetRejectTimer();
    void SetRejectTimer(uint16_t value);

    /**
     * Busy state timer.
     */
    uint16_t GetBusyStateTimer();
    void SetBusyStateTimer(uint16_t value);

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
