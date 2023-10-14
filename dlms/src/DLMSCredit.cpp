#include "DLMSCredit.h"
#include "BitString.h"
#include "DLMSClient.h"

#ifndef DLMS_IGNORE_CREDIT
//Constructor.
DLMSCredit::DLMSCredit() :
    DLMSCredit("", 0)
{
}

//SN Constructor.
DLMSCredit::DLMSCredit(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_CREDIT, ln, sn)
{

}

//LN Constructor.
DLMSCredit::DLMSCredit(std::string ln) :
    DLMSCredit(ln, 0)
{

}

// Returns amount of attributes.
int DLMSCredit::GetAttributeCount()
{
    return 11;
}

// Returns amount of methods.
int DLMSCredit::GetMethodCount()
{
    return 3;
}

void DLMSCredit::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_CurrentCreditAmount));
    values.push_back(Helpers::IntToString(m_Type));
    values.push_back(Helpers::IntToString(m_Priority));
    values.push_back(Helpers::IntToString(m_WarningThreshold));
    values.push_back(Helpers::IntToString(m_Limit));
    values.push_back(Helpers::IntToString(m_CreditConfiguration));
    values.push_back(Helpers::IntToString(m_Status));
    values.push_back(Helpers::IntToString(m_PresetCreditAmount));
    values.push_back(Helpers::IntToString(m_CreditAvailableThreshold));
    values.push_back(m_Period.ToString());
}

int DLMSCredit::UpdateAmount(DLMSClient* client, uint32_t value, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data(value);
    return client->Method(this, 1, data, reply);
}

int DLMSCredit::SetAmountToValue(DLMSClient* client, uint32_t value, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data(value);
    return client->Method(this, 2, data, reply);
}

int DLMSCredit::InvokeCredit(DLMSClient* client, DLMS_CREDIT_STATUS value, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data(value);
    return client->Method(this, 3, data, reply);
}

int DLMSCredit::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_CurrentCreditAmount += e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 2)
    {
        m_CurrentCreditAmount = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        if ((m_CreditConfiguration & DLMS_CREDIT_CONFIGURATION_CONFIRMATION) != 0 && m_Status == DLMS_CREDIT_STATUS_SELECTABLE)
        {
            m_Status = DLMS_CREDIT_STATUS_INVOKED;
        }
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}

void DLMSCredit::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    // CurrentCreditAmount
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    // Type
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    // Priority
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    // WarningThreshold
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    // Limit
    if (all || CanRead(6))
    {
        attributes.push_back(6);
    }
    // creditConfiguration
    if (all || CanRead(7))
    {
        attributes.push_back(7);
    }
    // Status
    if (all || CanRead(8))
    {
        attributes.push_back(8);
    }
    // PresetCreditAmount
    if (all || CanRead(9))
    {
        attributes.push_back(9);
    }
    // CreditAvailableThreshold
    if (all || CanRead(10))
    {
        attributes.push_back(10);
    }
    // Period
    if (all || CanRead(11))
    {
        attributes.push_back(11);
    }
}

int DLMSCredit::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    switch (index)
    {
    case 1:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    case 2:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 3:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 4:
        type = DLMS_DATA_TYPE_UINT8;
        break;
    case 5:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 6:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 7:
        type = DLMS_DATA_TYPE_BIT_STRING;
        break;
    case 8:
        type = DLMS_DATA_TYPE_ENUM;
        break;
    case 9:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 10:
        type = DLMS_DATA_TYPE_INT32;
        break;
    case 11:
        type = DLMS_DATA_TYPE_OCTET_STRING;
        break;
    default:
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSCredit::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    DLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        break;
    case 2:
        e.SetValue(m_CurrentCreditAmount);
        break;
    case 3:
        e.SetValue(m_Type);
        break;
    case 4:
        e.SetValue(m_Priority);
        break;
    case 5:
        e.SetValue(m_WarningThreshold);
        break;
    case 6:
        e.SetValue(m_Limit);
        break;
    case 7:
        e.SetValue(BitString::ToBitString(m_CreditConfiguration, 5));
        break;
    case 8:
        e.SetValue(m_Status);
        break;
    case 9:
        e.SetValue(m_PresetCreditAmount);
        break;
    case 10:
        e.SetValue(m_CreditAvailableThreshold);
        break;
    case 11:
        e.SetValue(m_Period);
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSCredit::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret;
    ByteBuffer bb;
    DLMSVariant tmp;
    switch (e.GetIndex())
    {
    case 1:
        return SetLogicalName(this, e.GetValue());
    case 2:
        m_CurrentCreditAmount = e.GetValue().ToInteger();
        break;
    case 3:
        m_Type = (DLMS_CREDIT_TYPE)e.GetValue().ToInteger();
        break;
    case 4:
        m_Priority = e.GetValue().ToInteger();
        break;
    case 5:
        m_WarningThreshold = e.GetValue().ToInteger();
        break;
    case 6:
        m_Limit = e.GetValue().ToInteger();
        break;
    case 7:
        m_CreditConfiguration = (DLMS_CREDIT_CONFIGURATION)e.GetValue().ToInteger();
        break;
    case 8:
        m_Status = (DLMS_CREDIT_STATUS)e.GetValue().ToInteger();
        break;
    case 9:
        m_PresetCreditAmount = e.GetValue().ToInteger();
        break;
    case 10:
        m_CreditAvailableThreshold = e.GetValue().ToInteger();
        break;
    case 11:
        if (e.GetValue().vt == 0)
        {
            return DLMS_ERROR_CODE_READ_WRITE_DENIED;
        }
        else
        {
            if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                e.GetValue().GetBytes(bb);
                DataInfo info;
                info.SetType(DLMS_DATA_TYPE_DATETIME);
                if ((ret = Helpers::GetData(&settings, bb, info, tmp)) != 0)
                {
                    return ret;
                }
                m_Period = tmp.dateTime;
            }
            else
            {
                m_Period = e.GetValue().dateTime;
            }
        }
        break;
    default:
        return DLMS_ERROR_CODE_READ_WRITE_DENIED;
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_CREDIT
