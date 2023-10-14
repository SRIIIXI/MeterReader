#include "DLMSPppSetupIPCPOption.h"
#include <sstream>

PPP_SETUP_IPCP_OPTION_TYPE DLMSPppSetupIPCPOption::GetType()
{
    return m_Type;
}
void DLMSPppSetupIPCPOption::SetType(PPP_SETUP_IPCP_OPTION_TYPE value)
{
    m_Type = value;
}

int DLMSPppSetupIPCPOption::GetLength()
{
    return m_Length;
}
void DLMSPppSetupIPCPOption::SetLength(int value)
{
    m_Length = value;
}

DLMSVariant& DLMSPppSetupIPCPOption::GetData()
{
    return m_Data;
}
void DLMSPppSetupIPCPOption::SetData(DLMSVariant& value)
{
    m_Data = value;
}

std::string DLMSPppSetupIPCPOption::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    sb << " ";
    sb << m_Length;
    return sb.str();
}
