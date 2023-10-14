#include "DLMSPrimeNbOfdmPlcMacNetworkAdministrationData.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA
//Constructor.
DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::DLMSPrimeNbOfdmPlcMacNetworkAdministrationData() :
    DLMSPrimeNbOfdmPlcMacNetworkAdministrationData("0.0.28.5.0.255", 0)
{
}

//SN Constructor.
DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::DLMSPrimeNbOfdmPlcMacNetworkAdministrationData(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA, ln, sn)
{

}

//LN Constructor.
DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::DLMSPrimeNbOfdmPlcMacNetworkAdministrationData(std::string ln) :
    DLMSPrimeNbOfdmPlcMacNetworkAdministrationData(ln, 0)
{

}

//Destructor.
DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::~DLMSPrimeNbOfdmPlcMacNetworkAdministrationData()
{
    ClearMulticastEntry();
    ClearDirectTable();
    ClearAvailableSwitches();
    ClearCommunications();
}

std::vector<MacMulticastEntry*>& DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetMulticastEntries()
{
    return m_MulticastEntries;
}

std::vector<short>& DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetSwitchTable()
{
    return m_SwitchTable;
}

std::vector<MacDirectTable*>& DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetDirectTable()
{
    return m_DirectTable;
}

std::vector<MacAvailableSwitch*>& DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAvailableSwitches()
{
    return m_AvailableSwitches;
}

std::vector<MacPhyCommunication*>& DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetCommunications()
{
    return m_Communications;
}

// Returns amount of attributes.
int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetMethodCount()
{
    return 1;
}

void DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    {
        for (std::vector<MacMulticastEntry*>::iterator it = m_MulticastEntries.begin(); it != m_MulticastEntries.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
        //Clear str.
        sb.str(std::string());
    }
    {
        sb << '[';
        empty = true;
        for (std::vector<short>::iterator it = m_SwitchTable.begin(); it != m_SwitchTable.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = DLMSVariant(*it).ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());

        //Clear str.
        sb.str(std::string());
    }
    {
        sb << '[';
        empty = true;
        for (std::vector<MacDirectTable*>::iterator it = m_DirectTable.begin(); it != m_DirectTable.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
        //Clear str.
        sb.str(std::string());
    }
    {
        sb << '[';
        empty = true;
        for (std::vector<MacAvailableSwitch*>::iterator it = m_AvailableSwitches.begin(); it != m_AvailableSwitches.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
        //Clear str.
        sb.str(std::string());
    }
    {//std::vector<CGXMacPhyCommunication*> m_Communications;
        sb << '[';
        empty = true;
        for (std::vector<MacPhyCommunication*>::iterator it = m_Communications.begin(); it != m_Communications.end(); ++it)
        {
            if (!empty)
            {
                sb << ", ";
            }
            empty = false;
            std::string str = (*it)->ToString();
            sb.write(str.c_str(), str.size());
        }
        sb << ']';
        values.push_back(sb.str());
    }
}


int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::Reset(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_MulticastEntries.clear();
        m_SwitchTable.clear();
        m_DirectTable.clear();
        m_AvailableSwitches.clear();
        m_Communications.clear();
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}


void DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // MulticastEntries
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // SwitchTable
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // DirectTable
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // AvailableSwitches
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // Communications
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetMulticastEntries(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    Helpers::SetObjectCount((unsigned long)m_MulticastEntries.size(), bb);
    for (std::vector<MacMulticastEntry*>::iterator it = m_MulticastEntries.begin(); it != m_MulticastEntries.end(); ++it)
    {
        DLMSVariant a = (*it)->GetId(), b = (*it)->GetMembers();
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(2)) != 0 ||
            (ret = Helpers::SetData(NULL, bb, DLMS_DATA_TYPE_INT8, a)) != 0 ||
            (ret = Helpers::SetData(NULL, bb, DLMS_DATA_TYPE_INT16, b)) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetSwitchTable(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    Helpers::SetObjectCount((unsigned long)m_SwitchTable.size(), bb);
    for (std::vector<short>::iterator it = m_SwitchTable.begin(); it != m_SwitchTable.end(); ++it)
    {
        if ((ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, *it)) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetDirectTable(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    Helpers::SetObjectCount((unsigned long)m_DirectTable.size(), bb);
    for (std::vector<MacDirectTable*>::iterator it = m_DirectTable.begin(); it != m_DirectTable.end(); ++it)
    {
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(7)) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, (*it)->GetSourceSId())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, (*it)->GetSourceLnId())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, (*it)->GetSourceLcId())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, (*it)->GetDestinationSId())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, (*it)->GetDestinationLnId())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, (*it)->GetDestinationLcId())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_OCTET_STRING, (*it)->GetDid())) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetAvailableSwitches(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    Helpers::SetObjectCount((unsigned long)m_AvailableSwitches.size(), bb);
    for (std::vector<MacAvailableSwitch*>::iterator it = m_AvailableSwitches.begin(); it != m_AvailableSwitches.end(); ++it)
    {
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(5)) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_OCTET_STRING, (*it)->GetSna())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT16, (*it)->GetLsId())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT8, (*it)->GetLevel())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxLevel())) != 0 ||
            (ret = Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxSnr())) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetCommunications(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    Helpers::SetObjectCount((unsigned long)m_Communications.size(), bb);
    for (std::vector<MacPhyCommunication*>::iterator it = m_Communications.begin(); it != m_Communications.end(); ++it)
    {
        if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
            (ret = bb.SetUInt8(9)) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_OCTET_STRING, (*it)->GetEui())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxPower())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxCoding())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxCoding())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxLvl())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetSnr())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxPowerModified())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetTxCodingModified())) != 0 ||
            (ret = Helpers::SetData2(&settings, bb, DLMS_DATA_TYPE_INT8, (*it)->GetRxCodingModified())) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        e.SetValue(bb);
    }
    return ret;
}

