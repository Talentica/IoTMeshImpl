/*
 * Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */

/** 
 * @brief BLE Heart Rate Collector application main file.
 *
 * This file contains the source code for a sample heart rate collector.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf_sdm.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_db_discovery.h"
#include "softdevice_handler.h"
#include "app_error.h"
#include "app_timer.h"
#include "app_util.h"
#include "boards.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "pstorage.h"
#include "device_manager.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "nrf_log.h"

#define CENTRAL_LINK_COUNT         1                                  /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT      0                                  /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define STRING_BUFFER_LEN          50
#define BOND_DELETE_ALL_BUTTON_ID  0                                  /**< Button used for deleting all bonded centrals during startup. */

#define APP_TIMER_PRESCALER        0                                  /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE    2                                  /**< Size of timer operation queues. */

#define APPL_LOG                   NRF_LOG_PRINTF                     /**< Logger macro that will be used in this file to do logging over UART or RTT based on nrf_log configuration. */
#define APPL_LOG_DEBUG             NRF_LOG_PRINTF_DEBUG               /**< Debug logger macro that will be used in this file to do logging of debug information over UART or RTT based on nrf_log configuration. This will only work if DEBUG is defined*/

#define SEC_PARAM_BOND             1                                  /**< Perform bonding. */
#define SEC_PARAM_MITM             1                                  /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC             0                                  /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS         0                                  /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES  BLE_GAP_IO_CAPS_NONE               /**< No I/O capabilities. */
#define SEC_PARAM_OOB              0                                  /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE     7                                  /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE     16                                 /**< Maximum encryption key size. */

#define SCAN_INTERVAL              0x00A0                             /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                0x0050                             /**< Determines scan window in units of 0.625 millisecond. */

#define MIN_CONNECTION_INTERVAL    MSEC_TO_UNITS(7.5, UNIT_1_25_MS)   /**< Determines minimum connection interval in millisecond. */
#define MAX_CONNECTION_INTERVAL    MSEC_TO_UNITS(30, UNIT_1_25_MS)    /**< Determines maximum connection interval in millisecond. */
#define SLAVE_LATENCY              0                                  /**< Determines slave latency in counts of connection events. */
#define SUPERVISION_TIMEOUT        MSEC_TO_UNITS(4000, UNIT_10_MS)    /**< Determines supervision time-out in units of 10 millisecond. */

#define TARGET_UUID                0x180D                             /**< Target device name that application is looking for. */
#define UUID16_SIZE                2                                  /**< Size of 16 bit UUID */

/**@breif Macro to unpack 16bit unsigned UUID from octet stream. */
#define UUID16_EXTRACT(DST, SRC) \
    do                           \
    {                            \
        (*(DST))   = (SRC)[1];   \
        (*(DST)) <<= 8;          \
        (*(DST))  |= (SRC)[0];   \
    } while (0)

/**@brief Variable length data encapsulation in terms of length and pointer to data */
typedef struct
{
    uint8_t     * p_data;                                             /**< Pointer to data. */
    uint16_t      data_len;                                           /**< Length of data. */
}data_t;

typedef enum
{
    BLE_NO_SCAN,                                                     /**< No advertising running. */
    BLE_WHITELIST_SCAN,                                              /**< Advertising with whitelist. */
    BLE_FAST_SCAN,                                                   /**< Fast advertising running. */
} ble_scan_mode_t;

static ble_db_discovery_t           m_ble_db_discovery;                  /**< Structure used to identify the DB Discovery module. */
static ble_gap_scan_params_t        m_scan_param;                        /**< Scan parameters requested for scanning and connection. */
static dm_application_instance_t    m_dm_app_id;                         /**< Application identifier. */
static dm_handle_t                  m_dm_device_handle;                  /**< Device Identifier identifier. */
static uint8_t                      m_peer_count = 0;                    /**< Number of peer's connected. */
static ble_scan_mode_t              m_scan_mode = BLE_FAST_SCAN;         /**< Scan mode used by application. */
static uint16_t                     m_conn_handle;                       /**< Current connection handle. */
static volatile bool                m_whitelist_temporarily_disabled = false; /**< True if whitelist has been temporarily disabled. */

static bool                         m_memory_access_in_progress = false; /**< Flag to keep track of ongoing operations on persistent memory. */

/**
 * @brief Connection parameters requested for connection.
 */
