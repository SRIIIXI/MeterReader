#ifndef DLMS_LONG_TRANSACTION_H
#define DLMS_LONG_TRANSACTION_H

#include <vector>
#include "Enums.h"
#include "ByteBuffer.h"
#include "DLMSValueEventArg.h"
#include "DLMSValueEventCollection.h"

class DLMSLongTransaction
{
private:
    /**
    * Executed command.
    */
    DLMS_COMMAND m_Command;

    /**
     * Targets.
     */
    DLMSValueEventCollection m_Targets;

    /**
     * Extra data from PDU.
     */
    ByteBuffer m_Data;

public:

    /**
     * Constructor.
     *
     * @param targets
     * @param command
     * @param data
     */
    DLMSLongTransaction(DLMSValueEventCollection& targets,
                           DLMS_COMMAND command, ByteBuffer& data)
    {
        m_Targets.insert(m_Targets.end(), targets.begin(), targets.end());
        targets.clear();
        m_Command = command;
        m_Data.Set(&data, data.GetPosition());
    }

    /**
 * Constructor.
 *
 * @param targets
 * @param command
 * @param data
 */
    DLMSLongTransaction(DLMS_COMMAND command, ByteBuffer& data)
    {
        m_Command = command;
        m_Data.Set(&data, data.GetPosition());
    }

    /**
     * @return Executed command.
     */
    DLMS_COMMAND GetCommand()
    {
        return m_Command;
    }

    /**
     * @return Targets.
     */
    std::vector<DLMSValueEventArg*>& GetTargets()
    {
        return m_Targets;
    }

    /**
     * @return data.
     */
    ByteBuffer& GetData()
    {
        return m_Data;
    }

    /**
     * @param value
     *            New data.
     */
    void SetData(ByteBuffer& value)
    {
        m_Data.Clear();
        m_Data.Set(&value, value.GetPosition());
    }
};
#endif
