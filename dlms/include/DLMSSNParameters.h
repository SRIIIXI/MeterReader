#ifndef DLMS_SN_PARAMETERS_H
#define DLMS_SN_PARAMETERS_H

#include "ByteBuffer.h"
#include "DLMSSettings.h"

/**
 * SN Parameters
 */
class DLMSSNParameters
{
private:

    /**
     * DLMS settings.
     */
    DLMSSettings *m_Settings;
    /**
     * DLMS command.
     */
    DLMS_COMMAND m_Command;
    /**
     * Request type.
     */
    int m_RequestType;
    /**
     * Attribute descriptor.
     */
    ByteBuffer* m_AttributeDescriptor;
    /**
     * Data.
     */
    ByteBuffer* m_Data;
    /**
     * Send date and time. This is used in Data notification messages.
     */
    struct tm* m_Time;
    /**
     * Item Count.
     */
    int m_Count;

    /**
     * Are there more data to send or more data to receive.
     */
    bool m_MultipleBlocks;

    /**
     * Block index.
     */
    unsigned short m_BlockIndex;

public:

    /**
     * Constructor.
     *
     * @param settings
     *            DLMS settings.
     * @param command
     *            Command.
     * @param commandType
     *            command type.
     * @param attributeDescriptor
     * @param data
     *            Attribute descriptor
     */
    DLMSSNParameters(DLMSSettings* settings,
                        DLMS_COMMAND command,
                        int count,
                        int commandType,
                        ByteBuffer* attributeDescriptor,
                        ByteBuffer* data);

    /**
     * @return DLMS settings.
     */
    DLMSSettings* GetSettings();

    /**
     * @return DLMS command.
     */
    DLMS_COMMAND GetCommand();

    /**
     * @return Request type.
     */
    int GetRequestType();

    /**
     * @param value
     *            Request type.
     */
    void SetRequestType(int value);

    /**
     * @return the attributeDescriptor
     */
    ByteBuffer* GetAttributeDescriptor();

    /**
     * @return the data
     */
    ByteBuffer* GetData();

    /**
     * @return the time
     */
    struct tm* GetTime();

    /**
     * @param value
     *            the time to set
     */
    void SetTime(struct tm* value);

    /**
     * @return the multipleBlocks
     */
    bool IsMultipleBlocks();

    /**
     * @param value
     *            the multipleBlocks to set
     */
    void SetMultipleBlocks(bool value);

    /**
     * @return Block index.
     */
    unsigned short GetBlockIndex();

    /**
     * @param value
     *            Block index.
     */
    void SetBlockIndex(unsigned short value);

    /**
     * @return Item Count.
     */
    int GetCount();
};

#endif
