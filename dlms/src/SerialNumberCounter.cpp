#include "ErrorCodes.h"
#include "SerialNumberCounter.h"
#include <stdio.h>

char toLower(char ch)
{
    if (ch <= 'Z' && ch >= 'A')
    {
        return ch - ('Z' - 'z');
    }
    return ch;
}

int SerialNumberCounter::FormatString(std::string& expression, std::string& value)
{
    if (expression.length() == 0)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    value = "";
    char ch;
    for (unsigned short pos = 0; pos != expression.length(); ++pos)
    {
        ch = toLower(expression[pos]);
        if (ch == '(' || ch == ')')
        {
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        // Is white space.
        if (ch == ' ')
        {
            continue;
        }
        else
        {
            value.push_back(ch);
        }
    }
    return 0;
}

int SerialNumberCounter::GetValues(std::string& expressions, std::vector<std::string>& values)
{
    int last = 0, index = 0;
    int count = (int)expressions.size();
    char ch;
    std::string tmp;
    for (int pos = 0; pos != count; ++pos)
    {
        ch = expressions[pos];
        switch (ch)
        {
        case '%':
        case '+':
        case '-':
        case '*':
        case '/':
            values.push_back(expressions.substr(last, index - last));
            tmp = ch;
            values.push_back(tmp);
            last = index + 1;
            break;
        default:
            break;
        }
        ++index;
    }
    if (index != last)
    {
        values.push_back(expressions.substr(last, index - last));
    }
    return 0;
}

int SerialNumberCounter::GetValue(std::string& value, int sn)
{
    if (value == "sn")
    {
        return sn;
    }
#if _MSC_VER > 1000
    if (sscanf_s(value.c_str(), "%d", &sn) == 1)
#else
    if (sscanf(value.c_str(), "%d", &sn) == 1)
#endif
    {
        return sn;
    }
    return -1;
}

/// <summary>
/// Count serial number using formula.
/// </summary>
/// <param name="sn">Serial number</param>
/// <param name="formula">Formula to used.</param>
/// <returns></returns>
int SerialNumberCounter::Count(unsigned long sn, const char* formula)
{
    int ret;
    std::string str;
    std::vector<std::string> values;
    std::string tmp = formula;
    unsigned long value;
    if ((ret = FormatString(tmp, str)) != 0)
    {
        return -1;
    }

    if ((ret = GetValues(str, values)) != 0)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (values.size() % 2 == 0)
    {
        return -1;
    }
    str.clear();
    value = GetValue(values.at(0), sn);
    for (unsigned short index = 1; index != values.size(); index += 2)
    {
        str = values.at(index);
        if (str == "%")
        {
            value = value % GetValue(values.at(index + 1), sn);
        }
        else if (str == "+")
        {
            value = value + GetValue(values.at(index + 1), sn);
        }
        else if (str == "-")
        {
            value = value - GetValue(values.at(index + 1), sn);
        }
        else if (str == "*")
        {
            value = value * GetValue(values.at(index + 1), sn);
        }
        else if (str == "/")
        {
            value = value / GetValue(values.at(index + 1), sn);
        }
        else
        {
            //"Invalid serial number formula.
            return -1;
        }
    }
    return value;
}
