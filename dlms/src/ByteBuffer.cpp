#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "ErrorCodes.h"
#include "ByteBuffer.h"
#include "Helpers.h"

//Constructor.
ByteBuffer::ByteBuffer()
{
    m_Capacity = 0;
    m_Data = NULL;
    m_Position = 0;
    m_Size = 0;
}

//Constructor.
ByteBuffer::ByteBuffer(int capacity)
{
    m_Capacity = 0;
    m_Data = NULL;
    m_Position = 0;
    m_Size = 0;
    Capacity(capacity);
}

//Copy constructor.
ByteBuffer::ByteBuffer(const ByteBuffer& value)
{
    m_Capacity = 0;
    m_Data = NULL;
    m_Position = 0;
    m_Size = 0;
    if (value.m_Size - value.m_Position != 0)
    {
        Set(value.m_Data + value.m_Position, value.m_Size - value.m_Position);
    }
}

//Destructor.
ByteBuffer::~ByteBuffer()
{
    Clear();
}

unsigned long ByteBuffer::Available()
{
    return m_Size - m_Position;
}

unsigned long ByteBuffer::GetSize()
{
    return m_Size;
}

void ByteBuffer::SetSize(unsigned long value)
{
    assert(!(value > m_Capacity));
    m_Size = value;
    if (m_Position > m_Size)
    {
        m_Position = m_Size;
    }
}

int ByteBuffer::IncreaseSize(unsigned long size)
{
    if (size > 1)
    {
        return -1;
    }
    m_Size += size;
    return m_Size;
}


//Returns position of the buffer.
unsigned long ByteBuffer::GetPosition()
{
    return m_Position;
}

//Sets position of the buffer.
void ByteBuffer::SetPosition(unsigned long value)
{
    assert(!(value > m_Size));
    m_Position = value;
}

// Allocate new size for the array in bytes.
int ByteBuffer::Capacity(unsigned long capacity)
{
    m_Capacity = capacity;
    if (capacity == 0)
    {
        if (m_Data != NULL)
        {
            free(m_Data);
            m_Data = NULL;
        }
        m_Size = 0;
        m_Position = 0;
    }
    else
    {
        unsigned char* tmp;
        if (m_Data == NULL)
        {
            tmp = (unsigned char*)malloc(m_Capacity);
        }
        else
        {
            tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        }
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
        if (m_Size > capacity)
        {
            m_Size = capacity;
        }
    }
    return 0;
}

unsigned long ByteBuffer::Capacity()
{
    return m_Capacity;
}

// Fill buffer it with zeros.
void ByteBuffer::Zero(unsigned long index, unsigned long count)
{
    if (index + count > m_Capacity)
    {
        ByteBuffer::Capacity(index + count);
    }
    if (m_Size < index + count)
    {
        m_Size = index + count;
    }
    memset(m_Data + index, 0, count);
}

// Push new data to the byteArray.
int ByteBuffer::SetUInt8(unsigned char item)
{
    int ret = SetUInt8(m_Size, item);
    if (ret == 0)
    {
        ++m_Size;
    }
    return ret;
}

