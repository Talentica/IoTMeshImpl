/***************************************************************************//**
* \file main.c
* \version 1.0
* 
* \brief
*  Contains the entry point for application and handling Smart Mesh LED commands.
* 
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include <stdbool.h>
#include <math.h>
#include <main.h>


#define ADV_TYPE_MANUFACTURER_DATA      (0xFF)
#define MANUFACTURER_ID_TALENTICA_MSB   (0x55)
#define MANUFACTURER_ID_TALENTICA_LSB   (0xAA)

#define SEND_NO_DATA                    (0)
#define SEND_DATA                       (1)

#define DEVICE_MESH                     (0)
#define DEVICE_PERIPHERAL               (1)

#define BIT_POS_IS_DATA                 (7)
#define BIT_POS_IS_PERIPHERAL           (6)
#define MASK_IS_DATA                    (0x80)
#define MASK_IS_PERIPHERAL              (0x40)
#define MASK_OPCODE                     (0x3F)

#define LIST_SIZE                       (10)

/*************************Global Variables***********************************/
typedef struct 
{
    uint16 sourceId;
    bool isEntryValid;
    uint8 timeCounter;
} DEVICES_T;

static DEVICES_T devicesCloseBy[10];
static uint8 numberOfDevices = 0;

static uint16 beaconId = 0;
static volatile bool isBeaconFlagSet = false;


/******************************Function Definitions***********************************/

static void AddDeviceToList(uint16 incomingSourceId)
{
    uint8 counter;
    
    for(counter = 0; counter < LIST_SIZE; counter++)
    {
        if(devicesCloseBy[counter].isEntryValid == false)
        {
            devicesCloseBy[counter].isEntryValid = true;
            devicesCloseBy[counter].sourceId = incomingSourceId;
            devicesCloseBy[counter].timeCounter = 10;
            break;
        }
    }
}


static int8 FindDeviceInList(uint16 incomingSourceId)
{
    uint8 counter;
    
    for(counter = 0; counter < LIST_SIZE; counter++)
    {
        if(devicesCloseBy[counter].isEntryValid)
        {
            if(devicesCloseBy[counter].sourceId == incomingSourceId)
            {
                return counter;
            }
        }
    }
    return -1;
}


void TimerIsr(void)
{
    /* Decrement the counters for all the valid devices nearby.
     * When a device counter expires, erase it from the list.
     */
    uint8 counter;
    
    printf("Timer\r\n");
    
    /* Every second, trigger a non-connectable beacon */
    isBeaconFlagSet = true;
    
    if(numberOfDevices == 0)
    {
        return;
    }
    
    for(counter = 0; counter < LIST_SIZE; counter++)
    {
        if(devicesCloseBy[counter].isEntryValid)
        {
            devicesCloseBy[counter].timeCounter--;
            
            if(devicesCloseBy[counter].timeCounter == 0)
            {
                devicesCloseBy[counter].isEntryValid = false;
                numberOfDevices--;
            }
        }
    }
}


void SendDataBeacon(const uint8 * payload, uint8 payloadLength)
{
    uint8 data[20];
    uint8 length = 0;
    
    /* Flags */
    data[0] = CYBLE_GAP_ADV_FLAGS_PACKET_LENGTH;
    data[1] = CYBLE_GAP_ADV_FLAGS;
    data[2] = CYBLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | CYBLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
    
    /* Manufacturer data */
    data[3] = payloadLength + 5;
    data[4] = ADV_TYPE_MANUFACTURER_DATA;
    data[5] = MANUFACTURER_ID_TALENTICA_MSB;
    data[6] = MANUFACTURER_ID_TALENTICA_LSB;
    data[7] = (SEND_DATA << BIT_POS_IS_DATA) | (DEVICE_MESH << BIT_POS_IS_PERIPHERAL) | (payload[0] & MASK_OPCODE);
    data[8] = (beaconId >> 7) & 0x00FF;
    data[9] = beaconId & 0x00FF;
    memcpy(&data[10], &payload[1], payloadLength - 1);
    
    length = payloadLength + 9;
    
    /* Call the mesh API to send custom beacon */
//    CyMesh_SendBeacon(data, length);
}


