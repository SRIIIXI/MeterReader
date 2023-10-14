#ifndef DLMS_DLMS_SECURITY_SETUP_H
#define DLMS_DLMS_SECURITY_SETUP_H

#include "Ignore.h"
#ifndef DLMS_IGNORE_SECURITY_SETUP
#include "DLMSObject.h"
#include "DLMSCertificateInfo.h"

//Global key types.
typedef enum
{
    /**
    * Global unicast encryption key. <br>
    * Client and server uses this message to send Ephemeral Public Key to other
    * party.
    */
    DLMS_GLOBAL_KEY_TYPE_UNICAST_ENCRYPTION,
    /**
     * Global broadcast encryption key.
    */
    DLMS_GLOBAL_KEY_TYPE_BROADCAST_ENCRYPTION,
    /**
     * Authentication key.
    */
    DLMS_GLOBAL_KEY_TYPE_AUTHENTICATION,
    /**
     * Key Encrypting Key, also known as Master key.
    */
    DLMS_GLOBAL_KEY_TYPE_KEK
}DLMS_GLOBAL_KEY_TYPE;


class DLMSSecuritySetup : public DLMSObject
{
    DLMS_SECURITY_POLICY m_SecurityPolicy;
    DLMS_SECURITY_SUITE m_SecuritySuite;
    ByteBuffer m_ServerSystemTitle;
    ByteBuffer m_ClientSystemTitle;
    std::vector<DLMSCertificateInfo*> m_Certificates;
public:
    //Constructor.
    DLMSSecuritySetup();

    //SN Constructor.
    DLMSSecuritySetup(std::string ln, unsigned short sn);


    //LN Constructor.
    DLMSSecuritySetup(std::string ln);

    //Used security policy.
    DLMS_SECURITY_POLICY GetSecurityPolicy();

    //Used security policy.
    void SetSecurityPolicy(DLMS_SECURITY_POLICY value);

    //Used security suite.
    DLMS_SECURITY_SUITE GetSecuritySuite();

    //Used security suite.
    void SetSecuritySuite(DLMS_SECURITY_SUITE value);

    ByteBuffer GetClientSystemTitle();

    void SetClientSystemTitle(ByteBuffer& value);

    ByteBuffer GetServerSystemTitle();

    void SetServerSystemTitle(ByteBuffer& value);

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Imports an X.509 v3 certificate of a public key.
    int ImportCertificate(
        DLMSClient* client,
        ByteBuffer& key,
        std::vector<ByteBuffer>& reply);

    /////////////////////////////////////////////////////////////////////////
    /// Exports an X.509 v3 certificate from the server using entity information.
    /// 
    /// client: DLMS client that is used to generate action.
    /// entity: Certificate entity.
    /// type: Certificate type.
    /// systemTitle: System title.
    /// Returns Generated action.
    /////////////////////////////////////////////////////////////////////////
    int ExportCertificateByEntity(
        DLMSClient* client,
        DLMS_CERTIFICATE_ENTITY entity,
        DLMS_CERTIFICATE_TYPE type,
        ByteBuffer& systemTitle,
        std::vector<ByteBuffer>& reply);

    /////////////////////////////////////////////////////////////////////////
    /// Exports an X.509 v3 certificate from the server using serial information.
    /// 
    /// client: DLMS client that is used to generate action.
    /// serialNumber: Serial number.
    /// issuer: Issuer
    /// Returns Generated action.
    /////////////////////////////////////////////////////////////////////////
    int ExportCertificateBySerial(
        DLMSClient* client,
        ByteBuffer& serialNumber,
        ByteBuffer& issuer,
        std::vector<ByteBuffer>& reply);

    /////////////////////////////////////////////////////////////////////////
    /// Removes X.509 v3 certificate from the server using entity.
    /// 
    /// client: DLMS client that is used to generate action.
    /// entity: Certificate entity type.
    /// type: Certificate type.
    /// systemTitle: System title.
    /// Returns Generated action.
    /////////////////////////////////////////////////////////////////////////
    int RemoveCertificateByEntity(
        DLMSClient* client,
        DLMS_CERTIFICATE_ENTITY entity,
        DLMS_CERTIFICATE_TYPE type,
        ByteBuffer& systemTitle,
        std::vector<ByteBuffer>& reply);

    /////////////////////////////////////////////////////////////////////////
    /// Removes X.509 v3 certificate from the server using serial number.
    /// 
    /// client: DLMS client that is used to generate action.
    /// serialNumber: Serial number.
    /// issuer: Issuer.
    /// Returns Generated action.
    /////////////////////////////////////////////////////////////////////////
    int RemoveCertificateBySerial(
        DLMSClient* client,
        ByteBuffer& serialNumber,
        ByteBuffer& issuer,
        std::vector<ByteBuffer>& reply);

    int Invoke(DLMSSettings& settings, DLMSValueEventArg& e);

    //Start to use new keys after reply is generated.
    int ApplyKeys(DLMSSettings& settings, DLMSValueEventArg& e);


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

    //Get certificates.
    std::vector<DLMSCertificateInfo*>& GetCertificates();


    // Activates and strengthens the security policy.
    // client: DLMS client that is used to generate action.
    // security: New security level.
    // reply: Generated action.
    int Activate(
        DLMSClient* client,
        DLMS_SECURITY security,
        std::vector<ByteBuffer>& reply);

    // Updates one or more global keys.
    // client: DLMS client that is used to generate action.
    // kek: Master key, also known as Key Encrypting Key.
    // list: List of Global key types and keys.
    // reply: Generated action.
    int GlobalKeyTransfer(
        DLMSClient* client,
        ByteBuffer& kek,
        std::vector<std::pair<DLMS_GLOBAL_KEY_TYPE, ByteBuffer&> >& list,
        std::vector<ByteBuffer>& reply);
};
#endif
#endif