// Returns value of given attribute.
int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    switch (e.GetIndex())
    {
    case 1:
    {
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
    }
    break;
    case 2:
        ret = GetMulticastEntries(settings, e);
        break;
    case 3:
        ret = GetSwitchTable(settings, e);
        break;
    case 4:
        ret = GetDirectTable(settings, e);
        break;
    case 5:
        ret = GetAvailableSwitches(settings, e);
        break;
    case 6:
        ret = GetCommunications(settings, e);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

void DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearMulticastEntry()
{
    for (std::vector<MacMulticastEntry*>::iterator it = m_MulticastEntries.begin(); it != m_MulticastEntries.end(); ++it)
    {
        delete (*it);
    }
    m_MulticastEntries.clear();
}

void DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearDirectTable()
{
    for (std::vector<MacDirectTable*>::iterator it = m_DirectTable.begin(); it != m_DirectTable.end(); ++it)
    {
        delete (*it);
    }
    m_DirectTable.clear();
}

void DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearAvailableSwitches()
{
    for (std::vector<MacAvailableSwitch*>::iterator it = m_AvailableSwitches.begin(); it != m_AvailableSwitches.end(); ++it)
    {
        delete (*it);
    }
    m_AvailableSwitches.clear();
}

void DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::ClearCommunications()
{
    for (std::vector<MacPhyCommunication*>::iterator it = m_Communications.begin(); it != m_Communications.end(); ++it)
    {
        delete (*it);
    }
    m_Communications.clear();
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetMulticastEntry(std::vector<DLMSVariant>& list)
{
    int ret = 0;
    ClearMulticastEntry();
    for (std::vector<DLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        MacMulticastEntry* v = new MacMulticastEntry();
        v->SetId(it->Arr.at(0).ToInteger());
        v->SetMembers(it->Arr.at(1).ToInteger());
        m_MulticastEntries.push_back(v);
    }
    return ret;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetSwitchTable(std::vector<DLMSVariant>& list)
{
    m_SwitchTable.clear();
    for (std::vector<DLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        m_SwitchTable.push_back(it->ToInteger());
    }
    return 0;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetDirectTable(std::vector<DLMSVariant>& list)
{
    ClearDirectTable();
    for (std::vector<DLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        ByteBuffer tmp;
        MacDirectTable* v = new MacDirectTable();
        v->SetSourceSId(it->Arr.at(0).ToInteger());
        v->SetSourceLnId(it->Arr.at(1).ToInteger());
        v->SetSourceLcId(it->Arr.at(2).ToInteger());
        v->SetDestinationSId(it->Arr.at(3).ToInteger());
        v->SetDestinationLnId(it->Arr.at(4).ToInteger());
        v->SetDestinationLcId(it->Arr.at(5).ToInteger());
        tmp.Set(it->Arr.at(6).byteArr, it->Arr.at(6).GetSize());
        v->SetDid(tmp);
        m_DirectTable.push_back(v);
    }
    return 0;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetAvailableSwitches(std::vector<DLMSVariant>& list)
{
    ClearAvailableSwitches();
    for (std::vector<DLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        ByteBuffer tmp;
        tmp.Set(it->Arr.at(0).byteArr, it->Arr.at(0).GetSize());
        MacAvailableSwitch* v = new MacAvailableSwitch();
        v->SetSna(tmp);
        v->SetLsId(it->Arr.at(1).ToInteger());
        v->SetLevel(it->Arr.at(2).ToInteger());
        v->SetRxLevel(it->Arr.at(3).ToInteger());
        v->SetRxSnr(it->Arr.at(4).ToInteger());
        m_AvailableSwitches.push_back(v);
    }
    return 0;
}

int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetCommunications(std::vector<DLMSVariant>& list)
{
    ClearCommunications();
    for (std::vector<DLMSVariant>::iterator it = list.begin(); it != list.end(); ++it)
    {
        ByteBuffer tmp;
        tmp.Set(it->Arr.at(0).byteArr, it->Arr.at(0).GetSize());
        MacPhyCommunication* v = new MacPhyCommunication();
        v->SetEui(tmp);
        v->SetTxPower(it->Arr.at(1).ToInteger());
        v->SetTxCoding(it->Arr.at(2).ToInteger());
        v->SetRxCoding(it->Arr.at(3).ToInteger());
        v->SetRxLvl(it->Arr.at(4).ToInteger());
        v->SetSnr(it->Arr.at(5).ToInteger());
        v->SetTxPowerModified(it->Arr.at(6).ToInteger());
        v->SetTxCodingModified(it->Arr.at(7).ToInteger());
        v->SetRxCodingModified(it->Arr.at(8).ToInteger());
        m_Communications.push_back(v);
    }
    return 0;
}

// Set value of given attribute.
int DLMSPrimeNbOfdmPlcMacNetworkAdministrationData::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        ret = SetMulticastEntry(e.GetValue().Arr);
        break;
    case 3:
        ret = SetSwitchTable(e.GetValue().Arr);
        break;
    case 4:
        ret = SetDirectTable(e.GetValue().Arr);
        break;
    case 5:
        ret = SetAvailableSwitches(e.GetValue().Arr);
        break;
    case 6:
        ret = SetCommunications(e.GetValue().Arr);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

#endif //DLMS_IGNORE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA
