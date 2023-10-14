#ifndef CHARGE_TABLE_H
#define CHARGE_TABLE_H

#include <string>

class ChargeTable
{
    std::string m_Index;
    short m_ChargePerUnit;
public:
    // Constructor.
    ChargeTable();

    /**
    *
    * @return Index.
    */
    std::string& GetIndex();

    /**
     *
     * @param value
     *            Index.
     */
    void SetIndex(std::string& value);

    /**
     *
     * @return Charge per unit.
     */
    short GetChargePerUnit();

    /**
     *
     * @param value
     *            Charge per unit.
     */
    void SetChargePerUnit(short value);

    std::string ToString();
};
#endif
