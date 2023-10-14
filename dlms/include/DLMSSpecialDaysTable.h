#ifndef DLMS_SPECIAL_DAYS_TABLE_H
#define DLMS_SPECIAL_DAYS_TABLE_H

#include "Ignore.h"

#ifndef DLMS_IGNORE_SPECIAL_DAYS_TABLE
#include "DLMSObject.h"
#include "DLMSSpecialDay.h"

class DLMSSpecialDaysTable : public DLMSObject
{
    std::vector<DLMSSpecialDay*> m_Entries;
public:
    //Constructor.
    DLMSSpecialDaysTable();

    //SN Constructor.
    DLMSSpecialDaysTable(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSSpecialDaysTable(std::string ln);

    ~DLMSSpecialDaysTable();

    std::vector<DLMSSpecialDay*>& GetEntries();

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    // Inserts a new entry in the table.
    // If a special day with the same index or with the same date as an already
    // defined day is inserted, the old entry will be overwritten.
    int Insert(DLMSClient* client, DLMSSpecialDay* entry, std::vector<ByteBuffer>& reply);

    // Deletes an entry from the table.
    int Delete(DLMSClient* client, DLMSSpecialDay* entry, std::vector<ByteBuffer>& reply);


    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    // Returns value of given attribute.
    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
