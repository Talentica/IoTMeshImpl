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
#include "ble_advertising.h"
#include "softdevice_handler.h"
#include "app_error.h"
#include "app_timer.h"
#include "app_util.h"
#include "boards.h"
#include "nrf_drv_clock.h"
#include "nrf_log.h"
#include "application.h"
#include "transport.h"

#define CENTRAL_LINK_COUNT         1                                  /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT      0                                  /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define APP_TIMER_OP_QUEUE_SIZE    2                                  /**< Size of timer operation queues. */

#define SCAN_INTERVAL              0x00A0                             /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                0x0050                             /**< Determines scan window in units of 0.625 millisecond. */


static ble_gap_scan_params_t        m_scan_param;                        /**< Scan parameters requested for scanning and connection. */


/**@brief Function for handling the Application's system events.
 *
 * @param[in]   sys_evt   system event.
 */
static void on_sys_evt(uint32_t sys_evt)
{
    switch (sys_evt)
    {
        default:
            break;
    }
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
    err_code = softdevice_ble_evt_handler_set(on_ble_evt);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for System events.
    err_code = softdevice_sys_evt_handler_set(on_sys_evt);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function to start scanning.
 */
static void scan_start(void)
{
    uint32_t              err_code;

    /* No devices in whitelist, hence non selective performed. */
    m_scan_param.active       = 0;            // Passive scanning set.
    m_scan_param.selective    = 0;            // Selective scanning not set.
    m_scan_param.interval     = SCAN_INTERVAL;// Scan interval.
    m_scan_param.window       = SCAN_WINDOW;  // Scan window.
    m_scan_param.p_whitelist  = NULL;         // No whitelist provided.
    m_scan_param.timeout      = 0x0000;       // No timeout.

    err_code = sd_ble_gap_scan_start(&m_scan_param);
    APP_ERROR_CHECK(err_code);
}


/** @brief Function for the Power manager.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}


int main(void)
{
    // Initialize.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);
    NRF_LOG_INIT();
    ble_stack_init();
    create_beacon_timer();

    /* Start scanning for beacons */
    scan_start();

    for (;;)
    {
        power_manage();

        mesh_transport_run();
    }
}

/* End of file */
