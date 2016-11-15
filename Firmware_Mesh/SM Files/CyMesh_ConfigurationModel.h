/***************************************************************************//**
* \file CyMesh_ConfigurationModel.h
* 
* \version 1.0
* 
* \brief
*  SmartMesh v1 Configuration model header
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined (CYMESH_CONFIG_MODEL_H)
#define CYMESH_CONFIG_MODEL_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include "CyMesh_Common.h"

/*******************************************************************************
* Compile time options
*******************************************************************************/
#define CYMESH_MODEL_CONFIGURATION_SERVER           (1)
#define CYMESH_MODEL_CONFIGURATION_CLIENT           (1)


#define CYMESH_MODEL_CONFIGURATION_COMPONENT_INDEX  (0)
#define CYMESH_MODEL_CONFIGURATION_MODEL_INDEX      (0)

/*******************************************************************************
* Enums
*******************************************************************************/
typedef enum
{
    CYMESH_STATE_NOT_RELAYING,
    CYMESH_STATE_RELAYING,
} CYMESH_RELAY_STATE_T;



/* Assumption made that each model ID is 3 bytes long. Not always true */
#define CYMESH_MAX_CONFIGURATION_DATA_BLOCK_SIZE        ((CYMESH_NUMBER_OF_COMPONENTS * ((CYMESH_MAX_MODELS_PER_COMPONENT * 4) + 4)) + 8)


/*******************************************************************************
* Externed functions
*******************************************************************************/
extern void CyMesh_ConfigurationModelStart(void);

#if CYMESH_MODEL_CONFIGURATION_CLIENT
extern void CyMesh_ConfigurationModelSetClient(uint8 defaultTtl);
extern void CyMesh_ConfigurationModelSetTargetDevice(uint16 deviceAddress, const uint8 * deviceKey);

extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelSetDefaultTtl(uint8 ttl);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelGetDefaultTtl(void);

extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelGetBlock(uint16 offset);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelResetDevice(void);

extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelSetRelay(bool isRelaying);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelGetRelay(void);


extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelSetPublication(uint8 componentIndex, 
                                                                   uint8 modelIndex, 
                                                                   uint16 publishAddress,
                                                                   uint8 appKeyIndex,
                                                                   uint8 defaultTtl);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelGetPublication(uint8 componentIndex, uint8 modelIndex);


extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelAddSubscription(uint8 componentIndex, 
                                                                    uint8 modelIndex, 
                                                                    uint16 subscribeAddress);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelDeleteSubscription(uint8 componentIndex, 
                                                                       uint8 modelIndex, 
                                                                       uint16 subscribeAddress);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelGetSubscription(uint8 componentIndex, 
                                                                    uint8 modelIndex, 
                                                                    uint8 startingIndex);

extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelSetComponentAddress(uint8 componentIndex, uint16 address);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelGetComponentAddress(uint8 componentIndex);

extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelAddAppKey(uint8 netKeyIndex, const uint8 * appKey);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelUpdateAppKey(uint8 appKeyIndex, const uint8 * appKey);
extern CYMESH_API_RETURN_T CyMesh_ConfigurationModelDeleteAppKey(uint8 appKeyIndex);

extern void CyMesh_ConfigurationCalculateCDB(void);

#endif  /* #if CYMESH_MODEL_CONFIGURATION_CLIENT */


#endif /* End of #if !defined (CYMESH_CONFIG_MODEL_H) */


/* [] END OF FILE */
