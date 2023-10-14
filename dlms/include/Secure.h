#ifndef SECURE_H
#define SECURE_H

#include "Enums.h"
#include "ByteBuffer.h"
#include "DLMSSettings.h"

class Secure
{
public:
    /**
    * Generates challenge.
    *
    * @param authentication
    *            Used authentication.
    * @return Generated challenge.
    */
    static int GenerateChallenge(DLMS_AUTHENTICATION authentication, ByteBuffer& challenge);
    static int GenerateChallenge(DLMS_AUTHENTICATION authentication, ByteBuffer& challenge, int len);

    /**
    * Chipher text.
    *
    * @param auth
    *            Authentication level.
    * @param data
    *            Text to chipher.
    * @param secret
    *            Secret.
    * @return Chiphered text.
    */
    static int SecureEncrypt(
        DLMSSettings& settings,
        Cipher* cipher,
        unsigned long ic,
        ByteBuffer& data,
        ByteBuffer& secret,
        ByteBuffer& reply);

    static int EncryptAesKeyWrapping(ByteBuffer& data, ByteBuffer& kek, ByteBuffer& reply);
    static int DecryptAesKeyWrapping(ByteBuffer& data, ByteBuffer& kek, ByteBuffer& reply);
};

#endif
