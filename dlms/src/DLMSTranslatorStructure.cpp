#include <assert.h>
#include "DLMSTranslatorStructure.h"
#ifndef DLMS_IGNORE_XML_TRANSLATOR
#include "Helpers.h"

std::string DLMSTranslatorStructure::GetTag(unsigned long tag)
{
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML || m_OmitNameSpace)
    {
        return m_Tags[tag];
    }
    return "x:" + m_Tags[tag];
}

DLMS_TRANSLATOR_OUTPUT_TYPE DLMSTranslatorStructure::GetOutputType()
{
    return m_OutputType;
}

bool DLMSTranslatorStructure::GetOmitNameSpace()
{
    return m_OmitNameSpace;
}

int DLMSTranslatorStructure::GetOffset()
{
    return m_Offset;
}

void DLMSTranslatorStructure::SetOffset(int value)
{
    if (value < 0)
    {
        assert(0);
    }
    m_Offset = value;
}

std::string DLMSTranslatorStructure::GetDataType(DLMS_DATA_TYPE type)
{
    return GetTag(DATA_TYPE_OFFSET + (unsigned long)type);
}

bool DLMSTranslatorStructure::GetShowStringAsHex()
{
    return m_ShowStringAsHex;
}

void DLMSTranslatorStructure::SetShowStringAsHex(bool value)
{
    m_ShowStringAsHex = value;
}

bool DLMSTranslatorStructure::GetComments()
{
    return m_Comments;
}

void DLMSTranslatorStructure::SetComments(bool value)
{
    m_Comments = value;
}

bool DLMSTranslatorStructure::GetIgnoreSpaces()
{
    return m_IgnoreSpaces;
}

void DLMSTranslatorStructure::SetIgnoreSpaces(bool value)
{
    m_IgnoreSpaces = value;
}

DLMSTranslatorStructure::DLMSTranslatorStructure(
    DLMS_TRANSLATOR_OUTPUT_TYPE type,
    bool omitNameSpace,
    bool numericsAshex,
    bool hex,
    bool comments,
    std::map<unsigned long, std::string> list)
{
    m_OutputType = type;
    m_OmitNameSpace = omitNameSpace;
    m_ShowNumericsAsHex = numericsAshex;
    m_ShowStringAsHex = hex;
    m_Comments = comments;
    m_Tags = list;
    m_IgnoreSpaces = false;
    m_Offset = 0;
}

std::string DLMSTranslatorStructure::ToString()
{
    return m_Sb.str();
}

void DLMSTranslatorStructure::AppendSpaces()
{
    if (m_IgnoreSpaces)
    {
        m_Sb << ' ';
    }
    else
    {
        for (int pos = 0; pos != m_Offset; ++pos)
        {
            m_Sb << "  ";
        }
    }
}

void DLMSTranslatorStructure::AppendLine(std::string str)
{
    if (m_IgnoreSpaces)
    {
        m_Sb << str;
    }
    else
    {
        AppendSpaces();
        m_Sb << str;
        m_Sb << "\r\n";
    }
}

void DLMSTranslatorStructure::AppendLine(unsigned long tag, std::string name, DLMSVariant& value)
{
    AppendLine(GetTag(tag), name, value);
}

void DLMSTranslatorStructure::AppendLine(unsigned long tag, std::string name, std::string& value)
{
    AppendLine(GetTag(tag), name, value);
}

