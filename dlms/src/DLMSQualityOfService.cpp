#include <sstream>
#include "DLMSQualityOfService.h"

int DLMSQualityOfService::GetPrecedence()
{
    return m_Precedence;
}
void DLMSQualityOfService::SetPrecedence(int value)
{
    m_Precedence = value;
}

int DLMSQualityOfService::GetDelay()
{
    return m_Delay;
}
void DLMSQualityOfService::SetDelay(int value)
{
    m_Delay = value;
}

int DLMSQualityOfService::GetReliability()
{
    return m_Reliability;
}
void DLMSQualityOfService::SetReliability(int value)
{
    m_Reliability = value;
}

int DLMSQualityOfService::GetPeakThroughput()
{
    return m_PeakThroughput;
}
void DLMSQualityOfService::SetPeakThroughput(int value)
{
    m_PeakThroughput = value;
}

int DLMSQualityOfService::GetMeanThroughput()
{
    return m_MeanThroughput;
}
void DLMSQualityOfService::SetMeanThroughput(int value)
{
    m_MeanThroughput = value;
}


std::string DLMSQualityOfService::ToString()
{
    std::stringstream sb;
    sb << m_Precedence;
    sb << " ";
    sb << m_Delay;
    sb << " ";
    sb << m_Reliability;
    sb << " ";
    sb << m_PeakThroughput;
    sb << " ";
    sb << m_MeanThroughput;
    return sb.str();
}
