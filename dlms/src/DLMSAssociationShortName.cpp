#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSAssociationShortName.h"
#include "DLMSObjectFactory.h"
#include "DLMSServer.h"
#include "Secure.h"

#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
int DLMSAssociationShortName::GetAccessRights(DLMSObject* pObj, DLMSServer* server, ByteBuffer& data)
{
    int ret, cnt = pObj->GetAttributeCount();
    DLMSVariant ln = pObj->GetShortName();
    DLMSValueEventArg e(server, pObj, 0);
    data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    data.SetUInt8(3);
    Helpers::SetData(NULL, data, DLMS_DATA_TYPE_INT16, ln);
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    Helpers::SetObjectCount(cnt, data);
    DLMSVariant empty, index, access;
    for (int pos = 0; pos != cnt; ++pos)
    {
        e.SetIndex(pos + 1);
        index = pos + 1;
        if (server != NULL)
        {
            access = server->GetAttributeAccess(&e);
        }
        else
        {
            access = DLMS_ACCESS_MODE_READ_WRITE;
        }
        //attribute_access_item
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(3);
        if ((ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_INT8, index)) != 0 ||
            (ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_ENUM, access)) != 0 ||
            (ret = Helpers::SetData(NULL, data, DLMS_DATA_TYPE_NONE, empty)) != 0)
        {
            return ret;
        }
    }
    cnt = pObj->GetMethodCount();
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    Helpers::SetObjectCount(cnt, data);
    for (int pos = 0; pos != cnt; ++pos)
    {
        e.SetIndex(pos + 1);
        index = pos + 1;
        if (server != NULL)
        {
            access = server->GetMethodAccess(&e);
        }
        else
        {
            access = DLMS_METHOD_ACCESS_MODE_ACCESS;
        }
        //attribute_access_item
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_INT8, index);
        Helpers::SetData(NULL, data, DLMS_DATA_TYPE_ENUM, access);
    }
    return DLMS_ERROR_CODE_OK;
}

void DLMSAssociationShortName::UpdateAccessRights(DLMSVariant& buff)
{
    for (std::vector<DLMSVariant>::iterator access = buff.Arr.begin(); access != buff.Arr.end(); ++access)
    {
        int sn = access->Arr[0].ToInteger();
        DLMSObject* pObj = m_ObjectList.FindBySN(sn);
        if (pObj != NULL)
        {
            for (std::vector<DLMSVariant>::iterator attributeAccess = access->Arr[1].Arr.begin();
                attributeAccess != access->Arr[1].Arr.end(); ++attributeAccess)
            {
                int id = attributeAccess->Arr[0].ToInteger();
                int tmp = attributeAccess->Arr[1].ToInteger();
                pObj->SetAccess(id, (DLMS_ACCESS_MODE)tmp);
            }
            for (std::vector<DLMSVariant>::iterator methodAccess = access->Arr[2].Arr.begin();
                methodAccess != access->Arr[2].Arr.end(); ++methodAccess)
            {
                int id = methodAccess->Arr[0].ToInteger();
                int tmp = methodAccess->Arr[1].ToInteger();
                pObj->SetMethodAccess(id, (DLMS_METHOD_ACCESS_MODE)tmp);
            }
        }
    }
}

//Constructor.
DLMSAssociationShortName::DLMSAssociationShortName() : DLMSObject(DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME)
{
    Helpers::SetLogicalName("0.0.40.0.0.255", m_LN);
    m_Secret.AddString("Gurux");
    m_SN = 0xFA00;
    m_Version = 2;
}

DLMSObjectCollection& DLMSAssociationShortName::GetObjectList()
{
    return m_ObjectList;
}

ByteBuffer& DLMSAssociationShortName::GetSecret()
{
    return m_Secret;
}
void DLMSAssociationShortName::SetSecret(ByteBuffer& value)
{
    m_Secret = value;
}

std::string& DLMSAssociationShortName::GetSecuritySetupReference()
{
    return m_SecuritySetupReference;
}
void DLMSAssociationShortName::SetSecuritySetupReference(std::string& value)
{
    m_SecuritySetupReference = value;
}


void DLMSAssociationShortName::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_ObjectList.ToString());
    values.push_back("");
    values.push_back(m_SecuritySetupReference);
}

void DLMSAssociationShortName::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ObjectList is static and read only once.
    if (all || !IsRead(2))
    {
        attributes.push_back(2);
    }
    if (m_Version > 1)
    {
        //AccessRightsList is static and read only once.
        if (all || !IsRead(3))
        {
            attributes.push_back(3);
        }
        //SecuritySetupReference is static and read only once.
        if (all || !IsRead(4))
        {
            attributes.push_back(4);
        }
    }
}

// Returns amount of attributes.
int DLMSAssociationShortName::GetAttributeCount()
{
    if (m_Version < 2)
    {
        return 2;
    }
    return 4;
}

// Returns amount of methods.
int DLMSAssociationShortName::GetMethodCount()
{
    return 8;
}

