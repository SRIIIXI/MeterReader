#include "DLMSSFSKMacCounters.h"

#ifndef DLMS_IGNORE_SFSK_MAC_COUNTERS
//Constructor.
DLMSSFSKMacCounters::DLMSSFSKMacCounters() :
    DLMSSFSKMacCounters("0.0.26.3.0.255", 0)
{
}

//SN Constructor.
DLMSSFSKMacCounters::DLMSSFSKMacCounters(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SFSK_MAC_COUNTERS, ln, sn)
{

}

//LN Constructor.
DLMSSFSKMacCounters::DLMSSFSKMacCounters(std::string ln) :
    DLMSSFSKMacCounters(ln, 0)
{
    m_PhysicalLayerDesynchronization = 0;
    m_TimeOutNotAddressedDesynchronization = 0;
    m_TimeOutFrameNotOkDesynchronization = 0;
    m_WriteRequestDesynchronization = 0;
    m_WrongInitiatorDesynchronization = 0;
    m_RepetitionsCounter = 0;
    m_TransmissionsCounter = 0;
    m_CrcOkFramesCounter = 0;
    m_CrcNOkFramesCounter = 0;
}

std::vector< std::pair<uint16_t, uint32_t> >& DLMSSFSKMacCounters::GetSynchronizationRegister()
{
    return m_SynchronizationRegister;
}

long DLMSSFSKMacCounters::GetPhysicalLayerDesynchronization()
{
    return m_PhysicalLayerDesynchronization;
}

void DLMSSFSKMacCounters::SetPhysicalLayerDesynchronization(uint32_t value)
{
    m_PhysicalLayerDesynchronization = value;
}

long DLMSSFSKMacCounters::GetTimeOutNotAddressedDesynchronization()
{
    return m_TimeOutNotAddressedDesynchronization;
}

void DLMSSFSKMacCounters::SetTimeOutNotAddressedDesynchronization(uint32_t value)
{
    m_TimeOutNotAddressedDesynchronization = value;
}

long DLMSSFSKMacCounters::GetTimeOutFrameNotOkDesynchronization()
{
    return m_TimeOutFrameNotOkDesynchronization;
}

void DLMSSFSKMacCounters::SetTimeOutFrameNotOkDesynchronization(long value)
{
    m_TimeOutFrameNotOkDesynchronization = value;
}

long DLMSSFSKMacCounters::GetWriteRequestDesynchronization()
{
    return m_WriteRequestDesynchronization;
}

void DLMSSFSKMacCounters::SetWriteRequestDesynchronization(uint32_t value)
{
    m_WriteRequestDesynchronization = value;
}

long DLMSSFSKMacCounters::GetWrongInitiatorDesynchronization()
{
    return m_WrongInitiatorDesynchronization;
}

void DLMSSFSKMacCounters::SetWrongInitiatorDesynchronization(uint32_t value)
{
    m_WrongInitiatorDesynchronization = value;
}

std::vector< std::pair<uint16_t, uint32_t> >& DLMSSFSKMacCounters::GetBroadcastFramesCounter()
{
    return m_BroadcastFramesCounter;
}

uint32_t DLMSSFSKMacCounters::GetRepetitionsCounter()
{
    return m_RepetitionsCounter;
}

void DLMSSFSKMacCounters::SetRepetitionsCounter(uint32_t value)
{
    m_RepetitionsCounter = value;
}

uint32_t DLMSSFSKMacCounters::GetTransmissionsCounter()
{
    return m_TransmissionsCounter;
}

void DLMSSFSKMacCounters::SetTransmissionsCounter(uint32_t value)
{
    m_TransmissionsCounter = value;
}

uint32_t DLMSSFSKMacCounters::GetCrcOkFramesCounter()
{
    return m_CrcOkFramesCounter;
}

void DLMSSFSKMacCounters::SetCrcOkFramesCounter(uint32_t value)
{
    m_CrcOkFramesCounter = value;
}

uint32_t DLMSSFSKMacCounters::GetCrcNOkFramesCounter()
{
    return m_CrcNOkFramesCounter;
}

void DLMSSFSKMacCounters::SetCrcNOkFramesCounter(uint32_t value)
{
    m_CrcNOkFramesCounter = value;
}

// Returns amount of attributes.
int DLMSSFSKMacCounters::GetAttributeCount()
{
    return 8;
}

// Returns amount of methods.
int DLMSSFSKMacCounters::GetMethodCount()
{
    return 1;
}

int GetArrayAsString(std::vector< std::pair<uint16_t, uint32_t> >& list, std::string& str)
{
    str.clear();
    std::stringstream sb;
    sb << "{";
    bool first = true;
    for (std::vector< std::pair<uint16_t, uint32_t> >::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            sb << ", ";
        }
        sb << "[";
        sb << Helpers::IntToString(it->first);
        sb << Helpers::IntToString(it->second);
        sb << "]";
    }
    sb << "}";
    str = sb.str();
    return 0;
}

