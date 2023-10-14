#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSActivityCalendar.h"
#include <sstream>

#ifndef DLMS_IGNORE_ACTIVITY_CALENDAR

DLMSActivityCalendar::DLMSActivityCalendar() :
    DLMSActivityCalendar("0.0.13.0.0.255", 0)
{
}

DLMSActivityCalendar::DLMSActivityCalendar(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR, ln, sn)
{

}

DLMSActivityCalendar::DLMSActivityCalendar(std::string ln) :
    DLMSActivityCalendar(ln, 0)
{

}

DLMSActivityCalendar::~DLMSActivityCalendar()
{
    for (std::vector<DLMSSeasonProfile*>::iterator it = m_SeasonProfileActive.begin(); it != m_SeasonProfileActive.end(); ++it)
    {
        delete *it;
    }
    m_SeasonProfileActive.clear();
    for (std::vector<DLMSWeekProfile*>::iterator it = m_WeekProfileTableActive.begin(); it != m_WeekProfileTableActive.end(); ++it)
    {
        delete *it;
    }
    m_WeekProfileTableActive.clear();
    for (std::vector<DLMSDayProfile*>::iterator it = m_DayProfileTableActive.begin(); it != m_DayProfileTableActive.end(); ++it)
    {
        delete *it;
    }
    m_DayProfileTableActive.clear();
    for (std::vector<DLMSSeasonProfile*>::iterator it = m_SeasonProfilePassive.begin(); it != m_SeasonProfilePassive.end(); ++it)
    {
        delete *it;
    }
    m_SeasonProfilePassive.clear();
    for (std::vector<DLMSWeekProfile*>::iterator it = m_WeekProfileTablePassive.begin(); it != m_WeekProfileTablePassive.end(); ++it)
    {
        delete *it;
    }
    m_WeekProfileTablePassive.clear();
    for (std::vector<DLMSDayProfile*>::iterator it = m_DayProfileTablePassive.begin(); it != m_DayProfileTablePassive.end(); ++it)
    {
        delete *it;
    }
    m_DayProfileTablePassive.clear();
}


std::string DLMSActivityCalendar::GetCalendarNameActive()
{
    return m_CalendarNameActive;
}
void DLMSActivityCalendar::SetCalendarNameActive(std::string value)
{
    m_CalendarNameActive = value;
}

std::vector<DLMSSeasonProfile*>& DLMSActivityCalendar::GetSeasonProfileActive()
{
    return m_SeasonProfileActive;
}
void DLMSActivityCalendar::SetSeasonProfileActive(std::vector<DLMSSeasonProfile*>& value)
{
    m_SeasonProfileActive = value;
}

std::vector<DLMSWeekProfile*>& DLMSActivityCalendar::GetWeekProfileTableActive()
{
    return m_WeekProfileTableActive;
}
void DLMSActivityCalendar::SetWeekProfileTableActive(std::vector<DLMSWeekProfile*>& value)
{
    m_WeekProfileTableActive = value;
}

std::vector<DLMSDayProfile*>& DLMSActivityCalendar::GetDayProfileTableActive()
{
    return m_DayProfileTableActive;
}
void DLMSActivityCalendar::SetDayProfileTableActive(std::vector<DLMSDayProfile*>& value)
{
    m_DayProfileTableActive = value;
}

std::string DLMSActivityCalendar::GetCalendarNamePassive()
{
    return m_CalendarNamePassive;
}
void DLMSActivityCalendar::SetCalendarNamePassive(std::string value)
{
    m_CalendarNamePassive = value;
}

std::vector<DLMSSeasonProfile*>& DLMSActivityCalendar::GetSeasonProfilePassive()
{
    return m_SeasonProfilePassive;
}

void DLMSActivityCalendar::SetSeasonProfilePassive(std::vector<DLMSSeasonProfile*>& value)
{
    m_SeasonProfilePassive = value;
}

std::vector<DLMSWeekProfile*>& DLMSActivityCalendar::GetWeekProfileTablePassive()
{
    return m_WeekProfileTablePassive;
}
void DLMSActivityCalendar::SetWeekProfileTablePassive(std::vector<DLMSWeekProfile*>& value)
{
    m_WeekProfileTablePassive = value;
}

std::vector<DLMSDayProfile*>& DLMSActivityCalendar::GetDayProfileTablePassive()
{
    return m_DayProfileTablePassive;
}
void DLMSActivityCalendar::SetDayProfileTablePassive(std::vector<DLMSDayProfile*>& value)
{
    m_DayProfileTablePassive = value;
}

DateTime& DLMSActivityCalendar::GetTime()
{
    return m_Time;
}
void DLMSActivityCalendar::SetTime(DateTime& value)
{
    m_Time = value;
}

// Returns amount of attributes.
int DLMSActivityCalendar::GetAttributeCount()
{
    return 10;
}

