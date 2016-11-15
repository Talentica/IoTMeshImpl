/***************************************************************************//**
* \file CyMesh_MessageQueue.h
* \version 1.0
* 
* \brief
*  Has the prototypes of the different Queues implemented at application layer.
* 
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined (CYMESH_MESSAGE_QUEUE_H)
#define CYMESH_MESSAGE_QUEUE_H
    
/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include "CyMesh_Common.h"
#include "CyMesh_Application.h"
#include "CyMesh_Transport.h"
    


/*******************************************************************************
* Macros
*******************************************************************************/

/************** ACK Queue ****************/    
#define CYMESH_ACK_Q_SIZE                               (10)
#define CYMESH_ACK_Q_FULL                               (0xFF)
#define CYMESH_ACK_Q_MATCH_FAILED                       (0xFF)

    
/************** Multi Message Queue ****************/
    
/* Maximum size per entry = Number of packets x 10, since every packet 
 * has opcode and TID mandatorily.
 */
#define CYMESH_MULTI_MESSAGE_Q_MAX_BUFFER_SIZE          (40)

#define CYMESH_MULTI_MESSAGE_Q_NUMBER_OF_BUFFERS        (3)
#define CYMESH_MULTI_MESSAGE_Q_FULL                     (0xFF)
#define CYMESH_MULTI_MESSAGE_Q_MATCH_FAILED             (0xFF)
#define CYMESH_MULTI_MESSAGE_Q_TIMEOUT                  (60000) /*60 seconds */


/************* Tx Message Queue *****************/
#define CYMESH_TX_MESSAGE_Q_SIZE                        (10)
#define CYMESH_TX_MESSAGE_Q_FULL                        (0xFF)
#define CYMESH_TX_MESSAGE_Q_MATCH_FAILED                (0xFF)
#define CYMESH_TX_MESSAGE_Q_RETRY_COUNT                 (3)
#define CYMESH_TX_MESSAGE_Q_DEFAULT_TIMEOUT             (5) /* 5 seconds */

#define CYMESH_APPLICATION_TIMER_MAX_VALUE              (0xFFFFFFFF)


/*******************************************************************************
* Structure and enums
*******************************************************************************/

/*********** ACK QUEUE *************/
typedef struct
{
    /* This includes opcode + TID + data */
    uint8 data[CYMESH_APP_MAX_DATA_LEN];
    uint8 length;
    
    uint8 netKeyIndex;
    uint8 appKeyIndex;
    bool isAppKeyUsed;
    uint8 modelIndex;
    uint8 componentIndex;
    uint8 identifier;
    bool fut;
	uint16 srcAddress;
    uint16 dstAddress;
	
    /* TRUE - buffer entry is free; FALSE - used up.
     * Do not change this value. This is handled internally.
     */
    bool isAvailable;
} CYMESH_ACK_NODE_T;

typedef struct
{
    CYMESH_ACK_NODE_T ackNode[CYMESH_ACK_Q_SIZE];
    
    /* Number of packets currently in the ACK queue */
    uint8 count;
} CYMESH_ACK_QUEUE_T;


/********** TX MESSAGE QUEUE ******************/
typedef struct
{
    uint32 opcode;
    
    /* Payload = data only; excludes opcode and TID bytes. */
    uint8 payload[10];
    uint8 payloadLength;
    uint8 isReliable;
    uint8 isEndBitSet;
    
    uint8 netKeyIndex;
    uint8 appKeyIndex;
    bool isAppKeyUsed;
    bool isOwnOrPeerDevKey;
    uint16 srcAddress;
    uint16 dstAddress;
    
    bool fut;
    uint8 ttl;
    uint8 retryCount;
    
    /* Following are used internally and will be overwritten */
    uint8 identifier;
    uint32 timeStamp;
    bool isWaiting;
    bool isAvailable;
} CYMESH_TX_MESSAGE_NODE_T;


typedef struct
{
    CYMESH_TX_MESSAGE_NODE_T messageNode[CYMESH_TX_MESSAGE_Q_SIZE];
    
    /* Number of packets currently in the Tx queue */
    uint8 count;
} CYMESH_TX_MESSAGE_QUEUE_T;


/********** MULTI MESSAGE QUEUE ***************/
typedef struct
{
    uint32 timeStamp;
    uint8 identifier;
    
    /* Number of bytes of data in the buffer currently;
     * Also the location of the next insert to happen in the queue.
     */
    uint8 paramLength;
    
    uint8 numberOfPacketsStored;
    uint8 dataBuffer[CYMESH_MULTI_MESSAGE_Q_MAX_BUFFER_SIZE];
    
    /* TRUE - slot is empty; FALSE - filled up */
    bool isAvailable;
} CYMESH_MULTI_MESSAGE_QUEUE_T;



/*******************************************************************************
* Function declarations
*******************************************************************************/

void CyMesh_ResetTransactionID(void);

/************************** Ack queue *****************************/

uint8 CyMesh_IsAckQueueFree(void);
bool IsAckQueueFull(void);
uint8 CyMesh_GetAckQueueMatch(uint8 matchTid);
void CyMesh_AckQueueInsert(uint8 queueIndex, CYMESH_ACK_NODE_T *applicationPacket);
void CyMesh_AckQueueFreeUp(uint8 queueIndex);
void CyMesh_ResetAckQueueCount(void);
CYMESH_API_RETURN_T CyMesh_SchedulePendingAckPackets(void);



/************************** Tx message queue *****************************/

uint8 CyMesh_GetTxMessageFreeDepth(void);
uint8 CyMesh_IsTxMessageQueueFree(void);
uint8 CyMesh_GetTxQueueMatch(uint8 matchTid);
CYMESH_API_RETURN_T CyMesh_TxMessageQueueInsert(uint8 queueIndex, CYMESH_TX_MESSAGE_NODE_T * packet, bool isNewPacket);
void CyMesh_TxMessageQueueFreeUp(uint8 queueIndex);
void CyMesh_ResetTxMessageQueueCount(void);
CYMESH_API_RETURN_T CyMesh_SchedulePendingTxMessagePackets(void);


/************************** Multi message queue ********************************/
uint8 CyMesh_GetMultiMessageQueueSlot(void);
uint8 CyMesh_GetMultiMessageQueueMatch(uint8 matchTid);
uint8 CyMesh_GetMultiMessageQueueNumberOfPackets(uint8 queueIndex);
uint8 CyMesh_GetMultiMessageQueueData(uint8 queueIndex, uint8 * data);
CYMESH_API_RETURN_T CyMesh_MultiMessageQueueInit(uint8 queueIndex, CYMESH_APPLICATION_PKT_T *applicationPacket);
CYMESH_API_RETURN_T CyMesh_MultiMessageQueueInsert(uint8 queueIndex, CYMESH_APPLICATION_PKT_T *applicationPacket);
void CyMesh_MultiMessageQueueFreeUp(uint8 queueIndex);
void CyMesh_HandleMultiMessageTimeout(void);



#endif /* End of #if !defined (CYMESH_MESSAGE_QUEUE_H) */

/* [] END OF FILE */
