/***************************************************************************//**
* \file CyMesh_LightLightnessModel.h
* 
* \version 1.0
* 
* \brief
*  SmartMesh v1 Light Lightness Model header.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CYMESH_LIGHT_LIGHTNESS_MODEL_H
#define CYMESH_LIGHT_LIGHTNESS_MODEL_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include "CyMesh_Common.h"



/*******************************************************************************
* Compile time options
*******************************************************************************/
#define CYMESH_MODEL_LIGHT_LIGHTNESS_SERVER           (1)
#define CYMESH_MODEL_LIGHT_LIGHTNESS_CLIENT           (1)

#define CYMESH_MODEL_LIGHT_LIGHTNESS_COMPONENT_INDEX  (1)
#define CYMESH_MODEL_LIGHT_LIGHTNESS_MODEL_INDEX      (1)

/*******************************************************************************
* Externed functions
*******************************************************************************/
extern void CyMesh_LightLightnessModelStart(void);

#if CYMESH_MODEL_LIGHT_LIGHTNESS_CLIENT
void CyMesh_LightLightnessModelSetPublishAddr(uint16 pubAddr, uint8 compIndex, uint8 modelIndex);
CYMESH_API_RETURN_T CyMesh_LightLightnessModelSetLightness(uint16 lightness, uint8 compIndex, uint8 modelIndex);
CYMESH_API_RETURN_T CyMesh_LightLightnessModelSetLightnessUnreliable(uint16 lightness, uint8 compIndex, uint8 modelIndex);
void CyMesh_LightLightnessModelGetLightness( uint8 compIndex, uint8 modelIndex);
#endif  /* #if CYMESH_MODEL_LIGHT_LIGHTNESS_CLIENT */


#endif  /* #ifndef CYMESH_LIGHT_LIGHTNESS_MODEL_H */

/* [] END OF FILE */
