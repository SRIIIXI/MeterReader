#ifndef DLMS_MD5_H
#define DLMS_MD5_H

#include "ByteBuffer.h"

class DLMSMD5
{
private:
    // Constants .
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21
    static void Encode(unsigned char * output, unsigned int *input, unsigned int len);
    static void Transform(unsigned char* block, unsigned int *state);
    static int Update(unsigned char* data, unsigned int len, unsigned char* buffer, unsigned int* count, unsigned int* state);

public:
    static int Encrypt(ByteBuffer& data, ByteBuffer& crypted);
};
#endif
