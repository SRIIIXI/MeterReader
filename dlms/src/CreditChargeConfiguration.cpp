#include "CreditChargeConfiguration.h"
#include "Helpers.h"
 // Constructor.
CreditChargeConfiguration::CreditChargeConfiguration()
{
}

std::string& CreditChargeConfiguration::GetCreditReference()
{
    return m_CreditReference;
}

void CreditChargeConfiguration::SetCreditReference(std::string& value)
{
    m_CreditReference = value;
}

std::string& CreditChargeConfiguration::GetChargeReference()
{
    return m_ChargeReference;
}

void CreditChargeConfiguration::SetChargeReference(std::string& value)
{
    m_ChargeReference = value;
}

DLMS_CREDIT_COLLECTION_CONFIGURATION CreditChargeConfiguration::GetCollectionConfiguration()
{
    return m_CollectionConfiguration;
}

void CreditChargeConfiguration::SetCollectionConfiguration(DLMS_CREDIT_COLLECTION_CONFIGURATION value)
{
    m_CollectionConfiguration = value;
}

std::string CreditChargeConfiguration::ToString()
{
    std::string str = m_CreditReference;
    str.append(", ");
    str.append(m_ChargeReference);
    str.append(", ");
    str.append(Helpers::IntToString(m_CollectionConfiguration));
    return str;
}
