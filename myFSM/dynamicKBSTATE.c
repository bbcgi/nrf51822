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
#include "myUTILITY.h"
#include "sha256.h"
#include "aes256.h"

typedef struct{
	uint16_t		iX ; 
	uint16_t		iY ; 
}__attribute__((packed)) COORD_t ;
typedef struct{
	uint8_t	idx	;
	uint8_t	idy ; 
}__attribute__((packed))INDEX_t  ; 

KEYBOARD_t m_keyBoard ;
static void coord2NUMBER(  uint8_t   volatile* pwdBUFF ,KEYBOARD_t const* p_keyBoard ) ; 
static bool isPWDValid(uint8_t   volatile*  , KEYBOARD_t const*  ) ;

uint8_t     randomKeyBoard[RANDOM_KB_LENGTH]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
};


 



void Generator_Random_Key(  uint8_t* kb) { 
	uint8_t	i ; 
	int32_t k ; 
	uint8_t c ; 
	
	
	for(i=0;i<RANDOM_KB_LENGTH;i++){
		uint8_t	_k ; 
		
		do{
			k = rand(); 
		}while( k  < 0 ) ; 
		
		k = k% RANDOM_KB_LENGTH ; 
		_k = (uint8_t)(k&0x0000FF) ; 
		c = kb[i] ; 
		kb[i] = kb[_k] ; 
		kb[_k] = c ; 
	}
	
	
	
}
typedef struct{
		uint16_t		_dirty ; 
		uint8_t			hash[32];
}__attribute__((packed)) TYTT_PWD_t ;


 
CmdExecStatus  dynamicKBSTATE(void){
	
	uint8_t   superPWD[4] ;
	uint16_t cmd = tyttDATA._uint16BUFF[1] ; 
	uint8_t txBUFFER[20] ; 


	switch( cmd )
	{
			
			case FIRST_OPEN_APP:
			{
		
				
				 
				
				memcpy( (uint8_t*)&m_keyBoard , &tyttDATA._uint8BUFF[4] , 16 ) ; 
				
				memset(txBUFFER , 0x00 , 20 ) ; 
				
				Generator_Random_Key( randomKeyBoard ) ; 
				memcpy( txBUFFER ,randomKeyBoard,16   ) ; 
				
			}
			return  CMD_EXEC_REPEAT ; 
		
	

		case GET_RND_KB_CMD:
			{
			
					memset( txBUFFER,0x00,20 ) ; 
					
					memcpy(txBUFFER ,randomKeyBoard,16 ) ; 
					//makeACKPKT( true , retBUFF ) ;
					BLE_putstring( txBUFFER, 20 ) ;
			}
			return  CMD_EXEC_REPEAT ; 
		case TOUCH_COORD_PKT:
			{
				uint16_t	buff__[10] ;
				
				 
				makeACKPKT( isPWDValid( superPWD , &m_keyBoard ) , buff__ ) ;
				
				BLE_putstring((uint8_t*)buff__ ,20 ) ;
			
				
			}
		
			return CMD_EXEC_DONE ; 
			
	
		
	}

	return CMD_EXEC_REPEAT ; 
}

static void saveNewPWDSHA256(uint8_t* resetPWD ,uint8_t  *volatile hash){
	uint32_t  _addr = (uint32_t)FLASH_SUPER_PWD_ADDR ; 

	uint16_t  u16HASH[17] ; 
	
	SHA256_CTX*  p_shaCTX = (SHA256_CTX*)malloc(sizeof(SHA256_CTX)/sizeof(uint8_t) ) ; 

	
		sd_flash_page_erase( FLASH_PWD_INDEX)  ; 
		nrf_delay_ms(33) ; 
	
		u16HASH[0] = DIRTY ; 
		tyttSHA256(p_shaCTX , resetPWD ,(uint8_t*)&u16HASH[1]) ;
		memcpy(hash ,(uint8_t*)&u16HASH[1] ,32 ) ; 
		free( p_shaCTX ) ;
		p_shaCTX = NULL ; 
		sd_flash_write((uint32_t*)_addr , (uint32_t*)u16HASH,10 ) ;
		nrf_delay_ms(33) ;
}



CmdExecStatus  resetPWDState(void){
	uint8_t  resetPWD[4] ; 

	uint16_t cmd = tyttDATA._uint16BUFF[1] ; 
	uint8_t txBUFFER[20] ; 
	
	switch(cmd){
		case RESET_PWD_CMD:
		{
				
				
				
				Generator_Random_Key( randomKeyBoard ) ; 
			
				memset( txBUFFER ,0x00,20 ) ; 
				memcpy(txBUFFER ,randomKeyBoard,16 ) ; 
					//makeACKPKT( true , retBUFF ) ;
				BLE_putstring( txBUFFER, 20 ) ;
			
		
			}
			
			return  CMD_EXEC_REPEAT ; 
			
			case TOUCH_COORD_PKT:
			{
				
				uint8_t  hash[32] ;
				 coord2NUMBER(resetPWD ,  &m_keyBoard  ) ;
			#ifdef	__DBG__
			{
				uint8_t i ; 
				uint8_t _buff[4] ; 
				
				uart_putstring("reset pwd:") ;
				ch2ASCII( (uint8_t const*)resetPWD , _buff ,4 ) ; 
				
				for( i=0;i<4;i++ ){
						uart_put(_buff[i] ) ; 
						uart_putstring(" , ") ; 
					}
				uart_putstring("\r\n") ; 
			}
			#endif
				//=======================================================
				//=========== after sha use the key to encrypt =========  
			  //=======================================================
				//saveNewPWDSHA256( resetPWD) ;
			saveNewPWDSHA256( resetPWD , hash) ; 
			//======= Load data from flash 
		/*	{
					uint8_t i ; 
					uint16_t  totalLen = 0 ; 
					for(i=0;i<TOTAL_USER_BLK_IDX;i++ )
					{
							uint32_t	_addr = (uint32_t)(FLASH_START_ADDR + i*USER_BLOCK_SIZE) ;
							
					}
			}
			*/
			//====== re encrypt the data 
				
			}
			return  CMD_EXEC_DONE ; 
			
			
	}
	
	
	return CMD_EXEC_REPEAT ; 
}

