#include "DLMSIp4SetupIpOption.h"
#include <sstream>

IP_OPTION_TYPE DLMSIp4SetupIpOption::GetType()
{
    return m_Type;
}
void DLMSIp4SetupIpOption::SetType(IP_OPTION_TYPE value)
{
    m_Type = value;
}

short DLMSIp4SetupIpOption::GetLength()
{
    return m_Length;
}
void DLMSIp4SetupIpOption::SetLength(short value)
{
    m_Length = value;
}

ByteBuffer& DLMSIp4SetupIpOption::GetData()
{
    return m_Data;
}

void DLMSIp4SetupIpOption::SetData(ByteBuffer& value)
{
    m_Data = value;
}

std::string DLMSIp4SetupIpOption::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    return sb.str();
}
