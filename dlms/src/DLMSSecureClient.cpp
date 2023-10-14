#include "DLMSSecureClient.h"
#include "Secure.h"

DLMSSecureClient::DLMSSecureClient(bool UseLogicalNameReferencing,
    int clientAddress,
    int serverAddress,
    //Authentication type.
    DLMS_AUTHENTICATION authentication,
    //Password if authentication is used.
    const char* password,
    DLMS_INTERFACE_TYPE intefaceType,
    const char* systemtitle) :
    DLMSClient(
        UseLogicalNameReferencing,
        clientAddress,
        serverAddress,
        authentication,
        password,
        intefaceType,
        systemtitle), m_Cipher(systemtitle)
{
    m_Settings.SetCipher(&m_Cipher);
}

DLMSSecureClient::~DLMSSecureClient()
{
}

Cipher* DLMSSecureClient::GetCiphering()
{
    return m_Settings.GetCipher();
}

int DLMSSecureClient::Encrypt(ByteBuffer& kek, ByteBuffer& data, ByteBuffer& reply)
{
    reply.Clear();
    if (kek.GetSize() < 16)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (kek.GetSize() % 8 != 0)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return Secure::EncryptAesKeyWrapping(data, kek, reply);
}

int DLMSSecureClient::Decrypt(ByteBuffer& kek, ByteBuffer& data, ByteBuffer& reply)
{
    reply.Clear();
    if (kek.GetSize() < 16)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (kek.GetSize() % 8 != 0)
    {
        //Key Encrypting Key.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (data.GetSize() < 16)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if (data.GetSize() % 8 != 0)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return Secure::DecryptAesKeyWrapping(data, kek, reply);
}