// Returns amount of methods.
int DLMSActivityCalendar::GetMethodCount()
{
    return 1;
}

void DLMSActivityCalendar::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    values.push_back(m_CalendarNameActive);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSSeasonProfile*>::iterator it = m_SeasonProfileActive.begin(); it != m_SeasonProfileActive.end(); ++it)
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
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<DLMSWeekProfile*>::iterator it = m_WeekProfileTableActive.begin(); it != m_WeekProfileTableActive.end(); ++it)
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
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<DLMSDayProfile*>::iterator it = m_DayProfileTableActive.begin(); it != m_DayProfileTableActive.end(); ++it)
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
    sb.str(std::string());

    values.push_back(m_CalendarNamePassive);
    sb << '[';
    empty = true;
    for (std::vector<DLMSSeasonProfile*>::iterator it = m_SeasonProfilePassive.begin(); it != m_SeasonProfilePassive.end(); ++it)
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
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<DLMSWeekProfile*>::iterator it = m_WeekProfileTablePassive.begin(); it != m_WeekProfileTablePassive.end(); ++it)
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
    sb.str(std::string());

    sb << '[';
    empty = true;
    for (std::vector<DLMSDayProfile*>::iterator it = m_DayProfileTablePassive.begin(); it != m_DayProfileTablePassive.end(); ++it)
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
    sb.str(std::string());
    values.push_back(m_Time.ToString());
}

void DLMSActivityCalendar::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //CalendarNameActive
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //SeasonProfileActive
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }

    //WeekProfileTableActive
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //DayProfileTableActive
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //CalendarNamePassive
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //SeasonProfilePassive
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    //WeekProfileTablePassive
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    //DayProfileTablePassive
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    //Time.
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
}

int DLMSActivityCalendar::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;

    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 9)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 10)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int GetSeasonProfile(DLMSSettings& settings, std::vector<DLMSSeasonProfile*>& items, ByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    unsigned long cnt = (unsigned long)items.size();
    //Add count
    Helpers::SetObjectCount(cnt, data);
    DLMSVariant tmp;
    for (std::vector<DLMSSeasonProfile*>::iterator it = items.begin(); it != items.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        tmp = (*it)->GetName();
        Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        tmp = (*it)->GetStart();
        Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        tmp = (*it)->GetWeekName();
        Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
    }
    return 0;
}

int GetWeekProfile(std::vector<DLMSWeekProfile*>& items, ByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    unsigned long cnt = (unsigned long)items.size();
    //Add count
    Helpers::SetObjectCount(cnt, data);
    DLMSVariant tmp;
    for (std::vector<DLMSWeekProfile*>::iterator it = items.begin(); it != items.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(8);
        tmp.Clear();
        tmp.Add((*it)->GetName());
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        tmp = (*it)->GetMonday();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetTuesday();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetWednesday();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetThursday();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetFriday();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetSaturday();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp);
        tmp = (*it)->GetSunday();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp);
    }
    return 0;
}

int GetDayProfile(DLMSSettings& settings, std::vector<DLMSDayProfile*>& items, ByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    unsigned long cnt = (unsigned long)items.size();
    //Add count
    Helpers::SetObjectCount(cnt, data);
    for (std::vector<DLMSDayProfile*>::iterator it = items.begin(); it != items.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        DLMSVariant tmp2 = (*it)->GetDayId();
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT8, tmp2);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        std::vector<DLMSDayProfileAction*>& schedules = (*it)->GetDaySchedules();
        Helpers::SetObjectCount((unsigned long)schedules.size(), data);
        DLMSVariant time, ln, selector;
        for (std::vector<DLMSDayProfileAction*>::iterator action = schedules.begin(); action != schedules.end(); ++action)
        {
            Helpers::SetLogicalName((*action)->GetScriptLogicalName().c_str(), ln);
            time = (*action)->GetStartTime();
            selector = (*action)->GetScriptSelector();
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, time);
            Helpers::SetData(NULL, data, DLMS_DATA_TYPE_OCTET_STRING, ln);
            Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT16, selector);
        }
    }
    return 0;
}

