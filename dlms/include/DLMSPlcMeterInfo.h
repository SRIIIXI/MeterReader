#ifndef DLMS_PLC_METERINFO_H
#define DLMS_PLC_METERINFO_H

#include "Enums.h"
#include "ByteBuffer.h"

//Information from the discovered PLC meter(s).
class DLMSPlcMeterInfo
{
private:
    /**
     * Source Address.
     */
    uint16_t m_SourceAddress;

    /**
     * Destination Address.
     */
    uint16_t m_DestinationAddress;
    /**
     * System title.
     */
    ByteBuffer m_SystemTitle;
    /**
     * Alarm descriptor.
     */
    short m_AlarmDescriptor;

public:

    uint16_t GetSourceAddress()
    {
        return m_SourceAddress;
    }

    void SetSourceAddress(uint16_t value)
    {
        m_SourceAddress = value;
    }

    /**
     * Destination Address.
     */
    uint16_t GetDestinationAddress()
    {
        return m_DestinationAddress;
    }

    void SetDestinationAddress(uint16_t value)
    {
        m_DestinationAddress = value;
    }

    /**
     * System title.
     */
    ByteBuffer& GetSystemTitle()
    {
        return m_SystemTitle;
    }

    void SetSystemTitle(ByteBuffer& value)
    {
        m_SystemTitle = value;
    }

    /**
     * Alarm descriptor.
     */
    short GetAlarmDescriptor()
    {
        return m_AlarmDescriptor;
    }

    void SetAlarmDescriptor(short value)
    {
        m_AlarmDescriptor = value;
    }
};

#endif