void DLMSSFSKMacCounters::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    GetArrayAsString(m_SynchronizationRegister, ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << "";
    sb << m_PhysicalLayerDesynchronization;
    sb << ", ";
    sb << m_TimeOutNotAddressedDesynchronization;
    sb << ", ";
    sb << m_TimeOutFrameNotOkDesynchronization;
    sb << ", ";
    sb << m_WriteRequestDesynchronization;
    sb << ", ";
    sb << m_WrongInitiatorDesynchronization;
    values.push_back(sb.str());
    GetArrayAsString(m_BroadcastFramesCounter, ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_RepetitionsCounter));
    values.push_back(Helpers::IntToString(m_TransmissionsCounter));
    values.push_back(Helpers::IntToString(m_CrcOkFramesCounter));
    values.push_back(Helpers::IntToString(m_CrcNOkFramesCounter));
}

void DLMSSFSKMacCounters::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //synchronizationRegister
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
    if (all || CanRead(7)) {
        attributes.push_back(7);
    }
    if (all || CanRead(8)) {
        attributes.push_back(8);
    }
}

int DLMSSFSKMacCounters::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    int ret = 0;
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
    case 4:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 3:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        type = DLMS_DATA_TYPE_UINT32;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Returns value of given attribute.
int DLMSSFSKMacCounters::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    ByteBuffer bb;
    switch (e.GetIndex())
    {
    case 1:
    {
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) == 0)
        {
            e.SetValue(tmp);
        }
    }
    break;
    case 2:
    {
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long) m_SynchronizationRegister.size(), bb);
        for (std::vector< std::pair<uint16_t, uint32_t> >::iterator it = m_SynchronizationRegister.begin();
            it != m_SynchronizationRegister.end(); ++it)
        {
            if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
                (ret = bb.SetUInt8(2)) != 0 ||
                (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT16, it->first)) != 0 ||
                (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, it->second)) != 0)
            {
                break;
            }
        }
        e.SetValue(bb);
    }
    break;
    case 3:
    {
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
            (ret = bb.SetUInt8(5)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, m_PhysicalLayerDesynchronization)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, m_TimeOutNotAddressedDesynchronization)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, m_TimeOutFrameNotOkDesynchronization)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, m_WriteRequestDesynchronization)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, m_WrongInitiatorDesynchronization)) == 0)
        {
            e.SetValue(bb);
        }
    }
    break;
    case 4:
    {
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_BroadcastFramesCounter.size(), bb);
        for (std::vector< std::pair<uint16_t, uint32_t> >::iterator it = m_BroadcastFramesCounter.begin();
            it != m_BroadcastFramesCounter.end(); ++it)
        {
            if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
                (ret = bb.SetUInt8(2)) != 0 ||
                (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT16, it->first)) != 0 ||
                (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, it->second)) != 0)
            {
                break;
            }
        }
        e.SetValue(bb);
    }
    break;
    case 5:
        e.SetValue(m_RepetitionsCounter);
        break;
    case 6:
        e.SetValue(m_TransmissionsCounter);
        break;
    case 7:
        e.SetValue(m_CrcOkFramesCounter);
        break;
    case 8:
        e.SetValue(m_CrcNOkFramesCounter);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;

}

// Set value of given attribute.
int DLMSSFSKMacCounters::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
    {
        m_SynchronizationRegister.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                m_SynchronizationRegister.push_back(std::pair<uint16_t, uint32_t>(it->Arr[0].ToInteger(), it->Arr[1].ToInteger()));
            }
        }
    }
    break;
    case 3:
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            m_PhysicalLayerDesynchronization = e.GetValue().Arr[0].ToInteger();
            m_TimeOutNotAddressedDesynchronization = e.GetValue().Arr[1].ToInteger();
            m_TimeOutFrameNotOkDesynchronization = e.GetValue().Arr[2].ToInteger();
            m_WriteRequestDesynchronization = e.GetValue().Arr[3].ToInteger();
            m_WrongInitiatorDesynchronization = e.GetValue().Arr[4].ToInteger();
        }
        else
        {
            m_PhysicalLayerDesynchronization = 0;
            m_TimeOutNotAddressedDesynchronization = 0;
            m_TimeOutFrameNotOkDesynchronization = 0;
            m_WriteRequestDesynchronization = 0;
            m_WrongInitiatorDesynchronization = 0;
        }
    }
    break;
    case 4:
    {
        m_BroadcastFramesCounter.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                m_BroadcastFramesCounter.push_back(std::pair<uint16_t, uint32_t>(it->Arr[0].ToInteger(), it->Arr[1].ToInteger()));
            }
        }
    }
    break;
    case 5:
        m_RepetitionsCounter = e.GetValue().ToInteger();
        break;
    case 6:
        m_TransmissionsCounter = e.GetValue().ToInteger();
        break;
    case 7:
        m_CrcOkFramesCounter = e.GetValue().ToInteger();
        break;
    case 8:
        m_CrcNOkFramesCounter = e.GetValue().ToInteger();
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

#endif //DLMS_IGNORE_SFSK_MAC_COUNTERS
