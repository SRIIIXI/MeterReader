#ifndef XML_READER_H
#define XML_READER_H

#include <vector>
#include <string>

class XmlReader
{
    enum XML_EVENT_TYPE
    {
        XML_EVENT_TYPE_NONE,
        XML_EVENT_TYPE_START_ELEMENT,
        XML_EVENT_TYPE_END_ELEMENT,
        XML_EVENT_TYPE_COMMENT,
        XML_EVENT_TYPE_SPACE,
        XML_EVENT_TYPE_CHARACTERS
    };
    XML_EVENT_TYPE m_EventType;
    int m_Index;
    int m_Size;
    std::string m_Name;
    std::string m_Value;
    char m_Buffer[256];
    FILE* m_f;
public:
    //Constructor.
    XmlReader(FILE* f);

    bool IsEOF();

    bool Read();

    bool IsStartElement();

    std::string& GetText();

    void GetNext();

    std::string& ReadElementContentAsString(const char* name);

    std::string& ReadElementContentAsString(const char* name, const char* defaultValue);

    int ReadElementContentAsInt(const char* name);

    int ReadElementContentAsInt(const char* name, int defaultValue);

    std::string& GetName();
};

#endif
