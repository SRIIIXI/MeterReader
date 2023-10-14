#ifndef SERVER_REPLY_H
#define SERVER_REPLY_H

#include "ByteBuffer.h"
#include "DLMSConnectionEventArgs.h"

class ServerReply
{
private:
    /*
    * Connection info.
    */
    DLMSConnectionEventArgs m_ConnectionInfo;

    /*
     * Server received data.
     */
    ByteBuffer m_Data;

    /*
     * Server reply message.
     */
    ByteBuffer m_Reply;

    /*
     * Message count to send.
     */
    int m_Count;
public:
    /*
    * Constructor.
    */
    ServerReply()
    {
        m_Count = 0;
    }

    /*
     * Constructor.
     *
     * @param value
     *            Received data.
     */
    ServerReply(ByteBuffer& value)
    {
        m_Count = 0;
        m_Data = value;
    }

    /*
     * returns the data
     */
    ByteBuffer& GetData()
    {
        return m_Data;
    }

    /*
     * value: The data to set.
     */
    void SetData(ByteBuffer& value)
    {
        m_Data = value;
    }

    /*
     * returns The reply message.
     */
    ByteBuffer& GetReply()
    {
        return m_Reply;
    }

    /*
     * value: the replyMessages to set
     */
    void SetReply(ByteBuffer& value)
    {
        m_Reply = value;
    }

    /*
     * returns Connection info.
     */
    DLMSConnectionEventArgs& GetConnectionInfo()
    {
        return m_ConnectionInfo;
    }

    /*
     * value: Connection info.
     */
    void SetConnectionInfo(DLMSConnectionEventArgs& value)
    {
        m_ConnectionInfo = value;
    }

    /*
     * returns Is GBT streaming in progress.
     */
    bool IsStreaming()
    {
        return GetCount() != 0;
    }

    /*
     * returns Message count to send.
     */
    int GetCount()
    {
        return m_Count;
    }

    /*
     * value: Message count to send.
     */
    void SetCount(int value)
    {
        m_Count = value;
    }
};

#endif
