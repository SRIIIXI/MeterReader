#include "DLMSNtpSetup.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_NTP_SETUP

//Constructor.
DLMSNtpSetup::DLMSNtpSetup() :
    DLMSNtpSetup("0.0.25.10.0.255", 0)
{
}

//SN Constructor.
DLMSNtpSetup::DLMSNtpSetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_NTP_SETUP, ln, sn)
{

}

//LN Constructor.
DLMSNtpSetup::DLMSNtpSetup(std::string ln) :
    DLMSNtpSetup(ln, 0)
{

}

bool DLMSNtpSetup::GetActivated()
{
    return m_Activated;
}

void DLMSNtpSetup::SetActivated(bool value)
{
    m_Activated = value;
}

std::string& DLMSNtpSetup::GetServerAddress()
{
    return m_ServerAddress;
}

void DLMSNtpSetup::SetServerAddress(std::string& value)
{
    m_ServerAddress = value;
}

uint16_t DLMSNtpSetup::GetPort()
{
    return m_Port;
}

void DLMSNtpSetup::SetPort(uint16_t value)
{
    m_Port = value;
}

DLMS_NTP_AUTHENTICATION_METHOD DLMSNtpSetup::GetAuthentication()
{
    return m_Authentication;
}

void DLMSNtpSetup::SetAuthentication(DLMS_NTP_AUTHENTICATION_METHOD value)
{
    m_Authentication = value;
}

std::map<uint32_t, ByteBuffer>& DLMSNtpSetup::GetKeys()
{
    return m_Keys;
}

ByteBuffer& DLMSNtpSetup::GetClientKey()
{
    return m_ClientKey;
}

void DLMSNtpSetup::SetClientKey(ByteBuffer& value)
{
    m_ClientKey = value;
}

// Returns amount of attributes.
int DLMSNtpSetup::GetAttributeCount()
{
    return 7;
}

// Returns amount of methods.
int DLMSNtpSetup::GetMethodCount()
{
    return 3;
}

void DLMSNtpSetup::GetValues(std::vector<std::string>& values)
{
    std::stringstream sb;
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_Activated));
    values.push_back(m_ServerAddress);
    values.push_back(Helpers::IntToString(m_Port));
    values.push_back(Helpers::IntToString(m_Authentication));
    bool empty = true;
    sb << '[';
    for (std::map<uint32_t, ByteBuffer>::iterator it = m_Keys.begin(); it != m_Keys.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        else
        {
            empty = false;
        }
        sb << '{';
        sb << it->first;
        sb << ", ";        
        sb << it->second.ToHexString();
        sb << '}';
    }
    sb << ']';
    values.push_back(m_ClientKey.ToString());
}

void DLMSNtpSetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Activated
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // ServerAddress
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // Port
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // Authentication
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // Keys
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
    // ClientKey
    if (all || CanRead(7)) {
        attributes.push_back(7);
    }
}

int DLMSNtpSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_BOOLEAN;
        break;
    case 3:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 4:
        type = DLMS_DATA_TYPE_UINT16;
        break;
    case 5:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 6:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 7:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSNtpSetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
    {
        DLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) == 0)
        {
            e.SetValue(tmp);
        }
        break;
    }
    case 2:
        e.SetValue(m_Activated);
        break;
    case 3:
        e.SetValue(m_ServerAddress);
        break;
    case 4:
        e.SetValue(m_Port);
        break;
    case 5:
        e.SetValue(m_Authentication);
        break;
    case 6:
    {
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_Keys.size(), data);
        for (std::map<uint32_t, ByteBuffer>::iterator it = m_Keys.begin(); it != m_Keys.end(); ++it)
        {
            if ((ret = data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) != 0 ||
                (ret = data.SetUInt8(2)) != 0 ||
                (ret = data.SetUInt8(DLMS_DATA_TYPE_UINT32)) != 0 ||
                (ret = data.SetUInt32(it->first)) != 0 ||
                (ret = data.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) != 0 ||
                (ret = Helpers::SetObjectCount(it->second.GetSize(), data)) != 0 ||
                (ret = data.Set(it->second.GetData(), it->second.GetSize())) != 0)
            {
                break;
            }
        }
        e.SetValue(data);
        break;
    }
    case 7:
        e.SetValue(m_ClientKey);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSNtpSetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = DLMS_ERROR_CODE_OK;
    switch (e.GetIndex())
    {
    case 1:
        ret = SetLogicalName(this, e.GetValue());
        break;
    case 2:
        m_Activated = e.GetValue().boolVal;
        break;
    case 3:
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_ServerAddress = e.GetValue().ToString();
        }
        else
        {
            DLMSVariant tmp;
            if ((ret = DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            m_ServerAddress = tmp.ToString();
        }
        break;
    case 4:
        m_Port = e.GetValue().ToInteger();
        break;
    case 5:
        m_Authentication = (DLMS_NTP_AUTHENTICATION_METHOD)e.GetValue().ToInteger();
        break;
    case 6:
    {
        m_Keys.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            ByteBuffer tmp;
            if ((ret = tmp.Set(it->Arr.at(1).byteArr, it->Arr.at(1).size)) != 0)
            {
                break;
            }
            m_Keys[it->Arr.at(0).ulVal] = tmp;
        }
        break;
    }
    case 7:
        e.GetValue().GetBytes(m_ClientKey);
        break;
    default:
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

int DLMSNtpSetup::Synchronize(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, DLMS_DATA_TYPE_ARRAY, reply);
}

int DLMSNtpSetup::AddAuthenticationKey(DLMSClient* client, uint32_t id, ByteBuffer& key, std::vector<ByteBuffer>& reply)
{
    int ret;
    ByteBuffer bb;
    if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 ||
        (ret = bb.SetUInt8(2)) == 0 ||
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_UINT32)) == 0 ||
        (ret = bb.SetUInt32(id)) == 0 ||
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) == 0 ||
        (ret = Helpers::SetObjectCount(key.GetSize(), bb)) == 0 ||
        (ret = bb.Set(key.GetData(), key.GetSize())) == 0)
    {
        DLMSVariant tmp = bb;
        ret = client->Method(this, 2, tmp, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int DLMSNtpSetup::DeleteAuthenticationKey(DLMSClient* client, uint32_t id, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data = id;
    return client->Method(this, 3, data, DLMS_DATA_TYPE_ARRAY, reply);
}

int DLMSNtpSetup::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        //Server must handle this. Do nothing...
    }
    else if (e.GetIndex() == 2)
    {
        ByteBuffer bb;
        if (e.GetParameters().vt != DLMS_DATA_TYPE_STRUCTURE)
        {
            return DLMS_ERROR_CODE_INCONSISTENT_CLASS_OR_OBJECT;
        }
        bb.Set(e.GetParameters().Arr[1].byteArr, e.GetParameters().Arr[1].GetSize());
        m_Keys[e.GetParameters().Arr[0].ulVal] = bb;
    }
    else if (e.GetIndex() == 3)
    {
        m_Keys.erase(e.GetParameters().ulVal);
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_NTP_SETUP
