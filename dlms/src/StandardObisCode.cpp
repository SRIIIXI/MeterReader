#include "StandardObisCode.h"

StandardObisCode::StandardObisCode(std::vector< std::string > obis,
    std::string& desc,
    std::string& interfaces, std::string& dataType)
{
    m_OBIS.clear();
    m_OBIS.insert(m_OBIS.end(), obis.begin(), obis.end());
    SetDescription(desc);
    SetInterfaces(interfaces);
    SetDataType(dataType);
}

std::vector< std::string >& StandardObisCode::GetOBIS()
{
    return m_OBIS;
}

void StandardObisCode::SetOBIS(std::vector< std::string >& value)
{
    m_OBIS.clear();
    m_OBIS.insert(m_OBIS.end(), value.begin(), value.end());
}

/**
 m_OBIS code description.
*/
std::string& StandardObisCode::GetDescription()
{
    return m_Description;
}
void StandardObisCode::SetDescription(std::string& value)
{
    m_Description = value;
}

/**
 m_Interfaces that are using this m_OBIS code.
*/
std::string& StandardObisCode::GetInterfaces()
{
    return m_Interfaces;
}
void StandardObisCode::SetInterfaces(std::string& value)
{
    m_Interfaces = value;
}

std::string& StandardObisCode::GetDataType()
{
    return m_DataType;
}

void StandardObisCode::SetDataType(std::string value)
{
    m_DataType = value;
}

std::string& StandardObisCode::GetUIDataType()
{
    return m_UIDataType;
}

void StandardObisCode::SetUIDataType(std::string value)
{
    m_UIDataType = value;
}
