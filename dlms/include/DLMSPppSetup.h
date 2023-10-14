#ifndef DLMS_PPP_SETUP_H
#define DLMS_PPP_SETUP_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_PPP_SETUP

#include "DLMSPppSetupIPCPOption.h"
#include "DLMSPppSetupLcpOption.h"
/**
PPP Authentication Type
 */
enum PPP_AUTHENTICATION_TYPE
{
    /**
     No authentication.
    */
    PPP_AUTHENTICATION_TYPE_NONE = 0,
    /**
     PAP Login
    */
    PPP_AUTHENTICATION_TYPE_PAP = 1,
    /**
     CHAP-algorithm
    */
    PPP_AUTHENTICATION_TYPE_CHAP = 2
};

class DLMSPppSetup : public DLMSObject
{
    std::vector<DLMSPppSetupIPCPOption> m_IPCPOptions;
    std::string m_PHYReference;
    std::vector<DLMSPppSetupLcpOption> m_LCPOptions;
    ByteBuffer m_UserName;
    ByteBuffer m_Password;
    PPP_AUTHENTICATION_TYPE m_Authentication;

public:
    //Constructor.
    DLMSPppSetup();

    //SN Constructor.
    DLMSPppSetup(std::string ln, unsigned short sn);

    //LN Constructor.
    DLMSPppSetup(std::string ln);

    PPP_AUTHENTICATION_TYPE GetAuthentication();
    void SetAuthentication(PPP_AUTHENTICATION_TYPE value);

#ifdef UNICODE
    //This fixes UNICODE for Windows.
    /**
    PPP authentication procedure user name.
    */
    ByteBuffer& GetUserNameW();
#endif
    /**
    PPP authentication procedure user name.
    */
    ByteBuffer& GetUserNamePPP();
    void SetUserNamePPP(ByteBuffer& value);

    /**
     PPP authentication procedure password.
    */
    ByteBuffer& GetPasswordPPP();
    void SetPasswordPPP(ByteBuffer value);

    std::string GetPHYReference();
    void SetPHYReference(std::string value);

    std::vector<DLMSPppSetupLcpOption>& GetLCPOptions();
    std::vector<DLMSPppSetupIPCPOption>& GetIPCPOptions();

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
