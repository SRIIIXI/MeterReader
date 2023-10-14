#ifndef XML_WRITER_H
#define XML_WRITER_H
#include <vector>
#include <string>
#include "DLMSObject.h"

class XmlWriter
{
    bool m_SkipDefaults;
    FILE* m_f;
    const char* NEW_LINE = "\n";
    std::vector<std::string> m_Elements;

    //
    // Append spaces to the buffer.
    // count: Amount of spaces.
    int AppendSpaces();

    int Write(const char* data);

    int WriteAttributeString(const char* name, const char* value);
    int WriteAttributeString(const char* name, const int value);
    int WriteArray(DLMSVariant& data);
public:
    //Constructor.
    XmlWriter(FILE* f, bool skipDefaults);
    int WriteStartDocument();

    int WriteStartElement(const char* name);

    int WriteElementString(const char* name, int value);
    int WriteElementString(const char* name, long value);
    int WriteElementString(const char* name, unsigned int value);
    int WriteElementString(const char* name, unsigned long value);
    int WriteElementString(const char* name, double value, double defaultValue);
    int WriteElementString(const char* name, const char* value);
    int WriteElementString(const char* name, DateTime& value);
    int WriteElementString(const char* name, Date& value);
    int WriteElementString(const char* name, Time& value);

    int WriteElementString(const char* name, const std::string& value);

    // Write End Element tag.
    int WriteEndElement(bool addSpaces);

    // Write End Element tag.
    int WriteEndElement();

    // Write End document tag.
    int WriteEndDocument();

    // Write object value to file.
    //
    // name: Object name.
    // value: Object value.
    int WriteElementObject(
        const char* name,
        DLMSVariant& value,
        DLMS_DATA_TYPE dt = DLMS_DATA_TYPE_NONE,
        DLMS_DATA_TYPE uiType = DLMS_DATA_TYPE_NONE);

    //Save DLMS object to the file.
    int Save(DLMSObject* obj);
};

#endif
