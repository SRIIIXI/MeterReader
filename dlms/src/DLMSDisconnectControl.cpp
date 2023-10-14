#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSDisconnectControl.h"
#include "DLMSConverter.h"

#ifndef DLMS_IGNORE_DISCONNECT_CONTROL
//Constructor.
DLMSDisconnectControl::DLMSDisconnectControl() :
    DLMSDisconnectControl("", 0)
{
}

//SN Constructor.
DLMSDisconnectControl::DLMSDisconnectControl(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_DISCONNECT_CONTROL, ln, sn)
{

}

//LN Constructor.
DLMSDisconnectControl::DLMSDisconnectControl(std::string ln) :
    DLMSDisconnectControl(ln, 0)
{

}

/**
 Output state of COSEM Disconnect Control object.
*/
bool DLMSDisconnectControl::GetOutputState()
{
    return m_OutputState;
}
void DLMSDisconnectControl::SetOutputState(bool value)
{
    m_OutputState = value;
}

/**
 Control state of COSEM Disconnect Control object.
*/
DLMS_CONTROL_STATE DLMSDisconnectControl::GetControlState()
{
    return m_ControlState;
}
void DLMSDisconnectControl::SetControlState(DLMS_CONTROL_STATE value)
{
    m_ControlState = value;
}

/**
Control mode of COSEM Disconnect Control object.
*/
DLMS_CONTROL_MODE DLMSDisconnectControl::GetControlMode()
{
    return m_ControlMode;
}
void DLMSDisconnectControl::SetControlMode(DLMS_CONTROL_MODE value)
{
    m_ControlMode = value;
}

int DLMSDisconnectControl::RemoteDisconnect(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 1, data, reply);
}

int DLMSDisconnectControl::RemoteReconnect(DLMSClient* client, std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)0);
    return client->Method(this, 2, data, reply);
}

// Returns amount of attributes.
int DLMSDisconnectControl::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int DLMSDisconnectControl::GetMethodCount()
{
    return 2;
}

void DLMSDisconnectControl::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSVariant(m_OutputState).ToString());
    values.push_back(DLMSConverter::ToString(m_ControlState));
    values.push_back(DLMSConverter::ToString(m_ControlMode));
}

void DLMSDisconnectControl::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //OutputState
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //ControlState
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //ControlMode
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
}

int DLMSDisconnectControl::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_BOOLEAN;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int DLMSDisconnectControl::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetValue(m_OutputState);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(m_ControlState);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        e.SetValue(m_ControlMode);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int DLMSDisconnectControl::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_OutputState = e.GetValue().boolVal;
    }
    else if (e.GetIndex() == 3)
    {
        m_ControlState = (DLMS_CONTROL_STATE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_ControlMode = (DLMS_CONTROL_MODE)e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //DLMS_IGNORE_DISCONNECT_CONTROL