void DLMSTranslatorStructure::AppendLine(std::string tag, std::string name, DLMSVariant& value)
{
    std::string str;
    if (value.vt == DLMS_DATA_TYPE_UINT8)
    {
        IntegerToHex((long)value.bVal, 2, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT8)
    {
        IntegerToHex((long)value.cVal, 2, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_UINT16)
    {
        IntegerToHex((unsigned long)value.uiVal, 4, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT16)
    {
        IntegerToHex((long)value.iVal, 4, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_UINT32)
    {
        IntegerToHex((unsigned long)value.ullVal, 8, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT32)
    {
        IntegerToHex(value.lVal, 8, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_UINT64)
    {
        IntegerToHex(value.ullVal, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_INT64)
    {
        IntegerToHex(value.llVal, str);
    }
    else if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        str = Helpers::BytesToHex(value.byteArr, value.GetSize());
    }
    else
    {
        str = value.ToString();
    }
    AppendLine(tag, name, str);
}

void DLMSTranslatorStructure::StartComment(std::string comment)
{
    if (m_Comments)
    {
        AppendSpaces();
        m_Sb << "<!--";
        m_Sb << comment;
        m_Sb << '\r';
        m_Sb << '\n';
        ++m_Offset;
    }
}

void DLMSTranslatorStructure::AppendLine(std::string tag, std::string name, std::string& value)
{
    AppendSpaces();
    m_Sb << '<';
    m_Sb << tag;
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        m_Sb << ' ';
        if (name == "")
        {
            m_Sb << "Value";
        }
        else
        {
            m_Sb << name;
        }
        m_Sb << "=\"";
    }
    else
    {
        m_Sb << '>';
    }
    m_Sb << value;
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        m_Sb << "\" />";
    }
    else
    {
        m_Sb << "</";
        m_Sb << tag;
        m_Sb << '>';
    }
    m_Sb << '\r';
    m_Sb << '\n';
}

void DLMSTranslatorStructure::EndComment()
{
    if (m_Comments)
    {
        --m_Offset;
        AppendSpaces();
        m_Sb << "-->";
        m_Sb << '\r';
        m_Sb << '\n';
    }
}

void DLMSTranslatorStructure::AppendComment(std::string comment)
{
    if (m_Comments)
    {
        AppendSpaces();
        m_Sb << "<!--";
        m_Sb << comment;
        m_Sb << "-->";
        m_Sb << '\r';
        m_Sb << '\n';
    }
}

void DLMSTranslatorStructure::Append(std::string& value)
{
    m_Sb << value;
}

void DLMSTranslatorStructure::Append(unsigned long tag, bool start)
{
    if (start)
    {
        m_Sb << '<';
    }
    else
    {
        m_Sb << "</";
    }
    m_Sb << GetTag(tag);
    m_Sb << '>';
}

void DLMSTranslatorStructure::AppendStartTag(unsigned long tag, std::string name, std::string& value)
{
    AppendStartTag(tag, name, value, false);
}

void DLMSTranslatorStructure::AppendStartTag(unsigned long tag, std::string name, std::string& value, bool plain)
{
    AppendSpaces();
    m_Sb << '<';
    m_Sb << GetTag(tag);
    if (m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML && !name.empty())
    {
        m_Sb << ' ';
        m_Sb << name;
        m_Sb << "=\"";
        m_Sb << value;
        m_Sb << "\" >";
    }
    else
    {
        m_Sb << ">";
    }
    if (!plain)
    {
        m_Sb << "\r\n";
    }
    ++m_Offset;
}

void DLMSTranslatorStructure::AppendStartTag(unsigned long cmd)
{
    AppendStartTag(GetTag(cmd), false);
}

void DLMSTranslatorStructure::AppendStartTag2(unsigned long cmd, bool plain)
{
    AppendStartTag(GetTag(cmd), plain);
}

void DLMSTranslatorStructure::AppendStartTag(unsigned long cmd, unsigned long type)
{
    AppendStartTag(GetTag(cmd << 8 | type));
}

void DLMSTranslatorStructure::AppendStartTag(std::string tag, bool plain)
{
    AppendSpaces();
    m_Sb << "<";
    m_Sb << tag;
    m_Sb << ">";
    if (!plain)
    {
        m_Sb << "\r\n";
    }
    ++m_Offset;
}

void DLMSTranslatorStructure::AppendStartTag(std::string tag)
{
    AppendStartTag(tag, false);
}

void DLMSTranslatorStructure::AppendEndTag(unsigned long cmd, unsigned long type)
{
    AppendEndTag(cmd << 8 | type);
}

void DLMSTranslatorStructure::AppendEndTag(unsigned long tag)
{
    AppendEndTag(tag, false);
}

void DLMSTranslatorStructure::AppendEndTag(unsigned long tag, bool plain)
{
    --m_Offset;
    if (!plain)
    {
        AppendSpaces();
    }
    m_Sb << "</";
    m_Sb << GetTag(tag);
    m_Sb << ">\r\n";
}

void DLMSTranslatorStructure::AppendEndTag(std::string& tag)
{
    --m_Offset;
    AppendSpaces();
    m_Sb << "</";
    m_Sb << tag;
    m_Sb << ">\r\n";
}

void DLMSTranslatorStructure::AppendEmptyTag(unsigned long tag)
{
    AppendEmptyTag(m_Tags[tag]);
}

void DLMSTranslatorStructure::AppendEmptyTag(std::string tag)
{
    AppendSpaces();
    m_Sb << "<";
    m_Sb << tag;
    m_Sb << "/>\r\n";
}

void DLMSTranslatorStructure::Trim()
{
    SetXmlLength(GetXmlLength() - 2);
}

int DLMSTranslatorStructure::GetXmlLength()
{
    return (int)m_Sb.tellp();
}

void DLMSTranslatorStructure::SetXmlLength(int value)
{
    m_Sb.seekp(value);
}

int DLMSTranslatorStructure::IntegerToHex(long value, int desimals, std::string& result)
{
    return IntegerToHex(value, desimals, false, result);
}

int DLMSTranslatorStructure::IntegerToHex(unsigned long value, int desimals, std::string& result)
{
    return IntegerToHex(value, desimals, false, result);
}

int DLMSTranslatorStructure::IntegerToHex(long value, int desimals, bool forceHex, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        std::string f = "%." + Helpers::IntToString(desimals);
        f.append("X");
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, f.c_str(), value);
#else
        sprintf(tmp, f.c_str(), value);
#endif
        //Negative values are handled wrong.
        if (value < 0)
        {
            f.clear();
            f.append(tmp);
            result = f.substr(f.length() - desimals);
            return 0;
        }
    }
    else
    {
#if _MSC_VER > 1000
            sprintf_s(tmp, 20, "%ld", value);
#else
            sprintf(tmp, "%ld", value);
#endif
    }
    result = tmp;
    return 0;
}

int DLMSTranslatorStructure::IntegerToHex(unsigned long value, int desimals, bool forceHex, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
        std::string f = "%." + Helpers::IntToString(desimals);
        f.append("X");
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, f.c_str(), value);
#else
        sprintf(tmp, f.c_str(), value);
#endif
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%lu", value);
#else
        sprintf(tmp, "%lu", value);
#endif
    }
    result = tmp;
    return 0;
}