void SendEmptyBeacon(void)
{
    uint8 data[10];
    uint8 length = 0;
    
    /* Flags */
    data[0] = CYBLE_GAP_ADV_FLAGS_PACKET_LENGTH;
    data[1] = CYBLE_GAP_ADV_FLAGS;
    data[2] = CYBLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | CYBLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
    
    /* Manufacturer data */
    data[3] = 6;    /* Length of manufacturer data field */
    data[4] = ADV_TYPE_MANUFACTURER_DATA;
    data[5] = MANUFACTURER_ID_TALENTICA_MSB;
    data[6] = MANUFACTURER_ID_TALENTICA_LSB;
    data[7] = (SEND_NO_DATA << BIT_POS_IS_DATA) | (DEVICE_MESH << BIT_POS_IS_PERIPHERAL);
    data[8] = (beaconId >> 7) & 0x00FF;
    data[9] = beaconId & 0x00FF;
    
    length = 10;
    
    /* Call the mesh API to send custom beacon */
//    CyMesh_SendBeacon(data, length);
}


/* Data consists of Opcode (1) + Source ID (2) + Destination ID (2) + Parameter
 *
 */
void SendMeshPacket(uint8 opcode, const uint8 * data, uint8 length)
{
    CYMESH_VARIABLE_DATA_T packet;
    
    packet.data[0] = opcode & MASK_OPCODE;
    memcpy(&packet.data[1], data, length);
    packet.len = length;
    
    CyMesh_VendorSpecificSendDataUnreliable(packet, 
                                            CYMESH_MODEL_VENDOR_SPECIFIC_COMPONENT_INDEX, 
                                            CYMESH_MODEL_VENDOR_SPECIFIC_MODEL_INDEX);
}


/******************************************************************************
* Function Name: MeshEventHandler
*******************************************************************************
* 
*  Callback function for Mesh Related events. All events are currently handled
*  by the Smart Mesh Stack. Thus this function is empty.
* 
*  \param 
*	event: The type of event raised
*   eventParam: The parameter associated with the event raised
*
*  \return None
*  
******************************************************************************/
void MeshEventHandler(uint32 event, void * eventParam,uint8 compIndex, uint8 modelIndex)
{
	/* Handle Mesh specific events. The eventParam depends on event raised */
	switch(event)
	{
		case CYMESH_EVT_STACK_ON:
			/* Initialize configInfoRam with information that is not present */
			DefineNodeinfo();
		    break;
		
        
		case CYMESH_EVT_MESSAGE_VENDOR_SPECIFIC_UNREL_SET:
		{
		    /* Message received from the mesh.
             * See if it belongs to us, if so - forward to the peripheral.
             */
			CYMESH_VARIABLE_DATA_T * vend_data = (CYMESH_VARIABLE_DATA_T*) eventParam;
			uint8 data_len = vend_data->len;
            uint8 * data = vend_data->data;
            uint8 incomingDestinationId = (data[1] << 7) | data[2];
            
            if(FindDeviceInList(incomingDestinationId) >= 0)
            {
                /* Send the data coming from mesh as a beacon.
                 * This API has to be supplied by Cypress.
                 * Ensure that the beacon goes out only once (should be default in the API). 
                 */
                SendDataBeacon(data, data_len);
            }
		    break;
		}

        
		default:
		    break;
	}
}


