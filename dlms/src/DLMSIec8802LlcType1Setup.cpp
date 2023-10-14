#include "DLMSIec8802LlcType1Setup.h"

#ifndef DLMS_IGNORE_IEC_8802_LLC_TYPE1_SETUP
//Constructor.
DLMSIec8802LlcType1Setup::DLMSIec8802LlcType1Setup() :
    DLMSIec8802LlcType1Setup("0.0.27.0.0.255", 0)
{
}

//SN Constructor.
DLMSIec8802LlcType1Setup::DLMSIec8802LlcType1Setup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE1_SETUP, ln, sn)
{

}

//LN Constructor.
DLMSIec8802LlcType1Setup::DLMSIec8802LlcType1Setup(std::string ln) :
    DLMSIec8802LlcType1Setup(ln, 0)
{

}

uint16_t DLMSIec8802LlcType1Setup::GetMaximumOctetsUiPdu()
{
    return m_MaximumOctetsUiPdu;
}

void DLMSIec8802LlcType1Setup::SetMaximumOctetsUiPdu(uint16_t value)
{
    m_MaximumOctetsUiPdu = 128;
}

// Returns amount of attributes.
int DLMSIec8802LlcType1Setup::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int DLMSIec8802LlcType1Setup::GetMethodCount()
{
    return 0;
}

void DLMSIec8802LlcType1Setup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(Helpers::IntToString(m_MaximumOctetsUiPdu));
}

void DLMSIec8802LlcType1Setup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //MaximumOctetsUiPdu
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
}

int DLMSIec8802LlcType1Setup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSIec8802LlcType1Setup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    int ret = 0;
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
        e.SetValue(m_MaximumOctetsUiPdu);
    }
    else
    {
        ret = DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

// Set value of given attribute.
int DLMSIec8802LlcType1Setup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_MaximumOctetsUiPdu = e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_IEC_8802_LLC_TYPE1_SETUP
