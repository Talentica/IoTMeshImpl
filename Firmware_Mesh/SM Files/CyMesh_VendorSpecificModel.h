/***************************************************************************//**
* \file CyMesh_VendorSpecificModel.h
* 
* \version 1.0
* 
* \brief
*  SmartMesh v1 vendor Specific Model.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CYMESH_VENDOR_SPECIFIC_MODEL_H
#define CYMESH_VENDOR_SPECIFIC_MODEL_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include "CyMesh_Common.h"

/*******************************************************************************
* Compile time options
*******************************************************************************/
#define CYMESH_MODEL_VENDOR_SPECIFIC_SERVER           		(1)
#define CYMESH_MODEL_VENDOR_SPECIFIC_CLIENT           		(1)

#define CYMESH_MODEL_VENDOR_SPECIFIC_COMPONENT_INDEX  		(2)
#define CYMESH_MODEL_VENDOR_SPECIFIC_MODEL_INDEX    		(0)
/*******************************************************************************
* Externed functions
*******************************************************************************/
extern void CyMesh_VendorSpecificModelStart(void);
extern void CyMesh_VendorSpecificSetPublishAddr(uint16 pubAddr, uint8 compIndex, uint8 modelIndex);
extern void CyMesh_VendorSpecificSendDataUnreliable(CYMESH_VARIABLE_DATA_T data, uint8 compIndex, uint8 modelIndex);
extern CYMESH_API_RETURN_T CyMesh_VendorSpecificSendDataReliable(CYMESH_VARIABLE_DATA_T data, uint8 compIndex, uint8 modelIndex);
extern void CyMesh_VenderSpecificSetReliableAckParam(uint8 ackParam);
extern void CyMesh_VenderSpecificSetPublishAddr(uint8 componentIndex, uint8 modelIndex, uint16 publishAddr);

#endif  /* #ifndef CYMESH_LIGHT_LIGHTNESS_MODEL_H */

/* [] END OF FILE */
