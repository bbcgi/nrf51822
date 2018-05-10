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



static uint16_t  isBlkDirty(uint32_t address  ) ; 
static uint16_t totalDataSize(void) ; 

//=============
//  Read all saved block in flash 
//==============
CmdExecStatus readSTATE(void){
				
		static uint16_t idx = 0 ; 
		uint8_t i ; 
		uint16_t	totalLength = 0 ; 
		uint8_t* readBUFF = NULL  ; 
	 
		totalLength = totalDataSize() ; 
	
		if( totalLength ){
				readBUFF = (uint8_t*)malloc(totalLength + 32); 
		}

		#define		DATA_START_OFFSET			0x0004 

		for( i=0;i< TOTAL_USER_BLK_IDX;i++ ){
					uint32_t	_addr = (uint32_t)(FLASH_START_ADDR + i*USER_BLOCK_SIZE) ; 
					uint32_t dataAddr = (uint32_t)_addr  ; 
					// dataAddr += DATA_START_OFFSET ; 
					uint16_t	tmpLen = 0 ; 
					tmpLen = isBlkDirty( dataAddr ) ; 
					if( tmpLen )
						memcpy( readBUFF , (uint8_t*)((uint32_t)dataAddr+(uint32_t)DATA_START_OFFSET) ,  tmpLen ) ; 
							 
					
	
		}
					
					
					
		do{
			uint8_t myBUFF[20] ; 
			memset( myBUFF,0x00,20 ) ; 
			memcpy( myBUFF , &readBUFF[idx] , 20 ) ;
								
			BLE_putstring(myBUFF ,20 ) ;
			idx += 20 ; 
								
		}while( idx < totalLength+2  ) ; 
		

		if( readBUFF ){
			free( readBUFF ) ; 
			readBUFF = NULL ; 
	}	
					
					
				
		
		
	
	

	return  CMD_EXEC_DONE ;
		
	
		
}


//================================
//  [Function]:
//     check flash block have data ?
//  [parameter]
//        [in] address : flash address ;
//				
//	[Returns]
//           length : if blk dirty 
//====================================
static uint16_t  isBlkDirty(uint32_t address  ) {
		uint16_t* ptr_read = 	(uint16_t*)address ; 
	

	
		return	( *ptr_read++ == DIRTY )?(*ptr_read):0 ; 
	
	

}

static uint16_t totalDataSize(void){
		uint16_t totalLen = 0 ; 
		uint8_t i ; 
		
		for( i=0;i< TOTAL_USER_BLK_IDX;i++ ){
				uint16_t 	tmpLen =  0  ; 
				uint32_t	_addr = (uint32_t)(FLASH_START_ADDR + i*USER_BLOCK_SIZE) ; 
				tmpLen = isBlkDirty( _addr ) ;

				if( tmpLen ){
						totalLen += tmpLen ; 
				}
		}
		
		return totalLen ; 
}
