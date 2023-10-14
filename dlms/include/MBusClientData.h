#ifndef MBUS_CLIENT_DATA_H
#define MBUS_CLIENT_DATA_H

#include "ByteBuffer.h"
#include "DLMSVariant.h"

/**
*  M-Bus data definition element.
*/
class MBusClientData
{
    /**
      * Data information block.
      */
    ByteBuffer m_DataInformation;
    /**
     * Value information block.
     */
    ByteBuffer m_ValueInformation;

    /**
     * Data.
     */
    DLMSVariant m_Data;
public:
    /**
     * Returns Data information block.
     */
    ByteBuffer& GetDataInformation()
    {
        return m_DataInformation;
    }

    /**
     * @param value
     *            Data information block.
     */
    void SetDataInformation(ByteBuffer& value)
    {
        m_DataInformation = value;
    }

    /**
     * Returns Value information block.
     */
    ByteBuffer& GetValueInformation()
    {
        return m_ValueInformation;
    }

    /**
     * @param value
     *            Value information block.
     */
    void SetValueInformation(ByteBuffer& value)
    {
        m_ValueInformation = value;
    }

    /**
     * Returns Data.
     */
    DLMSVariant& GetData()
    {
        return m_Data;
    }

    /**
     * @param value
     *            Data.
     */
    void SetData(DLMSVariant& value)
    {
        m_Data = value;
    }
};
#endif