// Returns value of given attribute.
int DLMSActivityCalendar::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    ByteBuffer data;
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
        DLMSVariant tmp;
        tmp.Add(m_CalendarNameActive.c_str(), (int)m_CalendarNameActive.size());
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        GetSeasonProfile(settings, m_SeasonProfileActive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        GetWeekProfile(m_WeekProfileTableActive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetByteArray(true);
        GetDayProfile(settings, m_DayProfileTableActive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.GetValue().Add(m_CalendarNamePassive);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetByteArray(true);
        GetSeasonProfile(settings, m_SeasonProfilePassive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.SetByteArray(true);
        GetWeekProfile(m_WeekProfileTablePassive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        e.SetByteArray(true);
        GetDayProfile(settings, m_DayProfileTablePassive, data);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 10)
    {
        e.SetValue(GetTime());
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

void AddSeasonProfile(std::vector<DLMSSeasonProfile*>& items, std::vector<DLMSVariant>& list)
{
    for (std::vector<DLMSSeasonProfile*>::iterator item = items.begin(); item != items.end(); ++item)
    {
        delete *item;
    }
    items.clear();
    ByteBuffer bb;
    DLMSVariant tmp;
    for (std::vector<DLMSVariant>::iterator item = list.begin(); item != list.end(); ++item)
    {
        DLMSSeasonProfile *it = new DLMSSeasonProfile();
        bb.Clear();
        bb.Set(item->Arr[0].byteArr, item->Arr[0].GetSize());
        it->SetName(bb);
        tmp.Clear();
        DLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_DATETIME, tmp);
        it->SetStart(tmp.dateTime);
        tmp.Clear();
        bb.Clear();
        bb.Set(item->Arr[2].byteArr, item->Arr[2].GetSize());
        it->SetWeekName(bb);
        items.push_back(it);
    }
}

void AddWeekProfileTable(std::vector<DLMSWeekProfile*>& items, std::vector<DLMSVariant>& list)
{
    for (std::vector<DLMSWeekProfile*>::iterator item = items.begin(); item != items.end(); ++item)
    {
        delete *item;
    }
    items.clear();
    ByteBuffer bb;
    for (std::vector<DLMSVariant>::iterator item = list.begin(); item != list.end(); ++item)
    {
        DLMSVariant tmp;
        DLMSWeekProfile *it = new DLMSWeekProfile();
        bb.Clear();
        bb.Set(item->Arr[0].byteArr, item->Arr[0].GetSize());
        it->SetName(bb);
        it->SetMonday((*item).Arr[1].lVal);
        it->SetTuesday((*item).Arr[2].lVal);
        it->SetWednesday((*item).Arr[3].lVal);
        it->SetThursday((*item).Arr[4].lVal);
        it->SetFriday((*item).Arr[5].lVal);
        it->SetSaturday((*item).Arr[6].lVal);
        it->SetSunday((*item).Arr[7].lVal);
        items.push_back(it);
    }
}

void AddDayProfileTable(std::vector<DLMSDayProfile*>& items, std::vector<DLMSVariant>& list)
{
    for (std::vector<DLMSDayProfile*>::iterator item = items.begin(); item != items.end(); ++item)
    {
        delete *item;
    }
    items.clear();

    for (std::vector<DLMSVariant>::iterator item = list.begin(); item != list.end(); ++item)
    {
        DLMSDayProfile* it = new DLMSDayProfile();
        it->SetDayId((*item).Arr[0].iVal);
        std::string ln;
        for (std::vector<DLMSVariant>::iterator it2 = (*item).Arr[1].Arr.begin(); it2 != (*item).Arr[1].Arr.end(); ++it2)
        {
            DLMSDayProfileAction * ac = new DLMSDayProfileAction();
            DLMSVariant tmp;
            DLMSClient::ChangeType(it2->Arr[0], DLMS_DATA_TYPE_TIME, tmp);
            ac->SetStartTime((Time&)tmp.dateTime);
            Helpers::GetLogicalName((*it2).Arr[1].byteArr, ln);
            ac->SetScriptLogicalName(ln);
            ac->SetScriptSelector((*it2).Arr[2].lVal);
            it->GetDaySchedules().push_back(ac);
        }
        items.push_back(it);
    }
}

// Set value of given attribute.
int DLMSActivityCalendar::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        std::string name;
        name.append(e.GetValue().byteArr, e.GetValue().byteArr + e.GetValue().GetSize());
        SetCalendarNameActive(name);
    }
    else if (e.GetIndex() == 3)
    {
        AddSeasonProfile(m_SeasonProfileActive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 4)
    {
        AddWeekProfileTable(m_WeekProfileTableActive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 5)
    {
        AddDayProfileTable(m_DayProfileTableActive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 6)
    {
        std::string name;
        name.append(e.GetValue().byteArr, e.GetValue().byteArr + e.GetValue().GetSize());
        SetCalendarNamePassive(name);
    }
    else if (e.GetIndex() == 7)
    {
        AddSeasonProfile(m_SeasonProfilePassive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 8)
    {
        AddWeekProfileTable(m_WeekProfileTablePassive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 9)
    {
        AddDayProfileTable(m_DayProfileTablePassive, e.GetValue().Arr);
    }
    else if (e.GetIndex() == 10)
    {
        int ret;
        DLMSVariant tmp;
        if ((ret = DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp)) != 0)
        {
            return ret;
        }
        SetTime(tmp.dateTime);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_ACTIVITY_CALENDAR
