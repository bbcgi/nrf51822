#ifndef		__SHA256_H__
#define		__SHA256_H__
#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


// #define uchar unsigned char // 8-bit byte
// #define uint unsigned int // 32-bit word



typedef struct
{
   uint8_t 		p_data[64];
   uint32_t 	datalen;
   uint32_t 	p_bitlen[2];
   uint32_t 	p_state[8];
}__attribute__((packed)) SHA256_CTX;





//int test_sha256(void);


//void print_hash(unsigned char hash[]);


void tyttSHA256(SHA256_CTX *ctx , uint8_t volatile text[],uint8_t hash[]) ; 



#endif 