//================================
//  [Function]: figure out index/offset by coordinates
//     
//  [parameter]
//        [in] 	input : coordinate(x or y)
//				[in]	orig:   the original  x/y
//				[in]  _wh :   button width/height
//				[in]  space:  size between buttons
//
//[Returns] offset/index of  randomKeyBoard
//==========================================================


static uint8_t getINDEX( uint16_t input , uint16_t orig , uint16_t _wh , uint16_t space ){
	uint8_t		idx = 0 ; 	
	int16_t tmp = (int16_t)( input - orig) ; 
	
		
	while(1){
							
		tmp -= (int16_t)_wh ; 

		if(tmp > (int16_t)space ) {
		
			tmp -= (int16_t)space ; 
			idx++ ;
		}
		else{
			break ; 
		}
	
	}
	
	return idx ;  
}
//================================
//  [Function]:extract N - (x,y) from packet  and turn into number 
//     
//  [parameter]
//        [out] pwdBUFF : random number / password   Buffer
//				[in]  p_keyBoard: app layout 
//
//==========================================================
static void coord2NUMBER(  uint8_t   volatile* pwdBUFF ,KEYBOARD_t const* p_keyBoard ){
							 
			COORD_t	userINPUT[4] ; 
		
			
		
			
			COORD_t* p_coord = (COORD_t*)&tyttDATA._uint16BUFF[2] ; 
			uint8_t i ; 
	
	
			for( i=0;i<4;i++ ){
				userINPUT[i] = *p_coord++ ; 
			}
		
			

			i = 0 ; 
			
			for( i=0;i<4;i++ ){
					
					uint8_t __idx = 0 ;  
					uint8_t dX = 0 ; 
					uint8_t dY = 0 ; 
					
				
					dX = getINDEX( userINPUT[i].iX ,p_keyBoard->origX , p_keyBoard->x_width , p_keyBoard->x_space ) ; 
					dY = getINDEX( userINPUT[i].iY ,p_keyBoard->origY , p_keyBoard->y_height , p_keyBoard->y_space ) ;
					
					__idx =  ( (dX + dY*4)% RANDOM_KB_LENGTH ) ; 
					pwdBUFF[i] = randomKeyBoard[ __idx ] ; 
				  
				
			}
		
				
}



static bool isPWDValid(uint8_t   volatile* superPWD , KEYBOARD_t const* p_keyBoard )
{
		uint32_t pwd_addr = (uint32_t)FLASH_SUPER_PWD_ADDR ;
		uint8_t 		oldHASH[34] ; 
		uint8_t   	newHASH[34] ; 
		
	 
		SHA256_CTX* p_ctx = (SHA256_CTX*)malloc(sizeof(SHA256_CTX)/sizeof(uint8_t) ) ;
	
	
	
		memcpy(oldHASH , (uint8_t*)pwd_addr , 34  ) ; 
	
		coord2NUMBER(superPWD , p_keyBoard ) ;
	
		#ifdef	__DBG__
		{
				uint8_t i ; 
				uint8_t _buff[4] ; 
				
				uart_putstring("input pwd:") ;
				ch2ASCII( (uint8_t const*)superPWD , _buff ,4 ) ; 
				
				for( i=0;i<4;i++ ){
						uart_put(_buff[i] ) ; 
						uart_putstring(" , ") ; 
					}
				uart_putstring("\r\n") ; 
			}
			#endif
		newHASH[0] = 0x7F ;
		newHASH[1] = 0x7F ;
		tyttSHA256(p_ctx , superPWD ,&newHASH[2] ) ;
	
		free(p_ctx) ; 
		p_ctx = NULL ; 
//  int memcmp ( const void * ptr1, const void * ptr2, size_t num );
/*
	<0:	the first byte that does not match in both memory blocks has 
	a lower value in ptr1 than in ptr2 (if evaluated as unsigned char 
	values)
	---------------------------------------------------------------------
	
	0:  the contents of both memory blocks are equal
	------------------------------------------------------------
	>0 :
	the first byte that does not match in both memory blocks has a greater 
	value in ptr1 than in ptr2 (if evaluated as unsigned char values)
	---------------------------------------------------------
	
*/ 
		
	
		return memcmp( oldHASH ,newHASH ,34 )== 0  ; 
	
		
}


