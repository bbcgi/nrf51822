#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_assert.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf51_bitfields.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_hids.h"
#include "ble_bas.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "boards.h"
#include "ble_sensorsim.h"
#include "app_scheduler.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_gpiote.h"
#include "device_manager.h"
#include "ble_error_log.h"
#include "app_button.h"
#include "pstorage.h"
#include "app_trace.h"

#include "nrf_nvmc.h"
#include "nrf_delay.h"
#include "packet_def.h"
#include "aes256.h"

#define 	F(x)   		(((x)<<1) ^ ((((x)>>7) & 1) * 0x1b))
#define 	FD(x)  		(((x) >> 1) ^ (((x) & 1) ? 0x8d : 0))








uint8_t tytt_rj_xtime(uint8_t x)  ; 

// #define rj_sbox(x)     sbox[(x)]
// #define rj_sbox_inv(x) sboxinv[(x)]
void tytt_aes_addRoundKey_cpy(uint8_t *buf, uint8_t *key, uint8_t *cpk) ; 
void tytt_aes_expandEncKey(uint8_t *k, uint8_t *rc , uint8_t const* _sBox ) ; 
void tytt_aes256_init(aes256_context *ctx, uint8_t *key,uint8_t const* rj_sbox ) ;
void tytt_aes_shiftRows(uint8_t *buf) ; 

void tytt_aes_shiftRows_inv(uint8_t *buf) ; 
void tytt_aes_mixColumns_inv(uint8_t *buf) ; 
void tytt_aes256_encrypt_ecb(aes256_context *ctx, uint8_t *buf, uint8_t const* _sBox) ; 
void tytt_aes_addRoundKey(uint8_t *buf, uint8_t *key) ; 
//void tytt_aes256_decrypt_ecb(aes256_context *ctx, uint8_t *buf) ; 

void tytt_aes_expandDecKey(uint8_t *k, uint8_t *rc , uint8_t const* _sBox ) ; 

void tytt_aes_subBytes_inv(uint8_t *buf , uint8_t const* _sboxinv ) ; 
void tytt_aes_subBytes(uint8_t *buf , uint8_t const* _sBox )  ; 

void tytt_aes256_decrypt_ecb(aes256_context *ctx, uint8_t *buf,uint8_t const* _sBox , uint8_t const* _sboxinv) ; 

void tytt_aes256_done(aes256_context *ctx)
{
    register uint8_t i;

    for (i = 0; i < sizeof(ctx->key); i++) 
        ctx->key[i] = ctx->enckey[i] = ctx->deckey[i] = 0;
} /* aes256_done */



void tytt_aes256_encrypt(aes256_context *ctx, uint8_t *key,uint8_t* buf ){
	uint8_t const sbox[256] = {
    		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    		0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    		0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    		0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    		0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    		0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    		0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    		0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    		0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    		0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    		0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    		0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    		0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    		0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    		0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    		0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    		0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	};
	
 
	
	
	tytt_aes256_init(ctx, key, sbox ) ; 
 	tytt_aes256_encrypt_ecb(ctx, buf , sbox );
	
}


void tytt_aes256_decrypt(aes256_context *ctx, uint8_t *key,uint8_t* buf ){
	 uint8_t const sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
	
 uint8_t const sboxinv[256] = {
			0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
			0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
			0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
			0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
			0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
			0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
			0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
			0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
			0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
			0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
			0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
			0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
			0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
			0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
			0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
			0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
			0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
			0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
			0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
			0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
			0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
			0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
			0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
			0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
			0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
			0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
			0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
			0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
			0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
			0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
			0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
			0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
	};
	
	
	tytt_aes256_init(ctx, key, sbox ) ; 
 	tytt_aes256_decrypt_ecb(ctx, buf,sbox ,  sboxinv ) ;
	
	tytt_aes256_done( ctx ) ; 

}





