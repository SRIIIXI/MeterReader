#ifndef DLMS_ASSOCIATION_SHORT_NAME_H
#define DLMS_ASSOCIATION_SHORT_NAME_H

#include "DLMSObjectCollection.h"
#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
#include "DLMSObject.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSAssociationShortName
*/
class DLMSAssociationShortName : public DLMSObject
{
    DLMSObjectCollection m_ObjectList;
    std::string m_SecuritySetupReference;
    ByteBuffer m_Secret;

    int GetAccessRights(DLMSObject* pObj, DLMSServer* server, ByteBuffer& data);
    void UpdateAccessRights(DLMSVariant& buff);

public:
    //Constructor.
    DLMSAssociationShortName();

    DLMSObjectCollection& GetObjectList();

    ByteBuffer& GetSecret();

    void SetSecret(ByteBuffer& value);

    std::string& GetSecuritySetupReference();

    void SetSecuritySetupReference(std::string& value);

    //Get attribute values of object.
    void GetValues(
        std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    int GetDataType(
        int index,
        DLMS_DATA_TYPE& type);

    // Returns SN Association View.
    int GetObjects(
        DLMSSettings& settings,
        DLMSValueEventArg& e,
        ByteBuffer& data);

    int Invoke(
        DLMSSettings& settings,
        DLMSValueEventArg& e);

    int GetValue(
        DLMSSettings& settings,
        DLMSValueEventArg& e);

    int SetValue(
        DLMSSettings& settings,
        DLMSValueEventArg& e);
};
#endif
#endif
