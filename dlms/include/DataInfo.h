#ifndef DATA_INFO
#define DATA_INFO

#include "Ignore.h"
#include "Enums.h"

class DLMSTranslatorStructure;

//This class is used in DLMS data parsing.
class DataInfo
{
private:
    // Last array index.
    int m_Index;

    // Items count in array.
    int m_Count;
    // Object data type.
    DLMS_DATA_TYPE m_Type;
    // Is data parsed to the end.
    bool m_Complete;
#ifndef DLMS_IGNORE_XML_TRANSLATOR
    DLMSTranslatorStructure* m_xml;
#endif //DLMS_IGNORE_XML_TRANSLATOR
public:
    //Constructor.
    DataInfo()
#ifndef DLMS_IGNORE_XML_TRANSLATOR
        : m_xml(NULL)
#endif //DLMS_IGNORE_XML_TRANSLATOR
    {
        Clear();
    }

    // Get Last array index.
    int GetIndex()
    {
        return m_Index;
    }

    //Set Last array index.
    void SetIndex(int value)
    {
        m_Index = value;
    }

    // Get items count in array.
    int GetCount()
    {
        return m_Count;
    }

    // Set Items count in array.
    void SetCount(int value)
    {
        m_Count = value;
    }

    //Get Object data type.
    DLMS_DATA_TYPE GetType()
    {
        return m_Type;
    }

    // Set Object data type.
    void SetType(DLMS_DATA_TYPE value)
    {
        m_Type = value;
    }

    // Is all data parsed.
    bool IsComplete()
    {
        return m_Complete;
    }

    //Is all data parsed.
    void SetComplete(bool value)
    {
        m_Complete = value;
    }

    //Clear settings.
    void Clear()
    {
        m_Index = 0;
        m_Count = 0;
        m_Type = DLMS_DATA_TYPE_NONE;
        m_Complete = true;
    }

#ifndef DLMS_IGNORE_XML_TRANSLATOR
    void SetXml(DLMSTranslatorStructure* value)
    {
        m_xml = value;
    }

    DLMSTranslatorStructure* GetXml()
    {
        return m_xml;
    }
#endif
};

#endif
