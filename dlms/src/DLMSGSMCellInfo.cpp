#include "DLMSGSMCellInfo.h"
#include <sstream>

DLMSGSMCellInfo::DLMSGSMCellInfo()
{
    m_CellId = 0;
    m_LocationId = 0;
    m_SignalQuality = 0;
    m_Ber = 0;
    m_MobileCountryCode = 0;
    m_MobileNetworkCode = 0;
    m_ChannelNumber = 0;
}

unsigned int DLMSGSMCellInfo::GetCellId()
{
    return m_CellId;
}

void DLMSGSMCellInfo::SetCellId(unsigned int value)
{
    m_CellId = value;
}

unsigned short DLMSGSMCellInfo::GetLocationId()
{
    return m_LocationId;
}

void DLMSGSMCellInfo::SetLocationId(unsigned short value)
{
    m_LocationId = value;
}

unsigned char DLMSGSMCellInfo::GetSignalQuality()
{
    return m_SignalQuality;
}

void DLMSGSMCellInfo::SetSignalQuality(unsigned char value)
{
    m_SignalQuality = value;
}

unsigned char DLMSGSMCellInfo::GetBer()
{
    return m_Ber;
}

void DLMSGSMCellInfo::SetBer(unsigned char value)
{
    m_Ber = value;
}

unsigned short DLMSGSMCellInfo::GetMobileCountryCode()
{
    return m_MobileCountryCode;
}

void DLMSGSMCellInfo::SetMobileCountryCode(unsigned short value)
{
    m_MobileCountryCode = value;
}

unsigned short DLMSGSMCellInfo::GetMobileNetworkCode() 
{
    return m_MobileNetworkCode;
}

void DLMSGSMCellInfo::SetMobileNetworkCode(unsigned short value)
{
    m_MobileNetworkCode = value;
}

unsigned long DLMSGSMCellInfo::GetChannelNumber()
{
    return m_ChannelNumber;
}

void DLMSGSMCellInfo::SetChannelNumber(unsigned long value)
{
    m_ChannelNumber = value;
}

std::string DLMSGSMCellInfo::ToString()
{
    std::stringstream sb;
    sb << m_CellId;
    sb << ", ";
    sb << m_LocationId;
    sb << ", ";
    sb << (int)m_SignalQuality;
    sb << ", ";
    sb << (int)m_Ber;
    return sb.str();
}
