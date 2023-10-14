#include <string.h>
#include "XmlReader.h"

XmlReader::XmlReader(FILE* f)
{
    m_f = f;
    m_Buffer[0] = 0;
    m_EventType = XML_EVENT_TYPE_NONE;
    m_Index = 0;
    m_Size = 0;
}

bool XmlReader::IsEOF()
{
    if (m_Size == m_Index)
    {
        m_Index = m_Size = 0;
        m_Buffer[0] = 0;
        char* s = fgets(m_Buffer, sizeof(m_Buffer), m_f);
        if (s == NULL)
        {
            return true;
        }
        m_Size = (int) strlen(m_Buffer);
    }
    return false;
}

bool XmlReader::Read()
{
    if (m_Index < m_Size)
    {
        if (m_Buffer[m_Index] == ' ')
        {
            ++m_Index;
            return true;
        }
        if (m_Buffer[m_Index] == '\n')
        {
            ++m_Index;
            return true;
        }
    }
    m_Index = m_Size = 0;
    return false;
}

bool XmlReader::IsStartElement()
{
    if (m_Buffer[m_Index] == '<' && m_Buffer[m_Index + 1] != '?')
    {
        char* s = strstr(m_Buffer + m_Index, ">");
        if (s != NULL)
        {
            m_Name.clear();
            m_Name.append(m_Buffer + m_Index + 1, s - m_Buffer - m_Index - 1);
            m_Index = (int)(s - m_Buffer + 1);
            return true;
        }
    }
    return false;
}

std::string& XmlReader::GetText()
{
    char tag[256];
#if _MSC_VER > 1000
    strcpy_s(tag, sizeof(tag), "</");
    strcat_s(tag, m_Name.c_str());
    strcat_s(tag, ">");
#else
    strcpy(tag, "</");
    strcat(tag, m_Name.c_str());
    strcat(tag, ">");
#endif
    char* s = strstr(m_Buffer + m_Index, tag);
    m_Value.append(m_Buffer + m_Index, s);
    m_Index = (int)(s - m_Buffer);
    Read();
    GetNext();
    return m_Value;
}

void XmlReader::GetNext()
{
    while (m_EventType == XML_EVENT_TYPE_COMMENT ||
        m_EventType == XML_EVENT_TYPE_SPACE ||
        m_EventType == XML_EVENT_TYPE_CHARACTERS)
    {
        Read();
    }
}

std::string& XmlReader::ReadElementContentAsString(const char* name)
{
    return ReadElementContentAsString(name, NULL);
}

std::string& XmlReader::ReadElementContentAsString(const char* name, const char* defaultValue)
{
    GetNext();
    m_Value.clear();
    if (m_Name == name)
    {
        return GetText();
    }
    if (defaultValue != NULL)
    {
        m_Value.append(defaultValue);
    }
    return m_Value;
}

int XmlReader::ReadElementContentAsInt(const char* name)
{
    return ReadElementContentAsInt(name, 0);
}

int XmlReader::ReadElementContentAsInt(const char* name, int defaultValue)
{
    GetNext();
    if (m_Name == name)
    {
        std::string str = GetText();
        int ret = atol(str.c_str());
        return ret;
    }
    return defaultValue;
}

std::string& XmlReader::GetName()
{
    return m_Name;
}
