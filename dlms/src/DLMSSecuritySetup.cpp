#include "DLMSVariant.h"
#include "DLMSClient.h"
#include "DLMSSecuritySetup.h"
#include "DLMSConverter.h"
#include "DLMSSecureClient.h"

#ifndef DLMS_IGNORE_SECURITY_SETUP

//Constructor.
DLMSSecuritySetup::DLMSSecuritySetup() : DLMSSecuritySetup("0.0.43.0.0.255", 0)
{
}

//SN Constructor.
DLMSSecuritySetup::DLMSSecuritySetup(std::string ln, unsigned short sn) :
    DLMSObject(DLMS_OBJECT_TYPE_SECURITY_SETUP, ln, sn)
{
    m_Version = 1;
    m_SecurityPolicy = DLMS_SECURITY_POLICY_NOTHING;
    m_SecuritySuite = DLMS_SECURITY_SUITE_V0;
}

//LN Constructor.
DLMSSecuritySetup::DLMSSecuritySetup(std::string ln) : DLMSSecuritySetup(ln, 0)
{

}

DLMS_SECURITY_POLICY DLMSSecuritySetup::GetSecurityPolicy()
{
    return m_SecurityPolicy;
}

void DLMSSecuritySetup::SetSecurityPolicy(DLMS_SECURITY_POLICY value)
{
    m_SecurityPolicy = value;
}

DLMS_SECURITY_SUITE DLMSSecuritySetup::GetSecuritySuite()
{
    return m_SecuritySuite;
}

void DLMSSecuritySetup::SetSecuritySuite(DLMS_SECURITY_SUITE value)
{
    m_SecuritySuite = value;
}

ByteBuffer DLMSSecuritySetup::GetClientSystemTitle()
{
    return m_ClientSystemTitle;
}

void DLMSSecuritySetup::SetClientSystemTitle(ByteBuffer& value)
{
    m_ClientSystemTitle = value;
}

ByteBuffer DLMSSecuritySetup::GetServerSystemTitle()
{
    return m_ServerSystemTitle;
}

void DLMSSecuritySetup::SetServerSystemTitle(ByteBuffer& value)
{
    m_ServerSystemTitle = value;
}

// Returns amount of attributes.
int DLMSSecuritySetup::GetAttributeCount()
{
    if (GetVersion() == 0)
    {
        return 5;
    }
    return 6;
}

// Returns amount of methods.
int DLMSSecuritySetup::GetMethodCount()
{
    if (GetVersion() == 0)
    {
        return 2;
    }
    return 8;
}

int DLMSSecuritySetup::Activate(
    DLMSClient* client,
    DLMS_SECURITY security,
    std::vector<ByteBuffer>& reply)
{
    DLMSVariant data((char)security);
    return client->Method(this, 1, data, reply);
}

