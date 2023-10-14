#include "DLMSCertificateInfo.h"

DLMSCertificateInfo::DLMSCertificateInfo()
{
    m_Entity = DLMS_CERTIFICATE_ENTITY_SERVER;
    m_Type = DLMS_CERTIFICATE_TYPE_DIGITAL_SIGNATURE;
}

DLMS_CERTIFICATE_ENTITY DLMSCertificateInfo::GetEntity() {
    return m_Entity;
}

void DLMSCertificateInfo::SetEntity(DLMS_CERTIFICATE_ENTITY value) {
    m_Entity = value;
}

DLMS_CERTIFICATE_TYPE DLMSCertificateInfo::GetType() {
    return m_Type;
}

void DLMSCertificateInfo::SetType(DLMS_CERTIFICATE_TYPE value) {
    m_Type = value;
}

std::string DLMSCertificateInfo::GetSerialNumber() {
    return m_SerialNumber;
}

void DLMSCertificateInfo::SetSerialNumber(std::string& value) {
    m_SerialNumber = value;
}

std::string DLMSCertificateInfo::GetIssuer() {
    return m_Issuer;
}

void DLMSCertificateInfo::SetIssuer(std::string& value) {
    m_Issuer = value;
}

std::string DLMSCertificateInfo::GetSubject() {
    return m_Subject;
}

void DLMSCertificateInfo::SetSubject(std::string& value) {
    m_Subject = value;
}

std::string DLMSCertificateInfo::GetSubjectAltName() {
    return m_SubjectAltName;
}

void DLMSCertificateInfo::SetSubjectAltName(std::string& value) {
    m_SubjectAltName = value;
}
