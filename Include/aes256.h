#ifndef		__AES256_H__
#define		__AES256_H__
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define		AES256_LENGTH		32 


typedef struct
{
	uint8_t key[AES256_LENGTH]; 
    uint8_t enckey[AES256_LENGTH]; 
    uint8_t deckey[AES256_LENGTH];
} __attribute__((packed)) aes256_context; 


void tytt_aes256_encrypt(aes256_context *ctx, uint8_t *key,uint8_t* buf ) ;
void tytt_aes256_decrypt(aes256_context *ctx, uint8_t *key,uint8_t* buf ) ; 

#endif



