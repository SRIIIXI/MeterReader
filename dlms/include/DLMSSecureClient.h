#ifndef DLMS_SECURE_CLIENT_H
#define DLMS_SECURE_CLIENT_H

#include "DLMSClient.h"

class DLMSSecureClient : public DLMSClient
{
private:
    Cipher m_Cipher;
public:
    /////////////////////////////////////////////////////////////////////////////
    //Constructor
    /////////////////////////////////////////////////////////////////////////////
    DLMSSecureClient(
        bool UseLogicalNameReferencing = true,
        int ClientAddress = 16,
        int ServerAddress = 1,
        //Authentication type.
        DLMS_AUTHENTICATION authentication = DLMS_AUTHENTICATION_NONE,
        //Password if authentication is used.
        const char* password = NULL,
        DLMS_INTERFACE_TYPE intefaceType = DLMS_INTERFACE_TYPE_HDLC,
        const char* systemtitle = NULL);

    /////////////////////////////////////////////////////////////////////////////
    //Destructor.
    /////////////////////////////////////////////////////////////////////////////
    ~DLMSSecureClient();

    /**
     * @return Ciphering settings.
     */
    Cipher* GetCiphering();

    /**
    * Encrypt data using Key Encrypting Key.
    *
    * @param kek
    *            Key Encrypting Key, also known as Master key.
    * @param data
    *            Data to encrypt.
    * @return Encrypt data.
    */
    static int Encrypt(
        ByteBuffer& kek,
        ByteBuffer& data,
        ByteBuffer& reply);

    /**
    * Decrypt data using Key Encrypting Key.
    *
    * @param kek
    *            Key Encrypting Key, also known as Master key.
    * @param data
    *            Data to decrypt.
    * @return Decrypted data.
    */
    static int Decrypt(
        ByteBuffer& kek,
        ByteBuffer& data,
        ByteBuffer& reply);
};
#endif
