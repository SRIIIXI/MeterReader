#include "DLMSRegisterMonitor.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_REGISTER_MONITOR
DLMSRegisterMonitor::DLMSRegisterMonitor() :
    DLMSRegisterMonitor("", 0)
{
}


DLMSRegisterMonitor::DLMSRegisterMonitor(std::string ln) :
    DLMSRegisterMonitor(ln, 0)
{
}

DLMSRegisterMonitor::DLMSRegisterMonitor(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_REGISTER_MONITOR, ln, sn)
{
}

DLMSRegisterMonitor::~DLMSRegisterMonitor()
{
    for (std::vector<DLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
    {
        delete *it;
    }
    m_Actions.clear();
    m_Thresholds.clear();
}

std::vector<DLMSVariant>& DLMSRegisterMonitor::GetThresholds()
{
    return m_Thresholds;
}
void DLMSRegisterMonitor::SetThresholds(std::vector<DLMSVariant>& value)
{
    m_Thresholds = value;
}

DLMSMonitoredValue& DLMSRegisterMonitor::GetMonitoredValue()
{
    return m_MonitoredValue;
}
void DLMSRegisterMonitor::SetMonitoredValue(DLMSMonitoredValue& value)
{
    m_MonitoredValue = value;
}

std::vector<DLMSActionSet*>& DLMSRegisterMonitor::GetActions()
{
    return m_Actions;
}

// Returns amount of attributes.
int DLMSRegisterMonitor::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int DLMSRegisterMonitor::GetMethodCount()
{
    return 0;
}

void DLMSRegisterMonitor::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSVariant>::iterator it = m_Thresholds.begin(); it != m_Thresholds.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
    values.push_back(m_MonitoredValue.ToString());

    //Clear str.
    sb.str(std::string());
    sb << '[';
    empty = true;
    for (std::vector<DLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
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

void DLMSRegisterMonitor::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Thresholds
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //MonitoredValue
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //Actions
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int DLMSRegisterMonitor::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
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
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

/*
 * Returns value of given attribute.
 */
int DLMSRegisterMonitor::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    ByteBuffer data;
    DLMSVariant ln;
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
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        Helpers::SetObjectCount((unsigned long)m_Thresholds.size(), data);
        int ret;
        DLMSVariant tmp;
        for (std::vector<DLMSVariant>::iterator it = m_Thresholds.begin(); it != m_Thresholds.end(); ++it)
        {
            tmp = *it;
            if ((ret = Helpers::SetData(&settings, data, it->vt, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        int ret;
        Helpers::SetLogicalName(m_MonitoredValue.GetLogicalName().c_str(), ln);
        DLMSVariant type = m_MonitoredValue.GetObjectType();
        DLMSVariant index = m_MonitoredValue.GetAttributeIndex();
        if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, type)) != DLMS_ERROR_CODE_OK ||  //ClassID
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != DLMS_ERROR_CODE_OK || //LN
            (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_INT8, index)) != DLMS_ERROR_CODE_OK)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int ret;
        //Add count
        Helpers::SetObjectCount((unsigned long)m_Actions.size(), data);
        DLMSVariant selector;
        for (std::vector<DLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            Helpers::SetLogicalName((*it)->GetActionUp().GetLogicalName().c_str(), ln);
            selector = (*it)->GetActionUp().GetScriptSelector();
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != DLMS_ERROR_CODE_OK ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, selector)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            Helpers::SetLogicalName((*it)->GetActionDown().GetLogicalName().c_str(), ln);
            selector = (*it)->GetActionDown().GetScriptSelector();
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != DLMS_ERROR_CODE_OK ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, selector)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

/*
 * Set value of given attribute.
 */
int DLMSRegisterMonitor::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    if (e.GetIndex() == 2)
    {
        SetThresholds(e.GetValue().Arr);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        GetMonitoredValue().SetObjectType((DLMS_OBJECT_TYPE)e.GetValue().Arr[0].ToInteger());
        std::string ln;
        Helpers::GetLogicalName(e.GetValue().Arr[1].byteArr, ln);
        m_MonitoredValue.SetLogicalName(ln);
        m_MonitoredValue.SetAttributeIndex(e.GetValue().Arr[2].ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        for (std::vector<DLMSActionSet*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
        {
            delete *it;
        }
        m_Actions.clear();
        std::string ln;
        for (std::vector<DLMSVariant>::iterator action_set = e.GetValue().Arr.begin(); action_set != e.GetValue().Arr.end(); ++action_set)
        {
            DLMSActionSet *set = new DLMSActionSet();
            DLMSVariant& up = action_set->Arr[0];
            Helpers::GetLogicalName(up.Arr[0].byteArr, ln);
            set->GetActionUp().SetLogicalName(ln);
            set->GetActionUp().SetScriptSelector(up.Arr[1].ToInteger());
            DLMSVariant& down = action_set->Arr[1];
            Helpers::GetLogicalName(down.Arr[0].byteArr, ln);
            set->GetActionDown().SetLogicalName(ln);
            set->GetActionDown().SetScriptSelector(down.Arr[1].ToInteger());
            m_Actions.push_back(set);
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
#endif //DLMS_IGNORE_REGISTER_MONITOR
