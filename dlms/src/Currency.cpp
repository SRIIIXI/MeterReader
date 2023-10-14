#include "Currency.h"
#include "Helpers.h"

// Constructor.
Currency::Currency()
{
    m_Scale = 0;
    m_Unit = DLMS_CURRENCY_TIME;
};

/**
 * @return Currency name.
 */
std::string& Currency::GetName()
{
    return m_Name;
}

/**
 * @param value
 *            Currency name.
 */
void Currency::SetName(std::string& value)
{
    m_Name = value;
}

/**
 * @return Currency scale.
 */
char Currency::GetScale()
{
    return m_Scale;
}

/**
 * @param value
 *            Currency scale.
 */
void Currency::SetScale(char value)
{
    m_Scale = value;
}

/**
 * @return Currency unit.
 */
DLMS_CURRENCY Currency::GetUnit()
{
    return m_Unit;
}

/**
 * @param value
 *            Currency unit.
 */
void Currency::SetUnit(DLMS_CURRENCY value)
{
    m_Unit = value;
}

std::string Currency::ToString()
{
    std::string str = m_Name;
    str.append(", ");
    str.append(Helpers::IntToString(m_Scale));
    str.append(", ");
    str.append(Helpers::IntToString(m_Unit));
    return str;
}
