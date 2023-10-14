#ifndef DLMS_SCHEDULE_H
#define DLMS_SCHEDULE_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_SCHEDULE
#include "DLMSObject.h"
#include "DLMSScheduleEntry.h"

class DLMSSchedule : public DLMSObject
{
    std::vector<DLMSScheduleEntry*> m_Entries;
    int RemoveEntry(unsigned short index);
    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);
public:
    //Constructor.
    DLMSSchedule();

    //SN Constructor.
    DLMSSchedule(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSSchedule(std::string ln);

    //Destructor.
    ~DLMSSchedule();

    //
    // Add entry to entries list.
    //
    // client: DLMS client.
    // entry: Schedule entry.
    // reply: Action bytes.
    int Insert(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply);

    //
    // Remove entry from entries list.
    //
    // client: DLMS client.
    // entry: Schedule entry.
    // reply: Action bytes.
    int Delete(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply);

    //
    // Enable entry from entries list.
    //
    // client: DLMS client.
    // entry: Schedule entry.
    // reply: Action bytes.
    int Enable(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply);

    //
    // Disable entry from entries list.
    //
    // client: DLMS client.
    // entry: Schedule entries.
    // reply: Action bytes.
    int Disable(DLMSClient* client, DLMSScheduleEntry* entry, std::vector<ByteBuffer>& reply);

    // Get value of COSEM Data object.
    std::vector<DLMSScheduleEntry*>& GetEntries();

    // Set value of COSEM Data object.
    void SetEntries(std::vector<DLMSScheduleEntry*>& value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

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
