#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf51_bitfields.h"
#include "myUTILITY.h"
#include "packet_def.h"

//=====================
// [Function] ack to app is valid or not ?
//  
//==================
 
void makeACKPKT( volatile bool isVALID , uint16_t  volatile* buff ){
	
	int16_t a = 0 ;
	int16_t b = 0 ;
	int16_t c = 0 ;
	int32_t	delta = 0 ; 
	
	buff[0] = HDR_55_AA ; 
	buff[1] = ACK_CMD ; 
	
	if( isVALID ){
	
			while(1){
				a = (int16_t)( (rand()% 999) + 1); 
				b = (int16_t)( (rand()% 999) +1 ); 
				c = (int16_t)( (rand()%999) + 1  ); 
			
				if( a && b && c ){
						delta = (int32_t)( b*b - 4*a*c ) ;
						
					  if( delta > 0 )
							break ; 
				}
			}
			
	
	}
	else{
			while(1){
				a = (int16_t)( (rand()% 999) + 1); 
				b = (int16_t)( (rand()% 999) +1 ); 
				c = (int16_t)( (rand()%999) + 1  ); 
			
				if( a && b && c ){
						delta = (int32_t)( b*b - 4*a*c ) ;
						
					  if( delta <  0 )
							break ; 
				}
			}
	}
	
	buff[2] = (uint16_t)a ; 
	buff[3] = (uint16_t)b ; 
	buff[4] = (uint16_t)c ;
	buff[5] = 0x0000 ; 
	buff[6] = 0x0000 ;
	buff[7] = 0x0000 ;
	buff[8] = 0x0000 ;
	buff[9] = 0x0000 ;
	
	
}
