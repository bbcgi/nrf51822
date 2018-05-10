#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "packet_def.h"

#include "sha256.h"
//#include "Double_Auth.h"


// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))








void sha256_transform(SHA256_CTX *ctx, uint8_t data[] ,uint32_t _SHA256_k[])
{  
   uint32_t  a,b,c,d,e,f,g,h,i,j,t1,t2,m[64];
      
   for (i=0,j=0; i < 16; ++i, j += 4)
      m[i] = (data[j] << 24) | (data[j+1] << 16) | (data[j+2] << 8) | (data[j+3]);
   for ( ; i < 64; ++i)
      m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];

   a = ctx->p_state[0];
   b = ctx->p_state[1];
   c = ctx->p_state[2];
   d = ctx->p_state[3];
   e = ctx->p_state[4];
   f = ctx->p_state[5];
   g = ctx->p_state[6];
   h = ctx->p_state[7];
   
   for (i = 0; i < 64; ++i)
	{
      //t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
			t1 = h + EP1(e) + CH(e,f,g) + _SHA256_k[i] + m[i];
		
      t2 = EP0(a) + MAJ(a,b,c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
   }
   
   ctx->p_state[0] += a;
   ctx->p_state[1] += b;
   ctx->p_state[2] += c;
   ctx->p_state[3] += d;
   ctx->p_state[4] += e;
   ctx->p_state[5] += f;
   ctx->p_state[6] += g;
   ctx->p_state[7] += h;
}  








void sha256_update(SHA256_CTX *ctx, uint8_t volatile data[], uint32_t len , uint32_t _SHA256_k[])
{  
   uint32_t  i;
   
   for (i=0; i < len; ++i)
	 { 
      ctx->p_data[ctx->datalen] = data[i]; 
      
			ctx->datalen++; 
      if (ctx->datalen == 64) 
			{ 
         sha256_transform(ctx,ctx->p_data ,  _SHA256_k);
         DBL_INT_ADD(ctx->p_bitlen[0],ctx->p_bitlen[1],512); 
         ctx->datalen = 0; 
      }  
   }  
}  

void sha256_final(SHA256_CTX *ctx, uint8_t hash[] , uint32_t _SHA256_k[] )
{  
//	uint8_t    j = 0 ; 
   uint32_t  i; 
   
   i = ctx->datalen; 
   
   // Pad whatever data is left in the buffer. 
   if (ctx->datalen < 56) 
	 { 
      ctx->p_data[i++] = 0x80; 
      while (i < 56) 
         ctx->p_data[i++] = 0x00; 
   }  
   else
   { 
      ctx->p_data[i++] = 0x80; 
      while (i < 64) 
         ctx->p_data[i++] = 0x00; 
      sha256_transform(ctx,ctx->p_data , _SHA256_k);
      memset(ctx->p_data,0,56); 
   }  
   
   // Append to the padding the total message's length in bits and transform. 
   DBL_INT_ADD(ctx->p_bitlen[0],ctx->p_bitlen[1],ctx->datalen * 8);
   ctx->p_data[63] = ctx->p_bitlen[0]; 
   ctx->p_data[62] = ctx->p_bitlen[0] >> 8; 
   ctx->p_data[61] = ctx->p_bitlen[0] >> 16; 
   ctx->p_data[60] = ctx->p_bitlen[0] >> 24; 
   ctx->p_data[59] = ctx->p_bitlen[1]; 
   ctx->p_data[58] = ctx->p_bitlen[1] >> 8; 
   ctx->p_data[57] = ctx->p_bitlen[1] >> 16;  
   ctx->p_data[56] = ctx->p_bitlen[1] >> 24; 
   sha256_transform(ctx,ctx->p_data ,_SHA256_k);
   
   // Since this implementation uses little endian byte ordering and SHA uses big endian,
   // reverse all the bytes when copying the final state to the output hash. 
   for (i=0; i < 4; ++i) 
	 { 
      hash[i]    = (ctx->p_state[0] >> (24-i*8)) & 0x000000ff; 
      hash[i+4]  = (ctx->p_state[1] >> (24-i*8)) & 0x000000ff; 
      hash[i+8]  = (ctx->p_state[2] >> (24-i*8)) & 0x000000ff;
      hash[i+12] = (ctx->p_state[3] >> (24-i*8)) & 0x000000ff;
      hash[i+16] = (ctx->p_state[4] >> (24-i*8)) & 0x000000ff;
      hash[i+20] = (ctx->p_state[5] >> (24-i*8)) & 0x000000ff;
      hash[i+24] = (ctx->p_state[6] >> (24-i*8)) & 0x000000ff;
      hash[i+28] = (ctx->p_state[7] >> (24-i*8)) & 0x000000ff;
   }  
}  




void tyttSHA256(SHA256_CTX *ctx , uint8_t volatile text[],uint8_t hash[]) {
	uint32_t cnt = (uint32_t)(sizeof(text)/sizeof(uint8_t)) ; 

	uint32_t SHA256_k[64] = {
		0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
		0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
		0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
		0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
		0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
		0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
		0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
		0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	};

	
	
	//sha256_init(ctx) ; 
	///======= init ==========
		 
   ctx->datalen = 0; 
   ctx->p_bitlen[0] = 0; 
   ctx->p_bitlen[1] = 0; 
	
	
	 
   ctx->p_state[0] = 0x6a09e667;
   ctx->p_state[1] = 0xbb67ae85;
   ctx->p_state[2] = 0x3c6ef372;
   ctx->p_state[3] = 0xa54ff53a;
   ctx->p_state[4] = 0x510e527f;
   ctx->p_state[5] = 0x9b05688c;
   ctx->p_state[6] = 0x1f83d9ab;
   ctx->p_state[7] = 0x5be0cd19;
	
	
//====== init ======

		
		
	
	
	
	
	sha256_update(ctx,text,cnt, SHA256_k );
  sha256_final(ctx,hash ,SHA256_k );
		
	
	
	
	
	
	
	
	

}


