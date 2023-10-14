#include "DLMSGSMDiagnostic.h"
#ifndef DLMS_IGNORE_GSM_DIAGNOSTIC
#include "DLMSClient.h"

//Constructor.
DLMSGSMDiagnostic::DLMSGSMDiagnostic() :
    DLMSGSMDiagnostic("0.0.25.6.0.255", 0)
{
}

//LN Constructor.
DLMSGSMDiagnostic::DLMSGSMDiagnostic(std::string ln) :
    DLMSGSMDiagnostic(ln, 0)
{
}

//SN Constructor.
DLMSGSMDiagnostic::DLMSGSMDiagnostic(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC, ln, sn)
{
    m_Status = DLMS_GSM_STATUS_NONE;
    m_CircuitSwitchStatus = DLMS_GSM_CIRCUIT_SWITCH_STATUS_INACTIVE;
    m_PacketSwitchStatus = DLMS_GSM_PACKET_SWITCH_STATUS_INACTIVE;
    m_Version = 1;
}

DLMSGSMDiagnostic::~DLMSGSMDiagnostic()
{
    for (std::vector<AdjacentCell*>::iterator it = m_AdjacentCells.begin(); it != m_AdjacentCells.end(); ++it)
    {
        delete *it;
    }
    m_AdjacentCells.clear();
}

std::string& DLMSGSMDiagnostic::GetOperator() {
    return m_Operator;
}

void DLMSGSMDiagnostic::SetOperator(std::string& value) {
    m_Operator = value;
}

DLMS_GSM_STATUS DLMSGSMDiagnostic::GetStatus() {
    return m_Status;
}

void DLMSGSMDiagnostic::SetStatus(DLMS_GSM_STATUS value) {
    m_Status = value;
}

DLMS_GSM_CIRCUIT_SWITCH_STATUS DLMSGSMDiagnostic::GetCircuitSwitchStatus() {
    return m_CircuitSwitchStatus;
}

void DLMSGSMDiagnostic::SetCircuitSwitchStatus(DLMS_GSM_CIRCUIT_SWITCH_STATUS value) {
    m_CircuitSwitchStatus = value;
}

DLMS_GSM_PACKET_SWITCH_STATUS DLMSGSMDiagnostic::GetPacketSwitchStatus()
{
    return m_PacketSwitchStatus;
}

void DLMSGSMDiagnostic::SetPacketSwitchStatus(DLMS_GSM_PACKET_SWITCH_STATUS value)
{
    m_PacketSwitchStatus = value;
}

DLMSGSMCellInfo& DLMSGSMDiagnostic::GetCellInfo()
{
    return m_CellInfo;
}

std::vector<AdjacentCell*>& DLMSGSMDiagnostic::GetAdjacentCells()
{
    return m_AdjacentCells;
}

DateTime& DLMSGSMDiagnostic::GetCaptureTime()
{
    return m_CaptureTime;
}

void DLMSGSMDiagnostic::SetCaptureTime(DateTime& value)
{
    m_CaptureTime = value;
}

// Returns amount of attributes.
int DLMSGSMDiagnostic::GetAttributeCount()
{
    return 8;
}

// Returns amount of methods.
int DLMSGSMDiagnostic::GetMethodCount()
{
    return 0;
}

void DLMSGSMDiagnostic::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_Operator);
    values.push_back(DLMSConverter::ToString(m_Status));
    values.push_back(DLMSConverter::ToString(m_CircuitSwitchStatus));
    values.push_back(DLMSConverter::ToString(m_PacketSwitchStatus));
    values.push_back(m_CellInfo.ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<AdjacentCell*>::iterator it = m_AdjacentCells.begin(); it != m_AdjacentCells.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        else
        {
            empty = false;
        }
        sb << (*it)->GetCellId();
        sb << ": ";
        sb << (*it)->GetSignalQuality();
    }
    sb << ']';
    values.push_back(sb.str());
    values.push_back(m_CaptureTime.ToString());
}

void DLMSGSMDiagnostic::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Operator
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // Status
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // CircuitSwitchStatus
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // PacketSwitchStatus
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    // CellInfo
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    // AdjacentCells
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    // CaptureTime
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
}

