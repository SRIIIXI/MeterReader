#include "DLMSSFSKMacSynchronizationTimeouts.h"

#ifndef DLMS_IGNORE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS
//Constructor.
DLMSSFSKMacSynchronizationTimeouts::DLMSSFSKMacSynchronizationTimeouts() :
    DLMSSFSKMacSynchronizationTimeouts("0.0.26.2.0.255", 0)
{
}

//SN Constructor.
DLMSSFSKMacSynchronizationTimeouts::DLMSSFSKMacSynchronizationTimeouts(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS, ln, sn)
{

}

//LN Constructor.
DLMSSFSKMacSynchronizationTimeouts::DLMSSFSKMacSynchronizationTimeouts(std::string ln) :
    DLMSSFSKMacSynchronizationTimeouts(ln, 0)
{
    m_SearchInitiatorTimeout = 0;
    m_SynchronizationConfirmationTimeout = 0;
    m_TimeOutNotAddressed = 0;
    m_TimeOutFrameNotOK = 0;
}

int DLMSSFSKMacSynchronizationTimeouts::GetSearchInitiatorTimeout()
{
    return m_SearchInitiatorTimeout;
}

void DLMSSFSKMacSynchronizationTimeouts::SetSearchInitiatorTimeout(uint32_t value)
{
    m_SearchInitiatorTimeout = value;
}

uint32_t DLMSSFSKMacSynchronizationTimeouts::GetSynchronizationConfirmationTimeout()
{
    return m_SynchronizationConfirmationTimeout;
}

void DLMSSFSKMacSynchronizationTimeouts::SetSynchronizationConfirmationTimeout(uint32_t value)
{
    m_SynchronizationConfirmationTimeout = value;
}

uint32_t DLMSSFSKMacSynchronizationTimeouts::GetTimeOutNotAddressed()
{
    return m_TimeOutNotAddressed;
}

void DLMSSFSKMacSynchronizationTimeouts::SetTimeOutNotAddressed(uint32_t value)
{
    m_TimeOutNotAddressed = value;
}

uint32_t DLMSSFSKMacSynchronizationTimeouts::GetTimeOutFrameNotOK()
{
    return m_TimeOutFrameNotOK;
}

void DLMSSFSKMacSynchronizationTimeouts::SetTimeOutFrameNotOK(uint32_t value)
{
    m_TimeOutFrameNotOK = value;
}


// Returns amount of attributes.
int DLMSSFSKMacSynchronizationTimeouts::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int DLMSSFSKMacSynchronizationTimeouts::GetMethodCount()
{
    return 0;
}

void DLMSSFSKMacSynchronizationTimeouts::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_SearchInitiatorTimeout));
    values.push_back(Helpers::IntToString(m_SynchronizationConfirmationTimeout));
    values.push_back(Helpers::IntToString(m_TimeOutNotAddressed));
    values.push_back(Helpers::IntToString(m_TimeOutFrameNotOK));
}

void DLMSSFSKMacSynchronizationTimeouts::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //SearchInitiatorTimeout
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // SynchronizationConfirmationTimeout
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // TimeOutNotAddressed
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // TimeOutFrameNotOK
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
}

int DLMSSFSKMacSynchronizationTimeouts::GetDataType(int index, DLMS_DATA_TYPE& type)
{
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
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSSFSKMacSynchronizationTimeouts::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
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
        e.SetValue(m_SearchInitiatorTimeout);
        break;
    case 3:
        e.SetValue(m_SynchronizationConfirmationTimeout);
        break;
    case 4:
        e.SetValue(m_TimeOutNotAddressed);
        break;
    case 5:
        e.SetValue(m_TimeOutFrameNotOK);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSSFSKMacSynchronizationTimeouts::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_SearchInitiatorTimeout = e.GetValue().ToInteger();
        break;
    case 3:
        m_SynchronizationConfirmationTimeout = e.GetValue().ToInteger();
        break;
    case 4:
        m_TimeOutNotAddressed = e.GetValue().ToInteger();
        break;
    case 5:
        m_TimeOutFrameNotOK = e.GetValue().ToInteger();
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
#endif //DLMS_IGNORE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS
