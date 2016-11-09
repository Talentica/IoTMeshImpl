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
#define OPCODE_LOCATION_GET             (0x7A)  /* Get the location of a peer device */
#define OPCODE_LOCATION_STATUS          (0x7B)  /* Send current location status of this device */


static uint16_t destination_id = 0xFFBB;


/** @brief Function is the trigger point to send data over mesh.
 *
 */
void mesh_get_peer_location(void)
{
    /* Change the packet to send a new data packet.
     * Todo: this packet should go out only once.
     * Then revert back to the original beacon packet.
     */
    uint8_t opcode = OPCODE_LOCATION_GET;
    uint8_t data[2];

    data[0] = (destination_id >> 8) & 0x00FF;
    data[1] = destination_id & 0x00FF;

    advertising_init(opcode, data, sizeof(data));
}


/** @brief Function sends the device's current location to the peer.
 *
 */
static void mesh_send_current_location(uint16_t msg_destination_id)
{
    uint8_t opcode = OPCODE_LOCATION_STATUS;
    uint8_t data[4];

    data[0] = (msg_destination_id >> 8) & 0x00FF;
    data[1] = msg_destination_id & 0x00FF;
    data[2] = beacons[min_index].beacon_id >> 8;
    data[3] = beacons[min_index].beacon_id;

    advertising_init(opcode, data, sizeof(data));
}


/** @brief Function is the main event handler for all opcodes received
 *  from the mesh network.
 */
void application_handler(uint8_t opcode, uint16_t msg_source_id, const uint8_t * data, uint8_t length)
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
        APPL_LOG("Peer device's current location is: %d\r\n", (data[0] << 8) | data[1]);
        break;

    default:
        break;
    }
}


/* End of file */
