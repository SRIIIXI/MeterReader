#include "DLMSPrimeNbOfdmPlcMacSetup.h"

#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_SETUP
//Constructor.
DLMSPrimeNbOfdmPlcMacSetup::DLMSPrimeNbOfdmPlcMacSetup() :
    DLMSPrimeNbOfdmPlcMacSetup("0.0.28.2.0.255", 0)
{
}

//SN Constructor.
DLMSPrimeNbOfdmPlcMacSetup::DLMSPrimeNbOfdmPlcMacSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP, ln, sn)
{
    m_MacMinSwitchSearchTime = m_MacMaxPromotionPdu = m_MacPromotionPduTxPeriod = 0;
    m_MacBeaconsPerFrame = m_MacScpMaxTxAttempts = m_MacCtlReTxTimer = m_MacMaxCtlReTx = 0;
}

//LN Constructor.
DLMSPrimeNbOfdmPlcMacSetup::DLMSPrimeNbOfdmPlcMacSetup(std::string ln) :
    DLMSPrimeNbOfdmPlcMacSetup(ln, 0)
{

}

unsigned char DLMSPrimeNbOfdmPlcMacSetup::GetMacMinSwitchSearchTime()
{
    return m_MacMinSwitchSearchTime;
}

void DLMSPrimeNbOfdmPlcMacSetup::SetMacMinSwitchSearchTime(unsigned char value)
{
    m_MacMinSwitchSearchTime = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacSetup::GetMacMaxPromotionPdu()
{
    return m_MacMaxPromotionPdu;
}

void DLMSPrimeNbOfdmPlcMacSetup::SetMacMaxPromotionPdu(unsigned char value)
{
    m_MacMaxPromotionPdu = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacSetup::GetMacPromotionPduTxPeriod()
{
    return m_MacPromotionPduTxPeriod;
}

void DLMSPrimeNbOfdmPlcMacSetup::SetMacPromotionPduTxPeriod(unsigned char value)
{
    m_MacPromotionPduTxPeriod = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacSetup::GetMacBeaconsPerFrame()
{
    return m_MacBeaconsPerFrame;
}

void DLMSPrimeNbOfdmPlcMacSetup::SetMacBeaconsPerFrame(unsigned char value)
{
    m_MacBeaconsPerFrame = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacSetup::GetMacScpMaxTxAttempts()
{
    return m_MacScpMaxTxAttempts;
}

void DLMSPrimeNbOfdmPlcMacSetup::SetMacScpMaxTxAttempts(unsigned char value)
{
    m_MacScpMaxTxAttempts = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacSetup::GetMacCtlReTxTimer()
{
    return m_MacCtlReTxTimer;
}

void DLMSPrimeNbOfdmPlcMacSetup::SetMacCtlReTxTimer(unsigned char value)
{
    m_MacCtlReTxTimer = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacSetup::GetMacMaxCtlReTx()
{
    return m_MacMaxCtlReTx;
}

void DLMSPrimeNbOfdmPlcMacSetup::SetMacMaxCtlReTx(unsigned char value)
{
    m_MacMaxCtlReTx = value;
}

// Returns amount of attributes.
int DLMSPrimeNbOfdmPlcMacSetup::GetAttributeCount()
{
    return 8;
}

// Returns amount of methods.
int DLMSPrimeNbOfdmPlcMacSetup::GetMethodCount()
{
    return 0;
}

void DLMSPrimeNbOfdmPlcMacSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_MacMinSwitchSearchTime).ToString());
    values.push_back(DLMSVariant(m_MacMaxPromotionPdu).ToString());
    values.push_back(DLMSVariant(m_MacPromotionPduTxPeriod).ToString());
    values.push_back(DLMSVariant(m_MacBeaconsPerFrame).ToString());
    values.push_back(DLMSVariant(m_MacScpMaxTxAttempts).ToString());
    values.push_back(DLMSVariant(m_MacCtlReTxTimer).ToString());
    values.push_back(DLMSVariant(m_MacMaxCtlReTx).ToString());
}

void DLMSPrimeNbOfdmPlcMacSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // MacMinSwitchSearchTime
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // MacMaxPromotionPdu
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // MacPromotionPduTxPeriod
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // MacBeaconsPerFrame
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // MacScpMaxTxAttempts
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
    // MacCtlReTxTimer
    if (all || CanRead(7)) {
        attributes.push_back(7);
    }
    // MacMaxCtlReTx
    if (all || CanRead(8)) {
        attributes.push_back(8);
    }
}

int DLMSPrimeNbOfdmPlcMacSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int DLMSPrimeNbOfdmPlcMacSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 2)
    {
        e.SetValue(m_MacMinSwitchSearchTime);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_MacMaxPromotionPdu);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_MacPromotionPduTxPeriod);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_MacBeaconsPerFrame);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue(m_MacScpMaxTxAttempts);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(m_MacCtlReTxTimer);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(m_MacMaxCtlReTx);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSPrimeNbOfdmPlcMacSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_MacMinSwitchSearchTime = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_MacMaxPromotionPdu = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_MacPromotionPduTxPeriod = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_MacBeaconsPerFrame = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        m_MacScpMaxTxAttempts = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_MacCtlReTxTimer = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_MacMaxCtlReTx = e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

#endif //DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_SETUP
