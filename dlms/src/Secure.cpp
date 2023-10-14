#include "Secure.h"
#include "DLMSMd5.h"
#include "DLMSSha1.h"
#include "DLMSSha256.h"

#include <stdlib.h> //rand

int Secure::GenerateChallenge(DLMS_AUTHENTICATION authentication, ByteBuffer& challenge)
{
    // Random challenge is 8 to 64 bytes.
    // Texas Instruments accepts only 16 byte long challenge.
    // For this reason challenge size is 16 bytes at the moment.
    int len = 16;
    //int len = rand() % 58 + 8;
    unsigned char val;
    for (int pos = 0; pos != len; ++pos)
    {
        val = rand();
        challenge.SetUInt8(val);
    }
    return 0;
}

int Secure::GenerateChallenge(DLMS_AUTHENTICATION authentication, ByteBuffer& challenge, int len)
{
    unsigned char val;
    for (int pos = 0; pos != len; ++pos)
    {
        val = rand();
        challenge.SetUInt8(val);
    }
    return 0;
}


static const unsigned char WRAP_IV[] = { 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6, 0xA6 };
int Secure::EncryptAesKeyWrapping(ByteBuffer& data, ByteBuffer& kek, ByteBuffer& reply)
{
    ByteBuffer buf, buf2;
    //unsigned char buf[16] = { 0 };
    unsigned char n, j, i;

    if (kek.GetSize() != 16 || data.GetSize() != 16)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    reply.Capacity(24);
    reply.SetSize(24);
    reply.SetPosition(0);
    // Amount of 64-bit blocks.
    n = (unsigned char)(data.GetSize() >> 3);
    memcpy(reply.GetData(), WRAP_IV, 8);
    memcpy(reply.GetData() + 8, data.GetData(), data.GetSize());
    for (j = 0; j != 6; j++)
    {
        for (i = 1; i <= n; i++)
        {
            kek.SetPosition(0);
            buf2.SetSize(0);
            buf2.Set(&kek);
            buf.SetSize(0);
            buf.Set(reply.GetData(), 8);
            buf.Set(reply.GetData() + (8 * i), 8);
            Cipher::Aes1Encrypt(buf, 0, buf2);
            unsigned int t = n * j + i;
            for (int k = 1; t != 0; k++)
            {
                unsigned char v = (unsigned char)t;
                buf.GetData()[sizeof(WRAP_IV) - k] ^= v;
                t = (int)((unsigned int)t >> 8);
            }
            memcpy(reply.GetData(), buf.GetData(), 8);
            memcpy(reply.GetData() + (8 * i), buf.GetData() + 8, 8);
        }
    }
    return 0;
}

int Secure::DecryptAesKeyWrapping(ByteBuffer& data, ByteBuffer& kek, ByteBuffer& reply)
{
    unsigned char a[8];
    ByteBuffer buf, buf2;
    signed char j, i;
    unsigned char k, v, n;
    unsigned short t;
    // Amount of 64-bit blocks.
    n = (unsigned char)(data.GetSize() >> 3);
    if (kek.GetSize() != 16 || data.GetSize() != n * 8)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    reply.Capacity(16);
    reply.SetSize(16);
    reply.SetPosition(0);
    memcpy(a, data.GetData(), sizeof(WRAP_IV));
    memcpy(reply.GetData(), data.GetData() + sizeof(WRAP_IV), data.GetSize() - sizeof(WRAP_IV));
    if (--n == 0)
    {
        n = 1;
    }
    for (j = 5; j >= 0; j--)
    {
        for (i = n; i >= 1; i--)
        {
            buf2.SetSize(0);
            buf2.Set(&kek);
            buf.SetSize(0);
            buf.Set(a, sizeof(WRAP_IV));
            buf.Set(reply.GetData() + 8 * (i - 1), 8);
            t = n * j + i;
            for (k = 1; t != 0; k++)
            {
                v = (unsigned char)t;
                buf.GetData()[sizeof(WRAP_IV) - k] ^= v;
                t = (unsigned short)(t >> 8);
            }
            Cipher::Aes1Decrypt(buf, buf2);
            memcpy(a, buf.GetData(), 8);
            memcpy(reply.GetData() + 8 * (i - 1), buf.GetData() + 8, 8);
        }
    }
    if (memcmp(a, WRAP_IV, sizeof(WRAP_IV)) != 0)
    {
        reply.SetSize(16);
        return DLMS_ERROR_CODE_FALSE;
    }
    return 0;
}

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
int Secure::SecureEncrypt(
    DLMSSettings& settings,
    Cipher* cipher,
    unsigned long ic,
    ByteBuffer& data,
    ByteBuffer& secret,
    ByteBuffer& reply)
{
    int ret = 0, pos;
    reply.Clear();
    if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH)
    {
        ByteBuffer s;
        int len = data.GetSize();
        if (len % 16 != 0)
        {
            len += (16 - (data.GetSize() % 16));
        }
        if (secret.GetSize() > data.GetSize())
        {
            len = secret.GetSize();
            if (len % 16 != 0)
            {
                len += (16 - (secret.GetSize() % 16));
            }
        }
        s.Set(&secret);
        s.Zero(s.GetSize(), len - s.GetSize());
        reply.Set(&data);
        reply.Zero(reply.GetSize(), len - reply.GetSize());
        for (pos = 0; pos < len / 16; ++pos)
        {
            Cipher::Aes1Encrypt(reply, pos * 16, s);
        }
        return 0;
    }
    // Get server Challenge.
    ByteBuffer challenge;
    // Get shared secret
    if (settings.GetAuthentication() != DLMS_AUTHENTICATION_HIGH_GMAC &&
        settings.GetAuthentication() != DLMS_AUTHENTICATION_HIGH_SHA256)
    {
        challenge.Set(&data);
        challenge.Set(&secret);
    }
    if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_MD5)
    {
        return DLMSMD5::Encrypt(challenge, reply);
    }
    else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA1)
    {
        return DLMSSha1::Encrypt(challenge, reply);
    }
    else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_SHA256)
    {
        return DLMSSha256::Encrypt(secret, reply);
    }
    else if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
    {
        ByteBuffer& key = settings.GetCipher()->GetBlockCipherKey();

        ret = cipher->Encrypt(DLMS_SECURITY_AUTHENTICATION, DLMS_COUNT_TYPE_TAG, ic, 0, secret, key, data, true);

        if (ret == 0)
        {
            reply.SetUInt8(DLMS_SECURITY_AUTHENTICATION);
            reply.SetUInt32(ic);
            reply.Set(&data);
        }
    }
    return ret;
}
