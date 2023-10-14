#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <string>

const unsigned char VECTOR_CAPACITY = 50;

class ByteBuffer
{
    friend class Cipher;
    unsigned char* m_Data;
    unsigned long m_Capacity;
    unsigned long m_Size;
    unsigned long m_Position;
public:
    //Constructor.
    ByteBuffer();

    //Constructor.
    ByteBuffer(int capacity);

    //Copy constructor.
    ByteBuffer(const ByteBuffer& value);

    //Destructor.
    ~ByteBuffer();

    //Amount of non read bytes in the buffer.
    unsigned long Available();

    //Returns buffer size.
    unsigned long GetSize();

    //Set buffer size.
    void SetSize(unsigned long value);

    //Increase buffer size.
    int IncreaseSize(unsigned long size);

    //Returns position of the buffer.
    unsigned long GetPosition();

    //Returns position of the buffer.
    void SetPosition(unsigned long value);

    //Allocate new size for the array in bytes.
    int Capacity(unsigned long capacity);

    //Get buffer capacity.
    unsigned long Capacity();

    //Fill buffer it with zeros.
    void Zero(unsigned long index, unsigned long count);

    //Push new data to the byteArray.
    int SetUInt8(unsigned char item);

    int SetUInt8(unsigned long index, unsigned char item);

    int SetUInt16(unsigned short item);
    int SetUInt16(unsigned long index, unsigned short item);

    int SetUInt32(unsigned long item);

    int SetUInt32ByIndex(unsigned long index, unsigned long item);

    int SetUInt64(unsigned long long item);

    int SetInt8(char item);

    int SetInt16(short item);

    int SetInt32(long item);

    int SetInt64(long long item);

    int SetFloat(float value);

    int SetDouble(double value);

    int Set(const void* pSource, unsigned long count);

    int Set(ByteBuffer* data, unsigned long index = 0, unsigned long count = -1);

    //Add string to byte buffer.
    int AddString(const std::string& value);

    int AddString(const char* value);

    int AttachString(char* value);

    void Clear();

    int GetUInt8(unsigned char* value);

    int GetUInt8(unsigned long index, unsigned char* value);

    int GetUInt16(unsigned short* value);

    int GetUInt24(unsigned long index, unsigned int* value);
    int GetUInt24(unsigned int* value);

    int GetUInt32(unsigned long* value);

    int GetInt16(short* value);

    int GetInt32(long* value);

    int GetInt64(long long* value);

    int GetUInt64(unsigned long long* value);

    int GetFloat(
        float* value);

    int GetDouble(
        double* value);

    int Get(unsigned char* value, unsigned long count);

    unsigned char* GetData();

    void ToArray(unsigned char*& value, unsigned long& count);

    /**
     * Remove handled bytes. This can be used in debugging to remove handled
     * bytes.
     */
    void Trim();


    /**
     * Compares, whether two given arrays are similar starting from current
     * position.
     *
     * @param arr
     *            Array to compare.
     * @return True, if arrays are similar. False, if the arrays differ.
     */
    bool Compare(unsigned char* buff, unsigned long length);

    int GetUInt16(unsigned long index, unsigned short* value);
    int GetUInt32(unsigned long index, unsigned long* value);
    int GetUInt64(unsigned long index, unsigned long long* value);
    int GetUInt128(unsigned long index, unsigned char* value);

    //Get byte array as a std::string.
    std::string ToString();

    //Get byte array as hex std::string.
    std::string ToHexString();

    //Get byte array as hex std::string.
    std::string ToHexString(bool addSpaces);

    //Get byte array as hex std::string.
    std::string ToHexString(unsigned long position, unsigned long count, bool addSpaces);

    //Add integer value to byte array as a std::string.
    void AddIntAsString(int value);

    //Add double value to byte array as a std::string.
    void AddDoubleAsString(double value);

    /**
        * Returns data as byte array.
        *
        * @param bb Byte buffer as a byte array.
        */
    int SubArray(unsigned long index, int count, ByteBuffer& bb);

    //Move data insize byte array.
    int Move(unsigned long srcPos, unsigned long destPos, unsigned long count);

    ByteBuffer& operator=(ByteBuffer& value);

    //Push the given hex string as byte array into this buffer at the current position, and then increments the position.
    void SetHexString(std::string& value);

    //Push the given hex string as byte array into this buffer at the current position, and then increments the position.
    void SetHexString(std::string value);

        //Push the given hex string as byte array into this buffer at the current position, and then increments the position.
    void SetHexString(char* value);

    //Check is byte buffer ASCII string.
    static bool IsAsciiString(unsigned char* value, unsigned long length);

    //Check is byte buffer ASCII string.
    bool IsAsciiString();

};

#endif
