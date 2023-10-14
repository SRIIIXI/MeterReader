#ifndef DLMS_SCRIPT_TABLE_H
#define DLMS_SCRIPT_TABLE_H

#include "Ignore.h"

#ifndef DLMS_IGNORE_SCRIPT_TABLE
#include "DLMSObject.h"
#include "DLMSScript.h"

class DLMSScriptTable : public DLMSObject
{
    std::vector<DLMSScript*> m_Scripts;

public:
    //Constructor.
    DLMSScriptTable();

    //SN Constructor.
    DLMSScriptTable(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSScriptTable(std::string ln);

    //Destructor.
    ~DLMSScriptTable();

    std::vector<DLMSScript*>& GetScripts();

    //Executes selected script.
    int Execute(DLMSClient* client, DLMSScript* script, std::vector<ByteBuffer>& reply);

    //Executes selected script by ID.
    int Execute(DLMSClient* client, unsigned short scriptId, std::vector<ByteBuffer>& reply);

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
