#include "myUART_HID.h"
#include "ble_nus.h"
#include "nordic_common.h"
#include "ble_srv_common.h"
#include <string.h>
#include "ble_hids.h"
#include "app_error.h"
#include "app_util.h"
#include "HidoverGATT.h"


typedef struct{
		uint8_t		ascii ; 
		uint8_t		hidCode ; 
}__attribute__((packed)) HIDKey_t ; 



bool isBufferHaveData = false ; 
DATA_BUF_t	tyttDATA ; 

void TYTT_nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length) {
	uint16_t  i;
  
    
    for ( i = 0; i <length ; i++)
    {
       tyttDATA._uint8BUFF[i] = p_data[i] ; 
 
 	} 
 	
	
	isBufferHaveData = true ; 
     
}

uint8_t binarysearch(HIDKey_t* data , uint8_t search , int n )    
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;

	    if (data[mid].ascii == search)
        {
            return data[mid].hidCode;
        }
        else if (data[mid].ascii > search)
        {
            high = mid - 1;
        }
        else if (data[mid].ascii < search)
        {
            low = mid + 1;
        }
    }

    return 0xFF ;
} 

//======================================
//  void parse2HIDForm( uint8_t const* text , uint16_t len,TYTT_HID_t* hidArr  )
//  [function]: parse ascii code into HID key code with shift support
//  [Parameters] 
//          [in]text: user data(Acc/Pwd) that saved in flash in Ascii format
//					[out] hidArr
//  [Returns] hid key code
//================================================
void parse2HIDForm( uint8_t const* text , uint16_t len,TYTT_HID_t* hidArr  )
{
		uint16_t i ;
		uint8_t modifyCh ; 
		HIDKey_t  hidMap[] ={
			{'\t',0x2B} , {'\n',0x28} , {'\'',0x34} , {',',0x36} , {0x2F,0x38} ,  
			{'-',0x2D} ,  {'.' ,0x37} , {'/',0x38} 	, {'0',0x27} , {'1',0x1E} ,
			{'2',0x1F} ,  {'3',0x20} ,  {'4',0x21} 	, {'5',0x22} , {'6',0x23} , 
			{'7',0x24} , 	{'8',0x25} ,  {'9',0x26} 	, {';',0x33} , {'=',0x2E} ,
			{'[', 0x2F} , {'\\',0x31} , {']' ,0x5D} , {'`',0x35},  {'a',0x04} , 
			{'b',0x05} ,  {'c',0x06} , 	{'d',0x07} 	, {'e',0x08} , {'f',0x09} ,
			{'g',0x0A} , 	{'h',0x0B} ,	{'i',0x0C} 	, {'j',0x0D} , {'k',0x0E} , 
			{'l',0x0F} ,  {'m',0x10} , 	{'n',0x11} 	, {'o',0x12} , {'p',0x13} ,
			{'q',0x14} ,  {'r',0x15} , 	{'s',0x16} 	, {'t',0x17} , {'u',0x18} , 
			{'v',0x19} ,  {'w',0x1A},  	{'x',0x1B} 	, {'y',0x1C} , {'z',0x1D} 
  

		};
		
		#define HID_MAP_SIZE  (sizeof(hidMap)/sizeof(HIDKey_t)) 
		
		for( i=0;i<len;i++ ){
				modifyCh = text[i] ; 
				hidArr[i].isSHPressed = 0 ;
				if( modifyCh >='A' && modifyCh <= 'Z' ){
					modifyCh += 32 ;
					hidArr[i].isSHPressed = 1 ; 
					hidArr[i].hidKey = binarysearch(hidMap , modifyCh , HID_MAP_SIZE ) ; 
				}
				else{
					
					switch( modifyCh )
					{
            case '!' :
						{  
							hidArr[i].isSHPressed = 1 ;
							hidArr[i].hidKey = 0x1E ; 
            }
						break ; 
            case '@':
						{
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x1F ; 
						}
							break ;
            case '#':
           		{  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x20 ; 
							}   
               break ;
           case '$' :
            	{  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x21 ; 
							}   
            break ; 
            case '%':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x22 ; 
							}              
                break ;
            case '^':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x23 ; 
							}     
               break ;
						case '&' :
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x24 ; 
							}   
            break ; 
            case '*':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x25 ; 
							}              
                break ;
            case '(':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x26 ; 
							}      
                break ;
           case ')' :
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x27 ; 
							}   
            break ; 
            case '_':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x2D ; 
							}              
                break ;
            case '+':
           		{  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x2E ; 
							}      
                break ;
           case '{' :
           		{  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x2F ; 
							}   
            break ; 
            case '}':
             {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x30 ; 
							}           
                break ;
            case '|':
             {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x31 ; 
							}       
                break ;
          case ':' :
             {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x33 ; 
							}   
            break ; 
            case '"':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x34 ; 
							}               
                break ;
            case '<':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x36 ; 
							}      
                break ;
            case '>':
             {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x37 ; 
							}      
                break ;
            case '?':
              {  
								hidArr[i].isSHPressed = 1 ;
								hidArr[i].hidKey = 0x38 ; 
							}      
                break ;
            default:
						{
                hidArr[i].isSHPressed =  0  ;
								hidArr[i].hidKey = binarysearch(hidMap , modifyCh , HID_MAP_SIZE ) ; 
						}
									break ; 
          
					}

					
						
					
				}
				
		
		}
		
		
		
		
}
 
 


