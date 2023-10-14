#ifndef REPLY_DATA_H
#define REPLY_DATA_H

#include "DLMSConverter.h"
#include "ByteBuffer.h"
#include "DLMSVariant.h"
#include "DLMSTranslatorStructure.h"

class ReplyData
{
private:
    /*
     * Is more data available.
     */
    DLMS_DATA_REQUEST_TYPES m_MoreData;
    /*
     * Received command.
     */
    DLMS_COMMAND m_Command;

    /*
    * Received command type.
    */
    unsigned char m_CommandType;

    /*
     * Received data.
     */
    ByteBuffer  m_Data;
    ByteBuffer* m_pData;
    /*
     * Is frame complete.
     */
    bool m_Complete;

    /*
     * Read value.
     */
    DLMSVariant m_DataValue;

    /*
     * Expected count of element in the array.
     */
    int m_TotalCount;

    /*
     * Last read position. This is used in peek to solve how far data is read.
     */
    unsigned long m_ReadPosition;

    /*
     * Packet Length.
     */
    int m_PacketLength;

    /*
     * Try Get value.
     */
    bool m_Peek;

    DLMS_DATA_TYPE m_DataType;

    /*
     * Cipher index is position where data is decrypted.
     */
    unsigned short m_CipherIndex;

    /*
     * Data notification date time.
     */
    struct tm* m_Time;

#ifndef DLMS_IGNORE_XML_TRANSLATOR
    DLMSTranslatorStructure* m_pXml;
#endif //DLMS_IGNORE_XML_TRANSLATOR

    /*
    * Invoke ID.
    */
    long m_InvokeId;

    /*
     * GBT block number.
     */
    int m_BlockNumber;
    /*
     * GBT block number ACK.
     */
    int m_BlockNumberAck;
    /*
     * Is GBT streaming in use.
     */
    bool m_Streaming;
    /*
     * GBT Window size. This is for internal use.
     */
    unsigned char m_GbtWindowSize;

    /*
     * Client address of the notification message. Notification message sets
     * this.
     */
    unsigned short m_ClientAddress;

    /*
     * Server address of the notification message. Notification message sets
     * this.
     */
    int m_ServerAddress;

    unsigned char m_CipheredCommand;

public:
    /*
     * Constructor.
     *
     * @param more
     *            Is more data available.
     * @param cmd
     *            Received command.
     * @param buff
     *            Received data.
     * @param forComplete
     *            Is frame complete.
     */
    ReplyData(
        DLMS_DATA_REQUEST_TYPES more,
        DLMS_COMMAND cmd,
        ByteBuffer* buff,
        bool complete);

    /*
     * Constructor.
     */
    ReplyData();

    DLMS_DATA_TYPE GetValueType();

    void SetValueType(DLMS_DATA_TYPE value);


    DLMSVariant& GetValue();


    void SetValue(DLMSVariant& value);

    unsigned long GetReadPosition();

    void SetReadPosition(unsigned long value);

    int GetPacketLength();

    void SetPacketLength(int value);

    void SetCommand(DLMS_COMMAND value);

    void SetCommandType(unsigned char value);

    unsigned char GetCommandType();


    void SetData(ByteBuffer& value);

    void SetComplete(bool value);

    void SetTotalCount(int value);

    /*
     * Reset data values to default.
     */
    void Clear();

    /*
     * @return Is more data available.
     */
    bool IsMoreData();

    /*
     * Is more data available.
     *
     * @return Return None if more data is not available or Frame or Block type.
     */
    DLMS_DATA_REQUEST_TYPES GetMoreData();

    void SetMoreData(DLMS_DATA_REQUEST_TYPES value);


    /*
     * Get received command.
     *
     * @return Received command.
     */
    DLMS_COMMAND GetCommand();

    /*
     * Get received data.
     *
     * @return Received data.
     */
    ByteBuffer& GetData();

    /*
     * Is frame complete.
     *
     * @return Returns true if frame is complete or false if bytes is missing.
     */
    bool IsComplete();

    /*
     * Get total count of element in the array. If this method is used peek must
     * be Set true.
     *
     * @return Count of element in the array.
     * @see peek
     * @see GetCount
     */
    int GetTotalCount();

    /*
     * Get count of read elements. If this method is used peek must be Set true.
     *
     * @return Count of read elements.
     * @see peek
     * @see GetTotalCount
     */
    int GetCount();

    /*
     * Get is value try to peek.
     *
     * @return Is value try to peek.
     * @see GetCount
     * @see GetTotalCount
     */
    bool GetPeek();

    /*
     * Set is value try to peek.
     *
     * @param value
     *            Is value try to peek.
     */
    void SetPeek(bool value);

    /*
     * @return Cipher index is position where data is decrypted.
     */
    unsigned short GetCipherIndex();

    /*
     * @param cipherIndex
     *            Cipher index is position where data is decrypted.
     */
    void SetCipherIndex(unsigned short value);

    /*
     * @return Data notification date time.
     */
    struct tm* GetTime();


    /*
     * @param time
     *            Data notification date time.
     */
    void SetTime(struct tm* value);

#ifndef DLMS_IGNORE_XML_TRANSLATOR
    /*
     * @return Data notification date time.
     */
    DLMSTranslatorStructure* GetXml();


    /*
     * @param time
     *            Data notification date time.
     */
    void SetXml(DLMSTranslatorStructure* value);
#endif //DLMS_IGNORE_XML_TRANSLATOR

    /**
    * Invoke ID.
    */
    long GetInvokeId();

    /**
     * Invoke ID.
     */
    void SetInvokeId(long value);

    /*
     * GBT block number.
     */
    int GetBlockNumber();

    /*
     * GBT block number.
     */
    void SetBlockNumber(int value);

    /**
     * GBT block number ACK.
     */
    int GetBlockNumberAck();

    /**
     * @param value
     *            GBT block number ACK.
     */
    void SetBlockNumberAck(int value);

    /**
     * @return Is GBT streaming in use.
     */
    bool GetStreaming();

    /**
     * Is GBT streaming in use.
     */
    void SetStreaming(bool value);

    /**
     * GBT Window size. This is for internal use.
     */
    unsigned char GetGbtWindowSize();

    /**
     * GBT Window size. This is for internal use.
     */
    void SetGbtWindowSize(unsigned char value);

    /**
     * Is GBT streaming.
     */
    bool IsStreaming();

    /**
     * Client address of the notification message. Notification message
     * sets this.
     */
    unsigned short GetClientAddress();

    /**
     *            Client address of the notification message. Notification
     *            message sets this.
     */
    void SetClientAddress(unsigned short value);

    /*
     * Server address of the notification message. Notification message
     *         sets this.
     */
    int GetServerAddress();

    /*
     *            Server address of the notification message. Notification
     *            message sets this.
     */
    void SetServerAddress(int value);

    /*
     * returns Received ciphered command.
     */
    unsigned char GetCipheredCommand();

    /*
     * value: Received ciphered command.
     */
    void SetCipheredCommand(unsigned char value);
};

#endif
