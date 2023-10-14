#ifndef DLMS_IP4_SETUP_IP_OPTION_H
#define DLMS_IP4_SETUP_IP_OPTION_H

#include "Ignore.h"
#include "DLMSObject.h"

class DLMSIp4SetupIpOption
{
    IP_OPTION_TYPE m_Type;
    short m_Length;
    ByteBuffer m_Data;
public:
    IP_OPTION_TYPE GetType();
    void SetType(IP_OPTION_TYPE value);

    short GetLength();
    void SetLength(short value);
    ByteBuffer& GetData();
    void SetData(ByteBuffer& value);

    std::string ToString();
};
#endif
