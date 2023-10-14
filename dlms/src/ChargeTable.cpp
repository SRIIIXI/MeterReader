#include "ChargeTable.h"
#include "Helpers.h"

ChargeTable::ChargeTable()
{
    m_ChargePerUnit = 0;
}

std::string& ChargeTable::GetIndex()
{
    return m_Index;
}

void ChargeTable::SetIndex(std::string& value)
{
    m_Index = value;
}

short ChargeTable::GetChargePerUnit()
{
    return m_ChargePerUnit;
}

void ChargeTable::SetChargePerUnit(short value)
{
    m_ChargePerUnit = value;
}

std::string ChargeTable::ToString()
{
    std::string str = m_Index + " " + Helpers::IntToString(m_ChargePerUnit);
    return str;
}