int DLMSTranslatorStructure::IntegerToHex(unsigned long value, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%.8X", value);
#else
        sprintf(tmp, "%.8lX", value);
#endif
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%lu", value);
#else
        sprintf(tmp, "%lu", value);
#endif
    }
    result = tmp;
    return 0;
}

int DLMSTranslatorStructure::IntegerToHex(long long value, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%.16llX", value);
#else
        sprintf(tmp, "%.16llX", value);
#endif
    }
    else
    {
#if defined(_WIN32) || defined(_WIN64)//Windows
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%lld", value);
#else
        sprintf(tmp, "%I64d", value);
#endif

#else
        sprintf(tmp, "%lld", value);
#endif
    }
    result = tmp;
    return 0;
}

int DLMSTranslatorStructure::IntegerToHex(unsigned long long value, std::string& result)
{
    if (m_ShowNumericsAsHex && m_OutputType == DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML)
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%.16I64X", value);
#else
        sprintf(tmp, "%.16llX", value);
#endif
    }
    else
    {
#if _MSC_VER > 1000
        sprintf_s(tmp, 20, "%I64u", value);
#else
        sprintf(tmp, "%llu", value);
#endif
    }
    result = tmp;
    return 0;
}
#endif //DLMS_IGNORE_XML_TRANSLATOR
