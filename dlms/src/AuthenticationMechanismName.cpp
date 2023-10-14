#include "AuthenticationMechanismName.h"
#include <sstream>
#include "DLMSVariant.h"

AuthenticationMechanismName::AuthenticationMechanismName()
{
    m_JointIsoCtt = 2;
    m_Country = 16;
    m_CountryName = 756;
    m_IdentifiedOrganization = 5;
    m_DlmsUA = 8;
    m_AuthenticationMechanismName = 2;
    m_MechanismId = DLMS_AUTHENTICATION_NONE;
}

int AuthenticationMechanismName::GetJointIsoCtt()
{
    return m_JointIsoCtt;
}
void AuthenticationMechanismName::SetJointIsoCtt(int value)
{
    m_JointIsoCtt = value;
}

int AuthenticationMechanismName::GetCountry()
{
    return m_Country;
}
void AuthenticationMechanismName::SetCountry(int value)
{
    m_Country = value;
}

int AuthenticationMechanismName::GetCountryName()
{
    return m_CountryName;
}
void AuthenticationMechanismName::SetCountryName(int value)
{
    m_CountryName = value;
}
int AuthenticationMechanismName::GetIdentifiedOrganization()
{
    return m_IdentifiedOrganization;
}
void AuthenticationMechanismName::SetIdentifiedOrganization(int value)
{
    m_IdentifiedOrganization = value;
}

int AuthenticationMechanismName::GetDlmsUA()
{
    return m_DlmsUA;
}
void AuthenticationMechanismName::SetDlmsUA(int value)
{
    m_DlmsUA = value;
}
int AuthenticationMechanismName::GetAuthenticationMechanismName()
{
    return m_AuthenticationMechanismName;
}
void AuthenticationMechanismName::SetAuthenticationMechanismName(int value)
{
    m_AuthenticationMechanismName = value;
}

DLMS_AUTHENTICATION AuthenticationMechanismName::GetMechanismId()
{
    return m_MechanismId;
}
void AuthenticationMechanismName::SetMechanismId(DLMS_AUTHENTICATION value)
{
    m_MechanismId = value;
}


std::string AuthenticationMechanismName::ToString()
{
    std::stringstream sb;
    sb << DLMSVariant(m_JointIsoCtt).ToString().c_str();
    sb << " ";
    sb << DLMSVariant(m_Country).ToString().c_str();
    sb << " ";
    sb << DLMSVariant(m_CountryName).ToString().c_str();
    sb << " ";
    sb << DLMSVariant(m_IdentifiedOrganization).ToString().c_str();
    sb << " ";
    sb << DLMSVariant(m_DlmsUA).ToString().c_str();
    sb << " ";
    sb << DLMSVariant(m_AuthenticationMechanismName).ToString().c_str();
    sb << " ";
    sb << DLMSVariant(m_MechanismId).ToString().c_str();
    return sb.str();
}
