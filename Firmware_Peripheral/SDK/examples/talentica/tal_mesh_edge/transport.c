/** File to handle transport content for mesh.
 *
 */

#include "app_error.h"
#include "app_timer.h"
#include "app_util.h"
#include "ble.h"
#include "ble_advertising.h"
#include "boards.h"
#include "nordic_common.h"
#include "nrf_log.h"
#include "nrf_sdm.h"
#include "softdevice_handler.h"
#include "transport.h"
#include "application.h"

#define MANUFACTURER_ID_TALENTICA_MSB   (0x55)
#define MANUFACTURER_ID_TALENTICA_LSB   (0xAA)
#define SEND_NO_DATA                    (0)
#define SEND_DATA                       (1)


BEACON_RSSI_ADDR_T beacons[3];
static int8_t max_rssi = 0, min_rssi = -127;
static uint8_t max_index = 0;
uint8_t min_index = 0;
static bool isMinChanged = false;
uint8_t numberOfElements = 0;
static uint8_t time_counter[3] = {10, 10, 10};  /* Given 1 second period, 10 second countdown */


static uint16_t source_id = 0xFFAA;
APP_TIMER_DEF(beacon_refresh_id);


/**@brief Function for handling advertising events.
 *
 * @details This function is called for advertising events that are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_IDLE:
            APPL_LOG("ADV Event Idle. \r\n");
            break;

        case BLE_ADV_EVT_PEER_ADDR_REQUEST:
            APPL_LOG("Peer Address request event. \r\n");
            err_code = ble_advertising_peer_addr_reply(&beacons[min_index].address);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            break;
    }
}


/** @brief Function to update advertisement packet data.
 *
 *  Need to write about parameters also.
 */
void advertising_init(uint8_t opcode, uint8_t * param, uint8_t param_length)
{
    uint32_t                    err_code;
    ble_advdata_t               advdata;
    ble_adv_modes_config_t      options = {0};
    ble_advdata_manuf_data_t    manuf_data;
    uint8_t                     payload[12];
    uint8_t                     size_of_payload;

    APPL_LOG("Calling ADV init. \r\n");
    memset(&advdata, 0, sizeof(advdata));

    /** Select the beacon to which we have to send data.
     *  Send a directed ADV to that beacon, so only that beacon
     *  would need to process it.
     */
    if(opcode == 0)
    {
        payload[0] = SEND_NO_DATA;
        payload[1] = (source_id >> 8) & 0x00FF;
        payload[2] = source_id & 0x00FF;
        size_of_payload = 3;
    }
    else
    {
        payload[0] = (SEND_DATA << 7) | (opcode & 0x7F);
        payload[1] = (source_id >> 8) & 0x00FF;
        payload[2] = source_id & 0x00FF;
        memcpy(&payload[3], param, param_length);
        size_of_payload = param_length + 3;
    }

    manuf_data.company_identifier = (MANUFACTURER_ID_TALENTICA_MSB << 8) | (MANUFACTURER_ID_TALENTICA_LSB);
    manuf_data.data.p_data = payload;
    manuf_data.data.size = size_of_payload;

    advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.p_manuf_specific_data = &manuf_data;

    options.ble_adv_whitelist_enabled     = BLE_ADV_WHITELIST_DISABLED;
    options.ble_adv_directed_enabled      = BLE_ADV_DIRECTED_ENABLED;
    options.ble_adv_directed_slow_enabled = BLE_ADV_DIRECTED_SLOW_DISABLED;
    options.ble_adv_fast_enabled          = BLE_ADV_FAST_DISABLED;
    options.ble_adv_slow_enabled          = BLE_ADV_SLOW_DISABLED;

    err_code = ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}


void advertising_start(void)
{
    uint32_t err_code;

    APPL_LOG("Calling ADV start. \r\n");
    err_code = ble_advertising_start(BLE_ADV_MODE_DIRECTED);
    APP_ERROR_CHECK(err_code);
}


void advertising_stop(void)
{
    uint32_t err_code;

    err_code = sd_ble_gap_adv_stop();
    m_adv_mode_current = BLE_ADV_MODE_IDLE;
    APP_ERROR_CHECK(err_code);
}


/** @brief Function to see if a device saw in scanning
 *  is already in our list of closest 3 beacons.
 *
 */
