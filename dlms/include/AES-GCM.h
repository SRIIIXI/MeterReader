#ifndef AES_GCM
#define AES_GCM

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

int aes_gcm_encrypt(unsigned char* output, const unsigned char* input, int input_length, const unsigned char* key, const size_t key_len, const unsigned char * iv, const size_t iv_len);
int aes_gcm_decrypt(unsigned char* output, const unsigned char* input, int input_length, const unsigned char* key, const size_t key_len, const unsigned char * iv, const size_t iv_len);

#endif