int DLMSGSMDiagnostic::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_STRING;
        break;
    case 3:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 4:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 5:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 6:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    case 7:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 8:
        type = DLMS_DATA_TYPE_DATETIME;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSGSMDiagnostic::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    DLMSVariant cellId, locationId, signalQuality, ber, mcc, mnc, channel;
    ByteBuffer bb;
    int ret;
    DLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        break;
    case 2:
        e.SetValue(m_Operator);
        break;
    case 3:
        e.SetValue(m_Status);
        break;
    case 4:
        e.SetValue(m_CircuitSwitchStatus);
        break;
    case 5:
        e.SetValue(m_PacketSwitchStatus);
        break;
    case 6:
        e.SetByteArray(true);
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(m_Version == 0 ? 4 : 7);
        cellId = m_CellInfo.GetCellId();
        locationId = m_CellInfo.GetLocationId();
        signalQuality = m_CellInfo.GetSignalQuality();
        ber = m_CellInfo.GetBer();
        if ((ret = Helpers::SetData(&settings, bb, m_Version == 0 ? DLMS_DATA_TYPE_UINT16 : DLMS_DATA_TYPE_UINT32, cellId)) != 0 ||
            (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT16, locationId)) != 0 ||
            (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT8, signalQuality)) != 0 ||
            (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT8, ber)) != 0)
        {
            return ret;
        }
        if (m_Version > 0)
        {
            mcc = m_CellInfo.GetMobileCountryCode();
            mnc = m_CellInfo.GetMobileNetworkCode();
            channel = m_CellInfo.GetChannelNumber();
            if ((ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT16, mcc)) != 0 ||
                (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT16, mnc)) != 0 ||
                (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT32, channel)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(bb);
        break;
    case 7:
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        bb.SetUInt8((unsigned char)m_AdjacentCells.size());
        for (std::vector<AdjacentCell*>::iterator it = m_AdjacentCells.begin(); it != m_AdjacentCells.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            bb.SetUInt8(2);
            cellId = (*it)->GetCellId();
            signalQuality = (*it)->GetSignalQuality();
            if ((ret = Helpers::SetData(&settings, bb, m_Version == 0 ? DLMS_DATA_TYPE_UINT16 : DLMS_DATA_TYPE_UINT32, cellId)) != 0 ||
                (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT8, locationId)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(bb);
        break;
    case 8:
        e.SetValue(m_CaptureTime);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSGSMDiagnostic::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
        break;
    case 2:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            m_Operator.clear();
            m_Operator.append(e.GetValue().byteArr, e.GetValue().byteArr + e.GetValue().GetSize());
        }
        else
        {
            m_Operator = e.GetValue().strVal;
        }
        break;
    case 3:
        m_Status = (DLMS_GSM_STATUS)e.GetValue().ToInteger();
        break;
    case 4:
        m_CircuitSwitchStatus = (DLMS_GSM_CIRCUIT_SWITCH_STATUS)e.GetValue().ToInteger();
        break;
    case 5:
        m_PacketSwitchStatus = (DLMS_GSM_PACKET_SWITCH_STATUS)e.GetValue().ToInteger();
        break;
    case 6:
        if (e.GetValue().Arr.size() == 4 || e.GetValue().Arr.size() == 7)
        {
            std::vector<DLMSVariant> tmp = (std::vector<DLMSVariant>) e.GetValue().Arr;
            m_CellInfo.SetCellId(tmp[0].ToInteger());
            m_CellInfo.SetLocationId(tmp[1].ToInteger());
            m_CellInfo.SetSignalQuality(tmp[2].ToInteger());
            m_CellInfo.SetBer(tmp[3].ToInteger());
            if (m_Version > 0)
            {
                m_CellInfo.SetMobileCountryCode(tmp[4].ToInteger());
                m_CellInfo.SetMobileNetworkCode(tmp[5].ToInteger());
                m_CellInfo.SetChannelNumber(tmp[6].ToInteger());
            }
        }
        break;
    case 7:
        m_AdjacentCells.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            std::vector<DLMSVariant> tmp = (std::vector<DLMSVariant>) it->Arr;
            AdjacentCell* ac = new AdjacentCell();
            ac->SetCellId(tmp[0].ToInteger());
            ac->SetSignalQuality(tmp[1].ToInteger());
            m_AdjacentCells.push_back(ac);
        }
        break;
    case 8:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            DLMSVariant tmp;
            if ((ret = DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp)) != 0)
            {
                return ret;
            }
            m_CaptureTime = tmp.dateTime;
        }
        else
        {
            m_CaptureTime = e.GetValue().dateTime;
        }
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_GSM_DIAGNOSTIC
