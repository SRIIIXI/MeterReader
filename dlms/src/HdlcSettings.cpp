#include "HdlcSettings.h"

HdlcSettings::HdlcSettings()
{
    m_MaxInfoTX = DEFAULT_MAX_INFO_TX;
    m_MaxInfoRX = DEFAULT_MAX_INFO_RX;
    m_WindowSizeTX = DEFAULT_WINDOWS_SIZE_TX;
    m_WindowSizeRX = DEFAULT_WINDOWS_SIZE_RX;
}

unsigned short HdlcSettings::GetMaxInfoTX()
{
    return m_MaxInfoTX;
}

void HdlcSettings::SetMaxInfoTX(unsigned short value)
{
    m_MaxInfoTX = value;
}

unsigned short HdlcSettings::GetMaxInfoRX()
{
    return m_MaxInfoRX;
}

void HdlcSettings::SetMaxInfoRX(unsigned short value)
{
    m_MaxInfoRX = value;
}

unsigned char HdlcSettings::GetWindowSizeTX()
{
    return m_WindowSizeRX;
}

void HdlcSettings::SetWindowSizeTX(unsigned char value)
{
    m_WindowSizeRX = value;
}

unsigned char HdlcSettings::GetWindowSizeRX()
{
    return m_WindowSizeTX;
}

void HdlcSettings::SetWindowSizeRX(unsigned char value)
{
    m_WindowSizeTX = value;
}
