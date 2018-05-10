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
#include "myUTILITY.h"
#include "myUART_HID.h"
#include "ble_nus.h"
#include "packet_def.h"




//
extern 	CmdExecStatus writeSTATE(void) ;  
extern 	CmdExecStatus eraseSTATE(void) ; 
extern	CmdExecStatus readSTATE(void) ; 
extern 	CmdExecStatus dynamicKBSTATE(void) ;
extern 	CmdExecStatus resetPWDState(void) ; 

typedef struct{
	uint16_t		cmd ; 
	pFUNC				execOneCmd ; 
}cmdMap_t ;


static cmdMap_t _cmdMAP[] = {
	{ FIRST_OPEN_APP , 	dynamicKBSTATE } , 
	{ RESET_PWD_CMD , 	resetPWDState } , 
	{ MAIN_WRITE_CMD , 	writeSTATE } , 
	{ MAIN_READ_CMD , 	readSTATE } , 
	{ MAIN_ERASE_CMD , 	eraseSTATE }  

};

#define	   FINITE_STATE_CNT  sizeof( _cmdMAP )/sizeof(cmdMap_t ) 

extern bool isBufferHaveData  ; 
 



extern void app_sched_execute(void) ; 
static pFUNC _cmdFUNC = NULL ; 
//======================================
//  
//  [function]: check first coming packet and disptch to appropriate function
//  [Parameters] 
//         	
//					[in/out] en : true ehrn UART has packet coming
//  
//================================================

static void cmdDispatch(   bool* en  ){
			if( *en == false ) 
				return  ; 
			
			if( _cmdFUNC )
				return ;
			
			else
			{
					uint16_t		main_cmd  = tyttDATA.mainPACKET.cmd ;
					uint8_t i ; 
				
					for(i=0;i<FINITE_STATE_CNT;i++){
						if( main_cmd == _cmdMAP[i].cmd ){
								_cmdFUNC = _cmdMAP[i].execOneCmd ;
									break ; 
							}	
						
					}
					// can't find valid cmd 
					if( i == FINITE_STATE_CNT )
					{
						*en  = false ; 
						
					}
					
			}
			
}
void tytt_system_state(void){
		
		static CmdExecStatus  status = CMD_EXEC_IDLE ;
		
		
		while(1){
		
				app_sched_execute() ; 
				cmdDispatch( &isBufferHaveData  ) ; 
				
				if(_cmdFUNC && isBufferHaveData){ 
						status = _cmdFUNC() ;
						isBufferHaveData = false ; 
				}

				if(status==CMD_EXEC_DONE){
						_cmdFUNC = NULL ; 
					 status = CMD_EXEC_IDLE ;
				}
		
	}
	
	
}