int DLMSSecuritySetup::GlobalKeyTransfer(
    DLMSClient* client,
    ByteBuffer& kek,
    std::vector<std::pair<DLMS_GLOBAL_KEY_TYPE, ByteBuffer&> >& list,
    std::vector<ByteBuffer>& reply)
{
    int ret = 0;
    DLMSVariant data;
    ByteBuffer bb, tmp;
    if (list.size() == 0)
    {
        //Invalid list. It is empty.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    bb.SetUInt8((unsigned char)list.size());
    for (std::vector<std::pair<DLMS_GLOBAL_KEY_TYPE, ByteBuffer&> >::iterator it = list.begin(); it != list.end(); ++it)
    {
        bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        bb.SetUInt8(2);
        data = (char)it->first;
        if ((ret = Helpers::SetData(NULL, bb, DLMS_DATA_TYPE_ENUM, data)) != 0 ||
            (ret = DLMSSecureClient::Encrypt(kek, it->second, tmp)) != 0)
        {
            break;
        }
        data = tmp;
        if ((ret = Helpers::SetData(NULL, bb, DLMS_DATA_TYPE_OCTET_STRING, data)) != 0)
        {
            break;
        }
    }
    if (ret == 0)
    {
        data = bb;
        ret = client->Method(this, 2, data, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int DLMSSecuritySetup::ImportCertificate(
    DLMSClient* client,
    ByteBuffer& key,
    std::vector<ByteBuffer>& reply)
{
    DLMSVariant data = key;
    reply.clear();
    return client->Method(this, 6, data, reply);
}

int DLMSSecuritySetup::ExportCertificateByEntity(
    DLMSClient* client,
    DLMS_CERTIFICATE_ENTITY entity,
    DLMS_CERTIFICATE_TYPE type,
    ByteBuffer& systemTitle,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    DLMSVariant data;
    ByteBuffer bb;
    reply.clear();
    if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(2)) == 0 &&
        //Add enum
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(0)) == 0 &&
        //Add certificate_identification_by_entity
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(3)) == 0 &&
        //Add certificate_entity
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(entity)) == 0 &&
        //Add certificate_type
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(type)) == 0 &&
        //system_title
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) == 0 &&
        (ret = Helpers::SetObjectCount((unsigned long)systemTitle.GetSize(), bb)) == 0 &&
        (ret = bb.Set(&systemTitle, 0, systemTitle.GetSize())) == 0)
    {
        data = bb;
        ret = client->Method(this, 7, data, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int DLMSSecuritySetup::ExportCertificateBySerial(
    DLMSClient* client,
    ByteBuffer& serialNumber,
    ByteBuffer& issuer,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    DLMSVariant data;
    ByteBuffer bb;
    reply.clear();
    if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(2)) == 0 &&
        //Add enum
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(1)) == 0 &&
        //Add certificate_identification_by_entity
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(2)) == 0 &&
        //serialNumber
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) == 0 &&
        (ret = Helpers::SetObjectCount((unsigned long)serialNumber.GetSize(), bb)) == 0 &&
        (ret = bb.Set(&serialNumber, 0, serialNumber.GetSize())) == 0 &&
        //issuer
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) == 0 &&
        (ret = Helpers::SetObjectCount((unsigned long)issuer.GetSize(), bb)) == 0 &&
        (ret = bb.Set(&issuer, 0, issuer.GetSize())) == 0)
    {
        data = bb;
        ret = client->Method(this, 7, data, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int DLMSSecuritySetup::RemoveCertificateByEntity(
    DLMSClient* client,
    DLMS_CERTIFICATE_ENTITY entity,
    DLMS_CERTIFICATE_TYPE type,
    ByteBuffer& systemTitle,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    DLMSVariant data;
    ByteBuffer bb;
    reply.clear();
    if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(2)) == 0 &&
        //Add enum
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(0)) == 0 &&
        //Add certificate_identification_by_entity
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(3)) == 0 &&
        //Add certificate_entity
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(entity)) == 0 &&
        //Add certificate_type
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(type)) == 0 &&
        //system_title
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) == 0 &&
        (ret = Helpers::SetObjectCount((unsigned long)systemTitle.GetSize(), bb)) == 0 &&
        (ret = bb.Set(&systemTitle, 0, systemTitle.GetSize())) == 0)
    {
        data = bb;
        ret = client->Method(this, 8, data, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int DLMSSecuritySetup::RemoveCertificateBySerial(
    DLMSClient* client,
    ByteBuffer& serialNumber,
    ByteBuffer& issuer,
    std::vector<ByteBuffer>& reply)
{
    int ret;
    DLMSVariant data;
    ByteBuffer bb;
    reply.clear();
    if ((ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(2)) == 0 &&
        //Add enum
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_ENUM)) == 0 &&
        (ret = bb.SetUInt8(1)) == 0 &&
        //Add certificate_identification_by_entity
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE)) == 0 &&
        (ret = bb.SetUInt8(2)) == 0 &&
        //serialNumber
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) == 0 &&
        (ret = Helpers::SetObjectCount((unsigned long)serialNumber.GetSize(), bb)) == 0 &&
        (ret = bb.Set(&serialNumber, 0, serialNumber.GetSize())) == 0 &&
        //issuer
        (ret = bb.SetUInt8(DLMS_DATA_TYPE_OCTET_STRING)) == 0 &&
        (ret = Helpers::SetObjectCount((unsigned long)issuer.GetSize(), bb)) == 0 &&
        (ret = bb.Set(&issuer, 0, issuer.GetSize())) == 0)
    {
        data = bb;
        ret = client->Method(this, 8, data, DLMS_DATA_TYPE_ARRAY, reply);
    }
    return ret;
}

