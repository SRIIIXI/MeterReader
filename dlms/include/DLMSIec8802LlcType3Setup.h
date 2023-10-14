#ifndef DLMS_IEC_8802_LLC_TYPE3_SETUP_H
#define DLMS_IEC_8802_LLC_TYPE3_SETUP_H

#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE3_SETUP
#include "DLMSObject.h"

class DLMSIec8802LlcType3Setup : public DLMSObject
{
    /**
    * Maximum number of octets in an ACn command PDU, N3.
    */
    uint16_t m_MaximumOctetsACnPdu;
    /**
    * Maximum number of times in transmissions N4.
    */
    unsigned char m_MaximumTransmissions;
    /**
    * Acknowledgement time, T1.
    */
    uint16_t m_AcknowledgementTime;
    /**
    * Receive lifetime variable, T2.
    */
    uint16_t m_ReceiveLifetime;
    /**
    * Transmit lifetime variable, T3.
    */
    uint16_t m_TransmitLifetime;
public:
    //Constructor.
    DLMSIec8802LlcType3Setup();

    //SN Constructor.
    DLMSIec8802LlcType3Setup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSIec8802LlcType3Setup(std::string ln);

    /**
    * Maximum number of octets in an ACn command PDU, N3.
    */
    uint16_t GetMaximumOctetsACnPdu();
    void SetMaximumOctetsACnPdu(uint16_t value);

    /**
     * Maximum number of times in transmissions N4.
     */
    unsigned char GetMaximumTransmissions();
    void SetMaximumTransmissions(unsigned char value);

    /**
     * Acknowledgement time, T1.
     */
    uint16_t GetAcknowledgementTime();
    void SetAcknowledgementTime(uint16_t value);

    /**
     * Receive lifetime variable, T2.
     */
    uint16_t GetReceiveLifetime();
    void SetReceiveLifetime(uint16_t value);

    /**
     * Transmit lifetime variable, T3.
     */
    uint16_t GetTransmitLifetime();
    void SetTransmitLifetime(uint16_t value);

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
