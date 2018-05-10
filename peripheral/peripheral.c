#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf51_bitfields.h"
#include "myUTILITY.h"
#include "boards.h"



void UART0_Config(bool hw){
        // Configure RX and TX pins.
    nrf_gpio_pin_set(TX_PIN_NUMBER);
    nrf_gpio_cfg_output(TX_PIN_NUMBER);
    nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_PULLUP);

    
    NRF_UART0->PSELTXD = TX_PIN_NUMBER ;
    NRF_UART0->PSELRXD = RX_PIN_NUMBER;
    
    if (hw)
    {
        nrf_gpio_cfg_output(CTS_PIN_NUMBER);
        nrf_gpio_cfg_input(RTS_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
        NRF_UART0->PSELCTS = RTS_PIN_NUMBER;
        NRF_UART0->PSELRTS = CTS_PIN_NUMBER;
        NRF_UART0->CONFIG  = (UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
    }
    
    
    NRF_UART0->BAUDRATE       = (UART_BAUDRATE_BAUDRATE_Baud115200 << UART_BAUDRATE_BAUDRATE_Pos);
    
    NRF_UART0->ENABLE         = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
    NRF_UART0->TASKS_STARTTX  = 1;
    NRF_UART0->TASKS_STARTRX  = 1;
    NRF_UART0->EVENTS_RXDRDY  = 0;     
    

  
}

void uart_put(uint8_t cr)
{
    NRF_UART0->TXD = cr;
    while (NRF_UART0->EVENTS_TXDRDY!=1)
    {
        // Do nothing.
    }
    NRF_UART0->EVENTS_TXDRDY=0;
}


void uart_putstring(const uint8_t* str)
{
		do{
			uart_put(*str) ; 
		}while(*str++ != '\0') ; 
  
}



uint8_t uart_get(void)
{
    uint8_t ch ; 
    while (NRF_UART0->EVENTS_RXDRDY != 1)
    {
        // Wait for RXD data to be received
    }
    
    ch =  (uint8_t)NRF_UART0->RXD;
    NRF_UART0->EVENTS_RXDRDY = 0;
    return ch ; 
}


