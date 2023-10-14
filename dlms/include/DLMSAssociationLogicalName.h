#ifndef DLMS_ASSOCIATION_LOGICAL_NAME_H
#define DLMS_ASSOCIATION_LOGICAL_NAME_H

#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
#include "DLMSObject.h"
#include "ApplicationContextName.h"
#include "DLMSContextType.h"
#include "AuthenticationMechanismName.h"
#include "DLMSObjectCollection.h"

class DLMSAssociationLogicalName : public DLMSObject
{
private:

    DLMS_ASSOCIATION_STATUS m_AssociationStatus;
    DLMSObjectCollection m_ObjectList;
    unsigned char m_ClientSAP;
    unsigned short m_ServerSAP;
    ApplicationContextName m_ApplicationContextName;
    DLMSContextType m_XDLMSContextInfo;
    AuthenticationMechanismName m_AuthenticationMechanismName;

    /**
    * Secret.
    */
    ByteBuffer m_Secret;

    std::string m_SecuritySetupReference;

    std::vector<std::pair<unsigned char, std::string> > m_UserList;

    std::pair<unsigned char, std::string> m_CurrentUser;

    void UpdateAccessRights(
        DLMSObject* pObj,
        DLMSVariant data);

    int GetAccessRights(
        DLMSObject* pItem,
        DLMSServer* server,
        ByteBuffer& data);

    // Returns LN Association View.
    int GetObjects(
        DLMSSettings& settings,
        DLMSValueEventArg& e,
        ByteBuffer& data);

    int GetUsers(
        DLMSSettings& settings,
        DLMSValueEventArg& e,
        ByteBuffer& data);
public:
    /**
     Constructor.
    */
    DLMSAssociationLogicalName();

    /**
     Constructor.
     ln: Logical Name of the object.
    */
    DLMSAssociationLogicalName(std::string ln);

    /**
     Destructor.
    */
    ~DLMSAssociationLogicalName();

    DLMSObjectCollection& GetObjectList();


    // Contains the identifiers of the COSEM client APs within the physical devices hosting these APs,
    // which belong to the AA modelled by the “Association LN” object.
    unsigned char GetClientSAP();
    void SetClientSAP(unsigned char value);

    // Contains the identifiers of the COSEM server (logical device) APs within the physical
    // devices hosting these APs, which belong to the AA modelled by the “Association LN” object.
    unsigned short GetServerSAP();
    void SetServerSAP(unsigned short value);

    ApplicationContextName& GetApplicationContextName();

    DLMSContextType& GetXDLMSContextInfo();

    AuthenticationMechanismName& GetAuthenticationMechanismName();

    ByteBuffer& GetSecret();

    void SetSecret(ByteBuffer& value);

    std::vector<std::pair<unsigned char, std::string> >& GetUserList();

    void SetUserList(std::vector<std::pair<unsigned char, std::string> >& value);

    std::pair<unsigned char, std::string>& GetCurrentUser();

    void SetCurrentUser(std::pair<unsigned char, std::string>& value);


    // Updates secret.
    int UpdateSecret(
        DLMSClient* client,
        std::vector<ByteBuffer>& reply);

    // Add user to user list.
    int AddUser(
        DLMSClient* client,
        unsigned char id,
        std::string name,
        std::vector<ByteBuffer>& reply);

    // Remove user fro user list.
    int RemoveUser(
        DLMSClient* client,
        unsigned char id,
        std::string name,
        std::vector<ByteBuffer>& reply);


    DLMS_ASSOCIATION_STATUS GetAssociationStatus();

    void SetAssociationStatus(DLMS_ASSOCIATION_STATUS value);

    std::string GetSecuritySetupReference();
    void SetSecuritySetupReference(std::string value);

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

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    int GetValue(DLMSSettings& settings, DLMSValueEventArg& e);
    int SetValue(DLMSSettings& settings, DLMSValueEventArg& e);
};
#endif
#endif
