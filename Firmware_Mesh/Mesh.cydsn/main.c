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
#define PERIPHERAL_PRESENCE_TIMEOUT_S   (10)

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
            devicesCloseBy[counter].timeCounter = PERIPHERAL_PRESENCE_TIMEOUT_S;
            
            printf("Adding to list. Index = %d. Device = %04x\r\n", counter, incomingSourceId);
            
            break;
        }
    }
}


static int8 FindDeviceInList(uint16 incomingSourceId)
{
    uint8 counter;
    
    for(counter = 0; counter < LIST_SIZE; counter++)
    {
        if(devicesCloseBy[counter].isEntryValid == true)
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
    
    TimerInterrupt_ClearPending();
    Timer_ClearInterrupt(Timer_INTR_MASK_TC);
    
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
                printf("Removing device @ index = %d. Device = %04x\r\n", counter, 
                                            devicesCloseBy[counter].sourceId);
                devicesCloseBy[counter].isEntryValid = false;
                numberOfDevices--;
            }
        }
    }
    
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
    
//    printf("Transmit\r\n");
    
    CyMesh_VendorSpecificSendDataUnreliable(packet, 
                                            CYMESH_MDL_VENDOR_SPECIFIC_COMP_3, 
                                            CYMESH_MDL_VENDOR_SPECIFIC_COMP_3_MDLIDX);
}


void SendDataBeacon(const uint8 * payload, uint8 payloadLength)
{
    uint8 data[20];
    uint8 length = 0;
    
    /* Flags */
    data[0] = CYBLE_GAP_ADV_FLAGS_PACKET_LENGTH;
    data[1] = CYBLE_GAP_ADV_FLAGS + 1;  /* TODO: This is a stack defect */
    data[2] = CYBLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | CYBLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
    
    /* Manufacturer data */
    data[3] = payloadLength + 5;
    data[4] = ADV_TYPE_MANUFACTURER_DATA;
    data[5] = MANUFACTURER_ID_TALENTICA_MSB;
    data[6] = MANUFACTURER_ID_TALENTICA_LSB;
    data[7] = (SEND_DATA << BIT_POS_IS_DATA) | (DEVICE_MESH << BIT_POS_IS_PERIPHERAL) | (payload[0] & MASK_OPCODE);
    data[8] = (beaconId >> 8) & 0x00FF;
    data[9] = beaconId & 0x00FF;
    memcpy(&data[10], &payload[1], payloadLength - 1);
    
    length = payloadLength + 9;
    
    /* Call the mesh API to send custom beacon */
    CyMesh_BearerSendData(data, length, CYMESH_BEARER_CUSTOM_ADV, 2, false, true);
}


void SendEmptyBeacon(void)
{
    uint8 data[10];
    uint8 length = 0;
    
    /* Flags */
    data[0] = CYBLE_GAP_ADV_FLAGS_PACKET_LENGTH;
    data[1] = CYBLE_GAP_ADV_FLAGS + 1;  /* TODO: This is a stack defect */
    data[2] = CYBLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | CYBLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
    
    /* Manufacturer data */
    data[3] = 6;    /* Length of manufacturer data field */
    data[4] = ADV_TYPE_MANUFACTURER_DATA;
    data[5] = MANUFACTURER_ID_TALENTICA_MSB;
    data[6] = MANUFACTURER_ID_TALENTICA_LSB;
    data[7] = (SEND_NO_DATA << BIT_POS_IS_DATA) | (DEVICE_MESH << BIT_POS_IS_PERIPHERAL);
    data[8] = (beaconId >> 8) & 0x00FF;
    data[9] = beaconId & 0x00FF;
    
    length = 10;
    
    CyMesh_BearerSendData(data, length, CYMESH_BEARER_CUSTOM_ADV, 1, false, true);  
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
void MeshEventHandler(uint32 event, void * eventParam, uint8 compIndex, uint8 modelIndex)
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
            uint16 incomingDestinationId = (data[3] << 8) | data[4];
            
//            printf("Receive\r\n");
            
            if(FindDeviceInList(incomingDestinationId) >= 0)
            {
                /* Send the data coming from mesh as a beacon.
                 * This API has to be supplied by Cypress.
                 * Ensure that the beacon goes out only once (should be default in the API). 
                 */
                SendDataBeacon(data, data_len);
            }
            else
            {
                /* Packet dropped */
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
                uint16 incomingBeaconId;
                uint16 incomingSourceId;

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
                
                /* If the beacon is just a keep-alive (no data), extract the source ID. */
                if((data[index] & MASK_IS_DATA) == SEND_NO_DATA)
                {
                    int8 index = FindDeviceInList(incomingSourceId);
                    
                    /* Either add to the list of devices closeby, or update timer */
                    if(index == -1)
                    {
                        if(numberOfDevices < LIST_SIZE)
                        {
                            AddDeviceToList(incomingSourceId);
                            numberOfDevices++;
                        }
                        else
                        {
                            /* Drop packet */
                        }
                    }
                    else
                    {
                        devicesCloseBy[index].timeCounter = PERIPHERAL_PRESENCE_TIMEOUT_S;
                    }
                }
                else
                {
                    uint8_t opcode = data[index] & MASK_OPCODE;
                    uint8_t length = data_len - index - 2;

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
		printf("******** Mesh device. ");
	#endif
    
	/* Start BLE Mesh and register all relevant functions */
    CyMesh_varInit();
    CyMesh_Start(GenericEventHandler, MeshEventHandler);
	
	/* Call CyMesh_ProcessEvents once to enable the Mesh Stack*/
	CyMesh_ProcessEvents();

    /* Initialize timer for regular beacon */
    TimerInterrupt_StartEx(TimerIsr);
    Timer_Start();
	
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
                
    printf("ID = %04x ******** \r\n\n", beaconId);
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
