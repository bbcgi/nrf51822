
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<stdbool.h>
#include 	<stdint.h>


extern void  myitoa(uint8_t num , uint8_t* b) ; 
extern void ch2ASCII( uint8_t const* , uint8_t* ,uint8_t ) ; 
extern uint32_t myAsciitoInt( uint8_t* arr) ;
extern void mySRAND(void) ; 
extern int32_t myRAND(int32_t*)  ;
extern void myPrintHEX( uint8_t const* buff ,uint16_t len ); 
extern void BLE_putstring(const uint8_t *str,uint16_t len ) ; 
extern void  my_uint16_to_a(uint16_t num , uint8_t* b) ; 
extern void my_print_coord( uint16_t iX , uint16_t iY ) ;  

//==============UART ======================
extern void UART0_Config(bool hw) ;
extern void uart_put(uint8_t cr) ;
extern void uart_putstring(const uint8_t* str) ; 
extern bool  my_strncmp(uint8_t* s1, uint8_t* s2 , uint8_t n) ;
//===========================================
extern  void makeACKPKT(volatile  bool isVALID , uint16_t  volatile* buff ) ; 
extern void saveDefaultPWD(void) ; 
extern void loadSHAFromFlash( uint8_t  * volatile hash ) ; 
//===========Flash===============
extern void my_nvmc_write_words(uint32_t address,  uint32_t const* src, uint32_t num_words) ; 
///=================================================
