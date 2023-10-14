#ifndef CIPHER_H
#define CIPHER_H

#include "ByteBuffer.h"
#include "Enums.h"

class Cipher
{
private:
    DLMS_SECURITY m_Security;
    /**
    * System title.
    */
    ByteBuffer m_SystemTitle;

    /**
    * Is data encrypted.
    */
    bool m_Encrypt;

    /**
    *  Block cipher key.
    */
    ByteBuffer m_BlockCipherKey;
    /**
    *  Block Authentication key.
    */
    ByteBuffer m_AuthenticationKey;

    // Dedicated key.
    ByteBuffer m_DedicatedKey;

    /**
     * Frame counter. AKA Invocation counter.
     */
    unsigned long m_FrameCounter;

    DLMS_SECURITY_SUITE m_SecuritySuite;


    static int GetAuthenticatedData(
        DLMS_SECURITY security,
        ByteBuffer& authenticationKey,
        ByteBuffer& plainText,
        ByteBuffer& result);

    void Init(
        unsigned char* systemTitle,
        unsigned char count);

    static int Int(uint32_t* rk,
        const unsigned char* cipherKey,
        unsigned short keyBits);

    /*
    * Make xor for 128 bits.
    */
    static void Xor(
        unsigned char* dst,
        const unsigned char* src);

    static void shift_right_block(unsigned char* v);

    static void MultiplyH(
        const unsigned char* x,
        const unsigned char* y,
        unsigned char* z);

    /*
    * Count GHash.
    */
    static void GetGHash(
        const unsigned char* h,
        const unsigned char* x,
        int xlen,
        unsigned char* y);

    static void Init_j0(
        const unsigned char* iv,
        unsigned char len,
        const unsigned char* H,
        unsigned char* J0);

    static void Inc32(unsigned char* block);

    static void Gctr(
        unsigned int* aes,
        const unsigned char* icb,
        unsigned char* in,
        int len,
        unsigned char* out);

    static void AesGcmGctr(
        unsigned int* aes,
        const unsigned char* J0,
        unsigned char* in,
        int len,
        unsigned char* out);

    static void AesGcmGhash(
        const unsigned char* H,
        const unsigned char* aad,
        int aad_len,
        const unsigned char* crypt,
        int crypt_len,
        unsigned char* S);

    static void AesEncrypt(
        const unsigned int* rk,
        unsigned int Nr,
        const unsigned char* pt,
        unsigned char* ct);
public:
    /**
    * Constructor.
    */
    Cipher(ByteBuffer& systemTitle);

    /**
    * Constructor.
    */
    Cipher(const char* systemTitle);

    /**
    * Constructor.
    */
    Cipher(
        unsigned char* systemTitle,
        unsigned char count);

    /**
    * Destructor.
    */
    ~Cipher();


    /**
      * Encrypt PDU.
      *
      * tag
      *            Tag.
      * systemTitle
      *            System Title.
      * plainText
      *            Data to encrypt.
      * encrypted
      *            Encrypted data.
      */
    int Encrypt(
        DLMS_SECURITY security,
        DLMS_COUNT_TYPE type,
        unsigned long frameCounter,
        unsigned char tag,
        ByteBuffer& systemTitle,
        ByteBuffer& key,
        ByteBuffer& input,
        bool encrypt);

    /**
      * Decrypt data.
      *
      * systemTitle: System Title.
      * data: Decrypted data.
      * security: Used security level.
      * suite: Used security suite.
      * InvocationCounter: Invocation counter value.
      */
    int Decrypt(
        ByteBuffer& title,
        ByteBuffer& key,
        ByteBuffer& data,
        DLMS_SECURITY& security,
        DLMS_SECURITY_SUITE& suite,
        uint64_t& InvocationCounter);

    /*
     * Encrypt data using AES.
     *
     * data: Encrypted data.
     * offset: Data offset.
     * secret: Secret.
     */
    static int Aes1Encrypt(
        ByteBuffer& data,
        unsigned short offset,
        ByteBuffer& secret);

    /*
     * Decrypt data using AES.
     *
     * data: Encrypted data.
     * secret:  Secret.
     */
    static int Aes1Decrypt(
        ByteBuffer& data,
        ByteBuffer& secret);

    /**
     * Returns is ciphering used.
     */
    bool IsCiphered();

    /**
     * Returns used security.
     */
    DLMS_SECURITY GetSecurity();

    /**
    * value: Used security.
    */
    void SetSecurity(DLMS_SECURITY value);

    /*
    * Returns Used security suite.
    */
    DLMS_SECURITY_SUITE GetSecuritySuite();

    /*
     * value: Used security suite.
     */
    void SetSecuritySuite(DLMS_SECURITY_SUITE value);

    /**
     * Returns System title.
     */
    ByteBuffer& GetSystemTitle();

    /**
    *  value: System title.
    */
    void SetSystemTitle(ByteBuffer& value);

    /**
     * Returns Block cipher key.
     */
    ByteBuffer& GetBlockCipherKey();

    /**
    *  value: Block cipher key.
    */
    void SetBlockCipherKey(ByteBuffer& value);

    /**
     * Returns Authentication key.
     */
    ByteBuffer& GetAuthenticationKey();

    /**
     * value
     *            Authentication key.
     */
    void SetAuthenticationKey(ByteBuffer& value);

    /**
     * Returns Frame counter. AKA. Invocation counter.
     */
    unsigned long GetFrameCounter();

    void SetFrameCounter(unsigned long value);

    /**
     * Returns Invocation counter. AKA Frame counter.
     */
    unsigned long GetInvocationCounter();

    void SetInvocationCounter(unsigned long value);

    void Reset();

    /**
    * Returns Dedicated key.
    */
    ByteBuffer& GetDedicatedKey();

    /**
     * value
     *            Dedicated key.
     */
    void SetDedicatedKey(ByteBuffer& value);

};
#endif
