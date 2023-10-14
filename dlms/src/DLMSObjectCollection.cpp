#include <stdio.h>
#include "DLMSObjectCollection.h"

//#include "DLMSConverter.h"

//#include "XmlWriter.h"
//#include "XmlReader.h"
//#include "DLMSObjectFactory.h"

DLMSObjectCollection::~DLMSObjectCollection()
{
}

DLMSObject* DLMSObjectCollection::FindByLN(DLMS_OBJECT_TYPE type, std::string& ln)
{
    unsigned char tmp[6];
    Helpers::SetLogicalName(ln.c_str(), tmp);
    for (DLMSObjectCollection::iterator it = this->begin(); it != end(); ++it)
    {
        if ((type == DLMS_OBJECT_TYPE_ALL || (*it)->GetObjectType() == type))
        {
            if (memcmp(tmp, (*it)->m_LN, 6) == 0)
            {
                return *it;
            }
        }
    }
    return NULL;
}

DLMSObject* DLMSObjectCollection::FindByLN(DLMS_OBJECT_TYPE type, unsigned char ln[6])
{
    for (DLMSObjectCollection::iterator it = this->begin(); it != end(); ++it)
    {
        if (type == DLMS_OBJECT_TYPE_ALL || (*it)->GetObjectType() == type)
        {
            if (memcmp(ln, (*it)->m_LN, 6) == 0)
            {
                return *it;
            }
        }
    }
    return NULL;
}

DLMSObject* DLMSObjectCollection::FindBySN(unsigned short sn)
{
    for (DLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        if ((*it)->GetShortName() == sn)
        {
            return *it;
        }
    }
    return NULL;
}

void DLMSObjectCollection::GetObjects(DLMS_OBJECT_TYPE type, DLMSObjectCollection& items)
{
    for (DLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        if ((*it)->GetObjectType() == type)
        {
            items.push_back(*it);
        }
    }
}

void DLMSObjectCollection::push_back(DLMSObject* item)
{
    std::vector<DLMSObject*>::push_back(item);
}

void DLMSObjectCollection::Free()
{
    for (DLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        delete (*it);
    }
    std::vector<DLMSObject*>::clear();
}

std::string DLMSObjectCollection::ToString()
{
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for (DLMSObjectCollection::iterator it = begin(); it != end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = (*it)->GetName().ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    return sb.str();
}

/*
int DLMSObjectCollection::Save(const char* fileName)
{
    XmlWriterSettings settings;
    return Save(fileName, settings);
}

int DLMSObjectCollection::Save(const char* fileName, XmlWriterSettings& settings)
{
    int ret;
    std::string ln;
#if defined(_WIN32) || defined(_WIN64)//Windows
    FILE* f = NULL;
    if (fopen_s(&f, fileName, "w") != 0)
    {
        return errno;
    }
#else
    FILE* f = fopen(fileName, "w");
    if (f == NULL)
    {
        return errno;
    }
#endif
    XmlWriter writer(f, settings.GetSkipDefaults());
    if ((ret = writer.WriteStartDocument()) == 0 &&
        (ret = writer.WriteStartElement("Objects")) == 0)
    {
        for (DLMSObjectCollection::iterator it = begin(); it != end(); ++it)
        {
            if ((ret = writer.WriteStartElement(DLMSConverter::ToString((*it)->GetObjectType()))) != 0)
            {
                break;
            }
            // Add SN
            if ((*it)->GetShortName() != 0)
            {
                if ((ret = writer.WriteElementString("SN", (*it)->GetShortName())) != 0)
                {
                    break;
                }
            }
            // Add LN
            (*it)->GetLogicalName(ln);
            if ((ret = writer.WriteElementString("LN", ln)) != 0)
            {
                break;
            }
            // Add Version
            if ((*it)->GetVersion() != 0)
            {
                if ((ret = writer.WriteElementString("Version", (*it)->GetVersion())) != 0)
                {
                    break;
                }
            }
            // Add description if given.
            std::string& d = (*it)->GetDescription();
            if (!d.empty())
            {
                if ((ret = writer.WriteElementString("Description", d)) != 0)
                {
                    break;
                }
            }
            if (settings.GetValues())
            {
                if ((ret = writer.Save(*it)) != 0)
                {
                    break;
                }
            }
            // Close object.
            if ((ret = writer.WriteEndElement()) != 0)
            {
                break;
            }
        }
        if (ret == 0)
        {
            if ((ret = writer.WriteEndElement()) != 0 ||
                (ret = writer.WriteEndDocument()) != 0)
            {

            }
        }
    }
    if (f != NULL)
    {
        fclose(f);
    }
    return ret;
}

int DLMSObjectCollection::Load(const char* fileName)
{
    int ret = 0;
    DLMSObject* obj = NULL;
    std::string  target;
    DLMS_OBJECT_TYPE type = DLMS_OBJECT_TYPE_NONE;
    #if defined(_WIN32) || defined(_WIN64)//Windows
    FILE* f = NULL;
    if (fopen_s(&f, fileName, "r") != 0)
    {
        return errno;
    }
    #else
    FILE* f = fopen(fileName, "r");
    if (f == NULL)
    {
        return errno;
    }
    #endif
    XmlReader reader(f);
    while (!reader.IsEOF())
    {
        if (reader.IsStartElement())
        {
            target = reader.GetName();
            if (target == "Objects")
            {
                // Skip.
                reader.Read();
            }
            else if (target.find("GXDLMS", 0, 5) == 0)
            {
                type = DLMSConverter::ValueOfObjectType(target.c_str());
                if (type == DLMS_OBJECT_TYPE_NONE)
                {
                    //Invalid object type.
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                reader.Read();
                obj = DLMSObjectFactory::CreateObject(type);
                this->push_back(obj);
            }
            else if (target == "SN")
            {
                if (obj != NULL)
                {
                    obj->SetShortName(reader.ReadElementContentAsInt("SN"));
                }
            }
            else if (target == "LN")
            {
                if (obj != NULL)
                {
                    if ((ret = DLMSObject::SetLogicalName(obj, reader.ReadElementContentAsString("LN"))) != 0)
                    {
                        break;
                    }
                    obj = NULL;
                }
            }
            else if (target == "Description")
            {
                if (obj != NULL)
                {
                    obj->SetDescription(reader.ReadElementContentAsString("Description"));
                }
            }
            else if (target == "Version")
            {
                if (obj != NULL)
                {
                    obj->SetVersion(reader.ReadElementContentAsInt("Version"));
                }
            }
            else
            {
                obj = NULL;
            }
        }
        else
        {
            reader.Read();
        }
    }
    if (f != NULL)
    {
        fclose(f);
    }
    return ret;
}
*/