static const ble_gap_conn_params_t m_connection_param =
{
    (uint16_t)MIN_CONNECTION_INTERVAL,   // Minimum connection
    (uint16_t)MAX_CONNECTION_INTERVAL,   // Maximum connection
    0,                                   // Slave latency
    (uint16_t)SUPERVISION_TIMEOUT        // Supervision time-out
};

typedef struct
{
	int8_t rssi;
	uint8_t address[6];
	bool isRssiValid;
} BEACON_RSSI_ADDR_T;
static BEACON_RSSI_ADDR_T	beacons[3];
static int8_t max_rssi = 0, min_rssi = -127;
static uint8_t max_index = 0, min_index = 0;
static bool isMinChanged = false;
static uint8_t numberOfElements = 0;

/* Given 1 second period, 10 second countdown */
static uint8_t time_counter[3] = {10, 10, 10};

APP_TIMER_DEF(beacon_refresh_id);

static void scan_start(void);


/**@brief Function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num     Line number of the failing ASSERT call.
 * @param[in] p_file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}


/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function should forward the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
}


/**@brief Callback handling device manager events.
 *
 * @details This function is called to notify the application of device manager events.
 *
 * @param[in]   p_handle      Device Manager Handle. For link related events, this parameter
 *                            identifies the peer.
 * @param[in]   p_event       Pointer to the device manager event.
 * @param[in]   event_status  Status of the event.
 */
static ret_code_t device_manager_event_handler(const dm_handle_t    * p_handle,
                                                 const dm_event_t     * p_event,
                                                 const ret_code_t     event_result)
{
    uint32_t err_code;

    switch (p_event->event_id)
    {
        case DM_EVT_CONNECTION:
        {
            APPL_LOG_DEBUG("[APPL]: >> DM_EVT_CONNECTION\r\n");
#ifdef ENABLE_DEBUG_LOG_SUPPORT
            ble_gap_addr_t * peer_addr;
            peer_addr = &p_event->event_param.p_gap_param->params.connected.peer_addr;
            APPL_LOG_DEBUG("[APPL]:[%02X %02X %02X %02X %02X %02X]: Connection Established\r\n",
                                peer_addr->addr[0], peer_addr->addr[1], peer_addr->addr[2],
                                peer_addr->addr[3], peer_addr->addr[4], peer_addr->addr[5]);
#endif // ENABLE_DEBUG_LOG_SUPPORT
            
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);

            m_conn_handle = p_event->event_param.p_gap_param->conn_handle;

            m_dm_device_handle = (*p_handle);

            // Initiate bonding.
            err_code = dm_security_setup_req(&m_dm_device_handle);
            APP_ERROR_CHECK(err_code);

            m_peer_count++;

            if (m_peer_count < CENTRAL_LINK_COUNT)
            {
                scan_start();
            }
            APPL_LOG_DEBUG("[APPL]: << DM_EVT_CONNECTION\r\n");
            break;
        }

        case DM_EVT_DISCONNECTION:
        {
            APPL_LOG_DEBUG("[APPL]: >> DM_EVT_DISCONNECTION\r\n");
            memset(&m_ble_db_discovery, 0 , sizeof (m_ble_db_discovery));

            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);

            if (m_peer_count == CENTRAL_LINK_COUNT)
            {
                scan_start();
            }
            m_peer_count--;
            APPL_LOG_DEBUG("[APPL]: << DM_EVT_DISCONNECTION\r\n");
            break;
        }

        case DM_EVT_SECURITY_SETUP:
        {
            APPL_LOG_DEBUG("[APPL]:[0x%02X] >> DM_EVT_SECURITY_SETUP\r\n", p_handle->connection_id);
            // Slave securtiy request received from peer, if from a non bonded device, 
            // initiate security setup, else, wait for encryption to complete.
            err_code = dm_security_setup_req(&m_dm_device_handle);
            APP_ERROR_CHECK(err_code);
            APPL_LOG_DEBUG("[APPL]:[0x%02X] << DM_EVT_SECURITY_SETUP\r\n", p_handle->connection_id);
            break;
        }

        case DM_EVT_SECURITY_SETUP_COMPLETE:
        {
            APPL_LOG_DEBUG("[APPL]: >> DM_EVT_SECURITY_SETUP_COMPLETE\r\n");
            APPL_LOG_DEBUG("[APPL]: << DM_EVT_SECURITY_SETUP_COMPLETE\r\n");
            break;
        }

        case DM_EVT_LINK_SECURED:
            APPL_LOG_DEBUG("[APPL]: >> DM_LINK_SECURED_IND\r\n");
            // Discover peer's services. 
            err_code = ble_db_discovery_start(&m_ble_db_discovery,
                                              p_event->event_param.p_gap_param->conn_handle);
            APP_ERROR_CHECK(err_code);
            APPL_LOG_DEBUG("[APPL]: << DM_LINK_SECURED_IND\r\n");
            break;

        case DM_EVT_DEVICE_CONTEXT_LOADED:
            APPL_LOG_DEBUG("[APPL]: >> DM_EVT_LINK_SECURED\r\n");
            APP_ERROR_CHECK(event_result);
            APPL_LOG_DEBUG("[APPL]: << DM_EVT_DEVICE_CONTEXT_LOADED\r\n");
            break;

        case DM_EVT_DEVICE_CONTEXT_STORED:
            APPL_LOG_DEBUG("[APPL]: >> DM_EVT_DEVICE_CONTEXT_STORED\r\n");
            APP_ERROR_CHECK(event_result);
            APPL_LOG_DEBUG("[APPL]: << DM_EVT_DEVICE_CONTEXT_STORED\r\n");
            break;

        case DM_EVT_DEVICE_CONTEXT_DELETED:
            APPL_LOG_DEBUG("[APPL]: >> DM_EVT_DEVICE_CONTEXT_DELETED\r\n");
            APP_ERROR_CHECK(event_result);
            APPL_LOG_DEBUG("[APPL]: << DM_EVT_DEVICE_CONTEXT_DELETED\r\n");
            break;

        default:
            break;
    }

    return NRF_SUCCESS;
}


