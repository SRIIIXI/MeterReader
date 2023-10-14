#include "DLMSModemInitialisation.h"
#include <sstream>

DLMSModemInitialisation::DLMSModemInitialisation()
{
    m_Delay = 0;
}

std::string DLMSModemInitialisation::GetRequest()
{
    return m_Request;
}
void DLMSModemInitialisation::SetRequest(std::string value)
{
    m_Request = value;
}

std::string DLMSModemInitialisation::GetResponse()
{
    return m_Response;
}
void DLMSModemInitialisation::SetResponse(std::string value)
{
    m_Response = value;
}

int DLMSModemInitialisation::GetDelay()
{
    return m_Delay;
}

void DLMSModemInitialisation::SetDelay(int value)
{
    m_Delay = value;
}

std::string DLMSModemInitialisation::ToString()
{
    std::stringstream sb;
    sb << m_Request.c_str();
    sb << " ";
    sb << m_Response.c_str();
    sb << " ";
    sb << m_Delay;
    return sb.str();
}