void tytt_aes_addRoundKey_cpy(uint8_t *buf, uint8_t *key, uint8_t *cpk)
{
    register uint8_t i = 16;

    while (i--)  
	{
		buf[i] ^= (cpk[i] = key[i]);
		cpk[16+i] = key[16 + i];
    }
	
} /* aes_addRoundKey_cpy */
void tytt_aes_expandEncKey(uint8_t *key , uint8_t *rc , uint8_t const* rj_sbox ){
	  register uint8_t i;

    	key[0] ^= rj_sbox[ key[29] ] ^ (*rc);
    	key[1] ^= rj_sbox[ key[30] ];
    	key[2] ^= rj_sbox[key[31] ];
    	key[3] ^= rj_sbox[key[28]];
    *rc = F( *rc);

    for(i = 4; i < 16; i += 4) { 
		key[i] ^= key[i-4] ;
		key[i+1] ^= key[i-3] ; 
        	key[i+2] ^= key[i-2] ;
		key[i+3] ^= key[i-1];
    }
	
    key[16] ^= rj_sbox[ key[12] ];
    key[17] ^= rj_sbox[ key[13] ];
    key[18] ^= rj_sbox[ key[14] ];
    key[19] ^= rj_sbox[ key[15] ];

    for(i = 20; i < 32; i += 4){ 

		key[i] ^= key[i-4] ; 
		key[i+1] ^= key[i-3] ; 
        	key[i+2] ^= key[i-2] ;
		key[i+3] ^= key[i-1];


    	}
}



void tytt_aes256_init(aes256_context *ctx, uint8_t *key,uint8_t const* rj_sbox ){
	uint8_t rcon = 1;
    	register uint8_t i;

	for (i = 0; i < sizeof(ctx->key); i++) 
		ctx->enckey[i] = ctx->deckey[i] = key[i];
    for (i = 8;--i;) 
		tytt_aes_expandEncKey(ctx->deckey, &rcon , rj_sbox);
		
	i = 0 ; 
}



void tytt_aes_shiftRows(uint8_t *buf)
{
    	register uint8_t i, j; /* to make it potentially parallelable :) */

    	i = buf[1]; 
	buf[1] = buf[5];
	buf[5] = buf[9];
	buf[9] = buf[13];
	buf[13] = i;
    	i = buf[10];
	buf[10] = buf[2]; 
	buf[2] = i;
    	j = buf[3]; 
	buf[3] = buf[15]; 
	buf[15] = buf[11]; 
	buf[11] = buf[7]; 
	buf[7] = j;
    	j = buf[14]; 
	buf[14] = buf[6]; 
	buf[6]  = j;
	

} /* aes_shiftRows */


void tytt_aes_subBytes(uint8_t *buf , uint8_t const* _sBox ) 
{
    	register uint8_t i = 16;

    	while (i--){

		buf[i] = _sBox[ buf[i] ];

	}
	
} 



void tytt_aes_shiftRows_inv(uint8_t *buf)
{
    	register uint8_t i, j; 

    	i = buf[1]; 
	buf[1] = buf[13]; 
	buf[13] = buf[9]; 
	buf[9] = buf[5]; 
	buf[5] = i;

	i = buf[2]; 
	buf[2] = buf[10]; 
	buf[10] = i;

	j = buf[3]; 
	buf[3] = buf[7]; 
	buf[7] = buf[11]; 
	buf[11] = buf[15]; 
	buf[15] = j;
    	j = buf[6]; 
	buf[6] = buf[14]; 
	buf[14] = j;

}
uint8_t tytt_rj_xtime(uint8_t x)  
{
    return (x & 0x80) ? ((x << 1) ^ 0x1b) : (x << 1);
} 

void tytt_aes_mixColumns(uint8_t *buf)
{
    register uint8_t i, a, b, c, d, e;

    for (i = 0; i < 16; i += 4)
    {
        	a = buf[i]; 
		b = buf[i + 1]; 
		c = buf[i + 2]; 
		d = buf[i + 3];
        	e = a ^ b ^ c ^ d;
        	buf[i] ^= e ^ tytt_rj_xtime(a^b);   
		buf[i+1] ^= e ^ tytt_rj_xtime(b^c);
        	buf[i+2] ^= e ^ tytt_rj_xtime(c^d); 
		buf[i+3] ^= e ^ tytt_rj_xtime(d^a);
    }
	
} 


