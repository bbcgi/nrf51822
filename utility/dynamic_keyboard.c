#include 	"dynamic_keyboard.h"
#include	"myUTILITY.h"
#include    "packet_def.h"

#define RANDOM_KB_LENGTH	16 
#define	PASSWD_LENGTH		6

typedef struct{
    uint16_t    iX ; 
    uint16_t    iY ; 
}VECTOR_t ; 

uint8_t     randomKeyBoard[RANDOM_KB_LENGTH]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
};
uint8_t		defalut_pwd[PASSWD_LENGTH] = {1,2,3,4,5,6} ; 
uint8_t  	user_pwd[PASSWD_LENGTH] ;
KEYBOARD_t  keyboardINFO  ; 


extern int32_t g_seed ; 







void getKeyBoardInfo(uint8_t* buff){
    KEYBOARD_t* k = (KEYBOARD_t*)buff ; 
    
    keyboardINFO.origX =  k->origX ; 
    keyboardINFO.origY =  k->origY ;
    keyboardINFO.endX =   k->endX ; 
    keyboardINFO.endY =   k->endY ;
    keyboardINFO.x_width = k->x_width ; 
    keyboardINFO.y_height = k->y_height ;
    keyboardINFO.x_space = k->x_space ; 
    keyboardINFO.y_space = k->y_space ;
    
    
    
   
}









void Generator_Random_Key(void) { 
	uint8_t	i ; 
	int32_t k ; 
	uint8_t c ; 
	
	
	for(i=0;i<RANDOM_KB_LENGTH;i++){
		uint8_t	_k ; 
		do{
			k = myRAND( &g_seed ) ; 
		}while( k  < 0 ) ; 
		
		k = k% RANDOM_KB_LENGTH ; 
		_k = (uint8_t)(k&0x0000FF) ; 
		c = randomKeyBoard[i] ; 
		randomKeyBoard[i] = randomKeyBoard[_k] ; 
		randomKeyBoard[_k] = c ; 
	}
	
	
	BLE_putstring(randomKeyBoard , RANDOM_KB_LENGTH ) ; 
}




