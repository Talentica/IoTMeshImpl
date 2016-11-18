/***************************************************************************//**
* \file Mesh_Config.c
* \version 1.0
* 
* \brief
*  Contains the functions for configuring the Smart Mesh stack.
* 
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "main.h"

volatile bool flag = false;

uint8 networkKey[16] = 
{0x89, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x96};

uint8 applicationKey[16] = 
{0x77, 0x01, 0x05, 0x02, 0x06, 0x03, 0x07, 0x04, 0x08, 0x05, 0x09, 0x06, 0x0A, 0x07, 0x0B, 0x11};

uint8 deviceKey[16] = 
{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x12};

uint32 ivIndex = 0x00000005;
/* Security beacon CMAC calculations */
uint8 beaconCmac[4];
extern uint8 cyMesh_ConfigurationBeaconCalculatedAuthValue[4];

/******************************************************************************
* Function Name: MyCallback
*******************************************************************************
* 
*  Event handler for storing the CMAC from external security beaocn received
* 
*  \param cmac: pointer to the location of CMAC in received packet
*
*  \return None
*  
******************************************************************************/
static void MyCallback(uint8 * cmac)
{
    memcpy(beaconCmac, &cmac[12], 4);
    
    flag = true;
}

/******************************************************************************
* Function Name: DefineNodeinfo
*******************************************************************************
* 
*  Sets the Device level Information such as address, UUID and role
* 
*  \param None
*
*  \return None
*  
******************************************************************************/
void DefineNodeinfo(void)
{
	uint8 i,j;
	
	for(i = 0; i < CYMESH_NUMBER_OF_COMPONENTS; i++)
    {
        cyMesh_ConfigInfoRam.deviceInfo.components[i].componentAddress = (CYMESHTEST_NET_DEVICE_SRC_ADDR + i) & 0x7FFF;
        
        for(j = 0; j < CYMESH_MAX_MODELS_PER_COMPONENT; j++)
        {
            cyMesh_ConfigInfoRam.deviceInfo.components[i].model[j].publishAddress = 0xFFFF;
            cyMesh_ConfigInfoRam.deviceInfo.components[i].model[j].subscriptionAddress[0] = 0;
            cyMesh_ConfigInfoRam.deviceInfo.components[i].model[j].numberOfSubscribedAddresses = 0;
            cyMesh_ConfigInfoRam.deviceInfo.components[i].model[j].modelDefaultTtl = 20;
        }
    }
	
	/* Assign Device UUID */
	for(i=0;i<CYMESH_CONFIG_DEVICE_UUID_LEN;i++)
	{
		cyMesh_ConfigInfoRam.device_UUID[i] = 0x0f - i;
	}	
	
	/* Modify the last two bytes of the device UUID so that each (or atleast most) device has a unique value */
	cyMesh_ConfigInfoRam.device_UUID[14] = CYBLE_SFLASH_DIE_X_REG;
	cyMesh_ConfigInfoRam.device_UUID[15] = CYBLE_SFLASH_DIE_Y_REG;
}

/******************************************************************************
* Function Name: DefineAppInfo
*******************************************************************************
* 
*  Sets the Application level Information for this device, including setting 
* of the application keys
* 
*  \param None
*
*  \return None
*  
******************************************************************************/
void DefineAppInfo(void)
{
	if(cyMesh_ConfigInfoRam.isConfigurationValid != true)
	{        
	    flag = false;
	    CyMesh_SecuritySetApplicationKey(0, 0, applicationKey, MyCallback);
	    
	    while(flag != true)
	    {
	        CyBle_ProcessEvents();
	        CyMesh_SecurityAesCmacProcess();
	    }
	}
}

/******************************************************************************
* Function Name: DefineNetInfo
*******************************************************************************
* 
*  Sets the network level Information such as network keys, IV index and Beacon
* CMACs
* 
*  \param None
*
*  \return None
*  
******************************************************************************/
void DefineNetInfo(void)
{
	if(cyMesh_ConfigInfoRam.isConfigurationValid != true)
	{
	    cyMesh_ConfigInfoRam.deviceInfo.deviceDefaultTtl = 10;
	    memcpy(cyMesh_ConfigInfoRam.deviceInfo.deviceKey, deviceKey, 16);

		cyMesh_ConfigInfoRam.bearerRole = CYMESH_ROLE_RELAY;
		
		flag = false;
	    
	    CyMesh_SecuritySetNetworkKey(0, networkKey, MyCallback);
	    
	    while(flag != true)
	    {
	        CyBle_ProcessEvents();
	        CyMesh_SecurityAesCmacProcess();
	    }

	    flag = false;
	    CyMesh_SecuritySetIVindex(0, ivIndex);
	    CyMesh_SecurityCalculateBeaconAuthValue(0, 0, ivIndex, MyCallback);
	    
	    while(flag != true)
	    {
	        CyBle_ProcessEvents();
	        CyMesh_SecurityAesCmacProcess();
	    }
	    memcpy(cyMesh_ConfigurationBeaconCalculatedAuthValue, beaconCmac, 4);
		
		/* Suppots one valid Mesh configuration */
	    cyMesh_ConfigInfoRam.isConfigurationValid = true;
	}
}
/* [] END OF FILE */
