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




#if 1
CmdExecStatus eraseSTATE(void){
		uint8_t i ; 
	
		for(i=0;i<15;i++ ){
			sd_flash_page_erase( (uint32_t)(FLASH_START_INDEX + i) )  ; 
			nrf_delay_ms(30) ; 
		}
	
		
	return  CMD_EXEC_DONE ;
}
#else
CmdExecStatus eraseSTATE(void){
		uint8_t i ; 
	
		for(i=0;i<15;i++ ){
			sd_flash_page_erase( (uint32_t)(FLASH_START_INDEX + i) )  ; 
			nrf_delay_ms(30) ; 
		}
	
		
	return  CMD_EXEC_DONE ;
}



#endif
























