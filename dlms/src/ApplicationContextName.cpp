#include "ApplicationContextName.h"
#include "DLMSVariant.h"

/// <summary>
/// Constructor
/// </summary>
ApplicationContextName::ApplicationContextName()
{
    m_JointIsoCtt = 2;
    m_Country = 16;
    m_CountryName = 756;
    m_IdentifiedOrganization = 5;
    m_DlmsUA = 8;
    m_ApplicationContext = 1;
    m_ContextId = DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME;
}

unsigned char ApplicationContextName::GetJointIsoCtt()
{
    return m_JointIsoCtt;
}
void ApplicationContextName::SetJointIsoCtt(unsigned char value)
{
    m_JointIsoCtt = value;
}

unsigned char ApplicationContextName::GetCountry()
{
    return m_Country;
}
void ApplicationContextName::SetCountry(unsigned char value)
{
    m_Country = value;
}

unsigned short ApplicationContextName::GetCountryName()
{
    return m_CountryName;
}
void ApplicationContextName::SetCountryName(unsigned short value)
{
    m_CountryName = value;
}

unsigned char ApplicationContextName::GetIdentifiedOrganization()
{
    return m_IdentifiedOrganization;
}
void ApplicationContextName::SetIdentifiedOrganization(unsigned char value)
{
    m_IdentifiedOrganization = value;
}

unsigned char ApplicationContextName::GetDlmsUA()
{
    return m_DlmsUA;
}
void ApplicationContextName::SetDlmsUA(unsigned char value)
{
    m_DlmsUA = value;
}

unsigned char ApplicationContextName::GetApplicationContext()
{
    return m_ApplicationContext;
}
void ApplicationContextName::SetApplicationContext(unsigned char value)
{
    m_ApplicationContext = value;
}

DLMS_APPLICATION_CONTEXT_NAME ApplicationContextName::GetContextId()
{
    return m_ContextId;
}
void ApplicationContextName::SetContextId(DLMS_APPLICATION_CONTEXT_NAME value)
{
    m_ContextId = value;
}

std::string ApplicationContextName::ToString()
{
    ByteBuffer bb;
    bb.AddString(m_LogicalName.c_str());
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_JointIsoCtt);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_Country);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_CountryName);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_IdentifiedOrganization);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_DlmsUA);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_ApplicationContext);
    bb.SetUInt8(' ');
    bb.AddIntAsString(m_ContextId);
    return bb.ToString();
}
