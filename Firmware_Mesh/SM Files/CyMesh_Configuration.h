/***************************************************************************//**
* \file CyMesh_Configuration.h
* \version 1.0
* 
* \brief
*  This is the header file for the Configuration block in the BLE SmartMesh v1.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined (CYMESH_CONFIGURATION_H)
#define CYMESH_CONFIGURATION_H

#include <project.h>
#include <stdbool.h>
#include "CyMesh_Common.h"
#include "CyMesh_Bearer.h"
#include "CyMesh_uECC.h"

/*******************************Pre processor Directives************************/
#define CYMESH_CONFIG_ENABLE_SECURE_NETWORK_BEACON			(0)
#define CYMESH_CONFIG_ENABLE_ECDH_BLOCK						(1)
	
/*****************************Macros*******************************************/	
#define CYMESH_MAX_NUMBER_OF_COMPONENTS						(4)
	
#define CYMESH_CONFIG_PROV_ALGO_LEN							(4u)
#define CYMESH_CONFIG_PROV_UUID_LIST_LEN					(2)
#define CYMESH_CONFIG_DEVICE_UUID_LEN						(0x10)
#define CYMESH_CONFIG_PROVISIONING_LINK_ID_LEN				(0x04)
#define CYMESH_CONFIG_PB_PDU_MAX_SIZE						(128u)
#define CYMESH_CONFIG_ECDH_PRIVATE_KEY_SIZE					(32u)		//for P256 mechanism
#define CYMESH_CONFIG_ECDH_PUBLIC_KEY_SIZE					(64u)
#define CYMESH_CONFIG_ECDH_SHARED_KEY_SIZE					(32u)
#define CYMESH_CONFIG_PROV_RANDOM_NUM_SIZE					(16u)
#define CYMESH_CONFIG_PROV_CONFIRM_VAL_SIZE					(16u)
#define CYMESH_CONFIG_PROV_CONFIRM_KEY_SIZE					(16u)
#define CYMESH_SECURITY_DEFAULT_KEY_LEN						(16)
	
#define CYMESH_CONFIG_PB_ADV_TX_COUNT						(1u)
#define CYMESH_CONFIG_PROV_DATA_MIC_LEN						(8u)
	
#define CYMESH_CONFIG_SECURE_BEACON_AUTH_VAL_LEN			(8u)
	
/* Sequence number update to Flash diff value*/
#define CYMESH_TRAN_SEQNO_FLASH_INCREMENT_VALUE				(200u)
	
#define CyMesh_IsConfigurationValid()						(cyMesh_ConfigInfoRam.isConfigurationValid)

typedef struct
{
	uint8 algo[CYMESH_CONFIG_PROV_ALGO_LEN];
	uint8 pub_key_type;
	uint8 pub_key_verb;
	uint8 output_OOB_size;
	uint8 output_OOB_verb;
	uint8 input_OOB_size;
	uint8 input_OOB_verb;
	uint8 static_OOB_type;
	uint8 static_OOB_verb;
}CYMESH_CONFIG_PROV_CAPABLITIES_T;

typedef struct
{
	uint8 algo;
	uint8 pub_key_used;
	uint8 output_OOB_used;
	uint8 input_OOB_used;
	uint8 static_OOB_used;
} CYMESH_CONFIG_PROV_START_FIELD_VAL_T;

typedef struct
{
	/* Data content of the Unprovisioned Beacons*/
	uint8 *data;
	
	/* BD Address of the device sending unprovisioned beacon */
	CYBLE_GAP_BD_ADDR_T device_addr;
}CYMESH_CONFIG_UNPROV_BEACON_CONTENT_T;

typedef struct
{
	/* Content of the Provisioning data */
	uint8 appKey[CYMESH_SECURITY_DEFAULT_KEY_LEN];
	uint8 netKey[CYMESH_SECURITY_DEFAULT_KEY_LEN];
	uint8 deviceKey[CYMESH_SECURITY_DEFAULT_KEY_LEN];
	uint32 ivIndex;
	uint16 srcAddr;

}CYMESH_CONFIG_PROV_DATA_T;

/******************************************************************************
* Function Name: CyMesh_ConfigurationSave
***************************************************************************//**
* 
*  Save the current information in cyMesh_configInfoRam to the Flash.
* 
*  \param None.
*
*  \return bool: true if flash save was successful
*				 false if the data could not be saved in the flash
* 
******************************************************************************/
extern bool CyMesh_ConfigurationSave(void);

