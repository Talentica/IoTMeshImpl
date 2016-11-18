/***************************************************************************//**
* \file CyMesh_Bearer.h
* \version 1.0
* 
* \brief
*  This is the header file for the BLE SmartMesh v1 Bearer Layer.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYMESH_BEARER_H
#define _CYMESH_BEARER_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include "project.h"
#include "CyMesh_Common.h"



/*******************************************************************************
* Compile Time Options
*******************************************************************************/
#define CYMESH_BEARER_ADV_RX_BUFFER_SIZE                    (4)
#define CYMESH_BEARER_ADV_TX_BUFFER_SIZE                    (10)
#define CYMESH_BEARER_GATT_MAX_DATA_LENGTH                  (68u)

#define CYMESH_BEARER_ADV_MAX_LENGTH						(31u)
#define CYMESH_BEARER_BD_ADDRESS_LENGTH						(6u)

/*******************************************************************************
* Macros
*******************************************************************************/
    
#define CYMESH_BEARER_NON_CONN_ADV_INTERVAL_MS              (105)
#define CYMESH_BEARER_NON_CONN_ADV_INTERVAL_COUNT           (168)
#define CYMESH_BEARER_NON_CONN_ADV_CHANNEL_MAP              (7)
	
#define CYMESH_BEARER_CONN_ADV_INTERVAL_MS              	(105)
#define CYMESH_BEARER_CONN_ADV_INTERVAL_COUNT           	(168)
#define CYMESH_BEARER_CONN_ADV_CHANNEL_MAP              	(7)

#define CYMESH_BEARER_BEACON_TYPE_UNPROVISIONED				(0x00)
#define CYMESH_BEARER_BEACON_TYPE_SECURE_BEACON				(0x01)
	
#define CYMESH_BEARER_ADV_FIELD_UNPROV_SERVICE_UUID_MSB     (0x7F)
#define CYMESH_BEARER_ADV_FIELD_UNPROV_SERVICE_UUID_LSB     (0xD1)
#define CYMESH_BEARER_ADV_FIELD_PROXY_SERVICE_UUID_MSB      (0x7F)
#define CYMESH_BEARER_ADV_FIELD_PROXY_SERVICE_UUID_LSB      (0xD2) 
	
#define CYMESH_BEARER_ADV_NO_OF_BEACONS_TYPE				(4)
		
#define CYMESH_BEARER_ADV_FIELD_BEACON_UNPROVISIONED_NODE   (0x01)
#define CYMESH_BEARER_ADV_FIELD_BEACON_SECURE_NETWORK       (0x02)

#define CYMESH_BEARER_GATT_PROV_NOTIF_BIT_MASK				(0x01)
#define CYMESH_BEARER_GATT_PROXY_NOTIF_BIT_MASK				(0x02)

/*******************************************************************************
* Enums
*******************************************************************************/
typedef enum
{
    CYMESH_BEARER_ADV,
	CYMESH_BEARER_BEACON_UNPROVISIONED_NODE,
    CYMESH_BEARER_BEACON_SECURE_NETWORK,
	CYMESH_BEARER_PB_ADV,
	CYMESH_BEARER_BEACON_PROV_SERVICE_NODE,
	CYMESH_BEARER_BEACON_PROXY_SERVICE_NODE,
	
	#if (CYMESH_BEARER_GATT_BEARER_ENABLED == 1)
	CYMESH_BEARER_PROXY_DATA,
	CYMESH_BEARER_PB_GATT,
	#endif
	
    CYMESH_BEARER_CUSTOM_ADV,                   /* Provision to define fully custom ADV packet */
	CYMESH_BEARER_BEACON_UNDEFINED = 0xFF,
	
} CYMESH_BEARER_PACKET_TYPE_T;


/* State of the TX Buffer */
typedef enum
{
    /* TX buffer is completely empty */
    CYMESH_BEARER_TX_BUFFER_EMPTY,
    
    /* TX buffer is not empty but can take more data */
    CYMESH_BEARER_TX_BUFFER_FREE,
    
    /* TX buffer is approaching its capacity */
    CYMESH_BEARER_TX_BUFFER_BUSY,
    
    /* TX buffer is completely full */
    CYMESH_BEARER_TX_BUFFER_FULL
} CYMESH_BEARER_TX_BUFFER_STATE_T;



/*******************************************************************************
* Data Structures
*******************************************************************************/
typedef struct
{
    /* Actual packet received */
    uint8 data[CYMESH_BEARER_ADV_MAX_LENGTH];
    
    /* Length of the packet */
    uint8 length;
    
    /* Whether Mesh ADV or Mesh Beacon */
    CYMESH_BEARER_PACKET_TYPE_T packetType; 
    
    /* RSSI of the packet received */
    int8 rssi;
	
	#if (CYMESH_BEARER_GATT_BEARER_ENABLED == 1)
	/* BD Address of the device sending the packet */
	CYBLE_GAP_BD_ADDR_T device_addr;
	#endif
} CYMESH_BEARER_RX_BUFFER_T;


typedef struct
{
    /* Follow with scan (1 bit) | Priority (1 bit) | Valid (1 bit) */
    uint8 info;     
    
    /* Actual data to be transmitted - does not include ADV headers */
    uint8 data[CYMESH_BEARER_ADV_MAX_LENGTH];
    
    /* Length of the packet */
    uint8 length;
    
    /* Number of times to transmit this packet */
    uint8 count;
    
    /* Time when this packet was sent last; Zero initializeds */
    uint32 timestamp;
	
	/* Packet type of the buffer content */
	CYMESH_BEARER_PACKET_TYPE_T packetType;
} CYMESH_BEARER_TX_BUFFER_T;



