#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSModemConfiguration.h"
#include "DLMSConverter.h"
#include <sstream>

#ifndef DLMS_IGNORE_MODEM_CONFIGURATION
DLMSModemConfiguration::DLMSModemConfiguration() :
    DLMSModemConfiguration("0.0.2.0.0.255", 0)
{
}

DLMSModemConfiguration::DLMSModemConfiguration(std::string ln) :
    DLMSModemConfiguration(ln, 0)
{
}

DLMSModemConfiguration::DLMSModemConfiguration(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_MODEM_CONFIGURATION, ln, sn)
{
    m_CommunicationSpeed = DLMS_BAUD_RATE_9600;
    m_ModemProfile.push_back("OK");
    m_ModemProfile.push_back("CONNECT");
    m_ModemProfile.push_back("RING");
    m_ModemProfile.push_back("NO CARRIER");
    m_ModemProfile.push_back("ERROR");
    m_ModemProfile.push_back("CONNECT 1200");
    m_ModemProfile.push_back("NO DIAL TONE");
    m_ModemProfile.push_back("BUSY");
    m_ModemProfile.push_back("NO ANSWER");
    m_ModemProfile.push_back("CONNECT 600");
    m_ModemProfile.push_back("CONNECT 2400");
    m_ModemProfile.push_back("CONNECT 4800");
    m_ModemProfile.push_back("CONNECT 9600");
    m_ModemProfile.push_back("CONNECT 14 400");
    m_ModemProfile.push_back("CONNECT 28 800");
    m_ModemProfile.push_back("CONNECT 33 600");
    m_ModemProfile.push_back("CONNECT 56 000");
}

DLMS_BAUD_RATE DLMSModemConfiguration::GetCommunicationSpeed()
{
    return m_CommunicationSpeed;
}
void DLMSModemConfiguration::SetCommunicationSpeed(DLMS_BAUD_RATE value)
{
    m_CommunicationSpeed = value;
}

std::vector<DLMSModemInitialisation>& DLMSModemConfiguration::GetInitialisationStrings()
{
    return m_InitialisationStrings;
}
void DLMSModemConfiguration::SetInitialisationStrings(std::vector<DLMSModemInitialisation>& value)
{
    m_InitialisationStrings = value;
}

std::vector< std::string >& DLMSModemConfiguration::GetModemProfile()
{
    return m_ModemProfile;
}

void DLMSModemConfiguration::SetModemProfile(std::vector< std::string >& value)
{
    m_ModemProfile = value;
}

// Returns amount of attributes.
int DLMSModemConfiguration::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int DLMSModemConfiguration::GetMethodCount()
{
    return 0;
}

void DLMSModemConfiguration::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSConverter::ToString(m_CommunicationSpeed));
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (std::vector<DLMSModemInitialisation>::iterator it = m_InitialisationStrings.begin(); it != m_InitialisationStrings.end(); ++it)
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
    Helpers::Join(", ", m_ModemProfile, ln);
    sb << ln;
    sb << ']';
    values.push_back(sb.str());

}

void DLMSModemConfiguration::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //CommunicationSpeed
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    //InitialisationStrings
    if (all || !IsRead(3))
    {
        attributes.push_back(3);
    }
    //ModemProfile
    if (all || !IsRead(4))
    {
        attributes.push_back(4);
    }
}

int DLMSModemConfiguration::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 4)
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
int DLMSModemConfiguration::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(m_CommunicationSpeed);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        int ret;
        unsigned long cnt = (unsigned long)m_InitialisationStrings.size();
        Helpers::SetObjectCount(cnt, data);
        DLMSVariant request, response, delay;
        for (std::vector<DLMSModemInitialisation>::iterator it = m_InitialisationStrings.begin();
            it != m_InitialisationStrings.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3); //Count
            request = it->GetRequest();
            response = it->GetResponse();
            delay = it->GetDelay();
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, request)) != 0 ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, response)) != 0 ||
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, delay)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        int ret;
        unsigned long cnt = (unsigned long)m_ModemProfile.size();
        Helpers::SetObjectCount(cnt, data);
        DLMSVariant tmp;
        for (std::vector< std::string >::iterator it = m_ModemProfile.begin();
            it != m_ModemProfile.end(); ++it)
        {
            tmp = *it;
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
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
int DLMSModemConfiguration::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_CommunicationSpeed = (DLMS_BAUD_RATE)e.GetValue().bVal;
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        m_InitialisationStrings.clear();
        int ret;
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            DLMSModemInitialisation item;
            DLMSVariant tmp;
            if ((ret = DLMSClient::ChangeType(it->Arr[0], DLMS_DATA_TYPE_STRING, tmp)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            item.SetRequest(tmp.ToString());
            if ((ret = DLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_STRING, tmp)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            item.SetResponse(tmp.ToString());
            if (it->Arr.size() > 2)
            {
                item.SetDelay(it->Arr[2].uiVal);
            }
            m_InitialisationStrings.push_back(item);
        }
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        m_ModemProfile.clear();
        int ret;
        for (std::vector<DLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
        {
            DLMSVariant tmp;
            if ((ret = DLMSClient::ChangeType(*it, DLMS_DATA_TYPE_STRING, tmp)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
            m_ModemProfile.push_back(tmp.ToString());
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
#endif //DLMS_IGNORE_MODEM_CONFIGURATION
