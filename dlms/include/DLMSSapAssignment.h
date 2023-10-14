#ifndef DLMS_SAP_ASSIGNMENT_H
#define DLMS_SAP_ASSIGNMENT_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_SAP_ASSIGNMENT
#include "DLMSObject.h"

class DLMSSapAssignment : public DLMSObject
{
    std::map<int, std::string > m_SapAssignmentList;
    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);
public:
    /**
     Constructor.
    */
    DLMSSapAssignment();

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    DLMSSapAssignment(std::string ln);

    /**
     Constructor.
     @param sn Short Name of the object.
    */
    DLMSSapAssignment(std::string ln, unsigned short sn);

    std::map<int, std::string >& GetSapAssignmentList();

    void SetSapAssignmentList(std::map<int, std::string >& value);

    //Add new SAP item.
    int AddSap(DLMSClient* client, uint16_t id, std::string& name, std::vector<ByteBuffer>& reply);

    //Remove SAP item
    int RemoveSap(DLMSClient* client, std::string& name, std::vector<ByteBuffer>& reply);


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

    /*
     * Set value of given attribute.
     */
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
