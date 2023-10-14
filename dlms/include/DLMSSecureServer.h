#ifndef DLMS_SECURE_SERVER_H
#define DLMS_SECURE_SERVER_H

#include "DLMSServer.h"

class DLMSSecureServer : public DLMSServer
{
private:
    Cipher m_Cipher;
public:
    /////////////////////////////////////////////////////////////////////////////
    //Constructor
    /////////////////////////////////////////////////////////////////////////////
    DLMSSecureServer(
        bool UseLogicalNameReferencing = true,
        DLMS_INTERFACE_TYPE intefaceType = DLMS_INTERFACE_TYPE_HDLC);

#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param hdlc
    *            HDLC settings.
    */
    DLMSSecureServer(
        DLMSAssociationLogicalName* ln, DLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    DLMSSecureServer(
        DLMSAssociationLogicalName* ln, DLMSTcpUdpSetup* wrapper);

#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param hdlc
    *            HDLC settings.
    */
    DLMSSecureServer(
        DLMSAssociationShortName* sn, DLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    DLMSSecureServer(
        DLMSAssociationShortName* sn, DLMSTcpUdpSetup* wrapper);

#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME
    /////////////////////////////////////////////////////////////////////////////
    //Destructor.
    /////////////////////////////////////////////////////////////////////////////
    ~DLMSSecureServer();

    /**
     * @return Ciphering settings.
     */
    Cipher* GetCiphering();


    /////////////////////////////////////////////////////////////////////////////
    // Returns Key Encrypting Key, also known as Master key.
    /////////////////////////////////////////////////////////////////////////////
    ByteBuffer& GetKek();

    /////////////////////////////////////////////////////////////////////////////
    // value: Key Encrypting Key, also known as Master key.
    /////////////////////////////////////////////////////////////////////////////
    int SetKek(ByteBuffer& value);
};
#endif
