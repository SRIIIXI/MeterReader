#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSSchedule.h"
#include "BitString.h"

#ifndef DLMS_IGNORE_SCHEDULE
//Constructor.
DLMSSchedule::DLMSSchedule() :
    DLMSSchedule("0.0.12.0.0.255", 0)
{
}

//SN Constructor.
DLMSSchedule::DLMSSchedule(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SCHEDULE, ln, sn)
{

}

//LN Constructor.
DLMSSchedule::DLMSSchedule(std::string ln) :
    DLMSSchedule(ln, 0)
{

}

DLMSSchedule::~DLMSSchedule()
{
    for (std::vector<DLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
    {
        delete* it;
    }
    m_Entries.clear();
}

// Get value of COSEM Data object.
std::vector<DLMSScheduleEntry*>& DLMSSchedule::GetEntries()
{
    return m_Entries;
}

// Set value of COSEM Data object.
void DLMSSchedule::SetEntries(std::vector<DLMSScheduleEntry*> &value)
{
    m_Entries = value;
}

// Returns amount of attributes.
int DLMSSchedule::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSSchedule::GetMethodCount()
{
    return 3;
}

void DLMSSchedule::GetValues(std::vector<std::string>& values)
{
    std::stringstream sb;
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::string str;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb << (*it)->GetIndex();
        sb << " ";
        sb << (*it)->GetEnable();
        sb << " ";
        sb << (*it)->GetLogicalName();
        sb << " ";
        sb << (*it)->GetScriptSelector();
        sb << " ";
        (*it)->GetSwitchTime().ToFormatString(str);
        sb << str;
        sb << " ";
        sb << (*it)->GetValidityWindow();
        sb << " ";
        sb << (*it)->GetExecWeekdays();
        sb << " ";
        sb << (*it)->GetExecSpecDays();
        sb << " ";
        (*it)->GetBeginDate().ToFormatString(str);
        sb << str;
        sb << " ";
        (*it)->GetEndDate().ToFormatString(str);
        sb << str;
    }
    values.push_back(sb.str());
}

