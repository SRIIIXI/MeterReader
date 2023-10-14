#include <string.h>
#include "Commodity.h"
#include "Helpers.h"

Commodity::Commodity()
{
    m_Type = DLMS_OBJECT_TYPE_NONE;
    memset(m_LogicalName, 0, 6);
    m_Index = 0;
}

DLMS_OBJECT_TYPE Commodity::GetType()
{
    return m_Type;
}

void Commodity::SetType(DLMS_OBJECT_TYPE value)
{
    m_Type = value;
}

unsigned char* Commodity::GetLogicalName()
{
    return m_LogicalName;
}

void Commodity::SetLogicalName(unsigned char* value)
{
    memcpy(m_LogicalName, value, 6);
}

unsigned char Commodity::GetIndex()
{
    return m_Index;
}

void Commodity::SetIndex(unsigned char value)
{
    m_Index = value;
}

std::string Commodity::ToString()
{
    std::string ln;
    std::string str = Helpers::IntToString(m_Type);
    str.append(" ");
    Helpers::GetLogicalName(m_LogicalName, ln);
    str.append(ln);
    str.append(" ");
    str = Helpers::IntToString(m_Index);
    return str;
}
