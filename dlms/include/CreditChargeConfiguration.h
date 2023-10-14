#ifndef CREDIT_CHARGE_CONFIGURATION_H
#define CREDIT_CHARGE_CONFIGURATION_H

#include "Enums.h"
#include <string>

/**
*
*/
class CreditChargeConfiguration
{
    std::string m_CreditReference;
    std::string m_ChargeReference;
    DLMS_CREDIT_COLLECTION_CONFIGURATION m_CollectionConfiguration;
public:

    // Constructor.
    CreditChargeConfiguration();

    /**
     * @return Credit reference.
     */
    std::string& GetCreditReference();

    /**
     * @param value
     *            Credit reference.
     */
    void SetCreditReference(std::string& value);

    /**
     * @return Charge reference.
     */
    std::string& GetChargeReference();

    /**
     * @param value
     *            Charge reference.
     */
    void SetChargeReference(std::string& value);

    /**
     * @return Collection configuration.
     */
    DLMS_CREDIT_COLLECTION_CONFIGURATION GetCollectionConfiguration();

    /**
     * @param value
     *            Collection configuration.
     */
    void SetCollectionConfiguration(DLMS_CREDIT_COLLECTION_CONFIGURATION value);

    std::string ToString();
};
#endif
