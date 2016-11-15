/***************************************************************************//**
* \file CyMesh_Transport.h
* 
* \version 1.0
* 
* \brief
*  Contains the function prototypes and constants for SmartMesh stack's 
*  transport layer.
* 
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined (CYMESH_TRANSPORT_H)
#define CYMESH_TRANSPORT_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include "project.h"
#include <CyMesh_Common.h>    

    
/*******************************************************************************
* Macros
*******************************************************************************/
#define CYMESH_TRANSPORT_MUTEX_LOCKED                   1
#define CYMESH_TRANSPORT_MUTEX_FREE                     0    

#define CYMESH_APP_MIN_DATA_LEN							(0x02)	/* opcode + parameter */
#define CYMESH_APP_MAX_DATA_LEN							(0x0C)  /* max payload (excluding the MICapp) */ 


/*******************************************************************************
* Structures and Enums
*******************************************************************************/
typedef struct
{
    uint8 * data; /* only consists of application payload */
    uint8 length;
	uint8 netKeyIndex;
    uint8 appKeyIndex;
    bool isAppKeyUsed;
    uint8 modelIndex;
    uint8 componentIndex;
    uint16 srcAddress;
} CYMESH_TRANSPORT_PKT_T;


typedef enum
{
	CYMESH_APPLICATION_PKT = 0x01
} CYMESH_TRANSPORT_CALLBACK_T;


/*******************************************************************************
* Function Declarations
*******************************************************************************/

/******************************************************************************
* Function Name: CyMesh_TransportStart
*******************************************************************************
* 
*  This function starts the transport layer and initilizes required variables.
* 
*  \param CYMESH_CALLBACK_T: function which transport layer will call to send
*							the packet to the upper layer.
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if transport layer started correctly
*								other values in case of failures
* 
******************************************************************************/
CYMESH_API_RETURN_T CyMesh_TransportStart(CYMESH_CALLBACK_T appCallback);

/******************************************************************************
* Function Name: CyMesh_TransportSendData
*******************************************************************************
* 
*  This function pushes the data from top layer to the transport layer to be  
* sent as a mesh packet. This function performs network layer encryption, obfuscation
* and setting correct headers.
* 
*  \param uint8*: pointer to the application layer data
*
*  \param uint8: length of the application layer packet
*
*  \param uint8: txCount indicating the number of times bearer layer will send 
*				this same packet
*
*  \param bool: one bit value indicating whether Application key was used or not 
*
*  \param bool: one bit value indicating whether own key will be used or peer device 
*				key for transport layer encryption.
*
*  \param bool: future bit. Set from the application layer
*
*  \param uint8: A 6-bit value indicating the Time-To-Live in the packet
*
*  \param uint16: Source address of the node sending the packet
*
*  \param uint16: Destination address of the node to which the packet will be sent
*
*  \param uint8: index of the network key that will be used for network layer 
*					encryption
*
*  \param uint8: index of the application key that will be used for transport layer 
*					encryption
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if transport layer sent the data 
*								correctly, other values in case of failures
* 
******************************************************************************/
CYMESH_API_RETURN_T CyMesh_TransportSendData(uint8 * data, 
													uint8 length, 
													uint8 txCount, 
													bool akf,
                                                    bool isOwnOrPeerDevKey,
													bool fut,
													uint8 ttl,
													uint16 src,
													uint16 dst,
													uint8 netKeyIndex, 
													uint8 appKeyIndex);

/******************************************************************************
* Function Name: CyMesh_ReadSequenceNumber
***************************************************************************//**
* 
*  Reads the mesh sequence number used by transport layer for last transaction.
* 
*  \param void: 
*
*  \return uint32: the value of sequence number that was used for last transaction.
*  
******************************************************************************/
uint32 CyMesh_ReadSequenceNumber(void);

#endif /* End of #if !defined (CYMESH_TRANSPORT_H) */

/* [] END OF FILE */
