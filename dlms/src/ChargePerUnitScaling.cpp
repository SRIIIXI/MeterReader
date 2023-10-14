#include "ChargePerUnitScaling.h"
#include "Helpers.h"

 // Constructor.
ChargePerUnitScaling::ChargePerUnitScaling()
{
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @return Commodity scale.
 */
signed char ChargePerUnitScaling::GetCommodityScale()
{
    return m_CommodityScale;
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @param value
 *            Commodity scale.
 */
void ChargePerUnitScaling::SetCommodityScale(signed char value)
{
    m_CommodityScale = value;
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @return Price scale.
 */
signed char ChargePerUnitScaling::GetPriceScale()
{
    return m_PriceScale;
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @param value
 *            Price scale.
 */
void ChargePerUnitScaling::SetPriceScale(signed char value)
{
    m_PriceScale = value;
}

std::string ChargePerUnitScaling::ToString()
{
    std::string str = Helpers::IntToString(m_CommodityScale);
    str.append(", ");
    str = Helpers::IntToString(m_PriceScale);
    return str;
}
