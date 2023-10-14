#ifndef XML_WRITER_SETTINGS
#define XML_WRITER_SETTINGS

class XmlWriterSettings
{
    bool m_Values;
    bool m_SkipDefaults;
public:
    //Constructor
    XmlWriterSettings();

    //Are attribute values also serialized.
    bool GetValues();

    //Are attribute values also serialized.
    void SetValues(bool value);

    //Are default values serialized.
    bool GetSkipDefaults();

    //Are default values serialized.
    void SetSkipDefaults(bool value);
};

#endif
