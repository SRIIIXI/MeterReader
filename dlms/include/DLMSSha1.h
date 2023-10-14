#ifndef DLMS_SHA1_H
#define DLMS_SHA1_H

#include "ByteBuffer.h"

//This class is used to handle SHA-1.
class DLMSSha1
{
private:
    static void Transform(unsigned long *block, unsigned int *digest, unsigned int *transforms);
    static void Update(ByteBuffer& data, unsigned int *digest, unsigned int *transforms);
    static int Final(ByteBuffer& data, unsigned int *digest, unsigned int *transforms, ByteBuffer& reply);

public:
    static int Encrypt(ByteBuffer& data, ByteBuffer& crypted);
};
#endif
