#ifndef DLMS_SFSK_MAC_COUNTERS_H
#define DLMS_SFSK_MAC_COUNTERS_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_SFSK_MAC_COUNTERS
#include "DLMSObject.h"

class DLMSSFSKMacCounters : public DLMSObject
{
    /**
     * List of synchronization registers.
     */
    std::vector< std::pair<uint16_t, uint32_t> > m_SynchronizationRegister;

    uint32_t m_PhysicalLayerDesynchronization;
    uint32_t m_TimeOutNotAddressedDesynchronization;

    uint32_t m_TimeOutFrameNotOkDesynchronization;

    uint32_t m_WriteRequestDesynchronization;
    uint32_t m_WrongInitiatorDesynchronization;
    /**
     * List of broadcast frames counter.
     */
    std::vector< std::pair<uint16_t, uint32_t> > m_BroadcastFramesCounter;
    /**
     * Repetitions counter.
     */
    uint32_t m_RepetitionsCounter;
    /**
     * Transmissions counter.
     */
    uint32_t m_TransmissionsCounter;

    /**
     * CRC OK frames counter.
     */
    uint32_t m_CrcOkFramesCounter;
    /**
     * CRC NOK frames counter.
     */
    uint32_t m_CrcNOkFramesCounter;
public:
    //Constructor.
    DLMSSFSKMacCounters();

    //SN Constructor.
    DLMSSFSKMacCounters(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSSFSKMacCounters(std::string ln);

    /**
    * List of synchronization registers.
    */
    std::vector< std::pair<uint16_t, uint32_t> >& GetSynchronizationRegister();

    long GetPhysicalLayerDesynchronization();
    void SetPhysicalLayerDesynchronization(uint32_t value);

    long GetTimeOutNotAddressedDesynchronization();
    void SetTimeOutNotAddressedDesynchronization(uint32_t value);

    long GetTimeOutFrameNotOkDesynchronization();
    void SetTimeOutFrameNotOkDesynchronization(long value);

    long GetWriteRequestDesynchronization();
    void SetWriteRequestDesynchronization(uint32_t value);

    long GetWrongInitiatorDesynchronization();
    void SetWrongInitiatorDesynchronization(uint32_t value);

    /*
     * List of broadcast frames counter.
     */
    std::vector< std::pair<uint16_t, uint32_t> >& GetBroadcastFramesCounter();

    /*
     * Repetitions counter.
     */
    uint32_t GetRepetitionsCounter();
    void SetRepetitionsCounter(uint32_t value);

    /**
     * Transmissions counter.
     */
    uint32_t GetTransmissionsCounter();
    void SetTransmissionsCounter(uint32_t value);

    /*
    * CRC OK frames counter.
    */
    uint32_t GetCrcOkFramesCounter();
    void SetCrcOkFramesCounter(uint32_t value);

    /**
     * @return CRC NOK frames counter.
     */
    uint32_t GetCrcNOkFramesCounter();
    void SetCrcNOkFramesCounter(uint32_t value);

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