void tytt_aes_mixColumns_inv(uint8_t *buf)
{
    register uint8_t i, a, b, c, d, e, x, y, z;

    for (i = 0; i < 16; i += 4)
    {
        	a = buf[i]; 
		b = buf[i + 1]; 
		c = buf[i + 2]; 
		d = buf[i + 3];
        	e = a ^ b ^ c ^ d;
        	z = tytt_rj_xtime(e);
        	x = e ^ tytt_rj_xtime(tytt_rj_xtime(z^a^c));  
		y = e ^ tytt_rj_xtime(tytt_rj_xtime(z^b^d));
        	buf[i] ^= x ^ tytt_rj_xtime(a^b);   
		buf[i+1] ^= y ^ tytt_rj_xtime(b^c);
        	buf[i+2] ^= x ^ tytt_rj_xtime(c^d); 
		buf[i+3] ^= y ^ tytt_rj_xtime(d^a);
    }

	
}


void tytt_aes256_encrypt_ecb(aes256_context *ctx, uint8_t *buf, uint8_t const* _sBox)
{
    uint8_t i, rcon;

    tytt_aes_addRoundKey_cpy(buf, ctx->enckey, ctx->key);
    for(i = 1, rcon = 1; i < 14; ++i)
    {
        tytt_aes_subBytes(buf,_sBox);
        tytt_aes_shiftRows(buf);
        tytt_aes_mixColumns(buf);
        
				if( i & 1 ) 
					tytt_aes_addRoundKey( buf, &ctx->key[16]);
        else {
		
					tytt_aes_expandEncKey(ctx->key, &rcon ,_sBox );
					tytt_aes_addRoundKey(buf, ctx->key );
				}
	}
    tytt_aes_subBytes(buf,_sBox);
    tytt_aes_shiftRows(buf);
    tytt_aes_expandEncKey(ctx->key, &rcon , _sBox); 
    tytt_aes_addRoundKey(buf, ctx->key);
}



void tytt_aes_addRoundKey(uint8_t *buf, uint8_t *key)
{
    register uint8_t i = 16;

    while (i--) 
		buf[i] ^= key[i];
} 

void aes_subBytes_inv(uint8_t *buf , uint8_t const* _sboxinv)
{
    register uint8_t i = 16;

    while (i--) 
		buf[i] = _sboxinv[ buf[i] ] ;
} 

void tytt_aes256_decrypt_ecb(aes256_context *ctx, uint8_t *buf,uint8_t const* _sBox , uint8_t const* _sboxinv)
{
    uint8_t i, rcon;

    tytt_aes_addRoundKey_cpy(buf, ctx->deckey, ctx->key);
    tytt_aes_shiftRows_inv(buf);
    tytt_aes_subBytes_inv(buf , _sboxinv);

    for (i = 14, rcon = 0x80; --i;)
    {
        if( ( i & 1 ) )           
        {
            tytt_aes_expandDecKey(ctx->key, &rcon , _sBox);
            tytt_aes_addRoundKey(buf, &ctx->key[16] );
        }
        else 
		tytt_aes_addRoundKey(buf, ctx->key);


	  	tytt_aes_mixColumns_inv(buf);
        tytt_aes_shiftRows_inv(buf);
        tytt_aes_subBytes_inv(buf , _sboxinv );
    }
	
    tytt_aes_addRoundKey( buf, ctx->key); 
} /* aes256_decrypt */


void tytt_aes_expandDecKey(uint8_t *k, uint8_t *rc , uint8_t const* _sBox ) 
{
    uint8_t i;

    for(i = 28; i > 16; i -= 4) k[i+0] ^= k[i-4], k[i+1] ^= k[i-3], 
        k[i+2] ^= k[i-2], k[i+3] ^= k[i-1];

    k[16] ^= _sBox [ k[12] ] ;
    k[17] ^= _sBox[ k[13] ];
    k[18] ^= _sBox[k[14] ];
    k[19] ^= _sBox[ k[15] ];

    for(i = 12; i > 0; i -= 4)  k[i+0] ^= k[i-4], k[i+1] ^= k[i-3],
        k[i+2] ^= k[i-2], k[i+3] ^= k[i-1];

    *rc = FD(*rc);
    k[0] ^= _sBox[k[29] ] ^ (*rc);
    k[1] ^= _sBox[k[30]];
    k[2] ^= _sBox[k[31]];
    k[3] ^= _sBox[k[28]];
} /* aes_expandDecKey */



void tytt_aes_subBytes_inv(uint8_t *buf , uint8_t const* _sboxinv )
{
    register uint8_t i = 16;

    while (i--) buf[i] = _sboxinv[buf[i] ];
} /* aes_subBytes_inv */

