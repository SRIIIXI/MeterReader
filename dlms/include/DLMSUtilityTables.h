#ifndef DLMS_Utility_Tables_H
#define DLMS_Utility_Tables_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_UTILITY_TABLES
#include "DLMSObject.h"

class DLMSUtilityTables : public DLMSObject
{
    unsigned short m_TableId;
    ByteBuffer m_Buffer;
public:
    //Constructor.
    DLMSUtilityTables();

    //SN Constructor.
    DLMSUtilityTables(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSUtilityTables(std::string ln);

    // Get table id.
    unsigned short GetTableId();

    // Set table id.
    void SetTableId(unsigned short value);

    //Get byte buffer.
    ByteBuffer& GetBuffer();

    //Set byte buffer.
    void SetBuffer(ByteBuffer& value);

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
