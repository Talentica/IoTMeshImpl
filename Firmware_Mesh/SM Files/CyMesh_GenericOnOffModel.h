/***************************************************************************//**
* \file CyMesh_GenericOnOffModel.h
* 
* \version 1.0
* 
* \brief
*  SmartMesh v1 Generic On Off Model header.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CYMESH_GENERIC_ONOFF_MODEL_H
#define CYMESH_GENERIC_ONOFF_MODEL_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include "CyMesh_Common.h"



/*******************************************************************************
* Compile time options
*******************************************************************************/
#define CYMESH_MODEL_GENERIC_ON_OFF_SERVER           (1)
#define CYMESH_MODEL_GENERIC_ON_OFF_CLIENT           (1)

//#define CYMESH_MODEL_GENERIC_ON_OFF_COMPONENT_INDEX  (1)
//#define CYMESH_MODEL_GENERIC_ON_OFF_MODEL_INDEX      (0)


/*******************************************************************************
* Externed functions
*******************************************************************************/
extern void CyMesh_GenericOnOffModelStart(void);


#if CYMESH_MODEL_GENERIC_ON_OFF_CLIENT
extern void CyMesh_GenericOnOffModelSetPublishAddr(uint8 pubAddr, uint8 compIndex, uint8 modelIndex);    
extern void CyMesh_GenericOnOffModelSetOnOff(bool state, uint8 transitionTime, uint8 compIndex, uint8 modelIndex);
extern void CyMesh_GenericOnOffModelSetOnOffUnreliable(bool state, uint8 transitionTime, uint8 compIndex, uint8 modelIndex);
extern void CyMesh_GenericOnOffModelGetStatus(uint8 compIndex, uint8 modelIndex);
#endif  /* #if CYMESH_MODEL_GENERIC_ON_OFF_CLIENT */



#endif  /* #ifndef CYMESH_GENERIC_ONOFF_MODEL_H */

/* [] END OF FILE */