int DLMSSecuritySetup::Invoke(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        m_SecurityPolicy = (DLMS_SECURITY_POLICY)e.GetParameters().ToInteger();
    }
    else if (e.GetIndex() == 2)
    {
        for (std::vector<DLMSVariant>::iterator it = e.GetParameters().Arr.begin(); it != e.GetParameters().Arr.end(); ++it)
        {
            DLMS_GLOBAL_KEY_TYPE type = (DLMS_GLOBAL_KEY_TYPE)it->Arr[0].ToInteger();
            ByteBuffer data, reply;
            ByteBuffer kek = settings.GetKek();
            data.Set(it->Arr[1].byteArr, it->Arr[1].GetSize());
            if (DLMSSecureClient::Decrypt(kek, data, reply) != 0 ||
                reply.GetSize() != 16)
            {
                e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                break;
            }
            //Keys are take in action after reply is generated.
            switch (type) {
            case DLMS_GLOBAL_KEY_TYPE_UNICAST_ENCRYPTION:
                break;
            case DLMS_GLOBAL_KEY_TYPE_BROADCAST_ENCRYPTION:
                // Invalid type
                e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
                break;
            case DLMS_GLOBAL_KEY_TYPE_AUTHENTICATION:
                break;
            case DLMS_GLOBAL_KEY_TYPE_KEK:
                break;
            default:
                e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
            }
        }
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return DLMS_ERROR_CODE_OK;
}


int DLMSSecuritySetup::ApplyKeys(DLMSSettings& settings, DLMSValueEventArg& e)
{
    for (std::vector<DLMSVariant>::iterator it = e.GetParameters().Arr.begin(); it != e.GetParameters().Arr.end(); ++it)
    {
        DLMS_GLOBAL_KEY_TYPE type = (DLMS_GLOBAL_KEY_TYPE)it->Arr[0].ToInteger();
        ByteBuffer data, reply;
        ByteBuffer kek = settings.GetKek();
        data.Set(it->Arr[1].byteArr, it->Arr[1].GetSize());
        if (DLMSSecureClient::Decrypt(kek, data, reply) != 0 ||
            reply.GetSize() != 16)
        {
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
            break;
        }
        switch (type) {
        case DLMS_GLOBAL_KEY_TYPE_UNICAST_ENCRYPTION:
            settings.GetCipher()->SetBlockCipherKey(reply);
            break;
        case DLMS_GLOBAL_KEY_TYPE_BROADCAST_ENCRYPTION:
            // Invalid type
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
            break;
        case DLMS_GLOBAL_KEY_TYPE_AUTHENTICATION:
            // if settings.Cipher is null non secure server is used.
            settings.GetCipher()->SetAuthenticationKey(reply);
            break;
        case DLMS_GLOBAL_KEY_TYPE_KEK:
            settings.SetKek(reply);
            break;
        default:
            e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
        }
    }
    return DLMS_ERROR_CODE_OK;
}

void DLMSSecuritySetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(DLMSConverter::ToString(m_SecurityPolicy));
    values.push_back(DLMSConverter::ToString(m_SecuritySuite));
    std::string str = m_ClientSystemTitle.ToHexString();
    values.push_back(str);
    str = m_ServerSystemTitle.ToHexString();
    values.push_back(str);
    if (GetVersion() > 0)
    {
        std::stringstream sb;
        bool empty = true;
        for (std::vector<DLMSCertificateInfo*>::iterator it = m_Certificates.begin(); it != m_Certificates.end(); ++it)
        {
            if (empty)
            {
                empty = false;
            }
            else
            {
                sb << ',';
            }
            sb << '[';
            sb << (int)(*it)->GetEntity();
            sb << ' ';
            sb << (int)(*it)->GetType();
            sb << ' ';
            sb << (*it)->GetSerialNumber();
            sb << ' ';
            sb << (*it)->GetIssuer();
            sb << ' ';
            sb << (*it)->GetSubject();
            sb << ' ';
            sb << (*it)->GetSubjectAltName();
            sb << ']';
        }
        values.push_back(sb.str());
    }
}