/**
 * @brief Parses advertisement data, providing length and location of the field in case
 *        matching data is found.
 *
 * @param[in]  Type of data to be looked for in advertisement data.
 * @param[in]  Advertisement report length and pointer to report.
 * @param[out] If data type requested is found in the data report, type data length and
 *             pointer to data will be populated here.
 *
 * @retval NRF_SUCCESS if the data type is found in the report.
 * @retval NRF_ERROR_NOT_FOUND if the data type could not be found.
 */
static uint32_t adv_report_parse(uint8_t type, data_t * p_advdata, data_t * p_typedata)
{
    uint32_t  index = 0;
    uint8_t * p_data;

    p_data = p_advdata->p_data;

    while (index < p_advdata->data_len)
    {
        uint8_t field_length = p_data[index];
        uint8_t field_type   = p_data[index+1];

        if (field_type == type)
        {
            p_typedata->p_data   = &p_data[index+2];
            p_typedata->data_len = field_length-1;
            return NRF_SUCCESS;
        }
        index += field_length + 1;
    }
    return NRF_ERROR_NOT_FOUND;
}

static int8_t is_device_in_list(uint8_t *addr)
{
	uint8_t counter;
	for(counter = 0; counter < 3; counter++)
	{
		if(memcmp(addr, beacons[counter].address, 6) == 0)
		{
			return counter;
		}
	}
	return -1;
}


/* @brief Function to calculate the minimum and maximum RSSI
 * beacons among the top 3 stored.
 * Also updates a global variable to indicate whether the
 * closest beacon to this device has changed.
 */
