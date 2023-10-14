#ifndef AES_HEADER
#define AES_HEADER

/******************************************************************************/
#define AES_DECRYPTION  0       // whether AES decryption is supported
/******************************************************************************/

#include <string.h>
#include <stdint.h>

#define ENCRYPT         1       // specify whether we're encrypting
#define DECRYPT         0       // or decrypting

//#if defined(_MSC_VER)
//    #include <basetsd.h>
//    typedef UINT32 uint32_t;
//#else
//    #include <inttypes.h>
//#endif

typedef unsigned char uchar;    // add some convienent shorter types
typedef unsigned int uint;


/******************************************************************************
 *  AES_INIT_KEYGEN_TABLES : MUST be called once before any AES use
 ******************************************************************************/
void aes_init_keygen_tables( void );


/******************************************************************************
 *  AES_CONTEXT : cipher context / holds inter-call data
 ******************************************************************************/
typedef struct {
    int mode;           // 1 for Encryption, 0 for Decryption
    int rounds;         // keysize-based rounds count
    uint32_t *rk;       // pointer to current round key
    uint32_t buf[68];   // key expansion buffer
} aes_context;


/******************************************************************************
 *  AES_SETKEY : called to expand the key for encryption or decryption
 ******************************************************************************/
int aes_setkey( aes_context *ctx,       // pointer to context
                int mode,               // 1 or 0 for Encrypt/Decrypt
                const uchar *key,       // AES input key
                uint keysize );         // size in bytes (must be 16, 24, 32 for
		                        // 128, 192 or 256-bit keys respectively)
                                        // returns 0 for success

/******************************************************************************
 *  AES_CIPHER : called to encrypt or decrypt ONE 128-bit block of data
 ******************************************************************************/
int aes_cipher( aes_context *ctx,       // pointer to context
                const uchar input[16],  // 128-bit block to en/decipher
                uchar output[16] );     // 128-bit output result block
                                        // returns 0 for success

#endif /* AES_HEADER */
