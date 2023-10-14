#include "DLMSTokenGateway.h"

#ifndef DLMS_IGNORE_TOKEN_GATEWAY
//Constructor.
DLMSTokenGateway::DLMSTokenGateway() :
    DLMSTokenGateway("", 0)
{
}

//SN Constructor.
DLMSTokenGateway::DLMSTokenGateway(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_TOKEN_GATEWAY, ln, sn)
{

}

//LN Constructor.
DLMSTokenGateway::DLMSTokenGateway(std::string ln) :
    DLMSTokenGateway(ln, 0)
{

}

// Returns amount of attributes.
int DLMSTokenGateway::GetAttributeCount()
{
    return 6;
}

// Returns amount of methods.
int DLMSTokenGateway::GetMethodCount()
{
    return 1;
}

void DLMSTokenGateway::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string tmp;
    GetLogicalName(tmp);
    values.push_back(tmp);
    values.push_back(m_Token.ToString());
    values.push_back(m_Time.ToString());
    tmp = "";
    for (std::vector<std::string>::iterator it = m_Descriptions.begin(); it != m_Descriptions.end(); ++it)
    {
        tmp += *it;
    }
    values.push_back(tmp);
    values.push_back(Helpers::IntToString(m_DeliveryMethod));
    values.push_back(Helpers::IntToString(m_Status) + ", " + m_DataValue);
}

void DLMSTokenGateway::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // Token
    if (all || CanRead(2)) {
        attributes.push_back(2);
    }
    // Time
    if (all || CanRead(3)) {
        attributes.push_back(3);
    }
    // Description
    if (all || CanRead(4)) {
        attributes.push_back(4);
    }
    // DeliveryMethod
    if (all || CanRead(5)) {
        attributes.push_back(5);
    }
    // Status
    if (all || CanRead(6)) {
        attributes.push_back(6);
    }
}

int DLMSTokenGateway::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 3:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 4:
        type = DLMS_DATA_TYPE_ARRAY;
        break;
    case 5:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 6:
        type = DLMS_DATA_TYPE_STRUCTURE;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSTokenGateway::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    ByteBuffer bb;
    int ret;
    DLMSVariant tmp;
    switch (e.GetIndex()) {
    case 1:
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        break;
    case 2:
        e.SetValue(m_Token);
        break;
    case 3:
        e.SetValue(m_Time);
        break;
    case 4:
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        bb.SetUInt8((unsigned char)m_Descriptions.size());
        for (std::vector<std::string>::iterator it = m_Descriptions.begin(); it != m_Descriptions.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING);
            bb.SetUInt8((unsigned char)it->length());
            bb.Set(it->c_str(), (unsigned long)it->length());
        }
        e.SetValue(bb);
        break;
    case 5:
        e.SetValue(m_DeliveryMethod);
        break;
    case 6:
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(2);
        tmp = m_Status;
        if ((ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_ENUM, tmp)) != 0)
        {
            return ret;
        }
        tmp = m_DataValue;
        if ((ret = Helpers::SetData(&settings, bb, DLMS_DATA_TYPE_BIT_STRING, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(bb);
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSTokenGateway::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    ByteBuffer bb;
    DLMSVariant tmp;
    std::string str;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
        m_Token.Clear();
        m_Token.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        break;
    case 3:
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            bb.Clear();
            e.GetValue().GetBytes(bb);
            DataInfo info;
            info.SetType(DLMS_DATA_TYPE_DATETIME);
            if ((ret = Helpers::GetData(&settings, bb, info, tmp)) != 0)
            {
                return ret;
            }
            m_Time = tmp.dateTime;
        }
        else
        {
            m_Time = e.GetValue().dateTime;
        }
        break;
    case 4:
        m_Descriptions.clear();
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            str.clear();
            str.append((const char*)it->byteArr, it->GetSize());
            m_Descriptions.push_back(str);
        }
        break;
    case 5:
        m_DeliveryMethod = (DLMS_TOKEN_DELIVERY)e.GetValue().ToInteger();
        break;
    case 6:
        m_Status = (DLMS_TOKEN_STATUS_CODE)e.GetValue().Arr[0].ToInteger();
        m_DataValue = e.GetValue().Arr[1].strVal;
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
        break;
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_TOKEN_GATEWAY