static void calc_rssi_min_max(void)
{
	uint8_t counter;
	uint8_t min_index_backup = min_index;

	uint8_t index = beacons[0].isRssiValid == true ? 0 :
				    beacons[1].isRssiValid == true ? 1 :
					beacons[2].isRssiValid == true ? 2 : 3;

	/* There is no beacon nearby. Min/Max are invalid.
	 */
	if(index == 3)
	{
		min_rssi = -127;
		max_rssi = 0;
		min_index = max_index = 0;
		return;
	}
	else
	{
		min_rssi = max_rssi = beacons[index].rssi;
		min_index = max_index = index;
	}

	/* Loop through and figure out which beacon is the closest,
	 * and also which is furthest (among top 3).
	 */
	for(counter = index + 1; counter < 3; counter++)
	{
		if(beacons[counter].isRssiValid)
		{
			if(beacons[counter].rssi > min_rssi)
			{
				min_rssi = beacons[counter].rssi;
				min_index = counter;
			}
			if(beacons[counter].rssi < max_rssi)
			{
				max_rssi = beacons[counter].rssi;
				max_index = counter;
			}
		}
	}

	/* See if the closest beacon to this device is now someone else. */
	if(min_index_backup != min_index)
	{
		isMinChanged = true;
	}
}


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                err_code;
    const ble_gap_evt_t   * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
    	/* Got a new advertisement packet */
        case BLE_GAP_EVT_ADV_REPORT:
        {
            data_t adv_data;
            data_t type_data;
            int8_t rssi = p_gap_evt->params.adv_report.rssi;
            uint8_t *addr = (uint8_t *)(p_gap_evt->params.adv_report.peer_addr.addr);
			int8_t position = is_device_in_list(addr);

			/* Maintain a list of top 3 beacons (with maximum RSSI) */
            if(position >= 0)
            {
            	/* If the beacon already is in the list, simply update
            	 * its RSSI value, and recalculate the min, max values.
            	 * Also reset the countdown for this beacon.
            	 */
            	beacons[position].rssi = rssi;
            	time_counter[position] = 10;
            	calc_rssi_min_max();
            }
            else
            {
            	/* The beacon doesn't already exist in the list.
            	 */
            	if(numberOfElements < 3)
            	{
            		/* There is atleast one empty space. Simply add the beacon,
            		 * reset its countdown, and recalculate min, max RSSI.
            		 * Update the number of elements in the array.
            		 */
            		uint8_t index = beacons[0].isRssiValid == false ? 0 :
            					    beacons[1].isRssiValid == false ? 1 : 2;

            		beacons[index].rssi = rssi;
            		memcpy(beacons[index].address, addr, 6);
            		beacons[index].isRssiValid = true;
            		numberOfElements++;
            		time_counter[index] = 10;
            		calc_rssi_min_max();
            	}
            	else
            	{
            		/* If the beacon's RSSI is better than the lowest in the
            		 * list, replace the lowest with this one. Then
            		 * recalculate min, max. Also reset timer countdown.
            		 */
            		if(rssi > max_rssi)
            		{
            			beacons[max_index].rssi = rssi;
            			memcpy(beacons[max_index].address, addr, 6);
            			time_counter[max_index] = 10;
            			calc_rssi_min_max();
            		}
            	}
            }

            /* Initialize advertisement report for parsing. */
            adv_data.p_data = (uint8_t *)p_gap_evt->params.adv_report.data;
            adv_data.data_len = p_gap_evt->params.adv_report.dlen;

            err_code = adv_report_parse(BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE,
                                        &adv_data,
                                        &type_data);

            if (err_code != NRF_SUCCESS)
            {
                // Compare short local name in case complete name does not match.
                err_code = adv_report_parse(BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE,
                                            &adv_data,
                                            &type_data);
            }

            // Verify if short or complete name matches target.
            if (err_code == NRF_SUCCESS)
            {
                uint16_t extracted_uuid;

                // UUIDs found, look for matching UUID
                for (uint32_t u_index = 0; u_index < (type_data.data_len/UUID16_SIZE); u_index++)
                {
                    UUID16_EXTRACT(&extracted_uuid,&type_data.p_data[u_index * UUID16_SIZE]);

                    APPL_LOG_DEBUG("\t[APPL]: %x\r\n",extracted_uuid);

                    if(extracted_uuid == TARGET_UUID)
                    {
                        // Stop scanning.
                        err_code = sd_ble_gap_scan_stop();

                        if (err_code != NRF_SUCCESS)
                        {
                            APPL_LOG_DEBUG("[APPL]: Scan stop failed, reason %d\r\n", err_code);
                        }
                        err_code = bsp_indication_set(BSP_INDICATE_IDLE);
                        APP_ERROR_CHECK(err_code);

                        m_scan_param.selective = 0; 
                        m_scan_param.p_whitelist = NULL;

                        // Initiate connection.
                        err_code = sd_ble_gap_connect(&p_gap_evt->params.adv_report.peer_addr,
                                                      &m_scan_param,
                                                      &m_connection_param);

                        m_whitelist_temporarily_disabled = false;

                        if (err_code != NRF_SUCCESS)
                        {
                            APPL_LOG_DEBUG("[APPL]: Connection Request Failed, reason %d\r\n", err_code);
                        }
                        break;
                    }
                }
            }
            break;
        }

        case BLE_GAP_EVT_TIMEOUT:
            if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_SCAN)
            {
                APPL_LOG_DEBUG("[APPL]: Scan timed out.\r\n");
                scan_start();
            }
            else if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN)
            {
                APPL_LOG_DEBUG("[APPL]: Connection Request timed out.\r\n");
            }
            break;
        case BLE_GAP_EVT_CONNECTED:
        {
            break;
        }
        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
            // Accepting parameters requested by peer.
            err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle,
                                                    &p_gap_evt->params.conn_param_update_request.conn_params);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            break;
    }
}


