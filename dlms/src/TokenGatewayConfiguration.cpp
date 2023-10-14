#include "TokenGatewayConfiguration.h"
#include "Helpers.h"

TokenGatewayConfiguration::TokenGatewayConfiguration()
{
};

std::string& TokenGatewayConfiguration::GetCreditReference() {
    return m_CreditReference;
}

void TokenGatewayConfiguration::SetCreditReference(std::string& value) {
    m_CreditReference = value;
}

unsigned char TokenGatewayConfiguration::GetTokenProportion() {
    return m_TokenProportion;
}

void TokenGatewayConfiguration::SetTokenProportion(unsigned char value) {
    m_TokenProportion = value;
}

std::string TokenGatewayConfiguration::ToString()
{
    std::string str = m_CreditReference;
    str.append(", ");
    str.append(Helpers::IntToString(m_TokenProportion));
    return str;
}
