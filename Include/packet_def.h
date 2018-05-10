#ifndef		__PACKET_DEF_H__
#define		__PACKET_DEF_H__
#include 	<stdio.h>
#include	<stdint.h>
#include	<stdbool.h>







//=============================
//  FLASH_INFO_ADDR ~  FLASH_START_ADDR : 65536 bytes
// 1 index occupy 4 bytes 
//==================================
#define			FLASH_SUPER_PWD_ADDR	(0x02A000UL)
#define     FLASH_PWD_INDEX			(168)
#define    	FLASH_START_ADDR   	(0x03A000UL) 
#define     USER_BLOCK_SIZE     (0x0200UL)
#define     ERASE_PAGE_SIZE     (0x400UL)
#define     FLASH_START_INDEX   (232)  // => 0x03A000 / 1024
#define			TOTAL_USER_BLK_IDX		30 //32
//================================
// remains 
//  
//===============================
#define		NOT_USED				(0xFFFF)
#define		DIRTY						(0x7F7F)
#define		DELETE					(0x3F3F)
#define		HDR_55_AA				0x55AA
#define		MAIN_WRITE_CMD		0x1314
#define		MAIN_READ_CMD			0x1201
#define		MAIN_ERASE_CMD			0x713C
#define		GET_RND_KB_CMD		0x0D02

#define		TOUCH_COORD_PKT		0x59BC
#define		APP_WAIT_FOR_ACK	0x61A5
#define		RESET_PWD_CMD		0x46B7
#define		FIRST_OPEN_APP			0x9E45

#define 		ACK_RAN_NUM         0x6351
#define		ACK_CMD				0x519A			



typedef struct{
	uint16_t			origX ; 
	uint16_t			origY ;

	uint16_t			x_width ; 
	uint16_t			y_height ; 
	uint16_t			x_space ;
	uint16_t			y_space ;
} __attribute__((packed)) KEYBOARD_t ; // 16 bytes



#define     UINT8_BUFF_SIZE    	20 
#define			UINT16_BUFF_SIZE		10
#define     UINT32_BUFF_SIZE   	5 

#define			UART_OVER_BLE_MAX		20
typedef struct{
	uint16_t	    header ; 
	uint16_t			cmd ;	
	uint16_t			totalLength;
	uint8_t				padding[14] ; 
}__attribute__((packed)) BLE_MAIN_PKT_t ; 





typedef union{	
	uint8_t		    			_uint8BUFF[UINT8_BUFF_SIZE] ;
	uint16_t						_uint16BUFF[UINT16_BUFF_SIZE] ; 
	BLE_MAIN_PKT_t			mainPACKET ; 
	
}DATA_BUF_t  ; 

 


#define RANDOM_KB_LENGTH	16
#define	PASSWD_LENGTH		6

typedef struct{
	uint8_t  	hidKey ;
	uint8_t		isSHPressed ; 
}__attribute__((packed))TYTT_HID_t ; 




typedef 	uint8_t		CmdExecStatus ; 

#define		CMD_EXEC_IDLE		3 
#define		CMD_EXEC_REPEAT		1
#define		CMD_EXEC_ERROR		2
#define		CMD_EXEC_DONE		0 


typedef CmdExecStatus (*pFUNC)(void) ; 
 
extern DATA_BUF_t	tyttDATA ;
#endif