int DLMSAssociationShortName::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns SN Association View.
int DLMSAssociationShortName::GetObjects(
    DLMSSettings& settings,
    DLMSValueEventArg& e,
    ByteBuffer& data)
{
    unsigned long pos = 0;
    int ret;
    //Add count only for first time.
    if (settings.GetIndex() == 0)
    {
        settings.SetCount((unsigned short)m_ObjectList.size());
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        Helpers::SetObjectCount((unsigned long)m_ObjectList.size(), data);
    }
    for (DLMSObjectCollection::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
    {
        ++pos;
        if (!(pos <= settings.GetIndex()))
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(4);//Count
            DLMSVariant type = (*it)->GetObjectType();
            DLMSVariant version = (*it)->GetVersion();
            DLMSVariant sn = (*it)->GetShortName();
            DLMSVariant ln((*it)->m_LN, 6, DLMS_DATA_TYPE_OCTET_STRING);
            if ((ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_INT16, sn)) != 0 || //base address.
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT16, type)) != 0 || //ClassID
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_UINT8, version)) != 0 || //Version
                (ret = Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0) //LN
            {
                return ret;
            }
            if (settings.IsServer())
            {
                settings.SetIndex(settings.GetIndex() + 1);
                //If PDU is full.
                if (!e.GetSkipMaxPduSize() && data.GetSize() >= settings.GetMaxPduSize())
                {
                    break;
                }
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}

int DLMSAssociationShortName::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    // Check reply_to_HLS_authentication
    if (e.GetIndex() == 8)
    {
        int ret;
        unsigned long ic = 0;
        ByteBuffer secret;
        ByteBuffer* readSecret;
        if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
        {
            unsigned char ch;
            readSecret = &settings.GetSourceSystemTitle();
            ByteBuffer bb;
            bb.Set(e.GetParameters().byteArr, e.GetParameters().GetSize());
            if ((ret = bb.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if ((ret = bb.GetUInt32(&ic)) != 0)
            {
                return ret;
            }
        }
        else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
        {
            secret.Set(&m_Secret);
            secret.Set(&settings.GetSourceSystemTitle());
            secret.Set(&settings.GetCipher()->GetSystemTitle());
            secret.Set(&settings.GetStoCChallenge());
            secret.Set(&settings.GetCtoSChallenge());
            readSecret = &secret;
        }
        else
        {
            readSecret = &m_Secret;
        }
        ByteBuffer serverChallenge;
        if ((ret = Secure::SecureEncrypt(settings, settings.GetCipher(), ic,
            settings.GetStoCChallenge(), *readSecret, serverChallenge)) != 0)
        {
            return ret;
        }

        if (serverChallenge.Compare(e.GetParameters().byteArr, e.GetParameters().GetSize()))
        {
            if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
            {
                readSecret = &settings.GetCipher()->GetSystemTitle();
                ic = settings.GetCipher()->GetFrameCounter();
            }
            else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
            {
                secret.Clear();
                secret.Set(&m_Secret);
                secret.Set(&settings.GetCipher()->GetSystemTitle());
                secret.Set(&settings.GetSourceSystemTitle());
                secret.Set(&settings.GetCtoSChallenge());
                secret.Set(&settings.GetStoCChallenge());
                readSecret = &secret;
            }
            else
            {
                readSecret = &m_Secret;
            }
            serverChallenge.Clear();
            if ((ret = Secure::SecureEncrypt(settings,
                settings.GetCipher(),
                ic,
                settings.GetCtoSChallenge(),
                *readSecret,
                serverChallenge)) != 0)
            {
                return ret;
            }
            e.SetValue(serverChallenge);
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() | DLMS_CONNECTION_STATE_HDLC));
        }
        else
        {
            settings.SetConnected((DLMS_CONNECTION_STATE)(settings.GetConnected() & ~DLMS_CONNECTION_STATE_HDLC));
            return 0;
        }
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return 0;
}

int DLMSAssociationShortName::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
        e.SetByteArray(true);
        ByteBuffer buff;
        int ret = GetObjects(settings, e, buff);
        e.SetValue(buff);
        return ret;
    }
    else if (e.GetIndex() == 3)
    {
        e.SetByteArray(true);
        int ret;
        //Add count
        unsigned long cnt = (unsigned long)m_ObjectList.size();
        ByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount(cnt, data);
        for (std::vector<DLMSObject*>::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
        {
            if ((ret = GetAccessRights(*it, e.GetServer(), data)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    else if (e.GetIndex() == 4)
    {
        e.SetByteArray(true);
        ByteBuffer data;
        DLMSVariant tmp;
        Helpers::SetLogicalName(m_SecuritySetupReference.c_str(), tmp);
        Helpers::SetData(&settings, data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        e.SetValue(data);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

int DLMSAssociationShortName::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ObjectList.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for (std::vector<DLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
            {
                int sn = item->Arr[0].ToInteger();
                DLMSObject* pObj = settings.GetObjects().FindBySN(sn);
                if (pObj == NULL)
                {
                    DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE)item->Arr[1].ToInteger();
                    int version = item->Arr[2].ToInteger();
                    std::string ln;
                    Helpers::GetLogicalName((*item).Arr[3].byteArr, ln);
                    pObj = DLMSObjectFactory::CreateObject(type, ln);
                    if (pObj != NULL)
                    {
                        settings.AddAllocateObject(pObj);
                        pObj->SetShortName(sn);
                        pObj->SetVersion(version);
                    }
                }
                if (pObj != NULL)
                {
                    m_ObjectList.push_back(pObj);
                }
            }
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            for (std::vector<DLMSObject*>::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
            {
                for (int pos = 1; pos != (*it)->GetAttributeCount(); ++pos)
                {
                    (*it)->SetAccess(pos, DLMS_ACCESS_MODE_NONE);
                }
            }
        }
        else
        {
            UpdateAccessRights(e.GetValue());
        }
    }
    else if (e.GetIndex() == 4)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_SecuritySetupReference = e.GetValue().ToString();
        }
        else
        {
            int ret;
            DLMSVariant tmp;
            if ((ret = DLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            m_SecuritySetupReference = tmp.ToString();
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME
