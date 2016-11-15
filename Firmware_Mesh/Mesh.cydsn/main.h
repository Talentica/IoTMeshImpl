/***************************************************************************//**
* \file main.h
* \version 1.0

********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(MAIN_H)
#define MAIN_H
	
#include <project.h>
#include "CyMesh_LightLightnessModel.h"
#include "CyMesh_Transport.h"
#include "CyMesh_Network.h"
#include "CyMesh_Security.h"
#include "CyMesh_SecurityPVT.h"
#include "CyMesh_Common.h"
#include "CyMesh_Bearer.h"
#include "CyMesh_Timer.h"
#include "CyMesh_Configuration.h"	
#include "CyMesh_Application.h"	
#include "CyMesh_VendorSpecificModel.h"
#include "CyMesh_LightLightnessModel.h"
	
    
/******************************Pre-processor Directives**********************************************/
#define CYMESHTEST_START_DIRECTLY_WITH_RELAY	(0x01)	/* This allows a device to be automatically configured without the need of Provisioning
														*	and start directly as relay */
/**************************************Macors**************************************************/
#define CYMESHTEST_NET_DEVICE_SRC_ADDR			((((uint16)CYBLE_SFLASH_DIE_X_REG & 0x00FF) << 8) | ((uint16)CYBLE_SFLASH_DIE_Y_REG & 0x00FF))//(0xAABB)
#define CYMESH_NET_BROADCAST_ADDR				(0xFFFF)

/*****************************Function Declarations**************************************/
void DefineNodeinfo(void);
void DefineAppInfo(void);
void DefineNetInfo(void);

#endif
/* [] END OF FILE */