/*******************************************************************************
* Externed Variables
*******************************************************************************/



/*******************************************************************************
* Externed functions
*******************************************************************************/


/******************************************************************************
* Function Name: CyMesh_BearerStart
***************************************************************************//**
* 
*  The function initializes the Bearer Layer. It performs the following things:
*  1. Starts the Bearer with an initial state as per the argument. 
*  2. Initializes the BLE component (includes BLE stack). 
*
*  If this function is called multiple times without calling CyMesh_BearerStop()
*  in between, the function returns a failure code.
*
*  The function checks for a valid callback pointer for two callbacks - the 
*  mesh specific callback (passed as an argument here), and the generic callback
*  (available as part of a higher layer). If any callback is found to be 
*  invalid (NULL) then the function returns with an error.
* 
*  \param meshEventCallback: Function to be called to inform about incoming
*         mesh events. 
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function 
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_CALLBACK</td>
*    <td>Atleast one of the two callback pointers is NULL.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_MULTIPLE_INITIALIZATION</td>
*    <td>Function called more than once without calling CyMesh_BearerStop()</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_BLE_OPERATION_FAILURE</td>
*    <td>BLE component failed to initialize.</td>
*  </tr>
*  </table>
* 
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_BearerStart(CYMESH_CALLBACK_T meshEventCallback);



/******************************************************************************
* Function Name: CyMesh_BearerStop
***************************************************************************//**
* 
*  This function stops all BLE activity. It resets the Bearer state. For any
*  further operation, CyMesh_BearerStart() must be called.
* 
*  \param None.
* 
*  \return None.
*  
* 
******************************************************************************/
extern void CyMesh_BearerStop(void);




/******************************************************************************
* Function Name: CyMesh_BearerSendData
***************************************************************************//**
* 
*  The function takes the data to be advertised, appends it with the required
*  ADV headers for transmission over BLE, and stores it in a TX buffer (if the
*  buffer has space).
* 
*  \param data: The actual data to be transmitted. This data is copied to the 
*         TX buffer, but the buffer is pre-appended by the required ADV headers
*         (such as ADV length, Mesh ADV type etc.) by this function.
*
*  \param length: Length of the payload. 
*
*  \param packetType: Type of the packet. Valid values come from the enum
*         CYMESH_BEARER_PACKET_TYPE_T. Depending on this parameter, the right
*         headers for the ADV packet are added.
*
*  \param txCount: Number of times to advertise this packet. Valid values from
*         0 to 255.
*
*  \param priority: Priority (1 or 0) of this packet. Priority=1 packets are
*         sent before other packets.
*
*  \param isScanFollowed: Whether to immediately follow the advertisement of 
*         this packet with a scan. Only valid for edge devices.
* 
*  \return CYMESH_API_RETURN_T: Return value indicates if the function 
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_BEARER_TX_BUFFER_FULL</td>
*    <td>The TX buffer is full and this packet is not accepted.</td>
*  </tr>
*  </table>
* 
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_BearerSendData(const uint8 * data, 
                                                        uint8 length, 
                                                        CYMESH_BEARER_PACKET_TYPE_T packetType, 
                                                        uint8 txCount, 
                                                        bool priority, 
                                                        bool isScanFollowed);



/******************************************************************************
* Function Name: CyMesh_BearerGetTxBufferStatus
***************************************************************************//**
* 
*  The function returns the current status of the TX buffer. This is an inline
*  function.
* 
*  \param None.
*
*  \return CYMESH_BEARER_TX_BUFFER_STATE_T: Current status of the TX Buffer.
* 
******************************************************************************/
extern CYMESH_BEARER_TX_BUFFER_STATE_T CyMesh_BearerGetTxBufferStatus(void);




/******************************************************************************
* Function Name: CyMesh_BearerStartScan
***************************************************************************//**
* 
*  The function schedules a scan if the Bearer is not already in scan mode.
*  Any ongoing advertisement is stopped.
* 
*  \param None.
*
*  \return None.
* 
******************************************************************************/
extern void CyMesh_BearerStartScan(void);



/******************************************************************************
* Function Name: CyMesh_BearerStopScan
***************************************************************************//**
* 
*  The function stops any ongoing scan. If any advertisement is ongoing, it is 
*  not affected.
* 
*  \param None.
*
*  \return None.
* 
******************************************************************************/
extern void CyMesh_BearerStopScan(void);

/******************************************************************************
* Function Name: CyMesh_BearerProcessEvents
***************************************************************************//**
* 
*  This function processes events at bearer layer, such as TX and RX path 
* handling.
* 
*  \param None.
*
*  \return None.
* 
******************************************************************************/
extern void CyMesh_BearerProcessEvents(void);

/******************************************************************************
* Function Name: CyMesh_BearerStopScan
***************************************************************************//**
* 
*  The function sends provisioning data over ntoification to the connected GATT
* Client device.
* 
*  \param packet: pointer to uint8 array os the data that has to be sent over
* notification.
*
*  \param length: Length of the data to be sent.
*
*  \return bool: Status of notification sent. true means notification pushed
* successfully.
* 
******************************************************************************/
extern bool CyMesh_BearerSendProvisionNotification(uint8 * packet, uint8 length);

#endif      /* #ifndef _CYMESH_BEARER_H */

/* [] END OF FILE */
