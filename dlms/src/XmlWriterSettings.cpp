#include "XmlWriterSettings.h"

XmlWriterSettings::XmlWriterSettings()
{
    m_Values = true;
    m_SkipDefaults = true;
}

bool XmlWriterSettings::GetValues()
{
    return m_Values;
}

void XmlWriterSettings::SetValues(bool value)
{
    m_Values = value;
}

//Are default values serialized.
bool XmlWriterSettings::GetSkipDefaults()
{
    return m_SkipDefaults;
}

//Are default values serialized.
void XmlWriterSettings::SetSkipDefaults(bool value)
{
    m_SkipDefaults = value;
}
