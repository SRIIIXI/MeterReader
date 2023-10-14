#include "DLMSPrimeNbOfdmPlcPhysicalLayerCounters.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS
//Constructor.
DLMSPrimeNbOfdmPlcPhysicalLayerCounters::DLMSPrimeNbOfdmPlcPhysicalLayerCounters() :
    DLMSPrimeNbOfdmPlcPhysicalLayerCounters("0.0.28.1.0.255", 0)
{
}

DLMSPrimeNbOfdmPlcPhysicalLayerCounters::DLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS, ln, sn)
{
    m_CrcIncorrectCount = m_CrcFailedCount = m_TxDropCount = m_RxDropCount = 0;
}

DLMSPrimeNbOfdmPlcPhysicalLayerCounters::DLMSPrimeNbOfdmPlcPhysicalLayerCounters(std::string ln) :
    DLMSPrimeNbOfdmPlcPhysicalLayerCounters(ln, 0)
{

}

unsigned short DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetCrcIncorrectCount()
{
    return m_CrcIncorrectCount;
}

void DLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetCrcIncorrectCount(unsigned short value)
{
    m_CrcIncorrectCount = value;
}

unsigned short DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetCrcFailedCount()
{
    return m_CrcFailedCount;
}

void DLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetCrcFailedCount(unsigned short value)
{
    m_CrcFailedCount = value;
}

unsigned short DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetTxDropCount()
{
    return m_TxDropCount;
}

void DLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetTxDropCount(unsigned short value)
{
    m_TxDropCount = value;
}

unsigned short DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetRxDropCount()
{
    return m_RxDropCount;
}

void DLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetRxDropCount(unsigned short value)
{
    m_RxDropCount = value;
}

// Returns amount of attributes.
int DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetMethodCount()
{
    return 1;
}

int DLMSPrimeNbOfdmPlcPhysicalLayerCounters::Reset(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int DLMSPrimeNbOfdmPlcPhysicalLayerCounters::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_CrcIncorrectCount = m_CrcFailedCount = m_TxDropCount = m_RxDropCount = 0;
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

void DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_CrcIncorrectCount).ToString());
    values.push_back(DLMSVariant(m_CrcFailedCount).ToString());
    values.push_back(DLMSVariant(m_TxDropCount).ToString());
    values.push_back(DLMSVariant(m_RxDropCount).ToString());
}

void DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // CrcIncorrectCount
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // CrcFailedCount
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // TxDropCount
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // RxDropCount
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
}

int DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_UINT16;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

// Returns value of given attribute.
int DLMSPrimeNbOfdmPlcPhysicalLayerCounters::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(m_CrcIncorrectCount);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_CrcFailedCount);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_TxDropCount);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_RxDropCount);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSPrimeNbOfdmPlcPhysicalLayerCounters::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_CrcIncorrectCount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_CrcFailedCount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_TxDropCount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_RxDropCount = e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

#endif //DLMS_IGNORE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS
