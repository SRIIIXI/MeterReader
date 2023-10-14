#ifndef CURRENCY_H
#define CURRENCY_H

#include "Enums.h"
#include <string>

class Currency
{
    /**
    * Currency name.
    */
    std::string m_Name;
    /**
     * Currency scale.
     */
    char m_Scale;
    /**
     * Currency unit.
     */
    DLMS_CURRENCY m_Unit;

public:
    // Constructor.
    Currency();

    /**
     * @return Currency name.
     */
    std::string& GetName();

    /**
     * @param value
     *            Currency name.
     */
    void SetName(std::string& value);

    /**
     * @return Currency scale.
     */
    char GetScale();

    /**
     * @param value
     *            Currency scale.
     */
    void SetScale(char value);

    /**
     * @return Currency unit.
     */
    DLMS_CURRENCY GetUnit();

    /**
     * @param value
     *            Currency unit.
     */
    void SetUnit(DLMS_CURRENCY value);

    std::string ToString();
};
#endif
