#ifndef HDLC_SETTINGS_H
#define HDLC_SETTINGS_H

#include "DLMSVariant.h"

class HdlcSettings
{
    unsigned short m_MaxInfoTX, m_MaxInfoRX;
    unsigned char m_WindowSizeRX, m_WindowSizeTX;
public:
    static const unsigned short DEFAULT_MAX_INFO_RX = 128;
    static const unsigned short DEFAULT_MAX_INFO_TX = 128;
    static const unsigned char DEFAULT_WINDOWS_SIZE_RX = 1;
    static const unsigned char DEFAULT_WINDOWS_SIZE_TX = 1;
    //Constructor.
    HdlcSettings();

    // The maximum information field length in transmit.
    // DefaultValue is 128.
    unsigned short GetMaxInfoTX();

    void SetMaxInfoTX(unsigned short value);

    /// The maximum information field length in receive.
    /// DefaultValue is 62.
    unsigned short GetMaxInfoRX();

    void SetMaxInfoRX(unsigned short value);

    /// The window size in transmit.
    /// DefaultValue is 1.
    unsigned char GetWindowSizeTX();

    void SetWindowSizeTX(unsigned char value);

    /// The window size in receive.
    /// DefaultValue is 1.
    unsigned char GetWindowSizeRX();

    void SetWindowSizeRX(unsigned char value);
};
#endif
