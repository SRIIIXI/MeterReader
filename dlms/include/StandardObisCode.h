#ifndef STANDARD_OBIS_CODE_H
#define STANDARD_OBIS_CODE_H

#include <string>
#include <vector>

class StandardObisCode
{
    std::vector< std::string > m_OBIS;
    std::string m_DataType;
    std::string m_UIDataType;
    std::string m_Interfaces;
    std::string m_Description;
public:
    // Constructor.
    StandardObisCode()
    {
    }

    // Constructor.
    StandardObisCode(std::vector< std::string > obis, std::string& desc,
                        std::string& interfaces, std::string& dataType);
    // OBIS code.
    std::vector< std::string >& GetOBIS();

    void SetOBIS(std::vector< std::string >& value);

    // OBIS code description.
    std::string& GetDescription();

    void SetDescription(std::string& value);

    // Interfaces that are using this m_OBIS code.
    std::string& GetInterfaces();
    void SetInterfaces(std::string& value);

    // Standard data types.
    std::string& GetDataType();
    void SetDataType(std::string value);

    // Standard data types.
    std::string& GetUIDataType();
    void SetUIDataType(std::string value);

};
#endif
