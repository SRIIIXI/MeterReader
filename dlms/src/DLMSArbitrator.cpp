#include "DLMSArbitrator.h"

#ifndef DLMS_IGNORE_ARBITRATOR
//Constructor.
DLMSArbitrator::DLMSArbitrator() :
    DLMSArbitrator("", 0)
{
}

//SN Constructor.
DLMSArbitrator::DLMSArbitrator(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_ARBITRATOR, ln, sn)
{

}

//LN Constructor.
DLMSArbitrator::DLMSArbitrator(std::string ln) :
    DLMSArbitrator(ln, 0)
{

}

// Returns amount of attributes.
int DLMSArbitrator::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int DLMSArbitrator::GetMethodCount()
{
    return 2;
}

void DLMSArbitrator::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << "{";
    bool empty = true;
    for (std::vector<DLMSActionItem>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
    {
        if (empty)
        {
            empty = false;
        }
        else
        {
            sb << ", ";
        }
        sb << "[";
        sb << it->GetLogicalName();
        sb << ", ";
        sb << it->GetScriptSelector();
        sb << "]";
    }
    sb << "}";
    values.push_back(sb.str());
    sb.clear();
    sb << "{";
    empty = true;
    for (std::vector<std::string>::iterator it = m_PermissionsTable.begin(); it != m_PermissionsTable.end(); ++it)
    {
        if (empty)
        {
            empty = false;
        }
        else
        {
            sb << ", ";
        }
        sb << *it;
    }
    sb << "}";
    values.push_back(sb.str());
    sb.clear();
    sb << "{";
    empty = true;
    for (std::vector<std::vector<uint16_t>>::iterator it = m_WeightingsTable.begin(); it != m_WeightingsTable.end(); ++it)
    {
        if (empty)
        {
            empty = false;
        }
        else
        {
            sb << ", ";
        }
        empty = true;
        for (std::vector<uint16_t>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
        {
            if (empty)
            {
                empty = false;
            }
            else
            {
                sb << ", ";
            }
            sb << *it2;
        }
    }
    sb << "}";
    values.push_back(sb.str());
    sb.clear();
    sb << "{";
    empty = true;
    for (std::vector<std::string>::iterator it = m_MostRecentRequestsTable.begin(); it != m_MostRecentRequestsTable.end(); ++it)
    {
        if (empty)
        {
            empty = false;
        }
        else
        {
            sb << ", ";
        }
    }
    sb << "}";
    values.push_back(sb.str());
    sb.clear();
    values.push_back(Helpers::IntToString(m_LastOutcome));
}

void DLMSArbitrator::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Actions
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //Permissions table
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //Weightings table
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //Most recent requests table
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //Last outcome
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
}

int DLMSArbitrator::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 6:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return 0;
}

// Returns value of given attribute.
int DLMSArbitrator::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    ByteBuffer data;
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
        unsigned char ln[6];
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_Actions.size(), data);
        for (std::vector<DLMSActionItem>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
        {
            DLMSVariant tmp(ln, 6, DLMS_DATA_TYPE_OCTET_STRING);
            if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
                //Count
                (ret = data.SetUInt8(2)) != 0 ||
                (ret = Helpers::SetLogicalName(it->GetLogicalName().c_str(), ln)) != 0 ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0 ||
                (ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_UINT16, it->GetScriptSelector())) != 0)
            {
                break;
            }
        }
        e.SetValue(data);
    }
    break;
    case 3:
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_PermissionsTable.size(), data);
        for (std::vector<std::string>::iterator it = m_PermissionsTable.begin(); it != m_PermissionsTable.end(); ++it)
        {
            if ((ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_BIT_STRING, *it)) != 0)
            {
                break;
            }
        }
        e.SetValue(data);
    }
    break;
    case 4:
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_WeightingsTable.size(), data);
        for (std::vector<std::vector<uint16_t>>::iterator it = m_WeightingsTable.begin(); it != m_WeightingsTable.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            Helpers::SetObjectCount((unsigned long)it->size(), data);
            for (std::vector<uint16_t>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
            {
                if ((ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_UINT16, *it2)) != 0)
                {
                    break;
                }
            }
        }
        e.SetValue(data);
    }
    break;
    case 5:
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_MostRecentRequestsTable.size(), data);
        for (std::vector<std::string>::iterator it = m_MostRecentRequestsTable.begin(); it != m_MostRecentRequestsTable.end(); ++it)
        {
            if ((ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_BIT_STRING, *it)) != 0)
            {
                break;
            }
        }
        e.SetValue(data);
    }
    break;
    case 6:
        e.SetValue(m_LastOutcome);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;

}

/// Create a new action.
int CreateAction(std::vector<DLMSVariant>& it, DLMSActionItem& item)
{
    std::string ln;
    Helpers::GetLogicalName(it[0].byteArr, ln);
    item.SetLogicalName(ln);
    item.SetScriptSelector(it[1].ToInteger());
    return 0;
}

// Set value of given attribute.
int DLMSArbitrator::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
    {
        m_Actions.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            DLMSActionItem item;
            if ((ret = CreateAction(it->Arr, item)) != 0)
            {
                break;
            }
            m_Actions.push_back(item);
        }
    }
    break;
    case 3:
    {
        m_PermissionsTable.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            m_PermissionsTable.push_back(it->ToString());
        }
    }
    break;
    case 4:
    {
        m_WeightingsTable.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            std::vector<uint16_t> list;
            for (std::vector<DLMSVariant>::iterator it2 = it->Arr.begin(); it2 != it->Arr.end(); ++it2)
            {
                list.push_back(it2->ToInteger());
            }
            m_WeightingsTable.push_back(list);
        }
    }
    break;
    case 5:
    {
        m_MostRecentRequestsTable.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            m_MostRecentRequestsTable.push_back(it->ToString());
        }
    }
    break;
    case 6:
        m_LastOutcome = e.GetValue().ToInteger();
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
        break;
    }
    return ret;
}

#endif //DLMS_IGNORE_ARBITRATOR
