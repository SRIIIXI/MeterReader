#ifndef UNIT_CHARGE_H
#define UNIT_CHARGE_H

#include <vector>
#include "ChargePerUnitScaling.h"
#include "Commodity.h"
#include "ChargeTable.h"
/**
* Time object. Date part is ignored.
*/
class UnitCharge
{
    /**
    * Charge per unit scaling. <br>
    * Online help:<br>
    * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
    */
    ChargePerUnitScaling m_ChargePerUnitScaling;

    /**
     * Commodity.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     */
    Commodity m_Commodity;

    /**
     * Charge tables.<br>
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     */
    std::vector<ChargeTable> m_ChargeTables;
public:
    // Constructor.
    UnitCharge();

    /**
    * Online help:<br>
    * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
    *
    * @return Charge per unit scaling.
    */
    ChargePerUnitScaling& GetChargePerUnitScaling();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Commodity
     */
    Commodity& GetCommodity();

    /**
     * Online help:<br>
     * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
     *
     * @return Charge tables.
     */
    std::vector<ChargeTable>& GetChargeTables();

    std::string ToString();

};
#endif
