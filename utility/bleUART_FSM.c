#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "packet_def.h"


 









#if 0
typedef enum{
    
        STATE_BLE_UART_IDLE ,
        STATE_READY
        STATE_WAIT_FOR_MEM, 
        STATE_BLE_UART_RECIEVE_START,
        STATE_BLE_UART_RECIEVING , 
        STATE_WRITE_TO_FLASH , 
        STATE_BLE_UART_READ , 
        STATE_BLE_UART_RECIEVE_COMPLETE 
    
    
}STATE_BLE_UART_t ; 

// static EVENT_BLE_UART_t  event_ble_uart = EVENT_BLE_UART_NO_DATA ;
static STATE_BLE_UART_t  curr_state_ble_uart = STATE_BLE_UART_IDLE ; 
static STATE_BLE_UART_t  pre_state_ble_uart = STATE_BLE_UART_IDLE ; 
static uint16_t          ble_pkt_total_length = 0 ;


STATE_BLE_UART_t ble_uart_IDLE(void){
    uint16_t  ctrlCMD = tyttDATA.ctrl.cmd ; 
    
    if( ctrlCMD == BLE_UART_WR_CMD ){
        return STATE_WAIT_FOR_MEM ;
    }
    else if( ctrlCMD == BLE_UART_RD_CMD ){
        return STATE_BLE_UART_READ ; 
    }        
     
}


void   ble_uart_Dispatch(bool* en){
    if( *en ==  false ){
        return ; 
    }
    else{
        *en  = false ;
        switch( curr_state_ble_uart ){
            case STATE_BLE_UART_IDLE:
                curr_state_ble_uart = ble_uart_IDLE() ; 
                break  ;
            case STATE_WAIT_FOR_MEM:
                break ; 
            case STATE_BLE_UART_RECIEVE_START:
                break ; 
            case STATE_BLE_UART_RECIEVING:
                break ; 
            case STATE_BLE_UART_RECIEVE_COMPLETE :
                break ; 
        }
        
         
    }
}


#endif


















