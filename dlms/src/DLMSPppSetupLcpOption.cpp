#include "DLMSPppSetupLcpOption.h"
#include <sstream>

PPP_SETUP_LCP_OPTION_TYPE DLMSPppSetupLcpOption::GetType()
{
    return m_Type;
}
void DLMSPppSetupLcpOption::SetType(PPP_SETUP_LCP_OPTION_TYPE value)
{
    m_Type = value;
}

int DLMSPppSetupLcpOption::GetLength()
{
    return m_Length;
}

void DLMSPppSetupLcpOption::SetLength(int value)
{
    m_Length = value;
}

DLMSVariant& DLMSPppSetupLcpOption::GetData()
{
    return m_Data;
}

void DLMSPppSetupLcpOption::SetData(DLMSVariant& value)
{
    m_Data = value;
}

std::string DLMSPppSetupLcpOption::ToString()
{
    std::stringstream sb;
    sb << m_Type;
    sb << " ";
    sb << m_Length;
    return sb.str();
}
