#ifndef DLMS_CONNECTIONEVENT_ARGS_H
#define DLMS_CONNECTIONEVENT_ARGS_H

class DLMSConnectionEventArgs
{
private:
    // Server ID that client try to use to make connection.
    int m_ServerAddress;
public:
    /**
    * Constructor.
    */
    DLMSConnectionEventArgs()
    {
        m_ServerAddress = 0;
    }

    int GetServerAddress()
    {
        return m_ServerAddress;
    }
    void SetServerAddress(int value)
    {
        m_ServerAddress = value;
    }
};

#endif