int ByteBuffer::SetUInt8(unsigned long index, unsigned char item)
{
    if (m_Capacity == 0 || index + 1 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[index] = item;
    return 0;
}

int ByteBuffer::SetUInt16(unsigned short item)
{
    int ret = SetUInt16(m_Size, item);
    if (ret == 0)
    {
        m_Size += 2;
    }
    return ret;
}

int ByteBuffer::SetUInt16(unsigned long index, unsigned short item)
{
    if (m_Capacity == 0 || index + 2 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[index] = (item >> 8) & 0xFF;
    m_Data[index + 1] = item & 0xFF;
    return 0;
}

int ByteBuffer::SetUInt32(unsigned long item)
{
    int ret = ByteBuffer::SetUInt32ByIndex(m_Size, item);
    if (ret == 0)
    {
        m_Size += 4;
    }
    return ret;
}

int ByteBuffer::SetUInt32ByIndex(unsigned long index, unsigned long item)
{
    if (m_Capacity == 0 || index + 4 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[index] = (item >> 24) & 0xFF;
    m_Data[index + 1] = (item >> 16) & 0xFF;
    m_Data[index + 2] = (item >> 8) & 0xFF;
    m_Data[index + 3] = item & 0xFF;
    return 0;
}

int ByteBuffer::SetUInt64(unsigned long long item)
{
    if (m_Capacity == 0 || m_Size + 8 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[m_Size] = (unsigned char)((item >> 56) & 0xFF);
    m_Data[m_Size + 1] = (item >> 48) & 0xFF;
    m_Data[m_Size + 2] = (item >> 40) & 0xFF;
    m_Data[m_Size + 3] = (item >> 32) & 0xFF;
    m_Data[m_Size + 4] = (item >> 24) & 0xFF;
    m_Data[m_Size + 5] = (item >> 16) & 0xFF;
    m_Data[m_Size + 6] = (item >> 8) & 0xFF;
    m_Data[m_Size + 7] = item & 0xFF;
    m_Size += 8;
    return 0;
}

int ByteBuffer::SetFloat(float value)
{
    typedef union
    {
        float value;
        char b[sizeof(float)];
    } HELPER;

    HELPER tmp;
    tmp.value = value;
    if (m_Capacity == 0 || m_Size + 4 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[m_Size] = tmp.b[3];
    m_Data[m_Size + 1] = tmp.b[2];
    m_Data[m_Size + 2] = tmp.b[1];
    m_Data[m_Size + 3] = tmp.b[0];
    m_Size += 4;
    return 0;
}

int ByteBuffer::SetDouble(double value)
{
    typedef union
    {
        double value;
        char b[sizeof(double)];
    } HELPER;

    HELPER tmp;
    tmp.value = value;
    if (m_Capacity == 0 || m_Size + 8 > m_Capacity)
    {
        m_Capacity += VECTOR_CAPACITY;
        unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
        //If not enought memory available.
        if (tmp == NULL)
        {
            return DLMS_ERROR_CODE_OUTOFMEMORY;
        }
        m_Data = tmp;
    }
    m_Data[m_Size] = tmp.b[7];
    m_Data[m_Size + 1] = tmp.b[6];
    m_Data[m_Size + 2] = tmp.b[5];
    m_Data[m_Size + 3] = tmp.b[4];
    m_Data[m_Size + 4] = tmp.b[3];
    m_Data[m_Size + 5] = tmp.b[2];
    m_Data[m_Size + 6] = tmp.b[1];
    m_Data[m_Size + 7] = tmp.b[0];
    m_Size += 8;
    return 0;
}

int ByteBuffer::SetInt8(char item)
{
    return ByteBuffer::SetUInt8((unsigned char)item);
}

int ByteBuffer::SetInt16(short item)
{
    return ByteBuffer::SetUInt16((unsigned short)item);
}

int ByteBuffer::SetInt32(long item)
{
    return ByteBuffer::SetUInt32((unsigned long)item);
}

int ByteBuffer::SetInt64(long long item)
{
    return ByteBuffer::SetUInt64((unsigned long long) item);
}

int ByteBuffer::Set(const void* pSource, unsigned long count)
{
    if (pSource != NULL && count != 0)
    {
        if (m_Size + count > m_Capacity)
        {
            //First time data is reserved only for the added data.
            if (m_Capacity == 0)
            {
                m_Capacity = count;
            }
            else
            {
                m_Capacity += count + VECTOR_CAPACITY;
            }
            unsigned char* tmp = (unsigned char*)realloc(m_Data, m_Capacity);
            //If not enought memory available.
            if (tmp == NULL)
            {
                return DLMS_ERROR_CODE_OUTOFMEMORY;
            }
            m_Data = tmp;
        }
        memcpy(m_Data + m_Size, pSource, count);
        m_Size += count;
    }
    return 0;
}

int ByteBuffer::Set(ByteBuffer* data, unsigned long index, unsigned long count)
{
    if (data != NULL)
    {
        if (count == (unsigned long)-1)
        {
            count = data->m_Size - index;
        }
        int ret = ByteBuffer::Set(data->m_Data + index, count);
        if (ret == 0)
        {
            data->m_Position += count;
        }
    }
    return 0;
}

int ByteBuffer::AddString(const char* value)
{
    if (value != NULL)
    {
        unsigned long len = (unsigned long)strlen(value);
        return ByteBuffer::Set(value, len);
    }
    return 0;
}

int ByteBuffer::AddString(const std::string& value)
{
    return ByteBuffer::Set(value.c_str(), (unsigned long)value.length());
}

int ByteBuffer::AttachString(char* value)
{
    unsigned long len = (unsigned long)strlen(value);
    int ret = ByteBuffer::Set(value, len);
    if (ret == 0)
    {
        free(value);
    }
    return ret;
}

void ByteBuffer::Clear()
{
    Capacity(0);
}

int ByteBuffer::GetUInt8(unsigned char* value)
{
    if (m_Position >= m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[m_Position];
    ++m_Position;
    return 0;
}

int ByteBuffer::GetUInt8(unsigned long index, unsigned char* value)
{
    if (index >= m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[index];
    return 0;
}


int ByteBuffer::GetUInt16(unsigned short* value)
{
    if (m_Position + 2 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = (((m_Data[m_Position] & 0xFF) << 8) | (m_Data[m_Position + 1] & 0xFF));
    m_Position += 2;
    return 0;
}

int ByteBuffer::GetUInt24(unsigned long index, unsigned int* value)
{
    if (index + 3 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[index] << 16 |
        m_Data[index + 1] << 8 |
        m_Data[index + 2];
    return 0;
}

int ByteBuffer::GetUInt24(unsigned int* value)
{
    int ret = GetUInt24(m_Position, value);
    if (ret == 0)
    {
        m_Position += 3;
    }
    return ret;
}

int ByteBuffer::GetUInt32(unsigned long* value)
{
    if (m_Position + 4 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[m_Position] << 24 |
        m_Data[m_Position + 1] << 16 |
        m_Data[m_Position + 2] << 8 |
        m_Data[m_Position + 3];
    m_Position += 4;
    return 0;
}

int ByteBuffer::GetInt16(short* value)
{
    if (m_Position + 2 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[m_Position] << 8 |
        m_Data[m_Position + 1];
    m_Position += 2;
    return 0;
}

int ByteBuffer::GetInt32(long* value)
{
    int ret = GetUInt32(m_Position, (unsigned long*)value);
    m_Position += 4;
    return ret;
}

int ByteBuffer::GetUInt32(unsigned long index, unsigned long* value)
{
    if (index + 4 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[index] << 24 |
        m_Data[index + 1] << 16 |
        m_Data[index + 2] << 8 |
        m_Data[index + 3];
    return 0;
}

int ByteBuffer::GetInt64(long long* value)
{
    int ret = ByteBuffer::GetUInt64(m_Position, (unsigned long long*) value);
    if (ret == 0)
    {
        m_Position += 8;
    }
    return ret;
}

int ByteBuffer::GetUInt64(unsigned long long* value)
{
    int ret = ByteBuffer::GetUInt64(m_Position, value);
    if (ret == 0)
    {
        m_Position += 8;
    }
    return ret;
}

int ByteBuffer::GetUInt64(unsigned long index, unsigned long long* value)
{
    if (index + 8 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = (unsigned long long)m_Data[index] << 56 |
        (unsigned long long) m_Data[index + 1] << 48 |
        (unsigned long long) m_Data[index + 2] << 40 |
        (unsigned long long) m_Data[index + 3] << 32 |
        (unsigned long long) m_Data[index + 4] << 24 |
        (unsigned long long) m_Data[index + 5] << 16 |
        (unsigned long long) m_Data[index + 6] << 8 |
        (unsigned long long) m_Data[index + 7];
    return 0;
}

int ByteBuffer::GetUInt128(unsigned long index, unsigned char* value)
{
    int ret = ByteBuffer::GetUInt32(index, (unsigned long*)value);
    if (ret == 0)
    {
        ret = ByteBuffer::GetUInt32(index + 4, (unsigned long*)value + 1);
        if (ret == 0)
        {
            ret = ByteBuffer::GetUInt32(index + 8, (unsigned long*)value + 2);
            if (ret == 0)
            {
                ret = ByteBuffer::GetUInt32(index + 12, (unsigned long*)value + 3);
            }
        }
    }
    return ret;
}

int ByteBuffer::GetFloat(float* value)
{
    if (m_Position + 4 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    typedef union
    {
        float value;
        char b[sizeof(float)];
    } HELPER;
    HELPER tmp;
    tmp.b[3] = m_Data[m_Position];
    tmp.b[2] = m_Data[m_Position + 1];
    tmp.b[1] = m_Data[m_Position + 2];
    tmp.b[0] = m_Data[m_Position + 3];
    *value = tmp.value;
    m_Position += 4;
    return 0;
}

int ByteBuffer::GetDouble(double* value)
{
    if (m_Position + 8 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    typedef union
    {
        double value;
        char b[sizeof(double)];
    } HELPER;
    HELPER tmp;
    tmp.b[7] = m_Data[m_Position];
    tmp.b[6] = m_Data[m_Position + 1];
    tmp.b[5] = m_Data[m_Position + 2];
    tmp.b[4] = m_Data[m_Position + 3];
    tmp.b[3] = m_Data[m_Position + 4];
    tmp.b[2] = m_Data[m_Position + 5];
    tmp.b[1] = m_Data[m_Position + 6];
    tmp.b[0] = m_Data[m_Position + 7];
    *value = tmp.value;
    m_Position += 8;
    return 0;
}

int ByteBuffer::Get(unsigned char* value, unsigned long count)
{
    if (value == NULL || m_Size - m_Position < count)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    memcpy(value, m_Data + m_Position, count);
    m_Position += count;
    return 0;
}

unsigned char* ByteBuffer::GetData()
{
    return m_Data;
}

int ByteBuffer::GetUInt16(unsigned long index, unsigned short* value)
{
    if (index + 2 > m_Size)
    {
        return DLMS_ERROR_CODE_OUTOFMEMORY;
    }
    *value = m_Data[index] << 8 |
        m_Data[index + 1];
    return 0;
}

std::string ByteBuffer::ToString()
{
    std::string str;
    if (m_Size != 0)
    {
        str.append(reinterpret_cast<char const*>(m_Data), m_Size);
    }
    return str;
}

std::string ByteBuffer::ToHexString()
{
    return Helpers::BytesToHex(m_Data, m_Size);
}

std::string ByteBuffer::ToHexString(bool addSpaces)
{
    return Helpers::BytesToHex(m_Data, m_Size, addSpaces);
}

std::string ByteBuffer::ToHexString(unsigned long position, unsigned long count, bool addSpaces)
{
    if (m_Data == NULL || count == 0)
    {
        return "";
    }
    return Helpers::BytesToHex(m_Data + position, count, addSpaces);
}

void ByteBuffer::AddIntAsString(int value)
{
    std::string str = Helpers::IntToString(value);
    ByteBuffer::AddString(str.c_str());
}

void ByteBuffer::AddDoubleAsString(double value)
{
    char buff[20];
    //Show as integer value if there is no fractal part.
    if (value - (long)value == 0)
    {
        ByteBuffer::AddIntAsString((int)value);
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(buff, 20, "%lf", value);
#else
        sprintf(buff, "%lf", value);
#endif
        ByteBuffer::AddString(buff);
    }
}

/**
    * Returns data as byte array.
    *
    * @return Byte buffer as a byte array.
    */
int ByteBuffer::SubArray(unsigned long index, int count, ByteBuffer& bb)
{
    bb.Clear();
    bb.Set(this, index, count);
    return 0;
}

int ByteBuffer::Move(unsigned long srcPos, unsigned long destPos, unsigned long count)
{
    if (count != 0)
    {
        if (destPos + count > m_Capacity)
        {
            int ret = Capacity(destPos + count);
            if (ret != 0)
            {
                return ret;
            }
        }
        //Do not use memcpy here!
        memmove(m_Data + destPos, m_Data + srcPos, count);
        m_Size = (destPos + count);
        if (m_Position > m_Size)
        {
            m_Position = m_Size;
        }
    }
    return DLMS_ERROR_CODE_OK;
}

void ByteBuffer::Trim()
{
    if (m_Size == m_Position)
    {
        m_Size = 0;
    }
    else
    {
        Move(m_Position, 0, m_Size - m_Position);
    }
    m_Position = 0;
}

/**
   * Compares, whether two given arrays are similar starting from current
   * position.
   *
   * @param arr
   *            Array to compare.
   * @return True, if arrays are similar. False, if the arrays differ.
   */
bool ByteBuffer::Compare(unsigned char* buff, unsigned long length)
{
    if (m_Size - m_Position < length)
    {
        return false;
    }
    bool equal = memcmp(m_Data + m_Position, buff, length) == 0;
    if (equal)
    {
        m_Position += length;
    }
    return equal;
}

void ByteBuffer::ToArray(unsigned char*& value, unsigned long& count)
{
    if (value != NULL)
    {
        free(value);
    }
    count = m_Size;
    value = (unsigned char*)malloc(count);
    memcpy(value, m_Data, count);
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer& value)
{
    Capacity(value.GetSize());
    m_Size = 0;
    if (value.GetSize() != 0)
    {
        Set(value.m_Data, value.m_Size);
    }
    return *this;
}

void ByteBuffer::SetHexString(std::string& value)
{
    ByteBuffer tmp;
    Helpers::HexToBytes(value, tmp);
    Set(&tmp);
}

void ByteBuffer::SetHexString(std::string value)
{
    ByteBuffer tmp;
    Helpers::HexToBytes(value, tmp);
    Set(&tmp);
}

void ByteBuffer::SetHexString(char* value)
{
    ByteBuffer tmp;
    std::string str = value;
    Helpers::HexToBytes(str, tmp);
    Set(&tmp);
}

bool ByteBuffer::IsAsciiString(unsigned char* value, unsigned long length)
{
    if (value != NULL)
    {
        for (unsigned long pos = 0; pos != length; ++pos)
        {
            unsigned char it = value[pos];
            if ((it < 32 || it > 127) && it != '\r' && it != '\n' && it != 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool ByteBuffer::IsAsciiString()
{
    return IsAsciiString(m_Data, m_Size);
}
