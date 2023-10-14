#include "DLMSNotify.h"

void DLMSNotify::SetCipher(Cipher* value)
{
    m_Settings.SetCipher(value);
}

DLMSSettings& DLMSNotify::GetSettings()
{
    return m_Settings;
}

DLMSNotify::DLMSNotify(bool useLogicalNameReferencing,
    int clientAddress,
    int serverAddress,
    DLMS_INTERFACE_TYPE interfaceType) : m_Settings(true)
{
    m_Settings.SetUseLogicalNameReferencing(useLogicalNameReferencing);
    m_Settings.SetClientAddress(clientAddress);
    m_Settings.SetServerAddress(serverAddress);
    m_Settings.SetInterfaceType(interfaceType);
}

DLMSObjectCollection& DLMSNotify::GetObjects()
{
    return m_Settings.GetObjects();
}

DLMSLimits& DLMSNotify::GetLimits()
{
    return m_Settings.GetHdlcSettings();
}

HdlcSettings& DLMSNotify::GetHdlcSettings()
{
    return m_Settings.GetHdlcSettings();
}

int DLMSNotify::GetMaxPduSize()
{
    return m_Settings.GetMaxPduSize();
}

void DLMSNotify::SetMaxReceivePDUSize(int value)
{
    m_Settings.SetMaxReceivePDUSize(value);
}

bool DLMSNotify::GetUseLogicalNameReferencing()
{
    return m_Settings.GetUseLogicalNameReferencing();
}

void DLMSNotify::SetUseLogicalNameReferencing(bool value)
{
    m_Settings.SetUseLogicalNameReferencing(value);
}

DLMS_PRIORITY DLMSNotify::GetPriority()
{
    return m_Settings.GetPriority();
}

void DLMSNotify::SetPriority(DLMS_PRIORITY value)
{
    m_Settings.SetPriority(value);
}

DLMS_SERVICE_CLASS DLMSNotify::GetServiceClass()
{
    return m_Settings.GetServiceClass();
}

void DLMSNotify::SetServiceClass(DLMS_SERVICE_CLASS value)
{
    m_Settings.SetServiceClass(value);
}

unsigned char DLMSNotify::GetInvokeID()
{
    return m_Settings.GetInvokeID();
}

void DLMSNotify::SetInvokeID(unsigned char value)
{
    m_Settings.SetInvokeID(value);
}

int DLMSNotify::GetData(ByteBuffer& reply, ReplyData& data)
{
    return DLMS::GetData(m_Settings, reply, data, NULL);
}

int DLMSNotify::AddData(
    DLMSObject* obj,
    unsigned char index,
    ByteBuffer& buff)
{
    int ret;
    DLMS_DATA_TYPE dt;
    DLMSValueEventArg e(NULL, obj, index);
    if ((ret = obj->GetValue(m_Settings, e)) != 0)
    {
        return ret;
    }
    if ((ret = obj->GetDataType(index, dt)) != 0)
    {
        return ret;
    }
    if (dt == DLMS_DATA_TYPE_ARRAY)
    {
        buff.Set(e.GetValue().byteArr, e.GetValue().GetSize());
        return 0;
    }
    if (dt == DLMS_DATA_TYPE_NONE)
    {
        dt = e.GetValue().vt;
    }
    return Helpers::SetData(&m_Settings, buff, e.GetValue().vt, e.GetValue());
}

int DLMSNotify::GenerateDataNotificationMessages(
    struct tm* time,
    ByteBuffer& data,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    if (GetUseLogicalNameReferencing())
    {
        DLMSLNParameters p(&m_Settings, 0, DLMS_COMMAND_DATA_NOTIFICATION, 0, NULL, &data, 0xff, DLMS_COMMAND_NONE);
        p.SetTime(time);
        ret = DLMS::GetLnMessages(p, reply);
    }
    else
    {
        DLMSSNParameters p(&m_Settings, DLMS_COMMAND_DATA_NOTIFICATION, 1, 0, &data, NULL);
        ret = DLMS::GetSnMessages(p, reply);
    }
    return ret;
}

int DLMSNotify::GenerateDataNotificationMessages(
    struct tm* date,
    std::vector<std::pair<DLMSObject*, unsigned char> >& objects,
    std::vector<ByteBuffer>& reply)
{
    ByteBuffer buff;
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    Helpers::SetObjectCount((unsigned long)objects.size(), buff);
    for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = objects.begin(); it != objects.end(); ++it)
    {
        AddData(it->first, it->second, buff);
    }
    return GenerateDataNotificationMessages(date, buff, reply);
}

#ifndef DLMS_IGNORE_PUSH_SETUP
int DLMSNotify::GeneratePushSetupMessages(
    struct tm* date,
    DLMSPushSetup* push,
    std::vector<ByteBuffer>& reply)
{
    if (push == NULL)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    ByteBuffer buff;
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    Helpers::SetObjectCount((unsigned long)push->GetPushObjectList().size(), buff);
    for (std::vector<std::pair<DLMSObject*, DLMSCaptureObject> >::iterator it = push->GetPushObjectList().begin(); it != push->GetPushObjectList().end(); ++it)
    {
        AddData(it->first, it->second.GetAttributeIndex(), buff);
    }
    return GenerateDataNotificationMessages(date, buff, reply);
}
#endif //DLMS_IGNORE_PUSH_SETUP

int DLMSNotify::ParsePush(std::vector<DLMSVariant>& data, std::vector<std::pair<DLMSObject*, unsigned char> >& items)
{
    DLMSObject *obj;
    int index, pos, ret;
    DLMS_DATA_TYPE dt;
    DLMSVariant tmp, value;
    DLMSVariant ln;
    for (std::vector<DLMSVariant>::iterator it = data.at(0).Arr.begin(); it != data.at(0).Arr.end(); ++it)
    {
        int classID = it->Arr[0].ToInteger() & 0xFFFF;
        if (classID > 0)
        {
            ln.Clear();
            if ((ret = DLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0)
            {
                return ret;
            }
            obj = GetObjects().FindByLN((DLMS_OBJECT_TYPE)classID, ln.strVal);
            if (obj == NULL)
            {
                obj = DLMSObjectFactory::CreateObject((DLMS_OBJECT_TYPE)classID, ln.strVal);
                GetObjects().push_back(obj);
            }
            items.push_back(std::pair<DLMSObject*, unsigned char>(obj, it->Arr[2].ToInteger()));
        }
    }
    pos = 0;
    for (std::vector<std::pair<DLMSObject*, unsigned char> >::iterator it = items.begin(); it != items.end(); ++it)
    {
        obj = it->first;
        value = data[pos];
        ++pos;
        index = it->second;
        if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            ret = obj->GetUIDataType(index, dt);
            if (dt != DLMS_DATA_TYPE_NONE)
            {
                tmp = value;
                if ((ret = DLMSClient::ChangeType(tmp, dt, value)) != 0)
                {
                    return ret;
                }
            }
        }
        DLMSValueEventArg e(NULL, obj, index);
        e.SetValue(value);
        obj->SetValue(m_Settings, e);
    }
    return 0;
}
