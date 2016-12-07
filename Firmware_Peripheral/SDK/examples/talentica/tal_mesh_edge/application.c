/** File to handle application content for mesh.
 *
 */


#include "nrf_log.h"
#include "application.h"
#include "transport.h"


/** Opcodes for mesh operation. These can be of three types:
 *  Get messages - that request some information
 *  Set messages - that set a state (control value)
 *  Status messages - that respond to a Get query
 */
#define OPCODE_LOCATION_GET             (0x3A)  /* Get the location of a peer device */
#define OPCODE_LOCATION_STATUS          (0x3B)  /* Send current location status of this device */


/** @brief Function is the trigger point to send data over mesh.
 *
 */
static void mesh_get_peer_location(uint16_t destination_id)
{
    uint8_t opcode = OPCODE_LOCATION_GET;
    uint8_t data[2];

    if(destination_id == source_id)
    {
    	APPL_LOG("Trying to ping ourselves... Sorry.\r\n");
    	return;
    }

    data[0] = destination_id & 0x00FF;
    data[1] = (destination_id >> 8) & 0x00FF;

    APPL_LOG("Asking for peer location...\r\n");

    advertising_change_data(opcode, data, sizeof(data));
}


/** @brief Function sends the device's current location to the peer.
 *
 */
static void mesh_send_current_location(uint16_t msg_destination_id)
{
    uint8_t opcode = OPCODE_LOCATION_STATUS;
    uint8_t data[4];

    data[0] = msg_destination_id & 0x00FF;
    data[1] = (msg_destination_id >> 8) & 0x00FF;
    data[2] = beacons[min_index].beacon_id;
    data[3] = beacons[min_index].beacon_id >> 8;

    APPL_LOG("Replying to peer location query...\r\n");

    advertising_change_data(opcode, data, sizeof(data));
}


/** @brief Function is the main event handler for all opcodes received
 *  from the mesh network.
 */
void application_event_handler(uint8_t opcode, uint16_t msg_source_id, const uint8_t * data, uint8_t length)
{
    switch(opcode)
    {
    case OPCODE_LOCATION_GET:
        /* Send our current location if there is a beacon closeby */
        if(numberOfElements > 0)
        {
            mesh_send_current_location(msg_source_id);
        }
        break;

    case OPCODE_LOCATION_STATUS:
        APPL_LOG("Peer device's current location is: %04x\r\n", (data[1] << 8) | data[0]);
        break;

    default:
        break;
    }
}


/** @brief Function to handle user input etc.
 *
 */
void mesh_application_run(void)
{
    char input_data;

    while(NRF_LOG_HAS_INPUT())
    {
        if(NRF_SUCCESS == NRF_LOG_READ_INPUT(&input_data))
        {
            if(input_data == '1')
            {
                APPL_LOG("Find a peer device. Which one? \r\n");
                while(!NRF_LOG_HAS_INPUT());
                if(NRF_SUCCESS == NRF_LOG_READ_INPUT(&input_data))
                {
                    APPL_LOG("%c.\r\n", input_data);
                    switch(input_data)
                    {
                    case '1':
                        mesh_get_peer_location(DEVICE_1_SOURCE_ID);
                        break;

                    case '2':
                        mesh_get_peer_location(DEVICE_2_SOURCE_ID);
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}


/* End of file */
