#include "DLMSSFSKPhyMacSetUp.h"

#ifndef DLMS_IGNORE_SFSK_PHY_MAC_SETUP
//Constructor.
DLMSSFSKPhyMacSetUp::DLMSSFSKPhyMacSetUp() :
    DLMSSFSKPhyMacSetUp("0.0.26.0.0.255", 0)
{
}

//SN Constructor.
DLMSSFSKPhyMacSetUp::DLMSSFSKPhyMacSetUp(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SFSK_PHY_MAC_SETUP, ln, sn)
{

}

//LN Constructor.
DLMSSFSKPhyMacSetUp::DLMSSFSKPhyMacSetUp(std::string ln) :
    DLMSSFSKPhyMacSetUp(ln, 0)
{

}

// Returns amount of attributes.
int DLMSSFSKPhyMacSetUp::GetAttributeCount()
{
    return 15;
}

// Returns amount of methods.
int DLMSSFSKPhyMacSetUp::GetMethodCount()
{
    return 0;
}

DLMS_INITIATOR_ELECTRICAL_PHASE DLMSSFSKPhyMacSetUp::GetInitiatorElectricalPhase()
{
    return m_InitiatorElectricalPhase;
}

void DLMSSFSKPhyMacSetUp::SetInitiatorElectricalPhase(DLMS_INITIATOR_ELECTRICAL_PHASE value)
{
    m_InitiatorElectricalPhase = value;
}

DLMS_DELTA_ELECTRICAL_PHASE DLMSSFSKPhyMacSetUp::GetDeltaElectricalPhase()
{
    return m_DeltaElectricalPhase;
}

void DLMSSFSKPhyMacSetUp::SetDeltaElectricalPhase(DLMS_DELTA_ELECTRICAL_PHASE value)
{
    m_DeltaElectricalPhase = value;
}

unsigned char DLMSSFSKPhyMacSetUp::GetMaxReceivingGain()
{
    return m_MaxReceivingGain;
}

void DLMSSFSKPhyMacSetUp::SetMaxReceivingGain(unsigned char value)
{
    m_MaxReceivingGain = value;
}

unsigned char DLMSSFSKPhyMacSetUp::GetMaxTransmittingGain()
{
    return m_MaxTransmittingGain;
}

void DLMSSFSKPhyMacSetUp::SetMaxTransmittingGain(unsigned char value)
{
    m_MaxTransmittingGain = value;
}

unsigned char DLMSSFSKPhyMacSetUp::GetSearchInitiatorThreshold()
{
    return m_SearchInitiatorThreshold;
}

void DLMSSFSKPhyMacSetUp::SetSearchInitiatorThreshold(unsigned char value)
{
    m_SearchInitiatorThreshold = value;
}

uint32_t DLMSSFSKPhyMacSetUp::GetMarkFrequency()
{
    return m_MarkFrequency;
}

void DLMSSFSKPhyMacSetUp::SetMarkFrequency(uint32_t value)
{
    m_MarkFrequency = value;
}

uint32_t DLMSSFSKPhyMacSetUp::GetSpaceFrequency()
{
    return m_SpaceFrequency;
}

void DLMSSFSKPhyMacSetUp::SetSpaceFrequency(uint32_t value)
{
    m_SpaceFrequency = value;
}

uint16_t DLMSSFSKPhyMacSetUp::GetMacAddress()
{
    return m_MacAddress;
}

void DLMSSFSKPhyMacSetUp::SetMacAddress(uint16_t value)
{
    m_MacAddress = value;
}

std::vector<uint16_t>& DLMSSFSKPhyMacSetUp::GetMacGroupAddresses()
{
    return m_MacGroupAddresses;
}

DLMS_REPEATER DLMSSFSKPhyMacSetUp::GetRepeater()
{
    return m_Repeater;
}

void DLMSSFSKPhyMacSetUp::SetRepeater(DLMS_REPEATER value)
{
    m_Repeater = value;
}

bool DLMSSFSKPhyMacSetUp::GetRepeaterStatus()
{
    return m_RepeaterStatus;
}

void DLMSSFSKPhyMacSetUp::SetRepeaterStatus(bool value)
{
    m_RepeaterStatus = value;
}

unsigned char DLMSSFSKPhyMacSetUp::GetMinDeltaCredit()
{
    return m_MinDeltaCredit;
}

void DLMSSFSKPhyMacSetUp::SetMinDeltaCredit(unsigned char value)
{
    m_MinDeltaCredit = value;
}

uint16_t DLMSSFSKPhyMacSetUp::GetInitiatorMacAddress()
{
    return m_InitiatorMacAddress;
}

void DLMSSFSKPhyMacSetUp::SetInitiatorMacAddress(uint16_t value)
{
    m_InitiatorMacAddress = value;
}

bool DLMSSFSKPhyMacSetUp::GetSynchronizationLocked()
{
    return m_SynchronizationLocked;
}

void DLMSSFSKPhyMacSetUp::SetSynchronizationLocked(bool value)
{
    m_SynchronizationLocked = value;
}

DLMS_BAUD_RATE DLMSSFSKPhyMacSetUp::GetTransmissionSpeed()
{
    return m_TransmissionSpeed;
}

void DLMSSFSKPhyMacSetUp::SetTransmissionSpeed(DLMS_BAUD_RATE value)
{
    m_TransmissionSpeed = value;
}


int GetMacGroupAddressesAsString(std::vector<uint16_t>& list, std::string& str)
{
    str.clear();
    std::stringstream sb;
    sb << "{";
    bool first = true;
    for (std::vector<uint16_t>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            sb << ", ";
        }
        sb << Helpers::IntToString(*it);
    }
    sb << "}";
    str = sb.str();
    return 0;
}


