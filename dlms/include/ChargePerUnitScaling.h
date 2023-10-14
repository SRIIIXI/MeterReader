#ifndef CHARGE_PER_UNIT_SCALING_H
#define CHARGE_PER_UNIT_SCALING_H

#include <string>

class ChargePerUnitScaling
{
    signed char m_CommodityScale;
    signed char m_PriceScale;
public:
    // Constructor.
    ChargePerUnitScaling();

    /**
     *
     * @return Commodity scale.
     */
    signed char GetCommodityScale();

    /**
     *
     * @param value
     *            Commodity scale.
     */
    void SetCommodityScale(signed char value);

    /**
     *
     * @return Price scale.
     */
    signed char GetPriceScale();

    /**
     *
     * @param value
     *            Price scale.
     */
    void SetPriceScale(signed char value);

    std::string ToString();
};
#endif
