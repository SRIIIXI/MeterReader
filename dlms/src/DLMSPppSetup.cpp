#include "DLMSPppSetup.h"
#include "DLMSClient.h"
#include <sstream>

#ifndef DLMS_IGNORE_PPP_SETUP
//Constructor.
DLMSPppSetup::DLMSPppSetup() :
    DLMSPppSetup("", 0)
{
    m_Authentication = PPP_AUTHENTICATION_TYPE_NONE;
}

//SN Constructor.
DLMSPppSetup::DLMSPppSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_PPP_SETUP, ln, sn)
{
    m_Authentication = PPP_AUTHENTICATION_TYPE_NONE;
}

//LN Constructor.
DLMSPppSetup::DLMSPppSetup(std::string ln) :
    DLMSPppSetup(ln, 0)
{
    m_Authentication = PPP_AUTHENTICATION_TYPE_NONE;
}

PPP_AUTHENTICATION_TYPE DLMSPppSetup::GetAuthentication()
{
    return m_Authentication;
}
void DLMSPppSetup::SetAuthentication(PPP_AUTHENTICATION_TYPE value)
{
    m_Authentication = value;
}

#ifdef UNICODE
ByteBuffer& DLMSPppSetup::GetUserNameW()
{
    return m_UserName;
}
#endif //UNICODE

ByteBuffer& DLMSPppSetup::GetUserNamePPP()
{
    return m_UserName;
}

void DLMSPppSetup::SetUserNamePPP(ByteBuffer& value)
{
    m_UserName = value;
}

/**
 PPP authentication procedure password.
*/

ByteBuffer& DLMSPppSetup::GetPasswordPPP()
{
    return m_Password;
}

void DLMSPppSetup::SetPasswordPPP(ByteBuffer value)
{
    m_Password = value;
}

std::string DLMSPppSetup::GetPHYReference()
{
    return m_PHYReference;
}
void DLMSPppSetup::SetPHYReference(std::string value)
{
    m_PHYReference = value;
}

std::vector<DLMSPppSetupLcpOption>& DLMSPppSetup::GetLCPOptions()
{
    return m_LCPOptions;
}


std::vector<DLMSPppSetupIPCPOption>& DLMSPppSetup::GetIPCPOptions()
{
    return m_IPCPOptions;
}

// Returns amount of attributes.
int DLMSPppSetup::GetAttributeCount()
{
    return 5;
}

// Returns amount of methods.
int DLMSPppSetup::GetMethodCount()
{
    return 0;
}

void DLMSPppSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_PHYReference);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSPppSetupLcpOption>::iterator it = m_LCPOptions.begin(); it != m_LCPOptions.end(); ++it)
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

    //Clear str.
    sb.str(std::string());
    sb << '[';
    empty = true;
    for (std::vector<DLMSPppSetupIPCPOption>::iterator it = m_IPCPOptions.begin(); it != m_IPCPOptions.end(); ++it)
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
    std::string str = m_UserName.ToString();
    str.append(" ");
    str.append(m_Password.ToString());
    values.push_back(str);
}

void DLMSPppSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //PHYReference
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //LCPOptions
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //IPCPOptions
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
    //PPPAuthentication
    if (all || !IsRead(5))
    {
        attributes.push_back(5);
    }
}

int DLMSPppSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 5)
    {
        if (m_UserName.GetSize() == 0)
        {
            type = DLMS_DATA_TYPE_NONE;
        }
        else
        {
            type = DLMS_DATA_TYPE_STRUCTURE;
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSPppSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        Helpers::SetLogicalName(m_PHYReference.c_str(), tmp);
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_LCPOptions.size(), data);
        DLMSVariant type, len;
        for (std::vector<DLMSPppSetupLcpOption>::iterator it = m_LCPOptions.begin(); it != m_LCPOptions.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            type = it->GetType();
            len = it->GetLength();
            DLMSVariant tmp = it->GetData();
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, type);
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, len);
            Helpers::SetData(&settings, data, it->GetData().vt, tmp);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_IPCPOptions.size(), data);
        DLMSVariant type, len;
        for (std::vector<DLMSPppSetupIPCPOption>::iterator it = m_IPCPOptions.begin(); it != m_IPCPOptions.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            type = it->GetType();
            len = it->GetLength();
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, type);
            Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, len);
            Helpers::SetData(&settings, data, it->GetData().vt, it->m_Data);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 5)
    {
        if (m_UserName.GetSize() != 0)
        {
            e.SetByteArray(true);
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            //Add username.
            data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            data.SetUInt8((unsigned char)m_UserName.GetSize());
            data.Set(&m_UserName, 0, -1);
            //Add password.
            data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            data.SetUInt8((unsigned char)m_Password.GetSize());
            data.Set(&m_Password, 0, -1);
            e.SetValue(data);
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSPppSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_PHYReference = e.GetValue().ToString();
        }
        else
        {
            Helpers::GetLogicalName(e.GetValue().byteArr, m_PHYReference);
        }
    }
    else if (e.GetIndex() == 3)
    {
        m_LCPOptions.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
            {
                DLMSPppSetupLcpOption it;
                it.SetType((PPP_SETUP_LCP_OPTION_TYPE)(*item).Arr[0].ToInteger());
                it.SetLength((*item).Arr[1].ToInteger());
                it.SetData((*item).Arr[2]);
                m_LCPOptions.push_back(it);
            }
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_IPCPOptions.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
            {
                DLMSPppSetupIPCPOption it;
                it.SetType((PPP_SETUP_IPCP_OPTION_TYPE)(*item).Arr[0].ToInteger());
                it.SetLength((*item).Arr[1].ToInteger());
                it.SetData((*item).Arr[2]);
                m_IPCPOptions.push_back(it);
            }
        }
    }
    else if (e.GetIndex() == 5)
    {
        m_UserName.Clear();
        m_Password.Clear();
        if (e.GetValue().Arr.size() == 2)
        {
            m_UserName.Set(e.GetValue().Arr[0].byteArr, e.GetValue().Arr[0].size);
            m_Password.Set(e.GetValue().Arr[1].byteArr, e.GetValue().Arr[1].size);
        }
        else if (e.GetValue().Arr.size() != 0)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_PPP_SETUP
