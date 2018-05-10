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


//==============================================
// writeSTATE.c is represented for write state change 
// when APP send Account/PWD data to F/W  and saved into 
// flash 
//
// write state include: write/erase/re-write  
//========================================

extern void launchKEYBOARD(void) ; 

static uint32_t isFlashAvailable( void) ; 


#define			WRITE_INIT				0
#define			QUEUE_TO_BUFF			1


static uint16_t* _queue = NULL ; 
CmdExecStatus writeSTATE(void){
		

		
		static uint32_t idx  = 0 ; 
		static uint32_t	wrADDR = 0x0000UL ; 
		static uint8_t write_state = WRITE_INIT ; 
	
	
		if( write_state== WRITE_INIT ){
		//	
			
			idx  = 0 ; 
			wrADDR = isFlashAvailable() ;
			
			if( wrADDR ){
			//	BLE_MAIN_PKT_t* p_main = (BLE_MAIN_PKT_t*)tyttDATA._uint8BUFF ; 
				uint16_t _length = tyttDATA._uint16BUFF[2] ; 
			
				_queue = (uint16_t*)malloc( _length + 4 ) ; 
				if( _queue )
				{
						
						_queue[0] = DIRTY ; 
						_queue[1]= _length ; 
				}
				else{
					return CMD_EXEC_DONE ; 
				}
			
				idx = 2 ; 
				write_state = QUEUE_TO_BUFF ;
			}
			
				return  CMD_EXEC_REPEAT ;
			
			
			
			
		}
		else if( write_state == QUEUE_TO_BUFF ){
						//  got end packet the data is finishing save to flash
				if( tyttDATA._uint8BUFF[0]=='E' && tyttDATA._uint8BUFF[1]=='n' && tyttDATA._uint8BUFF[2]=='d' && tyttDATA._uint8BUFF[3]=='W' ){
						
						if( wrADDR ){
								uint32_t  cnt = _queue[1]/4 + 2 ;
							                   
								sd_flash_write((uint32_t*)wrADDR , (uint32_t*)_queue,cnt ) ; 
                    
              nrf_delay_ms(99) ;

			
						
							write_state = 0 ;
							idx = 0 ; 
							wrADDR = 0x0000UL ; 
							
							free( _queue ) ; 
							_queue = NULL ; 
						}
					  
						return  CMD_EXEC_DONE ;
				}
				else
				{
						memcpy((uint8_t*)&_queue[idx] , tyttDATA._uint8BUFF ,  20  ) ; 
						
						idx += 10 ; 	
					 
					return  CMD_EXEC_REPEAT ;
				}
		}
	
		
		
			
					return  CMD_EXEC_REPEAT ;
}




static uint32_t isFlashAvailable( void){
    uint32_t i ;
    uint32_t _addr = (uint32_t)FLASH_START_ADDR ; 
    
	
    for( i=0;i<TOTAL_USER_BLK_IDX;i++ ){
			
				if(  (*(uint16_t*)_addr) == NOT_USED )
						return _addr ; 
				
				_addr = _addr + i*USER_BLOCK_SIZE ; 
    }
    
    return 0 ; 
}















