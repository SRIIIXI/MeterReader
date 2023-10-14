#ifndef DLMS_SFSK_REPORTING_SYSTEM_LIST_H
#define DLMS_SFSK_REPORTING_SYSTEM_LIST_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_SFSK_REPORTING_SYSTEM_LIST
#include "DLMSObject.h"

class DLMSSFSKReportingSystemList : public DLMSObject
{
    /**
        * Contains the system titles of the server systems which have made a
        * DiscoverReport request and which have not already been registered.
        */
    std::vector<ByteBuffer> m_ReportingSystemList;
public:
    //Constructor.
    DLMSSFSKReportingSystemList();

    //SN Constructor.
    DLMSSFSKReportingSystemList(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSSFSKReportingSystemList(std::string ln);

    /**
    * @return Contains the system titles of the server systems which have made
    *         a DiscoverReport request and which have not already been
    *         registered.
    */
    std::vector<ByteBuffer>& GetReportingSystemList()
    {
        return m_ReportingSystemList;
    }

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