/**@brief Function for handling the Application's system events.
 *
 * @param[in]   sys_evt   system event.
 */
static void on_sys_evt(uint32_t sys_evt)
{
    switch (sys_evt)
    {
        case NRF_EVT_FLASH_OPERATION_SUCCESS:
            /* fall through */
        case NRF_EVT_FLASH_OPERATION_ERROR:

            if (m_memory_access_in_progress)
            {
                m_memory_access_in_progress = false;
                scan_start();
            }
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the scheduler in the main loop after a BLE stack event has
 *  been received.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    dm_ble_evt_handler(p_ble_evt);
    ble_db_discovery_on_ble_evt(&m_ble_db_discovery, p_ble_evt);
//    bsp_btn_ble_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
}


/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{
    pstorage_sys_event_handler(sys_evt);
    on_sys_evt(sys_evt);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    uint32_t err_code;
		
    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
		
    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);
    
    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);
    
    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);
    
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for System events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for the Device Manager initialization.
 *
 * @param[in] erase_bonds  Indicates whether bonding information should be cleared from
 *                         persistent storage during initialization of the Device Manager.
 */
static void device_manager_init(bool erase_bonds)
{
    uint32_t               err_code;
    dm_init_param_t        init_param = {.clear_persistent_data = erase_bonds};
    dm_application_param_t register_param;

    err_code = pstorage_init();
    APP_ERROR_CHECK(err_code);

    err_code = dm_init(&init_param);
    APP_ERROR_CHECK(err_code);

    memset(&register_param.sec_param, 0, sizeof (ble_gap_sec_params_t));

    // Event handler to be registered with the module.
    register_param.evt_handler            = device_manager_event_handler;

    // Service or protocol context for device manager to load, store and apply on behalf of application.
    // Here set to client as application is a GATT client.
    register_param.service_type           = DM_PROTOCOL_CNTXT_GATT_CLI_ID;

    // Security parameters to be used for security procedures.
    register_param.sec_param.bond         = SEC_PARAM_BOND;
    register_param.sec_param.mitm         = SEC_PARAM_MITM;
    register_param.sec_param.lesc         = SEC_PARAM_LESC;
    register_param.sec_param.keypress     = SEC_PARAM_KEYPRESS;
    register_param.sec_param.io_caps      = SEC_PARAM_IO_CAPABILITIES;
    register_param.sec_param.oob          = SEC_PARAM_OOB;
    register_param.sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    register_param.sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
    register_param.sec_param.kdist_peer.enc = 1;
    register_param.sec_param.kdist_peer.id  = 1;

    err_code = dm_register(&m_dm_app_id, &register_param);
    APP_ERROR_CHECK(err_code);
}


/**
 * @brief Database discovery collector initialization.
 */
static void db_discovery_init(void)
{
    uint32_t err_code = ble_db_discovery_init(db_disc_handler);

    APP_ERROR_CHECK(err_code);
}


/**@brief Function to start scanning.
 */
