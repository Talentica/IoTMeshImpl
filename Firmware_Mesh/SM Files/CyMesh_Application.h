/***************************************************************************//**
* \file CyMesh_Application.h
* 
* \version 1.0
* 
* \brief
*  Contains the function prototypes and constants for SmartMesh stack's 
*  application layer.
* 
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined (CYMESH_APPLICATION_H)
#define CYMESH_APPLICATION_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include "CyMesh_Common.h"


/*******************************************************************************
* Macros
*******************************************************************************/
    
#define CYMESH_APPLICATION_INVALID_TID_INDEX            0x00
#define CYMESH_APPLICATION_OPCODE_TO_TID_OFFSET         0x01
#define CYMESH_APPLICATION_TID_MASK                     0x3F
#define CYMESH_APPLICATION_TID_RELIABLE_MASK            0x80
#define CYMESH_APPLICATION_TID_END_OF_TRANSACTION_MASK  0x40
#define CYMESH_TID_NOT_PRESENT                          0xFF

#define CYMESH_DEVICE_MIN_TRANSACTION_ID                1    
#define CYMESH_DEVICE_MAX_TRANSACTION_ID                63 
    
#define CYMESH_APPLICATION_OPCODE_INDEX                 (0)
#define CYMESH_APPLICATION_SINGLE_BYTE_OPCODE_MAX_VALUE (0x7F)
#define CYMESH_APPLICATION_SINGLE_BYTE_OPCODE_SIZE      (1)
#define CYMESH_APPLICATION_DOUBLE_BYTE_OPCODE_SIZE      (2)
#define CYMESH_APPLICATION_TRIPLE_BYTE_OPCODE_SIZE      (3)

#define CYMESH_APPLICATION_SINGLE_BYTE_OPCODE_MASK      (0x8000)
#define CYMESH_APPLICATION_DOUBLE_BYTE_OPCODE_MASK      (0xC000)
#define CYMESH_APPLICATION_TRIPLE_BYTE_OPCODE_MASK      (0xC00000)
    
#define CYMESH_APPLICATION_DOUBLE_BYTE_OPCODE_VALUE     (0x8000)
#define CYMESH_APPLICATION_TRIPLE_BYTE_OPCODE_VALUE     (0xC00000)
#define CYMESH_INVALID_OPCODE                           (0xFFFFFFFF)

#define CYMESH_VARIABLE_PARAM_LENGTH                    (0xFF)
    
#define CYMESH_INVALID_MESH_ID                          0xFF
#define CYMESH_INVALID_APP_ID                           0xFF
#define CYMESH_INVALID_DESTINATION_ADDRESS              0xFFFF

#define CYMESH_TTL_MIN                                  (1u)
#define CYMESH_TTL_MAX                                  (63u)   

#define CYMESH_APPLICATION_END_BIT_MASK                 (0x40)
#define CYMESH_APPLICATION_RELIABLE_BIT_MASK            (0x80)

    
#define CYMESH_PROFILES_MUTEX_LOCKED                    1
#define CYMESH_PROFILES_MUTEX_FREE                      0
    
    
#define CYMESH_DEVICE_DEFAULT_DESTINATION_ADDRESS       (CyMesh_dst_addr)


extern volatile uint16 CyMesh_dst_addr;
extern uint8 cyMesh_retryCount;
extern uint8 cyMesh_AppReliableMsgTimeout;

/*******************************************************************************
* Component and model structure
*******************************************************************************/

/* To enable or disable a component */
#define CYMESH_COMPONENT_0
#define CYMESH_COMPONENT_1


#ifdef CYMESH_COMPONENT_0
    /* User can change these macros to enable/disable a model in a component */
    #define CYMESH_COMPONENT_0_MODEL_LIGHTNESS                  (1)
    #define CYMESH_COMPONENT_0_MODEL_GENERIC_ON_OFF             (1)
    #define CYMESH_COMPONENT_0_MODEL_GENERIC_LEVEL              (0)
#endif  /* #ifdef CYMESH_COMPONENT_0 */

#ifdef CYMESH_COMPONENT_1
    /* User can change these macros to enable/disable a model in a component */
    #define CYMESH_COMPONENT_1_MODEL_LIGHTNESS                  (0)
    #define CYMESH_COMPONENT_1_MODEL_GENERIC_ON_OFF             (1)
    #define CYMESH_COMPONENT_1_MODEL_GENERIC_LEVEL              (0)
#endif  /* #ifdef CYMESH_COMPONENT_1 */

/*******************************************************************************
* Structures and Enums
*******************************************************************************/
/* Application layer profile mutex structure*/
typedef struct
{
    uint8 profileCallbackLock;
    uint8 profilesSendLock;
} CYMESH_PROFILES_MUTEX_T;

/* Application packet going up to the model */
typedef struct
{
    uint32 opcode;
    uint8 * parameter;
    uint8 parameterLength;
	uint8 netKeyIndex;
    uint8 appKeyIndex;
    uint8 componentIndex;
    uint8 modelIndex;
    bool isAppKeyUsed;
    uint8 identifier;
    bool isReliable;
    uint16 srcAddress;
} CYMESH_APPLICATION_PKT_T;

/* Application packet going up to the model */
typedef struct
{
    uint32 opcode;
    uint16 dstAddress;
    /*Added by VAVC*/
    /*==========================================*/
    uint8 *datapointer;
    uint8 payloadLength;
    /*==========================================*/
} CYMESH_APP_RELIABLE_MSG_TIMEOUT_PARAM_T;


/*******************************************************************************
* Variable Declarations
*******************************************************************************/
extern CYMESH_PROFILES_MUTEX_T profilesMutex;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
/*******************************************************************************
* Function Name: CyMesh_Start
********************************************************************************
* Summary:
*        Initializes the CyMesh protocol stack and configures all the underlying
* protocol layers.
*
* Parameters:
*       genericCallback - Mesh callback routine to the user application from the
* the mesh stack.
*
* Return:
*  void
*
*******************************************************************************/
void CyMesh_Start(CYBLE_CALLBACK_T genericCallback, CYBLE_MODEL_CALLBACK_T meshCallback);
/*******************************************************************************
* Function Name: CyMesh_Stop
********************************************************************************
* Summary:
*        Stops the Mesh stack as well as the BLE.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void CyMesh_Stop(void);

/******************************************************************************
* Function Name: CyMesh_ProfileTxMutexLock
***************************************************************************//**
* 
*  Lock if the profile transmit Mutex is free
* 
*  \param : None.
* 
*  \return
*  bool mutexLockState: mutex is locked or not.
* 
******************************************************************************/
bool CyMesh_ProfileTxMutexLock(void);

/******************************************************************************
* Function Name: CyMesh_ProfileTxMutexFree
***************************************************************************//**
* 
*  Free the transmit Mutex
* 
*  \param : None.
* 
*  \return: None.
* 
******************************************************************************/
void CyMesh_ProfileTxMutexFree(void);

#endif /* End of #if !defined (CYMESH_APPLICATION_H) */

/* [] END OF FILE */
