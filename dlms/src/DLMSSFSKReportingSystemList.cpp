#include "DLMSSFSKReportingSystemList.h"

#ifndef DLMS_IGNORE_SFSK_REPORTING_SYSTEM_LIST
//Constructor.
DLMSSFSKReportingSystemList::DLMSSFSKReportingSystemList() :
    DLMSSFSKReportingSystemList("0.0.26.6.0.255", 0)
{
}

//SN Constructor.
DLMSSFSKReportingSystemList::DLMSSFSKReportingSystemList(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SFSK_REPORTING_SYSTEM_LIST, ln, sn)
{

}

//LN Constructor.
DLMSSFSKReportingSystemList::DLMSSFSKReportingSystemList(std::string ln) :
    DLMSSFSKReportingSystemList(ln, 0)
{

}

// Returns amount of attributes.
int DLMSSFSKReportingSystemList::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSSFSKReportingSystemList::GetMethodCount()
{
    return 0;
}

void DLMSSFSKReportingSystemList::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<ByteBuffer>::iterator it = m_ReportingSystemList.begin(); it != m_ReportingSystemList.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        else
        {
            empty = false;
        }
        sb << it->ToHexString(false);
    }
    sb << ']';
    values.push_back(sb.str());
}

void DLMSSFSKReportingSystemList::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ReportingSystemList
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSSFSKReportingSystemList::GetDataType(int index, DLMS_DATA_TYPE& type)
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
int DLMSSFSKReportingSystemList::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    if (e.GetIndex() == 1)
    {
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) == 0)
        {
            e.SetValue(tmp);
        }
    }
    else if (e.GetIndex() == 2)
    {
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        // Add count
        Helpers::SetObjectCount((unsigned long)m_ReportingSystemList.size(), data);
        for (std::vector<ByteBuffer>::iterator it = m_ReportingSystemList.begin(); it != m_ReportingSystemList.end(); ++it)
        {
            if ((ret = Helpers::SetData2(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, *it)) != 0)
            {
                break;
            }
        }
        e.SetValue(data);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSSFSKReportingSystemList::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
    if (e.GetIndex() == 1)
    {
        ret = SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ReportingSystemList.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            ByteBuffer bb;
            for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                if ((ret = it->GetBytes(bb)) != 0)
                {
                    break;
                }
                m_ReportingSystemList.push_back(bb);
            }
        }
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

#endif //DLMS_IGNORE_SFSK_REPORTING_SYSTEM_LIST