static void scan_start(void)
{
    ble_gap_whitelist_t   whitelist;
    ble_gap_addr_t      * p_whitelist_addr[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
    ble_gap_irk_t       * p_whitelist_irk[BLE_GAP_WHITELIST_IRK_MAX_COUNT];
    uint32_t              err_code;
    uint32_t              count;

    // Verify if there is any flash access pending, if yes delay starting scanning until 
    // it's complete.
    err_code = pstorage_access_status_get(&count);
    APP_ERROR_CHECK(err_code);

    if (count != 0)
    {
        m_memory_access_in_progress = true;
        return;
    }

    // Initialize whitelist parameters.
    whitelist.addr_count = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;
    whitelist.irk_count  = 0;
    whitelist.pp_addrs   = p_whitelist_addr;
    whitelist.pp_irks    = p_whitelist_irk;

    // Request creating of whitelist.
    err_code = dm_whitelist_create(&m_dm_app_id,&whitelist);
    APP_ERROR_CHECK(err_code);

    if (((whitelist.addr_count == 0) && (whitelist.irk_count == 0)) ||
        (m_scan_mode != BLE_WHITELIST_SCAN)                        ||
        (m_whitelist_temporarily_disabled))
    {
        // No devices in whitelist, hence non selective performed.
        m_scan_param.active       = 0;            // Active scanning set.
        m_scan_param.selective    = 0;            // Selective scanning not set.
        m_scan_param.interval     = SCAN_INTERVAL;// Scan interval.
        m_scan_param.window       = SCAN_WINDOW;  // Scan window.
        m_scan_param.p_whitelist  = NULL;         // No whitelist provided.
        m_scan_param.timeout      = 0x0000;       // No timeout.
    }
    else
    {
        // Selective scanning based on whitelist first.
        m_scan_param.active       = 0;            // Active scanning set.
        m_scan_param.selective    = 1;            // Selective scanning not set.
        m_scan_param.interval     = SCAN_INTERVAL;// Scan interval.
        m_scan_param.window       = SCAN_WINDOW;  // Scan window.
        m_scan_param.p_whitelist  = &whitelist;   // Provide whitelist.
        m_scan_param.timeout      = 0x001E;       // 30 seconds timeout.
    }

    err_code = sd_ble_gap_scan_start(&m_scan_param);
    APP_ERROR_CHECK(err_code);

//    err_code = bsp_indication_set(BSP_INDICATE_SCANNING);
//    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void buttons_leds_init(bool * p_erase_bonds)
{
//    bsp_event_t startup_event;

//    uint32_t err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS,
//                                 APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),
//                                 bsp_event_handler);
//    APP_ERROR_CHECK(err_code);
//
//    err_code = bsp_btn_ble_init(NULL, &startup_event);
//    APP_ERROR_CHECK(err_code);

//    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}


/**@brief Function for initializing the nrf log module.
 */
static void nrf_log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT();
    APP_ERROR_CHECK(err_code);
}


/** @brief Function for the Power manager.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}


/** @brief Callback function to handle timers for beacons.
 *
 */
static void beacon_timer_handler(void * p_context)
{
	uint8_t counter;

	for(counter = 0; counter < 3; counter++)
	{
		if(beacons[counter].isRssiValid)
		{
			time_counter[counter]--;

			if(time_counter[counter] == 0)
			{
				/* Remove the corresponding entry in the beacon list */
				beacons[counter].isRssiValid = false;
				numberOfElements--;
			}
		}
	}
	calc_rssi_min_max();
}


/** @brief Function to create timer for beacon last-seen and expiry.
 *
 */
static void create_beacon_timer(void)
{
	uint32_t err_code;

	/* Create a timer to trigger every second */
	err_code = app_timer_create(&beacon_refresh_id,
								APP_TIMER_MODE_REPEATED,
								beacon_timer_handler);
	APP_ERROR_CHECK(err_code);
	err_code = app_timer_start(beacon_refresh_id,
							   APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER),
							   NULL);
	APP_ERROR_CHECK(err_code);
}


int main(void)
{
    bool erase_bonds = true;

    // Initialize.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);
    buttons_leds_init(&erase_bonds);
    nrf_log_init();
    APPL_LOG("Mesh: Peripheral.\r\n");
    ble_stack_init();
    device_manager_init(erase_bonds);
    db_discovery_init();
    create_beacon_timer();

    /* Start scanning for beacons and identify their RSSI.
     * Determine the beacon with highest RSSI and attempt to connect to it.
     */
    scan_start();

    for (;;)
    {
        power_manage();

        /* Print the beacon which is the closest. */
        if(isMinChanged)
        {
        	isMinChanged = false;
        	APPL_LOG("\r\nClosest beacon has the RSSI: %d. Address: ", min_rssi);
        	{
        		uint8_t counter;
        		for (counter = 0; counter < 6; counter++)
				{
        			APPL_LOG("%02x", beacons[min_index].address[counter]);
				}
        	}
        }
    }
}


