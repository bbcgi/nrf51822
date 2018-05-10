#ifndef  __HIDOVER_GATT_H__
#define	 __HIDOVER_GATT_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
#include "ble_debug_assert_handler.h"
#include "pstorage.h"
#include "app_trace.h"

//============ add by PC======

#include "nrf_delay.h"
#include "myUART_HID.h"

#define OUTPUT_REPORT_INDEX              0                                              /**< Index of Output Report. */
#define OUTPUT_REPORT_MAX_LEN            1                                              /**< Maximum length of Output Report. */
#define INPUT_REPORT_KEYS_INDEX          0                                              /**< Index of Input Report. */
#define OUTPUT_REPORT_BIT_MASK_CAPS_LOCK 0x02                                           /**< CAPS LOCK bit in Output Report (based on 'LED Page (0x08)' of the Universal Serial Bus HID Usage Tables). */
#define INPUT_REP_REF_ID                 1//0                                              /**< Id of reference to Keyboard Input Report. */
#define OUTPUT_REP_REF_ID                0                                              /**< Id of reference to Keyboard Output Report. */

#define MAX_BUFFER_ENTRIES               64 //5   
#define BASE_USB_HID_SPEC_VERSION        0x0101                                         /**< Version number of base USB HID Specification implemented by this application. */

#define INPUT_REPORT_KEYS_MAX_LEN        8  

#define MODIFIER_KEY_POS                 0                                              /**< Position of the modifier byte in the Input Report. */
#define SCAN_CODE_POS                    2                                              /**< This macro indicates the start position of the key scan code in a HID Report. As per the document titled 'Device Class Definition for Human Interface Devices (HID) V1.11, each report shall have one modifier byte followed by a reserved constant byte and then the key scan code. */
#define SHIFT_KEY_CODE                   0x02 



#define KEY_PRESS_BUTTON_PIN_NO          BUTTON_1                                       /**< Button used for sending keyboard text. */
#define BOND_DELETE_ALL_BUTTON_ID        BUTTON_2                                       /**< Button used for deleting all bonded centrals during startup. */
#define SHIFT_BUTTON                     BUTTON_3                                       /**< Button used as 'SHIFT' Key. */
#define SHIFT_BUTTON                     BUTTON_3                                       /**< Button used as 'SHIFT' Key. */

#define ADVERTISING_LED_PIN_NO           LED_1                                          /**< Is on when device is advertising. */
#define CONNECTED_LED_PIN_NO             LED_2                                          /**< Is on when device has connected. */
#define CAPS_ON_LED_PIN_NO               LED_3                                          /**< Pin for indicating that CAPS LOCK is on. */
#define ADV_DIRECTED_LED_PIN_NO          LED_1                                          /**< Is on when we are doing directed advertisement. */
#define ADV_WHITELIST_LED_PIN_NO         LED_2                                          /**< Is on when we are doing advertising with whitelist. */
#define ADV_INTERVAL_SLOW_LED_PIN_NO     LED_3                                          /**< Is on when we are doing slow advertising. */
#define ASSERT_LED_PIN_NO                LED_4    


extern ble_hids_t                        m_hids;
extern		bool   					m_in_boot_mode ; 
extern		dm_handle_t                       m_bonded_peer_handle;   
void hids_init(void) ; 
typedef enum
{
    BLE_NO_ADV,               /**< No advertising running. */
    BLE_DIRECTED_ADV,         /**< Direct advertising to the latest central. */
    BLE_FAST_ADV_WHITELIST,   /**< Advertising with whitelist. */
    BLE_FAST_ADV,             /**< Fast advertising running. */
    BLE_SLOW_ADV,             /**< Slow advertising running. */
    BLE_SLEEP,                /**< Go to system-off. */
} ble_advertising_mode_t;

/** Abstracts buffer element */
typedef struct hid_key_buffer
{
    uint8_t    data_offset;   /**< Max Data that can be buffered for all entries */
    uint8_t    data_len;      /**< Total length of data */
    uint8_t    * p_data;      /**< Scanned key pattern */
    ble_hids_t * p_instance;  /**< Identifies peer and service instance */
    //===== add by PC ==============
    uint8_t        enable ; 
    //==== end by PC ==============
}buffer_entry_t;


/** Circular buffer list */
typedef struct
{
    buffer_entry_t buffer[MAX_BUFFER_ENTRIES]; /**< Maximum number of entries that can enqueued in the list */
    uint8_t        rp;                         /**< Index to the read location */
    uint8_t        wp;                         /**< Index to write location */
    uint8_t        count;                      /**< Number of elements in the list */
}buffer_list_t;

extern buffer_list_t buffer_list; 
extern bool              m_caps_on ; 
extern uint32_t buffer_enqueue(			ble_hids_t *            p_hids,
                               uint8_t *               p_key_pattern,
                               uint16_t                pattern_len,
                               uint16_t                offset ,
																uint8_t                    enable) ;
							   
extern uint32_t buffer_dequeue(bool tx_flag) ; 
/**Buffer queue access macros
 *
 * @{ */
/** Initialization of buffer list */
#define BUFFER_LIST_INIT()                                                                        \
        do                                                                                        \
        {                                                                                         \
            buffer_list.rp = 0;                                                                   \
            buffer_list.wp = 0;                                                                   \
            buffer_list.count = 0;                                                                \
        } while (0)

/** Provide status of data list is full or not */
#define BUFFER_LIST_FULL()\
        ((MAX_BUFFER_ENTRIES == buffer_list.count) ? true : false)

/** Provides status of buffer list is empty or not */
#define BUFFER_LIST_EMPTY()\
        ((0 == buffer_list.count) ? true : false)

#define BUFFER_ELEMENT_INIT(i)\
        do                                                                                        \
        {                                                                                         \
            buffer_list.buffer[(i)].p_data = NULL;                                                \
        } while (0)
			
		
#define MAX_KEYS_IN_ONE_REPORT           (INPUT_REPORT_KEYS_MAX_LEN - SCAN_CODE_POS) 

extern void launchKEYBOARD(void) ;
extern void keys_send(uint8_t key_pattern_len, uint8_t * p_key_pattern ,uint8_t isShift) ;

extern void takePHOTO(void) ;




#endif 
