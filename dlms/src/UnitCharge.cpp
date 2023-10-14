#include "UnitCharge.h"

UnitCharge::UnitCharge()
{
}

ChargePerUnitScaling& UnitCharge::GetChargePerUnitScaling() {
    return m_ChargePerUnitScaling;
}

Commodity& UnitCharge::GetCommodity() {
    return m_Commodity;
}

std::vector<ChargeTable>& UnitCharge::GetChargeTables()
{
    return m_ChargeTables;
}

std::string UnitCharge::ToString()
{
    std::string str = m_ChargePerUnitScaling.ToString();
    str.append(", ");
    str.append(m_Commodity.ToString());
    str.append(", {");
    for (std::vector<ChargeTable>::iterator it = m_ChargeTables.begin(); it != m_ChargeTables.end(); ++it)
    {
        str.append(it->ToString());
        str.append(", ");
    }
    str.append("}");
    return str;
}
