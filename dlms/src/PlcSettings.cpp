#include "PlcSettings.h"
#include "DLMSSettings.h"
#include "DLMS.h"

unsigned char PlcSettings::GetInitialCredit()
{
    return m_InitialCredit;
}
void PlcSettings::SetInitialCredit(unsigned char value)
{
    m_InitialCredit = value;
}


unsigned char PlcSettings::GetCurrentCredit()
{
    return m_CurrentCredit;
}
void PlcSettings::SetCurrentCredit(unsigned char value)
{
    m_CurrentCredit = value;
}

unsigned char PlcSettings::GetDeltaCredit()
{
    return m_DeltaCredit;
}
void PlcSettings::SetDeltaCredit(unsigned char value)
{
    m_DeltaCredit = value;
}

ByteBuffer& PlcSettings::GetSystemTitle()
{
    if (m_Settings != NULL && m_Settings->GetInterfaceType() != DLMS_INTERFACE_TYPE_PLC && m_Settings->GetCipher() != NULL)
    {
        return m_Settings->GetCipher()->GetSystemTitle();
    }
    return m_SystemTitle;
}

void PlcSettings::SetSystemTitle(ByteBuffer& value)
{
    if (m_Settings != NULL && m_Settings->GetInterfaceType() != DLMS_INTERFACE_TYPE_PLC && m_Settings->GetCipher() != NULL)
    {
        m_Settings->GetCipher()->SetSystemTitle(value);
    }
    m_SystemTitle = value;
}

uint16_t PlcSettings::GetMacSourceAddress()
{
    return m_MacSourceAddress;
}

void PlcSettings::SetMacSourceAddress(uint16_t value)
{
    m_MacSourceAddress = value;
}

uint16_t PlcSettings::GetMacDestinationAddress()
{
    return m_MacDestinationAddress;
}

void PlcSettings::SetMacDestinationAddress(uint16_t value)
{
    m_MacDestinationAddress = value;
}

unsigned char PlcSettings::GetResponseProbability()
{
    return m_ResponseProbability;
}

void PlcSettings::SetResponseProbability(unsigned char value)
{
    m_ResponseProbability = value;
}

uint16_t PlcSettings::GetAllowedTimeSlots()
{
    return m_AllowedTimeSlots;
}

void PlcSettings::SetAllowedTimeSlots(uint16_t value)
{
    m_AllowedTimeSlots = value;
}

ByteBuffer& PlcSettings::GetClientSystemTitle()
{
    return m_ClientSystemTitle;
}
void PlcSettings::SetClientSystemTitle(ByteBuffer& value)
{
    m_ClientSystemTitle = value;
}

void PlcSettings::Reset()
{
    m_InitialCredit = 7;
    m_CurrentCredit = 7;
    m_DeltaCredit = 0;
    // New device addresses are used.
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC)
    {
        if (m_Settings->IsServer())
        {
            m_MacSourceAddress = DLMS_PLC_SOURCE_ADDRESS_NEW;
            m_MacDestinationAddress = DLMS_PLC_SOURCE_ADDRESS_INITIATOR;
        }
        else
        {
            m_MacSourceAddress = DLMS_PLC_SOURCE_ADDRESS_INITIATOR;
            m_MacDestinationAddress = DLMS_PLC_DESTINATION_ADDRESS_ALL_PHYSICAL;
        }
    }
    else
    {
        if (m_Settings->IsServer())
        {
            m_MacSourceAddress = DLMS_PLC_SOURCE_ADDRESS_NEW;
            m_MacDestinationAddress = DLMS_PLC_HDLC_SOURCE_ADDRESS_INITIATOR;
        }
        else
        {
            m_MacSourceAddress = DLMS_PLC_HDLC_SOURCE_ADDRESS_INITIATOR;
            m_MacDestinationAddress = DLMS_PLC_DESTINATION_ADDRESS_ALL_PHYSICAL;
        }
    }
    m_ResponseProbability = 100;
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC)
    {
        m_AllowedTimeSlots = 10;
    }
    else
    {
        m_AllowedTimeSlots = 0x14;
    }
}

PlcSettings::PlcSettings(DLMSSettings* settings)
{
    m_Settings = settings;
    Reset();
}

