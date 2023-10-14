#ifndef DLMS_Prime_NbOfdm_Plc_Mac_Counters_H
#define DLMS_Prime_NbOfdm_Plc_Mac_Counters_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_COUNTERS
#include "DLMSObject.h"

class DLMSPrimeNbOfdmPlcMacCounters : public DLMSObject
{
    /*
    * Count of successfully transmitted MSDUs.
    */
    unsigned long m_TxDataPktCount;

    /*
    * Count of successfully received MSDUs whose destination address was this
    * node.
    */
    unsigned long m_RxDataPktCount;
    /*
    * Count of successfully transmitted MAC control packets.
    */
    unsigned long m_TxCtrlPktCount;
    /*
    * Count of successfully received MAC control packets whose destination was
    * this node.
    */
    unsigned long m_RxCtrlPktCount;
    /*
    * Count of failed CSMA transmit attempts.
    */
    unsigned long m_CsmaFailCount;
    /*
    * Count of number of times this node has to back off SCP transmission due
    * to channel busy state.
    */
    unsigned long m_CsmaChBusyCount;

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);
public:
    //Constructor.
    DLMSPrimeNbOfdmPlcMacCounters();

    //SN Constructor.
    DLMSPrimeNbOfdmPlcMacCounters(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSPrimeNbOfdmPlcMacCounters(std::string ln);

    /*
     * Returns Count of successfully transmitted MSDUs.
     */
    unsigned int GetTxDataPktCount();

    /*
     * value:
     *            Count of successfully transmitted MSDUs.
     */
    void SetTxDataPktCount(unsigned int value);

    /*
     * Returns Count of successfully received MSDUs whose destination address
     *         was this node.
     */
    unsigned int GetRxDataPktCount();

    /*
     * value:
     *            Count of successfully received MSDUs whose destination address
     *            was this node.
     */
    void SetRxDataPktCount(unsigned int value);

    /*
     * Returns Count of successfully transmitted MAC control packets.
     */
    unsigned int GetTxCtrlPktCount();

    /*
     * value:
     *            Count of successfully transmitted MAC control packets.
     */
    void SetTxCtrlPktCount(unsigned int value);

    /*
     * Returns Count of successfully received MAC control packets whose
     *         destination was this node.
     */
    unsigned int GetRxCtrlPktCount();

    /*
     * value:
     *            Count of successfully received MAC control packets whose
     *            destination was this node.
     */
    void SetRxCtrlPktCount(unsigned int value);

    /*
     * Returns Count of failed CSMA transmit attempts.
     */
    unsigned int GetCsmaFailCount();

    /*
     * value:
     *            Count of failed CSMA transmit attempts.
     */
    void SetCsmaFailCount(unsigned int value);

    /*
     * Returns Count of number of times this node has to back off SCP
     *         transmission due to channel busy state.
     */
    unsigned int GetCsmaChBusyCount();

    /*
     * value:
     *            Count of number of times this node has to back off SCP
     *            transmission due to channel busy state.
     */
    void SetCsmaChBusyCount(unsigned int value);

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