static int8_t is_device_in_list(uint8_t *addr)
{
    uint8_t counter;
    for(counter = 0; counter < 3; counter++)
    {
        if(memcmp(addr, beacons[counter].address.addr, BLE_GAP_ADDR_LEN) == 0)
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
void on_ble_evt(ble_evt_t * p_ble_evt)
{
    const ble_gap_evt_t   * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
        /* Got a new advertisement packet */
        case BLE_GAP_EVT_ADV_REPORT:
        {
            int8_t rssi = p_gap_evt->params.adv_report.rssi;
            uint8_t *addr = (uint8_t *)(p_gap_evt->params.adv_report.peer_addr.addr);
            uint8_t addr_type = p_gap_evt->params.adv_report.peer_addr.addr_type;
            uint8_t *data = (uint8_t *)(p_gap_evt->params.adv_report.data);
            uint8_t data_len = p_gap_evt->params.adv_report.dlen;
            int8_t position;
            uint16_t beacon_id;

            /* First see if the device is a non-connectable beacon.
             * If it's not, ignore this device.
             */
            if(p_gap_evt->params.adv_report.type != BLE_GAP_ADV_TYPE_ADV_NONCONN_IND)
            {
                break;
            }

            /* Check the beacon's data format: Flags followed by Manuf. data.
             * There should be nothing else in the packet.
             */
            {
                /* Length included in flags field */
                uint8_t field_flags[] = {ADV_AD_TYPE_FIELD_SIZE + AD_TYPE_FLAGS_DATA_SIZE,
                                         BLE_GAP_AD_TYPE_FLAGS,
                                         BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE};

                /* Length not included in manuf. data since it can be variable */
                uint8_t field_manuf_data[] = {BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
                                              MANUFACTURER_ID_TALENTICA_MSB,
                                              MANUFACTURER_ID_TALENTICA_LSB};

                uint8_t index;

                if(memcmp(field_flags, data, sizeof(field_flags)) != 0)
                {
                    /* Flags field not present, or invalid */
                    break;
                }

                if(memcmp(field_manuf_data, &data[sizeof(field_flags) + 1], sizeof(field_manuf_data)) != 0)
                {
                    /* Manufacturing data not present, or invalid.
                     * Length ignored in check (hence the +1). */
                    break;
                }

                if((data[sizeof(field_flags)] + sizeof(field_flags)) != data_len)
                {
                    /* Length check: Packet length is invalid */
                    break;
                }

                /* Index of the opcode field for keep-alive (no data) beacons. */
                index = sizeof(field_flags) + 1 + sizeof(field_manuf_data);

                /* If the beacon is just a keep-alive (no data), extract the beacon ID. */
                if((data[index] & 0x80) == SEND_NO_DATA)
                {
                    beacon_id = (data[index + 1] << 8) | data[index + 2];
                }
                else
                {
                    uint16_t msg_source_id      = (data[index + 1] << 8) | data[index + 2];
                    uint16_t msg_destination_id = (data[index + 3] << 8) | data[index + 4];
                    uint8_t opcode = data[index] & 0x7F;
                    uint8_t length = data_len - index - 5;

                    if(msg_destination_id != source_id)
                    {
                        /* Packet is not meant for us */
                        break;
                    }

                    /* Send packet to the application */
                    application_handler(opcode, msg_source_id, &data[index + 5], length);

                    /* The processing is done. No need for further checks. */
                    break;
                }
            }

            /* The processing below is only for keep-alive beacons.
             * First, see if the device already exists in the list.
             */
            position = is_device_in_list(addr);

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
                    memcpy(beacons[index].address.addr, addr, BLE_GAP_ADDR_LEN);
                    beacons[index].address.addr_type = addr_type;
                    beacons[index].isRssiValid = true;
                    beacons[index].beacon_id = beacon_id;
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
                        memcpy(beacons[max_index].address.addr, addr, BLE_GAP_ADDR_LEN);
                        beacons[max_index].address.addr_type = addr_type;
                        beacons[max_index].beacon_id = beacon_id;
                        time_counter[max_index] = 10;
                        calc_rssi_min_max();
                    }
                }
            }

            break;
        }

        case BLE_GAP_EVT_TIMEOUT:
            break;

        default:
            break;
    }
}


/** @brief Callback function to handle timers for beacons. When a timer expires
 *  the corresponding beacon is removed from the list of closest beacons.
 *  The application is supposed to scan for beacons regularly and keep feeding
 *  the timer in order to sustain the beacon in the list.
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
void create_beacon_timer(void)
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


/** @brief Main function for mesh transport. Ensures that the device
 *  stays in touch with the closest beacon.
 *
 */
void mesh_transport_run(void)
{
    /* Once we have atleast one beacon closeby, advertise to it. */
    if(m_adv_mode_current == BLE_ADV_MODE_IDLE)
    {
        if(numberOfElements > 0)
        {
            advertising_init(0, NULL, 0);
            advertising_start();
        }
    }
    else
    {
        if(numberOfElements == 0)
        {
            /* We lost all beacons close by. Stop advertising. */
            advertising_stop();
        }
        else if(isMinChanged)
        {
            /* If a new beacon is closest now, start talking to that
             * one instead. Change the target device for directed ADV.
             */
            isMinChanged = false;
            advertising_stop();
            advertising_start();
        }
    }
}

/* End of file */