void DLMSSecuritySetup::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || DLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //SecurityPolicy
    if (all || CanRead(2))
    {
        attributes.push_back(2);
    }
    //SecuritySuite
    if (all || CanRead(3))
    {
        attributes.push_back(3);
    }
    //ClientSystemTitle
    if (all || CanRead(4))
    {
        attributes.push_back(4);
    }
    //ServerSystemTitle
    if (all || CanRead(5))
    {
        attributes.push_back(5);
    }
    if (GetVersion() > 0)
    {
        //Certificates
        if (all || CanRead(6))
        {
            attributes.push_back(6);
        }
    }
}

int DLMSSecuritySetup::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 6 && GetVersion() > 0)
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
int DLMSSecuritySetup::GetValue(DLMSSettings& settings, DLMSValueEventArg& e)
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
    else if (e.GetIndex() == 2)
    {
        DLMSVariant tmp = m_SecurityPolicy;
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 3)
    {
        DLMSVariant tmp = m_SecuritySuite;
        e.SetValue(tmp);
    }
    else if (e.GetIndex() == 4)
    {
        e.GetValue().Add(m_ClientSystemTitle.GetData(), m_ClientSystemTitle.GetSize());
    }
    else if (e.GetIndex() == 5)
    {
        e.GetValue().Add(m_ServerSystemTitle.GetData(), m_ServerSystemTitle.GetSize());
    }
    else if (e.GetIndex() == 6)
    {
        ByteBuffer bb;
        bb.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        Helpers::SetObjectCount((unsigned long)m_Certificates.size(), bb);
        for (std::vector<DLMSCertificateInfo*>::iterator it = m_Certificates.begin(); it != m_Certificates.end(); ++it)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            Helpers::SetObjectCount(6, bb);
            bb.SetUInt8(DLMS_DATA_TYPE_ENUM);
            bb.SetUInt8((*it)->GetEntity());
            bb.SetUInt8(DLMS_DATA_TYPE_ENUM);
            bb.SetUInt8((*it)->GetType());
            bb.AddString((*it)->GetSerialNumber());
            bb.AddString((*it)->GetIssuer());
            bb.AddString((*it)->GetSubject());
            bb.AddString((*it)->GetSubjectAltName());
        }
        e.SetValue(bb);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Set value of given attribute.
int DLMSSecuritySetup::SetValue(DLMSSettings& settings, DLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_SecurityPolicy = (DLMS_SECURITY_POLICY)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 3)
    {
        m_SecuritySuite = (DLMS_SECURITY_SUITE)e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        m_ClientSystemTitle.Clear();
        m_ClientSystemTitle.Set(e.GetValue().byteArr, e.GetValue().size);
    }
    else if (e.GetIndex() == 5)
    {
        m_ServerSystemTitle.Clear();
        m_ServerSystemTitle.Set(e.GetValue().byteArr, e.GetValue().size);
    }
    else if (e.GetIndex() == 6)
    {
        m_Certificates.clear();
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            std::string tmp;
            for (std::vector<DLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                DLMSCertificateInfo* info = new DLMSCertificateInfo();
                info->SetEntity((DLMS_CERTIFICATE_ENTITY)it->Arr[0].ToInteger());
                info->SetType((DLMS_CERTIFICATE_TYPE)it->Arr[1].ToInteger());
                tmp = it->Arr[2].ToString();
                info->SetSerialNumber(tmp);
                tmp = it->Arr[3].ToString();
                info->SetIssuer(tmp);
                tmp = it->Arr[4].ToString();
                info->SetSubject(tmp);
                tmp = it->Arr[5].ToString();
                info->SetSubjectAltName(tmp);
                m_Certificates.push_back(info);
            }
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

std::vector<DLMSCertificateInfo*>& DLMSSecuritySetup::GetCertificates()
{
    return m_Certificates;
}
#endif //DLMS_IGNORE_SECURITY_SETUP
