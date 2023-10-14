#include "DLMSPrimeNbOfdmPlcMacFunctionalParameters.h"

#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS
//Constructor.
DLMSPrimeNbOfdmPlcMacFunctionalParameters::DLMSPrimeNbOfdmPlcMacFunctionalParameters() :
    DLMSPrimeNbOfdmPlcMacFunctionalParameters("0.0.28.3.0.255", 0)
{
}

//SN Constructor.
DLMSPrimeNbOfdmPlcMacFunctionalParameters::DLMSPrimeNbOfdmPlcMacFunctionalParameters(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS, ln, sn)
{
    m_LnId = 0;
    m_LsId = 0;
    m_SId = 0;
    m_State = DLMS_MAC_STATE_DISCONNECTED;
    m_ScpLength = 0;
    m_NodeHierarchyLevel = 0;
    m_BeaconSlotCount = 0;
    m_BeaconRxSlot = 0;
    m_BeaconTxSlot = 0;
    m_BeaconRxFrequency = 0;
    m_BeaconTxFrequency = 0;
    m_Capabilities = DLMS_MAC_CAPABILITIES_NONE;
}

//LN Constructor.
DLMSPrimeNbOfdmPlcMacFunctionalParameters::DLMSPrimeNbOfdmPlcMacFunctionalParameters(std::string ln) :
    DLMSPrimeNbOfdmPlcMacFunctionalParameters(ln, 0)
{

}

unsigned short DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetLnId()
{
    return m_LnId;
}


void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetLnId(short value)
{
    m_LnId = value;
}


unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetLsId()
{
    return m_LsId;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetLsId(unsigned char value)
{
    m_LsId = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetSId()
{
    return m_SId;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetSId(unsigned char value)
{
    m_SId = value;
}

ByteBuffer& DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetSna()
{
    return m_Sna;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetSna(ByteBuffer& value)
{
    m_Sna = value;
}

DLMS_MAC_STATE DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetState()
{
    return m_State;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetState(DLMS_MAC_STATE value)
{
    m_State = value;
}

int DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetScpLength()
{
    return m_ScpLength;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetScpLength(int value)
{
    m_ScpLength = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetNodeHierarchyLevel()
{
    return m_NodeHierarchyLevel;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetNodeHierarchyLevel(unsigned char value)
{
    m_NodeHierarchyLevel = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetBeaconSlotCount()
{
    return m_BeaconSlotCount;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetBeaconSlotCount(unsigned char value)
{
    m_BeaconSlotCount = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetBeaconRxSlot()
{
    return m_BeaconRxSlot;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetBeaconRxSlot(unsigned char value)
{
    m_BeaconRxSlot = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetBeaconTxSlot()
{
    return m_BeaconTxSlot;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetBeaconTxSlot(unsigned char value)
{
    m_BeaconTxSlot = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetBeaconRxFrequency()
{
    return m_BeaconRxFrequency;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetBeaconRxFrequency(unsigned char value)
{
    m_BeaconRxFrequency = value;
}

unsigned char DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetBeaconTxFrequency()
{
    return m_BeaconTxFrequency;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetBeaconTxFrequency(unsigned char value)
{
    m_BeaconTxFrequency = value;
}

DLMS_MAC_CAPABILITIES DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetCapabilities()
{
    return m_Capabilities;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetCapabilities(DLMS_MAC_CAPABILITIES value)
{
    m_Capabilities = value;
}

// Returns amount of attributes.
int DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetAttributeCount()
{
    return 14;
}

// Returns amount of methods.
int DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetMethodCount()
{
    return 0;
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    values.push_back(DLMSVariant(m_LnId).ToString());
    values.push_back(DLMSVariant(m_LsId).ToString());
    values.push_back(DLMSVariant(m_SId).ToString());
    values.push_back(m_Sna.ToString());
    values.push_back(DLMSVariant(m_State).ToString());
    values.push_back(DLMSVariant(m_ScpLength).ToString());
    values.push_back(DLMSVariant(m_NodeHierarchyLevel).ToString());
    values.push_back(DLMSVariant(m_BeaconSlotCount).ToString());
    values.push_back(DLMSVariant(m_BeaconRxSlot).ToString());
    values.push_back(DLMSVariant(m_BeaconTxSlot).ToString());
    values.push_back(DLMSVariant(m_BeaconRxFrequency).ToString());
    values.push_back(DLMSVariant(m_BeaconTxFrequency).ToString());
    values.push_back(DLMSVariant(m_Capabilities).ToString());
}

void DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // LnId
    if (all || CanRead(2)) {
        attributes.push_back(2);
    }
    // LsId
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // SId
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // SNa
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // State
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
    // ScpLength
    if (all || CanRead(7)) {
        attributes.push_back(7);
    }
    // NodeHierarchyLevel
    if (all || CanRead(8)) {
        attributes.push_back(8);
    }
    // BeaconSlotCount
    if (all || CanRead(9)) {
        attributes.push_back(9);
    }
    // BeaconRxSlot
    if (all || CanRead(10)) {
        attributes.push_back(10);
    }
    // BeaconTxSlot
    if (all || CanRead(11)) {
        attributes.push_back(11);
    }
    // BeaconRxFrequency
    if (all || CanRead(12)) {
        attributes.push_back(12);
    }
    // BeaconTxFrequency
    if (all || CanRead(13)) {
        attributes.push_back(13);
    }
    // Capabilities
    if (all || CanRead(14)) {
        attributes.push_back(14);
    }
}

int DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;

    switch (index) {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_INT16;
        break;
    case 3:
    case 4:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 5:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 6:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 7:
        type = DLMS_DATA_TYPE_INT16;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 14:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Returns value of given attribute.
int DLMSPrimeNbOfdmPlcMacFunctionalParameters::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(m_LnId);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue(m_LsId);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetValue(m_SId);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetValue(m_Sna);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue(m_State);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue(m_ScpLength);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(m_NodeHierarchyLevel);
    }
    else if (e.GetIndex() == 9)
    {
        e.SetValue(m_BeaconSlotCount);
    }
    else if (e.GetIndex() == 10)
    {
        e.SetValue(m_BeaconRxSlot);
    }
    else if (e.GetIndex() == 11)
    {
        e.SetValue(m_BeaconTxSlot);
    }
    else if (e.GetIndex() == 12)
    {
        e.SetValue(m_BeaconRxFrequency);
    }
    else if (e.GetIndex() == 13)
    {
        e.SetValue(m_BeaconTxFrequency);
    }
    else if (e.GetIndex() == 14)
    {
        e.SetValue(m_Capabilities);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSPrimeNbOfdmPlcMacFunctionalParameters::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    DLMS_ERROR_CODE ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_LnId = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_LsId = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_SId = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 5)
    {
        m_Sna.Clear();
        m_Sna.Set(e.GetValue().byteArr, e.GetValue().GetSize());
    }
    else if (e.GetIndex() == 6)
    {
        m_State = (DLMS_MAC_STATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_ScpLength = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 8)
    {
        m_NodeHierarchyLevel = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 9)
    {
        m_BeaconSlotCount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 10)
    {
        m_BeaconRxSlot = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 11)
    {
        m_BeaconTxSlot = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 12)
    {
        m_BeaconRxFrequency = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 13)
    {
        m_BeaconTxFrequency = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 14)
    {
        m_Capabilities = (DLMS_MAC_CAPABILITIES)e.GetValue().ToInteger();
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

#endif //DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS
