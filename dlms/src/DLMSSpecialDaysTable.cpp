#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSSpecialDaysTable.h"
#include <sstream>

#ifndef DLMS_IGNORE_SPECIAL_DAYS_TABLE
//Constructor.
DLMSSpecialDaysTable::DLMSSpecialDaysTable() :
    DLMSSpecialDaysTable("", 0)
{
}

//SN Constructor.
DLMSSpecialDaysTable::DLMSSpecialDaysTable(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE, ln, sn)
{

}

//LN Constructor.
DLMSSpecialDaysTable::DLMSSpecialDaysTable(std::string ln) :
    DLMSSpecialDaysTable(ln, 0)
{

}

DLMSSpecialDaysTable::~DLMSSpecialDaysTable()
{
    for (std::vector<DLMSSpecialDay*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
    {
        delete *it;
    }
    m_Entries.clear();
}

std::vector<DLMSSpecialDay*>& DLMSSpecialDaysTable::GetEntries()
{
    return m_Entries;
}

// Returns amount of attributes.
int DLMSSpecialDaysTable::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSSpecialDaysTable::GetMethodCount()
{
    return 2;
}

int DLMSSpecialDaysTable::Insert(DLMSClient* client, DLMSSpecialDay* entry, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data;
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    bb.SetUInt8(3);
    Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_UINT16, entry->GetIndex());
    Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_OCTET_STRING, entry->GetDate());
    Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_UINT8, entry->GetDayId());
    data = bb;
    return client->Method(this, 1, data, DLMS_DATA_TYPE_ARRAY, reply);
}

int DLMSSpecialDaysTable::Delete(DLMSClient* client, DLMSSpecialDay* entry, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data = entry->GetIndex();
    data.vt = DLMS_DATA_TYPE_UINT16;
    return client->Method(this, 2, data, reply);
}


void DLMSSpecialDaysTable::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln, str;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSSpecialDay*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
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

void DLMSSpecialDaysTable::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
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

int DLMSSpecialDaysTable::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    //Entries
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int DLMSSpecialDaysTable::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetByteArray(true);
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        Helpers::SetObjectCount((unsigned long)m_Entries.size(), data);
        int ret;
        DLMSVariant index, date, id;
        for (std::vector<DLMSSpecialDay*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3); //Count
            index = (*it)->GetIndex();
            date = (*it)->GetDate();
            id = (*it)->GetDayId();
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, index)) != DLMS_ERROR_CODE_OK ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, date)) != DLMS_ERROR_CODE_OK ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, id)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSSpecialDaysTable::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
    else if (e.GetIndex() == 2)
    {
        for (std::vector<DLMSSpecialDay*>::iterator it = m_Entries.begin(); it != m_Entries.end(); ++it)
        {
            delete *it;
        }
        m_Entries.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            DLMSVariant tmp;
            for (std::vector<DLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
            {
                DLMSSpecialDay *it = new DLMSSpecialDay();
                it->SetIndex((*item).Arr[0].ToInteger());
                DLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_DATE, tmp);
                it->SetDate(tmp.dateTime);
                it->SetDayId((*item).Arr[2].ToInteger());
                m_Entries.push_back(it);
            }
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_SPECIAL_DAYS_TABLE
