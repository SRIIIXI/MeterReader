#ifndef DLMS_OBJECT_COLLECTION_H
#define DLMS_OBJECT_COLLECTION_H

#include <vector>
#include "DLMSObject.h"
#include "XmlWriterSettings.h"

class DLMSObjectCollection : public std::vector<DLMSObject*>
{
public:
    ~DLMSObjectCollection();

    DLMSObject* FindByLN(DLMS_OBJECT_TYPE type, std::string& ln);

    DLMSObject* FindByLN(DLMS_OBJECT_TYPE type, unsigned char ln[6]);

    DLMSObject* FindBySN(unsigned short sn);

    void GetObjects(DLMS_OBJECT_TYPE type, DLMSObjectCollection& items);

    void push_back(
        DLMSObject* item);

    void Free();

    std::string ToString();

    /**
    * Save COSEM objects to the file.
    *
    * fileName: File name.
    */
    //int Save(const char* fileName);

    /**
    * Save COSEM objects to the file.
    *
    * fileName: File name.
    * settings: XML write settings.
    */
    //int Save(const char* fileName,  XmlWriterSettings& settings);

    /**
    * Load COSEM objects from the file.
    *
    * fileName XML file name.
    */
    //int Load(const char* fileName);
};

#endif
