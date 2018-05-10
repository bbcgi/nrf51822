#ifndef  __MY_UART_HID__

#define	 __MY_UART_HID__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "myUTILITY.h"
#include "packet_def.h"
#include "ble_nus.h"
#include "ble_flash.h"


extern void parse2HIDForm( uint8_t const* text , uint16_t len,TYTT_HID_t* hidArr  ) ;















#endif 



