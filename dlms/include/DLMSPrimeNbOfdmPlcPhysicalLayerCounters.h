#ifndef DLMS_Prime_NbOfdm_Plc_Physical_Layer_Counters_H
#define DLMS_Prime_NbOfdm_Plc_Physical_Layer_Counters_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS
#include "DLMSObject.h"

class DLMSPrimeNbOfdmPlcPhysicalLayerCounters : public DLMSObject
{
    /*
     * Number of bursts received on the physical layer for which the CRC was
     * incorrect.
     */
    unsigned short m_CrcIncorrectCount;
    /*
     * Number of bursts received on the physical layer for which the CRC was
     * correct, but the Protocol field of PHY header had invalid value.
     */
    unsigned short m_CrcFailedCount;
    /*
     * Number of times when PHY layer received new data to transmit.
     */
    unsigned short m_TxDropCount;
    /*
     * Number of times when the PHY layer received new data on the channel.
     */
    unsigned short m_RxDropCount;
    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

public:
    //Constructor.
    DLMSPrimeNbOfdmPlcPhysicalLayerCounters();

    //SN Constructor.
    DLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln);

    /*
    * @return Number of bursts received on the physical layer for which the CRC
    *         was incorrect.
    */
    unsigned short GetCrcIncorrectCount();

    /*
     * @param value
     *            Number of bursts received on the physical layer for which the
     *            CRC was incorrect.
     */
    void SetCrcIncorrectCount(unsigned short value);

    /*
     * @return Number of bursts received on the physical layer for which the CRC
     *         was correct, but the Protocol field of PHY header had invalid
     *         value.
     */
    unsigned short GetCrcFailedCount();

    /*
     * @param value
     *            Number of bursts received on the physical layer for which the
     *            CRC was correct, but the Protocol field of PHY header had
     *            invalid value.
     */
    void SetCrcFailedCount(unsigned short value);

    /*
     * @return Number of times when PHY layer received new data to transmit.
     */
    unsigned short GetTxDropCount();

    /*
     * @param value
     *            Number of times when PHY layer received new data to transmit.
     */
    void SetTxDropCount(unsigned short value);

    /*
     * @return Number of times when the PHY layer received new data on the
     *         channel.
     */
    unsigned short GetRxDropCount();

    /*
     * @param value
     *            Number of times when the PHY layer received new data on the
     *            channel.
     */
    void SetRxDropCount(unsigned short value);

    //Reset the values.
    int Reset(DLMSClient* client, std::vector<ByteBuffer>& reply);

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