/******************************************************************************
* Function Name: GenericEventHandler
*******************************************************************************
* 
*  Callback function for Generic BLE related events. All events are currently 
*  handled by the Smart Mesh Stack. Thus this function is empty.
* 
*  \param 
*	event: The type of event raised
*   eventParam: The parameter associated with the event raised
*
*  \return None
*  
******************************************************************************/
void GenericEventHandler(uint32 event, void * eventParam)
{
	switch(event)
	{
        case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
        {
            CYBLE_GAPC_ADV_REPORT_T * advReport = (CYBLE_GAPC_ADV_REPORT_T *)eventParam;
            uint8 * data = advReport->data;
            uint8 data_len = advReport->dataLen;
            
            /* Ensure that only non-connectable ADV is parsed */
            if(advReport->eventType != CYBLE_GAPC_NON_CONN_UNDIRECTED_ADV)
            {
                break;
            }
            
            /* Check the beacon's data format: Flags followed by Manuf. data.
             * There should be nothing else in the packet.
             */
            {
                uint8_t field_flags[] = {CYBLE_GAP_ADV_FLAGS_PACKET_LENGTH,
                                         CYBLE_GAP_ADV_FLAGS,
                                         CYBLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | CYBLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED};

                /* Length not included in manuf. data since it can be variable */
                uint8_t field_manuf_data[] = {ADV_TYPE_MANUFACTURER_DATA,
                                              MANUFACTURER_ID_TALENTICA_MSB,
                                              MANUFACTURER_ID_TALENTICA_LSB};

                uint8_t index;
                uint8 incomingBeaconId;
                uint8 incomingSourceId;

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

                if((data[sizeof(field_flags)] + sizeof(field_flags) + 1) != data_len)
                {
                    /* Length check: Packet length is invalid */
                    break;
                }

                /* Index of the opcode field for keep-alive (no data) beacons. */
                index = sizeof(field_flags) + 1 + sizeof(field_manuf_data);

                /* Don't confuse mesh devices acting as beacons with actual peripherals */
                if((data[index] & MASK_IS_PERIPHERAL) == (DEVICE_MESH << BIT_POS_IS_PERIPHERAL))
                {
                    break;
                }
                
                incomingBeaconId = (data[index + 1] << 8) | data[index + 2];
                incomingSourceId = (data[index + 3] << 8) | data[index + 4];
                
                /* If this is not the target beacon, drop packet */
                if(incomingBeaconId != beaconId)
                {
                    break;
                }
                
                /* If the beacon is just a keep-alive (no data), extract the beacon ID. */
                if((data[index] & MASK_IS_DATA) == SEND_NO_DATA)
                {
                    int8 index = FindDeviceInList(incomingSourceId);
                    
                    /* Either add to the list of devices closeby, or update timer */
                    if((index == -1) && (numberOfDevices < LIST_SIZE))
                    {
                        AddDeviceToList(incomingSourceId);
                        numberOfDevices++;
                    }
                    else
                    {
                        devicesCloseBy[index].timeCounter = 10;
                    }
                }
                else
                {
                    uint8_t opcode = data[index] & MASK_OPCODE;
                    uint8_t length = data_len - index - 3;

                    /* If the incomingSourceId is not part of the list, drop packet */
                    if(FindDeviceInList(incomingSourceId) == -1)
                    {
                        break;
                    }
                    
                    /* Send packet to the mesh network */
                    SendMeshPacket(opcode, &data[index + 3], length);
                }
            }
            
            break;
        }
        
        
		default:
    		break;
	}
}


void MySwitchIsr(void)
{
    /* Add custom code here if required */
    
    
    /* Clear interrupt */
    SW2_ClearInterrupt();
    SW2_Interrupt_ClearPending();
}


/******************************************************************************
* Function Name: InitializeSystem
*******************************************************************************
* 
*  This is the first function called in the application after bootup. This 
* starts the relevant components, registers functions for callbacks and set
* default values. 
* Add your component start code here.
* 
*  \param None
*
*  \return None
*  
******************************************************************************/
void InitializeSystem(void)
{
	CyGlobalIntEnable; /* Enable global interrupts. */
    
	#ifdef CYMESH_DEBUG_ENABLED
		UART_Start();
		printf("BLE Mesh example. \r\n");
	#endif
    
	/* Start BLE Mesh and register all relevant functions */
    CyMesh_Start(GenericEventHandler, MeshEventHandler);
	
	/* Call CyMesh_ProcessEvents once to enable the Mesh Stack*/
	CyMesh_ProcessEvents();

	/* Register the application timing function with Mesh protocol Stack timer.
     * The timer would have a period of 10 ms, and we need 1 second period for 
     * the callback.
     */
	CyMesh_TimerRegisterCallback(0, TimerIsr, 100);
	
	/* Application Level node information setting. These information are must to allow
	* proper Smart Mesh functionality, if no provisioning can be done */
    #if (CYMESHTEST_START_DIRECTLY_WITH_RELAY)
        DefineAppInfo();
        DefineNetInfo();
    #endif
    
    /* Register an interrupt for the switch */
    SW2_Interrupt_StartEx(MySwitchIsr);
    
    /* Assign a random value to beaconId */
    beaconId = (*(uint32 *)CYREG_SFLASH_DIE_X << 8) | 
                *(uint32 *)CYREG_SFLASH_DIE_Y;
}

/******************************************************************************
* Function Name: main
*******************************************************************************
* 
*  The application entry point. It calls the function to initialize the system
* and calls event processing functions in forever loop.
* 
*  \param None
*
*  \return int
*  
******************************************************************************/
int main()
{
	/* Initialize the system */
    InitializeSystem();

	for(;;)
    {
		/* Function to process pending BLE and Mesh events. This MUST BE CALLED as often
		* as possible to prevent missing of events */
		CyMesh_ProcessEvents();
        
        if(isBeaconFlagSet == true)
        {
            SendEmptyBeacon();
            isBeaconFlagSet = false;
        }
    }
}

/* [] END OF FILE */