int DLMSSchedule::RemoveEntry(unsigned short index)
{
    for (std::vector<DLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
    {
        if ((*it)->GetIndex() == index)
        {
            delete* it;
            m_Entries.erase(it);
            break;
        }
    }
    return 0;
}

// Create a new entry.
int CreateEntry(DLMSSettings& settings, std::vector<DLMSVariant>& arr, DLMSScheduleEntry*& item)
{
    int ret;
    DLMSVariant tmp;
    Time t;
    Date d;
    item = new DLMSScheduleEntry();
    item->SetIndex(arr[0].ToInteger());
    item->SetEnable(arr[1].boolVal);
    if ((ret = DLMSClient::ChangeType(arr[2], DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    item->SetLogicalName(tmp.ToString());
    item->SetScriptSelector(arr[3].ToInteger());
    if ((ret = DLMSClient::ChangeType(arr[4], DLMS_DATA_TYPE_TIME, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    t = tmp.dateTime;
    item->SetSwitchTime(t);
    item->SetValidityWindow(arr[5].ToInteger());
    item->SetExecWeekdays((DLMS_WEEKDAYS)arr[6].ToInteger());
    item->SetExecSpecDays(arr[7].strVal);
    if ((ret = DLMSClient::ChangeType(arr[8], DLMS_DATA_TYPE_DATE, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    d = tmp.dateTime;
    item->SetBeginDate(d);
    if ((ret = DLMSClient::ChangeType(arr[9], DLMS_DATA_TYPE_DATE, tmp)) != 0)
    {
        delete item;
        return ret;
    }
    d = tmp.dateTime;
    item->SetEndDate(d);
    return ret;
}

int AddEntry(DLMSSettings& settings, DLMSScheduleEntry* it, ByteBuffer& data)
{
    unsigned char ln[6];
    int ret;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
        (ret = data.SetUInt8(10)) != 0 ||
        //Add index.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_UINT16)) != 0 ||
        (ret = data.SetUInt16(it->GetIndex())) != 0 ||
        //Add enable.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_BOOLEAN)) != 0 ||
        (ret = data.SetUInt8(it->GetEnable())) != 0 ||
        //Add logical Name.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) != 0 ||
        (ret = data.SetUInt8(6)) != 0 ||
        (ret = Helpers::SetLogicalName(it->GetLogicalName().c_str(), ln)) != 0 ||
        (ret = data.Set(ln, 6)) != 0 ||
        //Add script selector.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_UINT16)) != 0 ||
        (ret = data.SetUInt16(it->GetScriptSelector())) != 0 ||
        //Add switch time.
        (ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, it->GetSwitchTime())) != 0 ||
        //Add validity window.
        (ret = data.SetUInt8(DLMS_DATA_TYPE_UINT16)) != 0 ||
        (ret = data.SetUInt16(it->GetValidityWindow())) != 0 ||
        //Add exec week days.
        (ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_BIT_STRING, BitString::ToBitString((unsigned char)it->GetExecWeekdays(), 7))) != 0 ||
        //Add exec spec days.
        (ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_BIT_STRING, it->GetExecSpecDays())) != 0 ||
        //Add begin date.
        (ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, it->GetBeginDate())) != 0 ||
        //Add end date.
        (ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, it->GetEndDate())) != 0)
    {

    }
    return ret;
}

int DLMSSchedule::Insert(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply)
{
    int ret;
    ByteBuffer data;
    if ((ret = AddEntry(client->m_Settings, entry, data)) == 0)
    {
        DLMSVariant tmp = data;
        ret = client->Method(this, 2, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int DLMSSchedule::Delete(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply)
{
    int ret;
    ByteBuffer data;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        //Add structure size.
        (ret = data.SetUInt8(2)) == 0 &&
        //firstIndex
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        //lastIndex
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0)
    {
        DLMSVariant tmp = data;
        ret = client->Method(this, 3, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int DLMSSchedule::Enable(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply)
{
    int ret;
    ByteBuffer data;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        //Add structure size.
        (ret = data.SetUInt8(4)) == 0 &&
        //firstIndex
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        //lastIndex
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, 0)) == 0 &&
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, 0)) == 0)
    {
        DLMSVariant tmp = data;
        ret = client->Method(this, 1, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int DLMSSchedule::Disable(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply)
{
    int ret;
    ByteBuffer data;
    if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        //Add structure size.
        (ret = data.SetUInt8(4)) == 0 &&
        //firstIndex
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, 0)) == 0 &&
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, 0)) == 0 &&
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0 &&
        //lastIndex
        (ret = Helpers::SetData2(NULL, data, DLMS_DATA_TYPE_UINT16, entry->GetIndex())) == 0)
    {
        DLMSVariant tmp = data;
        ret = client->Method(this, 1, tmp, DLMS_DATA_TYPE_STRUCTURE, reply);
    }
    return ret;
}

int DLMSSchedule::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    unsigned short index;
    switch (e.GetIndex())
    {
        //Enable/disable entry
    case 1:
    {
        //Enable
        for (index = e.GetParameters().Arr[0].uiVal; index <= e.GetParameters().Arr[1].uiVal; ++index)
        {
            if (index != 0)
            {
                for (std::vector<DLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
                {
                    if ((*it)->GetIndex() == index)
                    {
                        (*it)->SetEnable(true);
                        break;
                    }
                }
            }
        }
        //Disable
        for (index = e.GetParameters().Arr[2].uiVal; index <= e.GetParameters().Arr[3].uiVal; ++index)
        {
            if (index != 0)
            {
                for (std::vector<DLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
                {
                    if ((*it)->GetIndex() == index)
                    {
                        (*it)->SetEnable(false);
                        break;
                    }
                }
            }
        }
    }
    break;
    //Insert entry
    case 2:
    {
        DLMSScheduleEntry* entry;
        if ((ret = CreateEntry(settings, e.GetParameters().Arr, entry)) == 0)
        {
            if ((ret = RemoveEntry(entry->GetIndex())) != 0)
            {
                break;
            }
            m_Entries.push_back(entry);
        }
    }
    break;
    //Delete entry
    case 3:
    {
        for (index = e.GetParameters().Arr[0].uiVal; index <= e.GetParameters().Arr[1].uiVal; ++index)
        {
            if ((ret = RemoveEntry(index)) != 0)
            {
                break;
            }
        }
    }
    break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

void DLMSSchedule::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Entries
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSSchedule::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSSchedule::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        int ret = 0;
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long) m_Entries.size(), data);
        for (std::vector<DLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
        {
            if ((ret = AddEntry(settings, *it, data)) != 0)
            {
                break;
            }
        }
        e.SetValue(data);
        return ret;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSSchedule::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    if (e.GetIndex() == 1)
    {
        ret = SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        for (std::vector<DLMSScheduleEntry*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
        {
            delete* it;
        }
        m_Entries.clear();
        DLMSScheduleEntry* item;
        for (std::vector<DLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            if ((ret = CreateEntry(settings, it->Arr, item)) != 0)
            {
                break;
            }
            m_Entries.push_back(item);
        }
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}
#endif //DLMS_IGNORE_SCHEDULE
