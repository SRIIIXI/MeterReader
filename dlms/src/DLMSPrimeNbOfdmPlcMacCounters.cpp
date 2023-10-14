#include "DLMSPrimeNbOfdmPlcMacCounters.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_COUNTERS
//Constructor.
DLMSPrimeNbOfdmPlcMacCounters::DLMSPrimeNbOfdmPlcMacCounters() :
    DLMSPrimeNbOfdmPlcMacCounters("0.0.28.4.0.255", 0)
{
}

//SN Constructor.
DLMSPrimeNbOfdmPlcMacCounters::DLMSPrimeNbOfdmPlcMacCounters(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS, ln, sn)
{
    m_TxDataPktCount = m_RxDataPktCount = m_TxCtrlPktCount = m_RxCtrlPktCount = m_CsmaFailCount = m_CsmaChBusyCount = 0;
}

//LN Constructor.
DLMSPrimeNbOfdmPlcMacCounters::DLMSPrimeNbOfdmPlcMacCounters(std::string ln) :
    DLMSPrimeNbOfdmPlcMacCounters(ln, 0)
{

}

unsigned int DLMSPrimeNbOfdmPlcMacCounters::GetTxDataPktCount()
{
    return m_TxDataPktCount;
}

void DLMSPrimeNbOfdmPlcMacCounters::SetTxDataPktCount(unsigned int value)
{
    m_TxDataPktCount = value;
}

unsigned int DLMSPrimeNbOfdmPlcMacCounters::GetRxDataPktCount()
{
    return m_RxDataPktCount;
}

void DLMSPrimeNbOfdmPlcMacCounters::SetRxDataPktCount(unsigned int value)
{
    m_RxDataPktCount = value;
}

unsigned int DLMSPrimeNbOfdmPlcMacCounters::GetTxCtrlPktCount()
{
    return m_TxCtrlPktCount;
}

void DLMSPrimeNbOfdmPlcMacCounters::SetTxCtrlPktCount(unsigned int value)
{
    m_TxCtrlPktCount = value;
}

unsigned int DLMSPrimeNbOfdmPlcMacCounters::GetRxCtrlPktCount()
{
    return m_RxCtrlPktCount;
}

void DLMSPrimeNbOfdmPlcMacCounters::SetRxCtrlPktCount(unsigned int value)
{
    m_RxCtrlPktCount = value;
}

unsigned int DLMSPrimeNbOfdmPlcMacCounters::GetCsmaFailCount()
{
    return m_CsmaFailCount;
}

void DLMSPrimeNbOfdmPlcMacCounters::SetCsmaFailCount(unsigned int value)
{
    m_CsmaFailCount = value;
}

unsigned int DLMSPrimeNbOfdmPlcMacCounters::GetCsmaChBusyCount()
{
    return m_CsmaChBusyCount;
}

void DLMSPrimeNbOfdmPlcMacCounters::SetCsmaChBusyCount(unsigned int value)
{
    m_CsmaChBusyCount = value;
}


int DLMSPrimeNbOfdmPlcMacCounters::Reset(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int DLMSPrimeNbOfdmPlcMacCounters::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_TxDataPktCount = m_RxDataPktCount = m_TxCtrlPktCount = m_RxCtrlPktCount = m_CsmaFailCount = m_CsmaChBusyCount = 0;
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns amount of attributes.
int DLMSPrimeNbOfdmPlcMacCounters::GetAttributeCount()
{
    return 7;
}

// Returns amount of methods.
int DLMSPrimeNbOfdmPlcMacCounters::GetMethodCount()
{
    return 1;
}

void DLMSPrimeNbOfdmPlcMacCounters::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_TxDataPktCount).ToString());
    values.push_back(DLMSVariant(m_RxDataPktCount).ToString());
    values.push_back(DLMSVariant(m_TxCtrlPktCount).ToString());
    values.push_back(DLMSVariant(m_RxCtrlPktCount).ToString());
    values.push_back(DLMSVariant(m_CsmaFailCount).ToString());
    values.push_back(DLMSVariant(m_CsmaChBusyCount).ToString());
}

void DLMSPrimeNbOfdmPlcMacCounters::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // TxDataPktCount
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }

    // RxDataPktCount
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // TxCtrlPktCount
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // RxCtrlPktCount
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    // CsmaFailCount
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    // CsmaChBusyCount
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
}

int DLMSPrimeNbOfdmPlcMacCounters::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_UINT32;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int DLMSPrimeNbOfdmPlcMacCounters::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    case 2:
        e.SetValue(m_TxDataPktCount);
        break;
    case 3:
        e.SetValue(m_RxDataPktCount);
        break;
    case 4:
        e.SetValue(m_TxCtrlPktCount);
        break;
    case 5:
        e.SetValue(m_RxCtrlPktCount);
        break;
    case 6:
        e.SetValue(m_CsmaFailCount);
        break;
    case 7:
        e.SetValue(m_CsmaChBusyCount);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSPrimeNbOfdmPlcMacCounters::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_TxDataPktCount = e.GetValue().ToInteger();
        break;
    case 3:
        m_RxDataPktCount = e.GetValue().ToInteger();
        break;
    case 4:
        m_TxCtrlPktCount = e.GetValue().ToInteger();
        break;
    case 5:
        m_RxCtrlPktCount = e.GetValue().ToInteger();
        break;
    case 6:
        m_CsmaFailCount = e.GetValue().ToInteger();
        break;
    case 7:
        m_CsmaChBusyCount = e.GetValue().ToInteger();
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

#endif //DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_COUNTERS
