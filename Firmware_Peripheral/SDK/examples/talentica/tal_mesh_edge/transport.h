/** @brief Transport.h file.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "ble.h"


#define APP_TIMER_PRESCALER        0                                  /**< Value of the RTC1 PRESCALER register. */
#define APPL_LOG                   NRF_LOG_PRINTF                     /**< Logger macro that will be used in this file to do logging over UART or RTT based on nrf_log configuration. */
#define APPL_LOG_DEBUG             NRF_LOG_PRINTF_DEBUG               /**< Debug logger macro that will be used in this file to do logging of debug information over UART or RTT based on nrf_log configuration. This will only work if DEBUG is defined*/

#define DEVICE_1_SOURCE_ID         (0xFFAA)
#define DEVICE_2_SOURCE_ID         (0xFFBB)



typedef struct
{
    ble_gap_addr_t address;
    uint16_t beacon_id;
    int8_t rssi;
    volatile bool isRssiValid;
} BEACON_RSSI_ADDR_T;


extern BEACON_RSSI_ADDR_T   beacons[3];
extern uint8_t min_index;
extern volatile uint8_t numberOfElements;


extern void on_ble_evt(ble_evt_t * p_ble_evt);
extern void create_beacon_timer(void);
extern void advertising_init(uint8_t opcode, uint8_t * param, uint8_t param_length);
extern void mesh_transport_run(void);


/* End of file */
