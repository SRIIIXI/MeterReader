#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSSapAssignment.h"
#include <sstream>

#ifndef DLMS_IGNORE_SAP_ASSIGNMENT
DLMSSapAssignment::DLMSSapAssignment() :
    DLMSSapAssignment("0.0.41.0.0.255", 0)
{
}


DLMSSapAssignment::DLMSSapAssignment(std::string ln) :
    DLMSSapAssignment(ln, 0)
{
}


DLMSSapAssignment::DLMSSapAssignment(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SAP_ASSIGNMENT, ln, sn)
{

}

std::map<int, std::string >& DLMSSapAssignment::GetSapAssignmentList()
{
    return m_SapAssignmentList;
}
void DLMSSapAssignment::SetSapAssignmentList(std::map<int, std::string >& value)
{
    m_SapAssignmentList = value;
}

// Returns amount of attributes.
int DLMSSapAssignment::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSSapAssignment::GetMethodCount()
{
    return 1;
}

void DLMSSapAssignment::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::map<int, std::string >::iterator it = m_SapAssignmentList.begin(); it != m_SapAssignmentList.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = DLMSVariant((it->first)).ToString();
        sb.write(str.c_str(), str.size());
        sb << ", ";
        sb.write(it->second.c_str(), it->second.size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void DLMSSapAssignment::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //SapAssignmentList
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSSapAssignment::GetDataType(int index, DLMS_DATA_TYPE& type)
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
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}


// Returns value of given attribute.
int DLMSSapAssignment::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        unsigned long cnt = (unsigned long)m_SapAssignmentList.size();
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        Helpers::SetObjectCount(cnt, data);
        int ret;
        if (cnt != 0)
        {
            DLMSVariant f, s;
            ByteBuffer bb;
            for (std::map<int, std::string >::iterator it = m_SapAssignmentList.begin();
                it != m_SapAssignmentList.end(); ++it)
            {
                data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
                data.SetUInt8(2); //Count
                f = it->first;
                bb.Clear();
                bb.AddString(it->second);
                s = bb;
                if ((ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_UINT16, f)) != 0 ||
                    (ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_OCTET_STRING, s)) != 0)
                {
                    return ret;
                }
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
int DLMSSapAssignment::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    if (e.GetIndex() == 2)
    {
        m_SapAssignmentList.clear();
        for (std::vector<DLMSVariant>::iterator item = e.GetValue().Arr.begin();
            item != e.GetValue().Arr.end(); ++item)
        {
            std::string str;
            if ((*item).Arr[1].vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                DLMSVariant tmp;
                DLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_STRING, tmp);
                str = tmp.strVal;
            }
            else
            {
                str = (*item).Arr[1].ToString();
            }
            m_SapAssignmentList[(*item).Arr[0].ToInteger()] = str;
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int DLMSSapAssignment::AddSap(DLMSClient* client, uint16_t id, std::string& name, std::vector<ByteBuffer>& reply)
{
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    //Add structure size.
    bb.SetUInt8(2);
    Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_UINT16, id);
    Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_OCTET_STRING, name);
    DLMSVariant data = bb;
    return client->Method(this, 1, data, DLMS_DATA_TYPE_STRUCTURE, reply);
}


int DLMSSapAssignment::RemoveSap(DLMSClient* client, std::string& name, std::vector<ByteBuffer>& reply)
{
    ByteBuffer bb;
    bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    //Add structure size.
    bb.SetUInt8(2);
    Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_UINT16, 0);
    Helpers::SetData2(NULL, bb, DLMS_DATA_TYPE_OCTET_STRING, name);
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}


int DLMSSapAssignment::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        uint16_t id = e.GetParameters().Arr[0].ToInteger();
        std::string str;
        if (e.GetParameters().Arr[1].vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
           str.append(reinterpret_cast<char const*>(e.GetParameters().Arr[1].byteArr), e.GetParameters().Arr[1].GetSize());
        }
        else
        {
            str = e.GetParameters().Arr[1].ToString();
        }
        if (id == 0)
        {
            for (std::map<int, std::string >::iterator it = m_SapAssignmentList.begin(); it != m_SapAssignmentList.end(); ++it)
            {
                if (it->second.compare(str) == 0)
                {
                    m_SapAssignmentList.erase(it);
                    break;
                }
            }
        }
        else
        {
            m_SapAssignmentList[id] = str;
        }
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_SAP_ASSIGNMENT