int PlcSettings::DiscoverRequest(ByteBuffer& data)
{
    if (m_Settings->GetInterfaceType() != DLMS_INTERFACE_TYPE_PLC && m_Settings->GetInterfaceType() != DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    int ret;
    ByteBuffer bb;
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        bb.Set(LLC_SEND_BYTES, 3);
    }
    bb.SetUInt8(DLMS_COMMAND_DISCOVER_REQUEST);
    bb.SetUInt8(GetResponseProbability());
    bb.SetUInt16(GetAllowedTimeSlots());
    // DiscoverReport initial credit
    bb.SetUInt8(0);
    // IC Equal credit
    bb.SetUInt8(0);
    int val = 0;
    uint32_t clientAddress = m_Settings->GetClientAddress();
    uint32_t serverAddress = m_Settings->GetServerAddress();
    uint16_t da = m_Settings->GetPlcSettings().GetMacDestinationAddress();
    uint16_t sa = m_Settings->GetPlcSettings().GetMacSourceAddress();
    // 10.4.6.4 Source and destination APs and addresses of CI-PDUs
    // Client address is No-station in discoverReport.
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        m_Settings->GetPlcSettings().SetInitialCredit(0);
        m_Settings->GetPlcSettings().SetCurrentCredit(0);
        m_Settings->GetPlcSettings().SetMacSourceAddress(0xC01);
        m_Settings->GetPlcSettings().SetMacDestinationAddress(0xFFF);
        m_Settings->SetClientAddress(0x66);
        // All-station
        m_Settings->SetServerAddress(0x33FF);
    }
    else
    {
        val = m_Settings->GetPlcSettings().GetInitialCredit() << 5;
        val |= m_Settings->GetPlcSettings().GetCurrentCredit() << 2;
        val |= m_Settings->GetPlcSettings().GetDeltaCredit() & 0x3;
        m_Settings->GetPlcSettings().SetMacSourceAddress(0xC00);
        m_Settings->SetClientAddress(1);
        m_Settings->SetServerAddress(0);
    }
    ret = DLMS::GetMacFrame(*m_Settings, 0x13, val, &bb, data);
    //Return original values.
    m_Settings->SetClientAddress(clientAddress);
    m_Settings->SetServerAddress(serverAddress);
    m_Settings->GetPlcSettings().SetMacDestinationAddress(da);
    m_Settings->GetPlcSettings().SetMacSourceAddress(sa);
    return ret;
}

int PlcSettings::DiscoverReport(ByteBuffer& systemTitle, bool newMeter, ByteBuffer& data)
{
    if (m_Settings->GetInterfaceType() != DLMS_INTERFACE_TYPE_PLC && m_Settings->GetInterfaceType() != DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    ByteBuffer bb;
    int ret;
    unsigned char alarmDescription;
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC)
    {
        alarmDescription = (newMeter ? 1 : 0x82);
    }
    else
    {
        alarmDescription = 0;
    }
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        bb.Set(LLC_REPLY_BYTES, sizeof(LLC_REPLY_BYTES));
    }
    bb.SetUInt8(DLMS_COMMAND_DISCOVER_REPORT);
    bb.SetUInt8(1);
    bb.Set(&systemTitle);
    if (alarmDescription != 0)
    {
        bb.SetUInt8(1);
    }
    bb.SetUInt8(alarmDescription);
    int clientAddress = m_Settings->GetClientAddress();
    int serverAddress = m_Settings->GetServerAddress();
    int macSourceAddress = m_Settings->GetPlcSettings().GetMacSourceAddress();
    int macTargetAddress = m_Settings->GetPlcSettings().GetMacDestinationAddress();
    // 10.4.6.4 Source and destination APs and addresses of CI-PDUs
    // Client address is No-station in discoverReport.
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        m_Settings->GetPlcSettings().SetMacDestinationAddress(DLMS_PLC_HDLC_SOURCE_ADDRESS_INITIATOR);
    }
    else
    {
        m_Settings->SetClientAddress(0);
        m_Settings->SetServerAddress(0xFD);
    }
    ret = DLMS::GetMacFrame(*m_Settings, 0x13, 0, &bb, data);
    //Restore original values.
    m_Settings->SetClientAddress(clientAddress);
    m_Settings->SetServerAddress(serverAddress);
    m_Settings->GetPlcSettings().SetMacSourceAddress(macSourceAddress);
    m_Settings->GetPlcSettings().SetMacDestinationAddress(macTargetAddress);
    return ret;
}

int PlcSettings::ParseDiscover(ByteBuffer& value, uint16_t sa, uint16_t da, std::vector<DLMSPlcMeterInfo>& list)
{
    int ret;
    unsigned char ch, count;
    if ((ret = value.GetUInt8(&count)) == 0)
    {
        // Get System title.
        ByteBuffer st;
        if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
        {
            st.SetSize(8);
        }
        else
        {
            st.SetSize(6);
        }
        for (int pos = 0; pos != count; ++pos)
        {
            DLMSPlcMeterInfo info;
            info.SetSourceAddress(sa);
            info.SetDestinationAddress(da);
            // Get System title.
            value.Get(st.GetData(), st.GetSize());
            info.SetSystemTitle(st);
            // Alarm descriptor of the reporting system.
            // Alarm-Descriptor presence flag
            if ((ret = value.GetUInt8(&ch)) != 0)
            {
                break;
            }
            if (ch != 0)
            {
                if ((ret = value.GetUInt8(&ch)) != 0)
                {
                    break;
                }
                // Alarm-Descriptor
                info.SetAlarmDescriptor(ch);
            }
            list.push_back(info);
        }
    }
    return ret;
}

