#include "DLMSIp4Setup.h"
#include "DLMSClient.h"
#include <sstream>

#ifndef DLMS_IGNORE_IP4_SETUP
#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <Winsock.h> //Add support for sockets
#endif
#if defined(__linux__)//linux includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

//Constructor.
DLMSIp4Setup::DLMSIp4Setup() :
    DLMSIp4Setup("0.0.25.1.0.255", 0)
{
}

//SN Constructor.
DLMSIp4Setup::DLMSIp4Setup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_IP4_SETUP, ln, sn)
{
    m_IPAddress = 0;
    m_SubnetMask = 0;
    m_GatewayIPAddress = 0;
    m_UseDHCP = false;
    m_PrimaryDNSAddress = 0;
    m_SecondaryDNSAddress = 0;
}

//LN Constructor.
DLMSIp4Setup::DLMSIp4Setup(std::string ln) :
    DLMSIp4Setup(ln, 0)
{
}

std::string& DLMSIp4Setup::GetDataLinkLayerReference()
{
    return m_DataLinkLayerReference;
}
void DLMSIp4Setup::SetDataLinkLayerReference(std::string value)
{
    m_DataLinkLayerReference = value;
}

#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
unsigned int ToAddress(std::string& value)
{
    if (value.empty())
    {
        return 0;
    }
    struct sockaddr_in add;
    add.sin_addr.s_addr = inet_addr(value.c_str());
    //If address is give as name
    if (add.sin_addr.s_addr == INADDR_NONE)
    {
        struct hostent* Hostent = gethostbyname(value.c_str());
        if (Hostent == NULL)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        };
        add.sin_addr = *(in_addr*)(void*)Hostent->h_addr_list[0];
    };
    return add.sin_addr.s_addr;
}
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

void GetAddress(unsigned int value, std::string& add)
{
    ByteBuffer bb;
    bb.AddIntAsString((value >> 24) & 0xFF);
    bb.SetInt8('.');
    bb.AddIntAsString((value >> 16) & 0xFF);
    bb.SetInt8('.');
    bb.AddIntAsString((value >> 8) & 0xFF);
    bb.SetInt8('.');
    bb.AddIntAsString(value & 0xFF);
    add = bb.ToString();
}

unsigned int DLMSIp4Setup::GetIPAddress()
{
    return m_IPAddress;
}

void DLMSIp4Setup::SetIPAddress(unsigned int value)
{
    m_IPAddress = value;
}

void DLMSIp4Setup::GetIPAddress(std::string& value)
{
    GetAddress(m_IPAddress, value);
}

#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
void DLMSIp4Setup::SetIPAddress(std::string& value)
{
    m_IPAddress = ToAddress(value);
}
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

std::vector<unsigned int>& DLMSIp4Setup::GetMulticastIPAddress()
{
    return m_MulticastIPAddress;
}

std::vector<DLMSIp4SetupIpOption>& DLMSIp4Setup::GetIPOptions()
{
    return m_IPOptions;
}
void DLMSIp4Setup::SetIPOptions(std::vector<DLMSIp4SetupIpOption>& value)
{
    m_IPOptions.clear();
    for (std::vector<DLMSIp4SetupIpOption>::iterator it = value.begin(); it != value.end(); ++it)
    {
        m_IPOptions.push_back(*it);
    }
}

unsigned int DLMSIp4Setup::GetSubnetMask()
{
    return m_SubnetMask;
}

void DLMSIp4Setup::SetSubnetMask(unsigned int value)
{
    m_SubnetMask = value;
}

void DLMSIp4Setup::GetSubnetMask(std::string& value)
{
    GetAddress(m_SubnetMask, value);
}

#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
void DLMSIp4Setup::SetSubnetMask(std::string& value)
{
    m_SubnetMask = ToAddress(value);
}
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

unsigned int DLMSIp4Setup::GetGatewayIPAddress()
{
    return m_GatewayIPAddress;
}

void DLMSIp4Setup::SetGatewayIPAddress(unsigned int value)
{
    m_GatewayIPAddress = value;
}

void DLMSIp4Setup::GetGatewayIPAddress(std::string& value)
{
    GetAddress(m_GatewayIPAddress, value);
}

#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
void DLMSIp4Setup::SetGatewayIPAddress(std::string& value)
{
    m_GatewayIPAddress = ToAddress(value);
}
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

bool DLMSIp4Setup::GetUseDHCP()
{
    return m_UseDHCP;
}
void DLMSIp4Setup::SetUseDHCP(bool value)
{
    m_UseDHCP = value;
}

unsigned int DLMSIp4Setup::GetPrimaryDNSAddress()
{
    return m_PrimaryDNSAddress;
}

void DLMSIp4Setup::SetPrimaryDNSAddress(unsigned int value)
{
    m_PrimaryDNSAddress = value;
}
void DLMSIp4Setup::GetPrimaryDNSAddress(std::string& value)
{
    GetAddress(m_PrimaryDNSAddress, value);
}

#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
void DLMSIp4Setup::SetPrimaryDNSAddress(std::string& value)
{
    m_PrimaryDNSAddress = ToAddress(value);
}
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

unsigned int DLMSIp4Setup::GetSecondaryDNSAddress()
{
    return m_SecondaryDNSAddress;
}

void DLMSIp4Setup::SetSecondaryDNSAddress(unsigned int value)
{
    m_SecondaryDNSAddress = value;
}

void DLMSIp4Setup::GetSecondaryDNSAddress(std::string& value)
{
    GetAddress(m_SecondaryDNSAddress, value);
}