void DLMSSFSKPhyMacSetUp::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_InitiatorElectricalPhase));
    values.push_back(Helpers::IntToString(m_DeltaElectricalPhase));
    values.push_back(Helpers::IntToString(m_MaxReceivingGain));
    values.push_back(Helpers::IntToString(m_MaxTransmittingGain));
    values.push_back(Helpers::IntToString(m_SearchInitiatorThreshold));
    ln = "[";
    ln.append(Helpers::IntToString(m_MarkFrequency));
    ln.append(Helpers::IntToString(m_SpaceFrequency));
    ln = "]";
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_MacAddress));
    GetMacGroupAddressesAsString(m_MacGroupAddresses, ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_Repeater));
    values.push_back(Helpers::IntToString(m_RepeaterStatus));
    values.push_back(Helpers::IntToString(m_MinDeltaCredit));
    values.push_back(Helpers::IntToString(m_InitiatorMacAddress));
    values.push_back(Helpers::IntToString(m_SynchronizationLocked));
    values.push_back(Helpers::IntToString(m_TransmissionSpeed));
}

void DLMSSFSKPhyMacSetUp::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // InitiatorElectricalPhase
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // DeltaElectricalPhase
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // MaxReceivingGain,
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // MaxTransmittingGain
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    // SearchInitiatorThreshold
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    // MarkFrequency, SpaceFrequency
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    // Macpush_backress
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    // MacGrouppush_backresses
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    // Repeater
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
    // RepeaterStatus
    attributes.push_back(11);

    // MinDeltaCredit
    attributes.push_back(12);

    // InitiatorMacpush_backress,
    attributes.push_back(13);

    // SynchronizationLocked
    attributes.push_back(14);

    // TransmissionSpeed
    if (all || CanRead(15))
    {
        attributes.push_back(15);
    }
}

int DLMSSFSKPhyMacSetUp::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    int ret = DLMS_ERROR_CODE_OK;
    // LogicalName
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 4:
    case 5:
    case 6:
    case 12:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 7:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 8:
    case 13:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    case 9:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 2:
    case 3:
    case 10:
    case 15:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 11:
    case 14:
        type = DLMS_DATA_TYPE_BOOLEAN;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Returns value of given attribute.
int DLMSSFSKPhyMacSetUp::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(m_InitiatorElectricalPhase);
        break;
    case 3:
        e.SetValue(m_DeltaElectricalPhase);
        break;
    case 4:
        e.SetValue(m_MaxReceivingGain);
        break;
    case 5:
        e.SetValue(m_MaxTransmittingGain);
        break;
    case 6:
        e.SetValue(m_SearchInitiatorThreshold);
        break;
    case 7:
    {
        ByteBuffer bb;
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
            (ret = bb.SetUInt8(2)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, m_MarkFrequency)) == 0 &&
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT32, m_SpaceFrequency)) == 0)
        {
            e.SetValue(bb);
        }
        break;
    }
    case 8:
        e.SetValue(m_MacAddress);
        break;
    case 9:
    {
        ByteBuffer bb;
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_MacGroupAddresses.size(), bb);
        for (std::vector<uint16_t>::iterator it = m_MacGroupAddresses.begin(); it != m_MacGroupAddresses.end(); ++it)
        {
            if ((ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_UINT16, *it)) != 0)
            {
                break;
            }
        }
        e.SetValue(bb);
    }
    break;
    case 10:
        e.SetValue(m_Repeater);
        break;
    case 11:
        e.SetValue(m_RepeaterStatus);
        break;
    case 12:
        e.SetValue(m_MinDeltaCredit);
        break;
    case 13:
        e.SetValue(m_InitiatorMacAddress);
        break;
    case 14:
        e.SetValue(m_SynchronizationLocked);
        break;
    case 15:
        e.SetValue(m_TransmissionSpeed);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;

}

// Set value of given attribute.
int DLMSSFSKPhyMacSetUp::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_InitiatorElectricalPhase = (DLMS_INITIATOR_ELECTRICAL_PHASE)e.GetValue().ToInteger();
        break;
    case 3:
        m_DeltaElectricalPhase = (DLMS_DELTA_ELECTRICAL_PHASE)e.GetValue().ToInteger();
        break;
    case 4:
        m_MaxReceivingGain = e.GetValue().ToInteger();
        break;
    case 5:
        m_MaxTransmittingGain = e.GetValue().ToInteger();
        break;
    case 6:
        m_SearchInitiatorThreshold = e.GetValue().ToInteger();
        break;
    case 7: {
        if (e.GetValue().Arr[0].vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            m_MarkFrequency = e.GetValue().Arr[0].ToInteger();
            m_SpaceFrequency = e.GetValue().Arr[0].ToInteger();
        }
        else
        {
            m_MarkFrequency = 0;
            m_SpaceFrequency = 0;
        }
        break;
    }
    case 8:
        m_MacAddress = e.GetValue().ToInteger();
        break;
    case 9: {
        m_MacGroupAddresses.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                m_MacGroupAddresses.push_back(it->ToInteger());
            }
        }
        break;
    }
    case 10:
        m_Repeater = (DLMS_REPEATER)e.GetValue().ToInteger();
        break;
    case 11:
        m_RepeaterStatus = e.GetValue().ToInteger();
        break;
    case 12:
        m_MinDeltaCredit = e.GetValue().ToInteger();
        break;
    case 13:
        m_InitiatorMacAddress = e.GetValue().ToInteger();
        break;
    case 14:
        m_SynchronizationLocked = e.GetValue().ToInteger();
        break;
    case 15:
        m_TransmissionSpeed = (DLMS_BAUD_RATE) e.GetValue().ToInteger();
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}
#endif //DLMS_IGNORE_SFSK_PHY_MAC_SETUP