/******************************************************************************
* Function Name: CyMesh_ConfigurationLoad
***************************************************************************//**
* 
*  This function loads the saved configuration data from the Flash to 
* cyMesh_configInfoRam. It is called internall at start of the Mesh to load
* the saved config to SRAM.
* 
*  \param void:
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if load from Flash was successful
*								CYMESH_ERROR_INVALID_CONFIGURATION if checksum in 
*								Flash is invalid.
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_ConfigurationLoad(void);

/******************************************************************************
* Function Name: CyMesh_ConfigurationProcess
***************************************************************************//**
* 
*  This function sets the security keys and identifiers with respect to the data
* stored in cyMesh_ConfigInfoRam.
* 
*  \param void.
*
*  \return void.
* 
******************************************************************************/
extern void CyMesh_ConfigurationProcess(void);

/******************************************************************************
* Function Name: CyMesh_ProvisioningProcess
***************************************************************************//**
* 
*  This function runs the complete provisioning state machine, including provisioning
* message SAR, decoding PDUs and setting the next state of provisioning protocol.
* 
*  \param void.
*
*  \return void.
* 
******************************************************************************/
extern void CyMesh_ProvisioningProcess(void);

/******************************************************************************
* Function Name: CyMesh_ConfigurationIncomingBeacon
***************************************************************************//**
* 
*  This function calls relevant routines to process Mesh beacons such as 
* unprovisioned, security, key refresh, etc.
* 
*  \param uint32: Type of Beacon. Type used is CYMESH_EVENT_T
*			CYMESH_BEARER_RX_BUFFER_T *: Pointer to the defined struct where
						the beacons data is stored
*
*  \return void:
* 
******************************************************************************/
extern void CyMesh_ConfigurationIncomingBeacon(uint32 , CYMESH_BEARER_RX_BUFFER_T * );

/******************************************************************************
* Function Name: CyMesh_ConfigGenerateRandNumber
***************************************************************************//**
* 
*  This function enables/disables the GATT advertising so that the node can be  
* connected and data sent over the Mesh network.
* 
*  \param uint8_t*: pointer to array where the generated random number will be stored
*		unsigned: length of the random number to be generated
*
*  \return int: true if the random number was generated
*				false if the random number could not be generated
* 
******************************************************************************/
int CyMesh_ConfigGenerateRandNumber(uint8_t *, unsigned );

#if (CYMESH_ENABLE_PROVISIONING == 1)
/******************************************************************************
* Function Name: ProvConfirmCMACcallback
***************************************************************************//**
* 
*  This is a callback function called from the Security module when the 
*	provisioning confirmation value has been calculated.
* 
*  \param uint8*: calculated value
*
*  \return none
* 
******************************************************************************/
void ProvConfirmCMACcallback(uint8 * );

/******************************************************************************
* Function Name: CyMesh_BearerSetGattAdvertising
***************************************************************************//**
* 
*  This is a callback function called from the Security module when the 
*	confirmation value from the other device has been calculated.
* 
*  \param uint8*: calculated value
*
*  \return none
* 
******************************************************************************/
void ProvConfirmRandCMACcallback(uint8 * );

/******************************************************************************
* Function Name: CyMesh_ProvisionerProvisionNewDevice
*******************************************************************************
* 
*  This user level function allows application to provision an unprovisioned device.
* This function should be called from the 'CYMESH_EVT_MESH_BEACON_UNPROVISIONED_NODE'
* event in the Mesh event handler.
* 
*  \param state: CYMESH_CONFIG_UNPROV_BEACON_CONTENT_T* = Unprovisioned node
*					beacon data that will be used to start the provisioning.
*				CYMESH_CONFIG_PROV_DATA_T* = provisioning data that the application
*					wants to use for provisioning the device.
*				uint8: Mode of provisioning. Its value is always 0
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if provisioning started
*								CYMESH_ERROR_INVALID_PARAM if parameter is incorrect
* 
******************************************************************************/
CYMESH_API_RETURN_T CyMesh_ProvisionerProvisionNewDevice(CYMESH_CONFIG_UNPROV_BEACON_CONTENT_T* , CYMESH_CONFIG_PROV_DATA_T*, uint8 );

/******************************************************************************
* Function Name: CyMesh_UnprovisionedReprovisionDevice
***************************************************************************//**
* 
*  This function is used by an already provisioned device to trigger a reprovision 
* by setting its data validity in cyMesh_ConfigInfoRam to 0. The device will
* restart mesh unprovisioned beacon.
* 
*  \param none:
*
*  \return none:
* 
******************************************************************************/
void CyMesh_UnprovisionedReprovisionDevice(void);

#endif	/* #if (CYMESH_ENABLE_PROVISIONING == 1) */
#endif
/* [] END OF FILE */