#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)

void DLMSIp4Setup::SetSecondaryDNSAddress(std::string& value)
{
    m_SecondaryDNSAddress = ToAddress(value);
}
#endif//defined(_WIN32) || defined(_WIN64) || defined(__linux__)

// Returns amount of attributes.
int DLMSIp4Setup::GetAttributeCount()
{
    return 10;
}

// Returns amount of methods.
int DLMSIp4Setup::GetMethodCount()
{
    return 3;
}

void DLMSIp4Setup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_DataLinkLayerReference);
    values.push_back(DLMSVariant(m_IPAddress).ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<unsigned int>::iterator it = m_MulticastIPAddress.begin(); it != m_MulticastIPAddress.end(); ++it)
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
    sb << '[';
    empty = true;
    for (std::vector<DLMSIp4SetupIpOption>::iterator it = m_IPOptions.begin(); it != m_IPOptions.end(); ++it)
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

    values.push_back(DLMSVariant(m_SubnetMask).ToString());
    values.push_back(DLMSVariant(m_GatewayIPAddress).ToString());
    values.push_back(DLMSVariant(m_UseDHCP).ToString());
    values.push_back(DLMSVariant(m_PrimaryDNSAddress).ToString());
    values.push_back(DLMSVariant(m_SecondaryDNSAddress).ToString());
}

void DLMSIp4Setup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //DataLinkLayerReference
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //IPAddress
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //MulticastIPAddress
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //IPOptions
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    //SubnetMask
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    //GatewayIPAddress
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    //UseDHCP
    if (all || !IsRead(8))
    {
        attributes.push_back(8);
    }
    //PrimaryDNSAddress
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    //SecondaryDNSAddress
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
}

int DLMSIp4Setup::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_BOOLEAN;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_UINT32;
    }
    else if (index == 10)
    {
        type = DLMS_DATA_TYPE_UINT32;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSIp4Setup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
    }
    else if (e.GetIndex() == 2)
    {
        DLMSVariant tmp;
        Helpers::SetLogicalName(m_DataLinkLayerReference.c_str(), tmp);
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 3)
    {
        e.SetValue((unsigned long)m_IPAddress);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_MulticastIPAddress.size(), data);
        int ret;
        DLMSVariant tmp;
        for (std::vector<unsigned int>::iterator it = m_MulticastIPAddress.begin(); it != m_MulticastIPAddress.end(); ++it)
        {
            tmp = *it;
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT32, tmp)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    else if (e.GetIndex() == 5)
    {
        e.SetByteArray(true);
        ByteBuffer bb;
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_IPOptions.size(), bb);
        int ret;
        DLMSVariant type, len, data;
        for (std::vector<DLMSIp4SetupIpOption>::iterator it = m_IPOptions.begin(); it != m_IPOptions.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            bb.SetUInt8(3);
            type = it->GetType();
            len = it->GetLength();
            data = it->GetData();
            if ((ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT8, type)) != 0 ||
                (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_UINT8, len)) != 0 ||
                (ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_OCTET_STRING, data)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(bb);
    }
    else if (e.GetIndex() == 6)
    {
        e.SetValue((unsigned long)m_SubnetMask);
    }
    else if (e.GetIndex() == 7)
    {
        e.SetValue((unsigned long)m_GatewayIPAddress);
    }
    else if (e.GetIndex() == 8)
    {
        e.SetValue(m_UseDHCP);
    }
    else if (e.GetIndex() == 9)
    {
        e.SetValue((unsigned long)m_PrimaryDNSAddress);
    }
    else if (e.GetIndex() == 10)
    {
        e.SetValue((unsigned long)m_SecondaryDNSAddress);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSIp4Setup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_DataLinkLayerReference = e.GetValue().ToString();
        }
        else
        {
            m_DataLinkLayerReference.clear();
            Helpers::GetLogicalName(e.GetValue().byteArr, m_DataLinkLayerReference);
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().vt != DLMS_DATA_TYPE_UINT32)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        m_IPAddress = e.GetValue().ulVal;
    }
    else if (e.GetIndex() == 4)
    {
        m_MulticastIPAddress.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                m_MulticastIPAddress.push_back((*it).ToInteger());
            }
        }
    }
    else if (e.GetIndex() == 5)
    {
        m_IPOptions.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                DLMSIp4SetupIpOption item;
                item.SetType((IP_OPTION_TYPE)it->Arr[0].ToInteger());
                item.SetLength(it->Arr[1].ToInteger());
                ByteBuffer tmp;
                tmp.Set(it->Arr[2].byteArr, it->Arr[2].size);
                item.SetData(tmp);
                m_IPOptions.push_back(item);
            }
        }
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetValue().vt != DLMS_DATA_TYPE_UINT32)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        m_SubnetMask = e.GetValue().ulVal;
    }
    else if (e.GetIndex() == 7)
    {
        if (e.GetValue().vt != DLMS_DATA_TYPE_UINT32)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        m_GatewayIPAddress = e.GetValue().ulVal;
    }
    else if (e.GetIndex() == 8)
    {
        m_UseDHCP = e.GetValue().boolVal;
    }
    else if (e.GetIndex() == 9)
    {
        if (e.GetValue().vt != DLMS_DATA_TYPE_UINT32)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        m_PrimaryDNSAddress = e.GetValue().ulVal;
    }
    else if (e.GetIndex() == 10)
    {
        if (e.GetValue().vt != DLMS_DATA_TYPE_UINT32)
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        m_SecondaryDNSAddress = e.GetValue().ulVal;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_IP4_SETUP