int PlcSettings::RegisterRequest(ByteBuffer& initiatorSystemTitle, ByteBuffer& systemTitle, ByteBuffer& data)
{
    ByteBuffer bb;
    bb.SetUInt8(DLMS_COMMAND_REGISTER_REQUEST);
    bb.Set(initiatorSystemTitle.GetData(), initiatorSystemTitle.GetSize());
    // LEN
    bb.SetUInt8(0x1);
    bb.Set(systemTitle.GetData(), systemTitle.GetSize());
    // MAC address.
    bb.SetUInt16(GetMacSourceAddress());
    int ret;
    int val = m_Settings->GetPlcSettings().GetInitialCredit() << 5;
    val |= m_Settings->GetPlcSettings().GetCurrentCredit() << 2;
    val |= m_Settings->GetPlcSettings().GetDeltaCredit() & 0x3;
    int clientAddress = m_Settings->GetClientAddress();
    int serverAddress = m_Settings->GetServerAddress();
    int macSourceAddress = m_Settings->GetPlcSettings().GetMacSourceAddress();
    int macTargetAddress = m_Settings->GetPlcSettings().GetMacDestinationAddress();
    // 10.4.6.4 Source and destination APs and addresses of CI-PDUs
    // Client address is No-station in discoverReport.
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC) {
        m_Settings->GetPlcSettings().SetInitialCredit(0);
        m_Settings->GetPlcSettings().SetCurrentCredit(0);
        m_Settings->GetPlcSettings().SetMacSourceAddress(0xC01);
        m_Settings->GetPlcSettings().SetMacDestinationAddress(0xFFF);
        m_Settings->SetClientAddress(0x66);
        // All-station
        m_Settings->SetServerAddress(0x33FF);
    }
    else
    {
        m_Settings->SetClientAddress(1);
        m_Settings->SetServerAddress(0);
        m_Settings->GetPlcSettings().SetMacSourceAddress(0xC00);
        m_Settings->GetPlcSettings().SetMacDestinationAddress(0xFFF);
    }
    ret = DLMS::GetMacFrame(*m_Settings, 0x13, val, &bb, data);
    m_Settings->SetClientAddress(clientAddress);
    m_Settings->SetServerAddress(serverAddress);
    m_Settings->GetPlcSettings().SetMacSourceAddress(macSourceAddress);
    m_Settings->GetPlcSettings().SetMacDestinationAddress(macTargetAddress);
    return ret;
}

int PlcSettings::ParseRegisterRequest(ByteBuffer& value)
{
    // Get System title.
    ByteBuffer st;
    if (m_Settings->GetInterfaceType() == DLMS_INTERFACE_TYPE_PLC_HDLC)
    {
        st.SetSize(8);
    }
    else
    {
        st.SetSize(6);
    }
    value.Get(st.GetData(), st.GetSize());
    int ret;
    unsigned char pos, count;
    uint16_t v;
    if ((ret = value.GetUInt8(&count)) != 0)
    {
        return ret;
    }
    for (pos = 0; pos != count; ++pos)
    {
        value.Get(st.GetData(), st.GetSize());
        SetSystemTitle(st);
        // MAC address.
        if ((ret = value.GetUInt16(&v)) != 0)
        {
            break;
        }
        SetMacSourceAddress(v);
    }
    return ret;
}

int PlcSettings::ParseDiscoverRequest(ByteBuffer& value, DLMSPlcRegister& reg)
{
    int ret;
    unsigned char ch;
    uint16_t v;
    if ((ret = value.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    reg.SetResponseProbability(ch);
    if ((ret = value.GetUInt16(&v)) != 0)
    {
        return ret;
    }
    reg.SetAllowedTimeSlots(v);
    if ((ret = value.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    reg.SetDiscoverReportInitialCredit(ch);
    if ((ret = value.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    reg.SetICEqualCredit(ch);
    return 0;
}

int PlcSettings::PingRequest(ByteBuffer& systemTitle, ByteBuffer& data)
{
    ByteBuffer bb;
    // Control byte.
    bb.SetUInt8(DLMS_COMMAND_PING_REQUEST);
    bb.Set(systemTitle.GetData(), systemTitle.GetSize());
    return DLMS::GetMacFrame(*m_Settings, 0x13, 0, &bb, data);
}

int PlcSettings::ParsePing(ByteBuffer& data, ByteBuffer& value)
{
    data.Clear();
    return value.Set(&data, 1, 6);
}

int PlcSettings::RepeaterCallRequest(ByteBuffer& data)
{
    ByteBuffer bb;
    // Control byte.
    bb.SetUInt8(DLMS_COMMAND_REPEAT_CALL_REQUEST);
    // MaxAdrMac.
    bb.SetUInt16(0x63);
    // Nb_Tslot_For_New
    bb.SetUInt8(0);
    // Reception-Threshold default value
    bb.SetUInt8(0);
    return DLMS::GetMacFrame(*m_Settings, 0x13, 0xFC, &bb, data);
}
