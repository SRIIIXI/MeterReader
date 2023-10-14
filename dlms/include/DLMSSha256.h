#ifndef DLMS_SHA_256_H
#define DLMS_SHA_256_H

#include "ByteBuffer.h"

//This class is used to handle SHA-256.
class DLMSSha256
{
private:
    static void Transform(unsigned int *h, const unsigned char *message, unsigned int block_nb);
    static int Final(unsigned int *h, unsigned char *block, unsigned char *digest, unsigned int len, unsigned int totalLen);
    static int Update(unsigned int *h, unsigned char *block, ByteBuffer&  data, unsigned int *len, unsigned int *totalLen);
public:
    static int Encrypt(ByteBuffer& data, ByteBuffer& crypted);
};
#endif
