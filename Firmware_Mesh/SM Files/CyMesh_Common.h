/***************************************************************************//**
* \file CyMesh_Common.h
* \version 1.0
* 
* \brief
*  This is the common header file for the BLE SmartMesh v1 solution.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYMESH_COMMON_H
#define _CYMESH_COMMON_H

/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include <stdbool.h>
    
/*******************************************************************************
* Component-Model mappings for the User.
* NOTE: 
* 1) Set *_COMP_? to CYMESH_UNUSED if no model is to be mapped for given component index
* 2) DO NOT USE component index 0, model index 0 for any model.
* 3) Two models should not be mapped on to the same component and model index
* 4) One component index should be mapped to only one model of the particular type. There can be 
*    multiple models of different types on a single component upto CYMESH_MAX_MODELS_PER_COMPONENT
*******************************************************************************/
    
//For Light Lightness MODEL
#define CYMESH_MDL_LL_COMP_0  		                (CYMESH_UNUSED)     //Must be unused
#define CYMESH_MDL_LL_COMP_0_MDLIDX                 (CYMESH_UNUSED)     //Must be unused
                   
#define CYMESH_MDL_LL_COMP_1  		                (1)
#define CYMESH_MDL_LL_COMP_1_MDLIDX                 (0)
                   
#define CYMESH_MDL_LL_COMP_2  		                (CYMESH_UNUSED)
#define CYMESH_MDL_LL_COMP_2_MDLIDX                 (CYMESH_UNUSED)
                   
#define CYMESH_MDL_LL_COMP_3            		    (CYMESH_UNUSED)
#define CYMESH_MDL_LL_COMP_3_MDLIDX                 (CYMESH_UNUSED)
    
//For Generic On Off MODEL
#define CYMESH_MDL_GenOnOff_COMP_0                  (CYMESH_UNUSED)     //Must be unused
#define CYMESH_MDL_GenOnOff_COMP_0_MDLIDX           (CYMESH_UNUSED)     //Must be unused
                                                    
#define CYMESH_MDL_GenOnOff_COMP_1  		        (CYMESH_UNUSED)
#define CYMESH_MDL_GenOnOff_COMP_1_MDLIDX           (CYMESH_UNUSED)
                                                    
#define CYMESH_MDL_GenOnOff_COMP_2  		        (2)
#define CYMESH_MDL_GenOnOff_COMP_2_MDLIDX           (0)
                                                    
#define CYMESH_MDL_GenOnOff_COMP_3  		        (CYMESH_UNUSED)
#define CYMESH_MDL_GenOnOff_COMP_3_MDLIDX           (CYMESH_UNUSED)

//For VENDOR SPECIFIC MODEL
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_0  		    (CYMESH_UNUSED)     //Must be unused
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_0_MDLIDX    (CYMESH_UNUSED)     //Must be unused
    
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_1  		    (CYMESH_UNUSED)
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_1_MDLIDX    (CYMESH_UNUSED)
    
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_2  		    (CYMESH_UNUSED)
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_2_MDLIDX    (CYMESH_UNUSED)
    
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_3  		    (3)
#define CYMESH_MDL_VENDOR_SPECIFIC_COMP_3_MDLIDX    (0)    
    

/*******************************************************************************
* Compile Time Options
*******************************************************************************/
#define CYMESH_MAX_NETWORK_KEYS                 (1)
#define CYMESH_MAX_APPLICATION_KEYS             (2)
#define CYMESH_MAX_FRIENDS_PER_NETWORK          (4)
#define CYMESH_MAX_SUBSCRIPTION_ADDRESSES       (11)

#define CYMESH_NUMBER_OF_COMPONENTS             (4)
#define CYMESH_MAX_MODELS_PER_COMPONENT         (2)
    
#define CYMESH_MAX_MESH_PACKET_SIZE             (31u)
#define CYMESH_DEVICE_UUID_LENGTH               (16)

#define CYMESH_MODEL_VAR_DATA_SIZE				(8)
/**************Pre-processor Directives*******************/
#define CYMESH_ENABLE_PROVISIONING				(1)
#define CYMESH_IS_PROVISIONER_ENABLED_OR_UNPROV	(0)		//1 = Provisioner running, 0 = Unprovisioned running
	
#define CYMESH_ENABLE_FRIENDSHIP				(0)
#define CYMESH_IS_LPN_ENABLED_OR_FR				(0)		//1 = LPN enabled, 0 = FR Enabled

#define CYMESH_BEARER_GATT_BEARER_ENABLED		(1)
#define CYMESH_CONFIG_PB_GATT_SUPPORTED			(1)		/* to enable provisioning over GATT  */
	
#define CYMESH_ENABLE_CONFIGINFO_PERIODIC_UPDATE (1)
/*******************************************************************************
* Macros
*******************************************************************************/
#define CYMESH_INVALID_MODEL_INDEX              (0xFF)
#define CYMESH_INVALID_COMPONENT_INDEX          (0xFF)
    
#define CYMESH_NUMBER_OF_TRIPLETS_PER_MESSAGE   (0x01)
    
/*******************************************************************************
* Enumerations
*******************************************************************************/

#define CYMESH_UNUSED   CYMESH_INVALID_COMPONENT_INDEX

extern uint8  cyMesh_Model_VenSp_Comp0;
extern uint8  cyMesh_Model_VenSp_Comp0_MdlIdx;
extern uint8  cyMesh_Model_VenSp_Comp1;
extern uint8  cyMesh_Model_VenSp_Comp1_MdlIdx;
extern uint8  cyMesh_Model_VenSp_Comp2;
extern uint8  cyMesh_Model_VenSp_Comp2_MdlIdx;
extern uint8  cyMesh_Model_VenSp_Comp3;
extern uint8  cyMesh_Model_VenSp_Comp3_MdlIdx;    
    
extern uint8  cyMesh_Model_LL_Comp0;
extern uint8  cyMesh_Model_LL_Comp0_MdlIdx;
extern uint8  cyMesh_Model_LL_Comp1;
extern uint8  cyMesh_Model_LL_Comp1_MdlIdx;
extern uint8  cyMesh_Model_LL_Comp2;
extern uint8  cyMesh_Model_LL_Comp2_MdlIdx;
extern uint8  cyMesh_Model_LL_Comp3;
extern uint8  cyMesh_Model_LL_Comp3_MdlIdx;
    
extern uint8  cyMesh_Model_GenOnOff_Comp0;
extern uint8  cyMesh_Model_GenOnOff_Comp0_MdlIdx;
extern uint8  cyMesh_Model_GenOnOff_Comp1;
extern uint8  cyMesh_Model_GenOnOff_Comp1_MdlIdx;
extern uint8  cyMesh_Model_GenOnOff_Comp2;
extern uint8  cyMesh_Model_GenOnOff_Comp2_MdlIdx;
extern uint8  cyMesh_Model_GenOnOff_Comp3;
extern uint8  cyMesh_Model_GenOnOff_Comp3_MdlIdx;
    
    
/*******************************************************************************
* Enumerations
*******************************************************************************/

/* Common events generated by the mesh protocol */
typedef enum
{
	CYMESH_EVT_STACK_ON = 0x00,
    /* Mesh ADV packet received by Bearer Layer. Event parameter is a pointer
     * to CYMESH_BEARER_RX_BUFFER_T.
     */
	
    CYMESH_EVT_MESH_ADV,
    
   	/* Mesh Beacon from Unprovisioned node*/
    CYMESH_EVT_MESH_BEACON_UNPROVISIONED_NODE,
    
	 /* Mesh Beacon ADV packet for Secure Network Broadcast received by Bearer  
     * Layer. Event parameter is a uint8 pointer to the complete packet 
     * (starting at KR/NID).
     */
    CYMESH_EVT_MESH_BEACON_SECURE_NETWORK,
	
	/* Mesh Beacon for Smart Mesh provisioning Service*/
    CYMESH_EVT_MESH_SM_PROV_SERVICE_NODE,
	
	/* Mesh Beacon for SM Proxy service */
    CYMESH_EVT_MESH_SM_PROXY_SERVICE_NODE,
    
    /* Network layer packet event to the transport layer.
     * Event parameter contains the actual error code 
     */
    CYMESH_EVT_NETWORK_PACKET,
    
    /* Transport layer error event.
     * Event parameter contains the actual error code 
     */
    CYMESH_EVT_TRANSPORT_ERROR,
    
    /* Configuration in RAM changed; user should call CyMesh_ConfigurationSave()
     * API to write the information to flash. Event Parameter is NULL.
     */
    CYMESH_EVT_PENDING_FLASH_WRITE,
    
	/* This event is triggered when provisioning data is received and has been authenticated 
	* over the established link. The parameters are used to update the cyMesh_ConfigInfoRam
	* structure. This event will indicate to the user to start the key set procedure and update
	* the flash as if required */
	CYMESH_EVT_PROVISIONING_DATA,
	
	 /* roit: Events for Provisioning authentication */
    CYMESH_EVT_CONFIG_PROV_OUTPUT_OOB,
	
	CYMESH_EVT_CONFIG_PROV_INPUT_OOB,
	
	CYMESH_EVT_CONFIG_PROV_STATIC_OOB,
    
    /* The reliable message sent by this device did not receive a response.
     * Event parameter is the message for which this happened - a pointer to
     * CYMESH_MESSAGE_T.
     */
    CYMESH_EVT_RELIABLE_MESSAGE_TIMEOUT,
    
    
    /*********************** Model specific events ***********************/

    /*** Configuration Client Model messages ***/
    
    /* Config Block Status message received. Parameter is undefined yet. */
    CYMESH_EVT_CONFIG_BLOCK_STATUS,
    
    /* Config Relay Status message received. Parameter is a pointer to 
     * uint8 for Relay status (1 = Relaying, 0 = Not relaying).
     */
    CYMESH_EVT_CONFIG_RELAY_STATUS,
    
    /* Config Default TTL Status message received. Parameter is a pointer 
     * to uint8 for Default TTL value.
     */
    CYMESH_EVT_CONFIG_DEFAULT_TTL_STATUS,
    
    /* Config App Net Status message received. */
    CYMESH_EVT_CONFIG_APP_NET_STATUS,
    
    /* AppKey OK Status message received. Parameter is pointer to uint8 for 
     * app key index.  
     */
    CYMESH_EVT_APPKEY_STATUS_OK,
    
    /* AppKey Error Status message received. Parameter is a pointer to
     * CYMESH_STATUS_CODE_T.
     */
    CYMESH_EVT_APPKEY_STATUS_ERROR,
    
    /* Component Address Status OK message received. Parameter is pointer to 
     * uint16 for component address.
     */ 
    CYMESH_EVT_COMPONENT_ADDRESS_STATUS_OK,
    
    /* Component Address Error Status message received. Parameter is a 
     * pointer to CYMESH_STATUS_CODE_T.
     */
    CYMESH_EVT_COMPONENT_ADDRESS_STATUS_ERROR,
    
    /* Model App Status OK message received. Parameter is pointer to uint32 
     * for appKeyMapping.
     */
    CYMESH_EVT_MODEL_APP_STATUS_OK,
    
    /* Model App Error Status received. Parameter is a pointer to 
     * CYMESH_STATUS_CODE_T. 
     */
    CYMESH_EVT_MODEL_APP_STATUS_ERROR,
    
    /* Model Publication Status OK message received. Parameter is a pointer
     * to CYMESH_MODEL_PUBLICATION_STATUS_T.
     */
    CYMESH_EVT_MODEL_PUBLICATION_STATUS_OK,
    
    /* Model Publication Error Status received. Paramater is a pointer to
     * CYMESH_STATUS_CODE_T.
     */
    CYMESH_EVT_MODEL_PUBLICATION_STATUS_ERROR,
    
    /* Model Subscription Status OK received. Parameter is a pointer to 
     * CYMESH_MODEL_SUBSCRIPTION_STATUS_T.
     */
    CYMESH_EVT_MODEL_SUBSCRIPTION_STATUS_OK,
    
    /* Model Subscription Error Status received. Parameter is a pointer to 
     * CYMESH_STATUS_CODE_T.
     */
    CYMESH_EVT_MODEL_SUBSCRIPTION_STATUS_ERROR,
    
    /* New Key Part message received. */
    CYMESH_EVT_NEW_KEY_PART_ACK,
    
    /* Ping TTL Status message received. */
    CYMESH_EVT_PING_TTL_STATUS,

    
    /*** Generic On/Off Client Model messages ***/
    
    /* Generic On/Off Status message received. Parameter is a pointer to 
     * uint8 for On/Off (1/0) status value.
     */
	CYMESH_EVT_MESSAGE_GENERIC_ON_OFF_SET,
	
    CYMESH_EVT_GENERIC_ON_OFF_STATUS,
    
    
    /*** Light Lightness Client Model messages ***/
    
    /* Light Lightness Status message received. Parameter is a pointer to 
     * uint16 for lightness value.
     */
	CYMESH_EVT_MESSAGE_LIGHT_LIGHTNESS_SET,
	
    CYMESH_EVT_LIGHT_LIGHTNESS_STATUS,
    
	 /*** Vendor Specific  Model messages ***/
    
    /* Light Lightness Status message received. Parameter is a pointer to 
     * uint16 for lightness value.
     */
	CYMESH_EVT_MESSAGE_VENDOR_SPECIFIC_UNREL_SET,
	
    CYMESH_EVT_MESSAGE_VENDOR_SPECIFIC_REL_SET,
    
    CYMESH_EVT_MESSAGE_VENDOR_SPECIFIC_STATUS
} CYMESH_EVENT_T;


/* List of status codes defined in the SmartMesh spec. */
typedef enum
{
    CYMESH_STATUS_SUCCESS,
    CYMESH_STATUS_INVALID_ADDRESS,
    CYMESH_STATUS_INVALID_COMPONENT,
    CYMESH_STATUS_BAD_MESSAGE,
    CYMESH_STATUS_INSUFFICIENT_PERMISSIONS,
    CYMESH_STATUS_INVALID_MODEL,
    CYMESH_STATUS_INVALID_APPKEY,
    CYMESH_STATUS_INVALID_NETKEY,
    CYMESH_STATUS_INVALID_TTL,
    CYMESH_STATUS_INSUFFICIENT_RESOURCES,
    CYMESH_STATUS_RESOURCE_NOT_FOUND,
    CYMESH_STATUS_MISSING_FIRST_KEY_PART,
    CYMESH_STATUS_BINDING_ALREADY_EXISTS,
    CYMESH_STATUS_NO_SUCH_BINDING,
    CYMESH_STATUS_NOT_PUBLISH_MODEL,
    CYMESH_STATUS_NOT_SUBSCRIBE_MODEL
    
} CYMESH_STATUS_CODE_T;

typedef enum
{
	CYMESH_CONFIG_MODEL_ID_PING_SERVER = 0x0000,
	CYMESH_CONFIG_MODEL_ID_CONFIG_SERVER,
	CYMESH_CONFIG_MODEL_ID_PROVISIONING_SERVER,
	CYMESH_CONFIG_MODEL_ID_FRIEND_SERVER,
	CYMESH_CONFIG_MODEL_ID_REMOTE_PROV_SERVER,
	
	CYMESH_CONFIG_MODEL_ID_GENERIC_STATUS_SERVER = 0x1000,
	CYMESH_CONFIG_MODEL_ID_GENERIC_ONOFF_SERVER,
	CYMESH_CONFIG_MODEL_ID_GENERIC_LEVEL_SERVER,
	CYMESH_CONFIG_MODEL_ID_GENERIC_POWER_SERVER,
	CYMESH_CONFIG_MODEL_ID_GENERIC_SENSOR_SERVER,
	CYMESH_CONFIG_MODEL_ID_GENERIC_TIME_SERVER,
	CYMESH_CONFIG_MODEL_ID_GENERIC_BATTERY_SERVER,
	
	CYMESH_CONFIG_MODEL_ID_SCENE_REGISTER_SERVER = 0x1100,
	CYMESH_CONFIG_MODEL_ID_SCENE_REGISTER_SETUP_SERVER,
	CYMESH_CONFIG_MODEL_ID_SCENE_SCHEDULER_SERVER,
	CYMESH_CONFIG_MODEL_ID_SCENE_SCHEDULER_SETUP_SERVER,
	
	CYMESH_CONFIG_MODEL_ID_LIGHT_LIGHTNESS_SERVER = 0x1200,
	CYMESH_CONFIG_MODEL_ID_LIGHT_CTL_SERVER,
	CYMESH_CONFIG_MODEL_ID_LIGHT_CTL_TEMPERATURE_SERVER,
	CYMESH_CONFIG_MODEL_ID_LIGHT_HSL_SERVER,
	CYMESH_CONFIG_MODEL_ID_LIGHT_HSL_HUE_SERVER,
	CYMESH_CONFIG_MODEL_ID_LIGHT_HSL_SATURATION_SERVER,
	CYMESH_CONFIG_MODEL_ID_LIGHT_XYL_SERVER,
	
	CYMESH_CONFIG_MODEL_ID_PING_CLIENT = 0x8000,
	CYMESH_CONFIG_MODEL_ID_CONFIG_CLIENT,
	CYMESH_CONFIG_MODEL_ID_PROVISIONING_CLIENT,
	CYMESH_CONFIG_MODEL_ID_FRIEND_CLIENT,
	
	CYMESH_CONFIG_MODEL_ID_GENERIC_STATUS_CLIENT = 0x9000,
	CYMESH_CONFIG_MODEL_ID_GENERIC_ONOFF_CLIENT,
	CYMESH_CONFIG_MODEL_ID_GENERIC_LEVEL_CLIENT,
	CYMESH_CONFIG_MODEL_ID_GENERIC_POWER_CLIENT,
	CYMESH_CONFIG_MODEL_ID_GENERIC_SENSOR_CLIENT,
	CYMESH_CONFIG_MODEL_ID_GENERIC_TIME_CLIENT,
	CYMESH_CONFIG_MODEL_ID_GENERIC_BATTERY_CLIENT,
	
	CYMESH_CONFIG_MODEL_ID_SCENE_REGISTER_CLIENT = 0x9100,
	CYMESH_CONFIG_MODEL_ID_SCENE_REGISTER_SETUP_CLIENT,
	CYMESH_CONFIG_MODEL_ID_SCENE_SCHEDULER_CLIENT,
	CYMESH_CONFIG_MODEL_ID_SCENE_SCHEDULER_SETUP_CLIENT,
	
	CYMESH_CONFIG_MODEL_ID_LIGHT_LIGHTNESS_CLIENT = 0x9200,
	CYMESH_CONFIG_MODEL_ID_LIGHT_CTL_CLIENT,
	CYMESH_CONFIG_MODEL_ID_LIGHT_CTL_TEMPERATURE_CLIENT,
	CYMESH_CONFIG_MODEL_ID_LIGHT_HSL_CLIENT,
	CYMESH_CONFIG_MODEL_ID_LIGHT_HSL_HUE_CLIENT,
	CYMESH_CONFIG_MODEL_ID_LIGHT_HSL_SATURATION_CLIENT,
	CYMESH_CONFIG_MODEL_ID_LIGHT_XYL_CLIENT,
	
	CYMESH_CONFIG_MODEL_ID_VENDOR_COMPAMY_ID = 0xFFFF,
	CYMESH_CONFIG_MODEL_ID_VENDOR_ASSIGNED_ID = 0x0010,
} CYMESH_CONFIG_MODEL_ID_T;

/* List of messages defined in the SmartMesh spec, with their opcodes */
typedef enum
{
    CYMESH_MESSAGE_CONFIG_BLOCK_STATUS                      = 0x00,
    CYMESH_MESSAGE_MODEL_PUBLICATION_SET					= 0x03,
	
	CYMESH_MESSAGE_CONFIG_BLOCK_GET                         = 0x8000,
	CYMESH_MESSAGE_CONFIG_DEFAULT_TTL_GET                   = 0x8004,
    CYMESH_MESSAGE_CONFIG_DEFAULT_TTL_SET                   = 0x8005,
    CYMESH_MESSAGE_CONFIG_DEFAULT_TTL_STATUS                = 0x8006,
	CYMESH_MESSAGE_CONFIG_IDENTITY_GET                   	= 0x8007,
    CYMESH_MESSAGE_CONFIG_IDENTITY_SET_REL                  = 0x8008,
    CYMESH_MESSAGE_CONFIG_IDENTITY_STATUS                	= 0x8009,
	
	CYMESH_MESSAGE_MODEL_PUBLICATION_GET                    = 0x800A,
    CYMESH_MESSAGE_MODEL_PUBLICATION_STATUS                 = 0x800C,
	CYMESH_MESSAGE_MODEL_SUBSCRIPTION_ADD_REL               = 0x800D,
    CYMESH_MESSAGE_MODEL_SUBSCRIPTION_DELETE_REL            = 0x800E,
    CYMESH_MESSAGE_MODEL_SUBSCRIPTION_GET                   = 0x800F,
    CYMESH_MESSAGE_MODEL_SUBSCRIPTION_STATUS                = 0x8010,
	CYMESH_MESSAGE_MODEL_SUBSCRIPTION_OVERWRITE_REL			= 0x8011,
    
	CYMESH_MESSAGE_CONFIG_RELAY_GET                         = 0x8012,
    CYMESH_MESSAGE_CONFIG_RELAY_SET_REL                     = 0x8013,
    CYMESH_MESSAGE_CONFIG_RELAY_STATUS                      = 0x8014,
    
	CYMESH_MESSAGE_CONFIG_PROXY_GET                         = 0x8015,
    CYMESH_MESSAGE_CONFIG_PROXY_SET_REL                     = 0x8016,
    CYMESH_MESSAGE_CONFIG_PROXY_STATUS                      = 0x8017,
	
	CYMESH_MESSAGE_CONFIG_BEACON_GET                        = 0x8018,
    CYMESH_MESSAGE_CONFIG_BEACON_SET_REL                    = 0x8019,
    CYMESH_MESSAGE_CONFIG_BEACON_STATUS                     = 0x801A,
    
	/* TODO friend messages - check values once spec is mature */
	CYMESH_MESSAGE_FRIEND_REQUEST                           = 0x8040,
    CYMESH_MESSAGE_FRIEND_INQUIRY                           = 0x8041,
	CYMESH_MESSAGE_FRIEND_CONFIRMATION                      = 0x8042,
    CYMESH_MESSAGE_FRIEND_SUBSCRIPTION_LIST_ADD             = 0x8043,
    CYMESH_MESSAGE_FRIEND_SUBSCRIPTION_LIST_CONFIRM         = 0x8044,
    CYMESH_MESSAGE_FRIEND_SUBSCRIPTION_LIST_REMOVE          = 0x8045,
	CYMESH_MESSAGE_FRIEND_NEGOTIATION                       = 0x8046,
	CYMESH_MESSAGE_FRIEND_CLEAR								= 0x8047,
	
	CYMESH_MESSAGE_PING           			                = 0x8080,
    CYMESH_MESSAGE_PONG                       			    = 0x8081,
	
	CYMESH_MESSAGE_PROV_APPKEY_ADD                          = 0x8090,
    CYMESH_MESSAGE_PROV_APPKEY_DELETE                       = 0x8092,
    CYMESH_MESSAGE_PROV_APPKEY_STATUS                       = 0x8093,
    CYMESH_MESSAGE_PROV_APPKEY_UPDATE                       = 0x8094,
	
	CYMESH_MESSAGE_PROV_MODEL_APP_BIND                      = 0x8097,
    CYMESH_MESSAGE_PROV_MODEL_APP_GET                       = 0x8098,
    CYMESH_MESSAGE_PROV_MODEL_APP_STATUS                    = 0x8099,
	CYMESH_MESSAGE_PROV_MODEL_APP_UNBIND                    = 0x809A,
	
	CYMESH_MESSAGE_PROV_NETKEY_ADD                          = 0x809B,
    CYMESH_MESSAGE_PROV_NETKEY_DELETE                       = 0x809D,
    CYMESH_MESSAGE_PROV_NETKEY_STATUS                       = 0x809E,
    CYMESH_MESSAGE_PROV_NETKEY_UPDATE                       = 0x809F,
	
	CYMESH_MESSAGE_PROV_NEW_KEY                     		= 0x80A0,
    CYMESH_MESSAGE_PROV_NEW_KEY_ACK                      	= 0x80A1,
	
    CYMESH_MESSAGE_PROV_CONFIG_NODE_RESET                   = 0x80A3,
	CYMESH_MESSAGE_PROV_CONFIG_NODE_RESET_STATUS            = 0x80A4,
	
    CYMESH_MESSAGE_APP_NET_GET                              = 0x8050,
    CYMESH_MESSAGE_APP_NET_STATUS                           = 0x8051,
    
    CYMESH_MESSAGE_COMPONENT_ADDRESS_GET                    = 0x8070,
    CYMESH_MESSAGE_COMPONENT_ADDRESS_SET                    = 0x8071,
    CYMESH_MESSAGE_COMPONENT_ADDRESS_STATUS                 = 0x8072,
    
    CYMESH_MESSAGE_KEY_UPDATE_REQUEST                       = 0x80D0,
    
    /* Generic Messages */
    CYMESH_MESSAGE_GENERIC_DEFAULT_TRANSITION_TIME_GET      = 0x8202,
    CYMESH_MESSAGE_GENERIC_DEFAULT_TRANSITION_TIME_SET      = 0x8203,
    CYMESH_MESSAGE_GENERIC_DEFAULT_TRANSITION_TIME_STATUS   = 0x8204,
    
    CYMESH_MESSAGE_GENERIC_LEVEL_GET                        = 0x8209,
    CYMESH_MESSAGE_GENERIC_LEVEL_SET                        = 0x820A,
    CYMESH_MESSAGE_GENERIC_LEVEL_STATUS                     = 0x820B,
    
    CYMESH_MESSAGE_GENERIC_ON_OFF_GET                       = 0x820D,
    CYMESH_MESSAGE_GENERIC_ON_OFF_SET                       = 0x820E,
    CYMESH_MESSAGE_GENERIC_ON_OFF_STATUS                    = 0x820F,
    
    
    /* Light Messages */
    CYMESH_MESSAGE_LIGHT_CTL_GET                            = 0x8300,
    CYMESH_MESSAGE_LIGHT_CTL_SET                            = 0x8301,
    CYMESH_MESSAGE_LIGHT_CTL_STATUS                         = 0x8302,
    CYMESH_MESSAGE_LIGHT_CTL_TEMP_RANGE_GET                 = 0x8303,
    CYMESH_MESSAGE_LIGHT_CTL_TEMP_RANGE_STATUS              = 0x8304,
    CYMESH_MESSAGE_LIGHT_HSL_GET                            = 0x8305,
    CYMESH_MESSAGE_LIGHT_HSL_SET                            = 0x8306,
    CYMESH_MESSAGE_LIGHT_HSL_STATUS                         = 0x8307,
    CYMESH_MESSAGE_LIGHT_HSL_TARGET_GET                     = 0x8308,
    CYMESH_MESSAGE_LIGHT_HSL_TARGET_STATUS                  = 0x8309,
    CYMESH_MESSAGE_LIGHT_LIGHTNESS_GET                      = 0x830A,
    CYMESH_MESSAGE_LIGHT_LIGHTNESS_SET                      = 0x830B,
    CYMESH_MESSAGE_LIGHT_LIGHTNESS_STATUS                   = 0x830C,
    CYMESH_MESSAGE_LIGHT_XYL_GET                            = 0x830D,
    CYMESH_MESSAGE_LIGHT_XYL_SET                            = 0x830E,
    CYMESH_MESSAGE_LIGHT_XYL_STATUS                         = 0x830F,
    CYMESH_MESSAGE_LIGHT_XYL_TARGET_GET                     = 0x8310,
    CYMESH_MESSAGE_LIGHT_XYL_TARGET_STATUS                  = 0x8311,

	//roit: Added for vendor specific command
    CYMESH_MESSAGE_VENDOR_SPECIFIC_UNREL_SET_1				=0x00FF0001,
	CYMESH_MESSAGE_VENDOR_SPECIFIC_REL_SET_1				=0x00FF0002,
	CYMESH_MESSAGE_VENDOR_SPECIFIC_STATUS_1					=0x00FF0003,
    
    CYMESH_MESSAGE_VENDOR_SPECIFIC_UNREL_SET_2				=0x00FF0004,
	CYMESH_MESSAGE_VENDOR_SPECIFIC_REL_SET_2				=0x00FF0005,
	CYMESH_MESSAGE_VENDOR_SPECIFIC_STATUS_2					=0x00FF0006,
    
    CYMESH_MESSAGE_VENDOR_SPECIFIC_UNREL_SET_3				=0x00FF0007,
	CYMESH_MESSAGE_VENDOR_SPECIFIC_REL_SET_3				=0x00FF0008,
	CYMESH_MESSAGE_VENDOR_SPECIFIC_STATUS_3					=0x00FF0009,
    
} CYMESH_MESSAGE_T;



/* Common Return types from APIs */
typedef enum
{
    /* Operation successful */
    CYMESH_ERROR_OK = 0x00,
    
    /* AES-CMAC operation happening; caller must wait for that to finish */
    CYMESH_ERROR_AES_CMAC_BUSY,
    
    /* AES-CCM encryption failed - either at application or network layer. */
    CYMESH_ERROR_AES_CCM_ENCRYPTION_FAILED,
    
    /* AES-CCM decryption failed - either at application or network layer. */
    CYMESH_ERROR_AES_CCM_DECRYPTION_FAILED,
    
    /* Invalid Mesh ID being used. */
    CYMESH_ERROR_INVALID_MESH_ID,
    
    /* Invalid AppKey index being used */
    CYMESH_ERROR_INVALID_APP_KEY_INDEX,
    
    /* Invalid callback assigned */
    CYMESH_ERROR_INVALID_CALLBACK,
    
	/* Invalid parameter provided */
	CYMESH_ERROR_INVALID_PARAM,
	
    /* Configuration stored in flash is corrupted */
    CYMESH_ERROR_INVALID_CONFIGURATION,
    
    /* API initialized multiple times without following procedure */
    CYMESH_ERROR_MULTIPLE_INITIALIZATION,
    
    /* BLE component related failure - more details in the corresponding API header */
    CYMESH_ERROR_BLE_OPERATION_FAILURE,
    
    /* Bearer's TX Buffer is full */
    CYMESH_ERROR_BEARER_TX_BUFFER_FULL,
    
	/* TTL value is invalid */
	CYMESH_NET_ERROR_TTL_INVALID,
	
	/* Current message is in cache */
    CYMESH_NET_ERROR_MSG_IN_CACHE,
	
	/* Message added in cache */
    CYMESH_NET_MSG_ADDED_IN_CACHE,
	
	/* Network header Clarification failed */
    CYMESH_NET_ERROR_CLARIFY_FAILED,
	
    /* Network header Obfuscation failed */
    CYMESH_NET_ERROR_OBFUSCATE_FAILED,
	
	/* Key Refresh has been started */
	CYMESH_NET_KEY_REFRESH_STARTED,
	
	/* Current message is in cache */
    CYMESH_NET_ERROR_MSG_REPLAY_PROT,
	
    /* Transport layer error codes */
    /*-------------------------------*/
    CYMESH_ERROR_TRANSPORT_INVALID_PACKET_LENGTH,
    
    CYMESH_ERROR_TRANSPORT_INVALID_PARAM,
    
    CYMESH_ERROR_THREAD_BUSY,
    
    /* Application layer error codes */
    CYMESH_ERROR_APPLICATION_INVALID_OPCODE,
    
    CYMESH_ERROR_APPLICATION_INVALID_PACKET_LENGTH,
    
    CYMESH_ERROR_APPLICATION_BUFFER_FULL,
    
    /* API specific error (more information in the corresponding API header */
    CYMESH_ERROR_OTHER = 0xFF
    
} CYMESH_API_RETURN_T;



/* Current state of the node */
typedef enum
{
    /* The device is waiting to be provisioned and hence not part of a mesh network. */
    CYMESH_ROLE_UNPROVISIONED = 0x00,
    
    /* The device is provisioned but not configured */
    CYMESH_ROLE_PROVISIONED,
    
    /* The device has been configured and is an Edge node using advertisement Bearer */
    CYMESH_ROLE_EDGE,
    
    /* The device has been configured and is a Relay node using advertisement Bearer */
    CYMESH_ROLE_RELAY,
	
	/* The device has been configured and is a Relay node using advertisement Bearer */
    CYMESH_ROLE_PROVISIONER,
    
    /* Idle state - no activity */
    CYMESH_ROLE_IDLE = 0xFF
    
} CYMESH_ROLE_T;




/*******************************************************************************
* Data Structures
*******************************************************************************/

/* Event Callback */
typedef CYMESH_API_RETURN_T (*CYMESH_CALLBACK_T) (uint32 event, void * eventParam);

/** Event callback function prototype to receive events from the MODELS */
typedef void (* CYBLE_MODEL_CALLBACK_T) (uint32 eventCode, void *eventParam, uint8 componentIndex, uint8 modelIndex);

/* Parameter for the event CYMESH_EVT_MODEL_PUBLICATION_STATUS_OK
 * going up to the user.
 */
typedef struct
{
    uint16 publishAddress;
    uint8 appKeyIndex;
    uint8 publishTtl;
} CYMESH_MODEL_PUBLICATION_STATUS_T;


/* Parameter for the event CYMESH_EVT_MODEL_SUBSCRIPTION_STATUS_OK
 * going up to the user.
 */
typedef struct
{
    uint8 numberOfAddresses;
    uint8 startingIndex;
    uint8 addressBlock[6];
} CYMESH_MODEL_SUBSCRIPTION_STATUS_T;



/************** Friendship information *****************/
typedef struct
{
    /* Which mesh this friend is a part of */
    uint8 meshId;   
    
    /* Source address of the friend */
    uint16 srcAddress;

    /* Group address of the friend */
    uint16 groupAddress;
} CYMESH_FRIEND_INFO_T;


typedef struct
{
    /* Zero signifies no friends */
    uint8 numberOfFriends;

    /* Complete list of friends */
    CYMESH_FRIEND_INFO_T friendList[CYMESH_MAX_FRIENDS_PER_NETWORK];

    /* Each bit to hold 1/0 indicating validity of an entry in the list.
     * This is provided so that friends can be added to or removed
     * from the list at runtime.
     */
    uint32 isFriendValid;       
} CYMESH_FRIENDLIST_T;



/*********** Complete device information **************/
typedef enum
{
    CYMESH_RELIABLE = true,
    CYMESH_NOT_RELIABLE = false,
    CYMESH_EITHER = 0xFF
} CYMESH_RELIABLE_T;

typedef struct
{
    uint32 opcode;
    CYMESH_RELIABLE_T isReliable;
    uint8 numberOfPackets;
    uint8 paramLength;
} CYMESH_MODEL_OPCODE_DETAIL_T;


typedef struct
{
    /* Each model has a unique ID */
    CYMESH_CONFIG_MODEL_ID_T modelId;
    
	/* Tells if this model is Vendor Defined or SIG defined*/
	uint8 isVendorSpecificModel;
	
    /* Messages go to this address when issued by this model,
     * and use this appkey. 
     */
    uint16 publishAddress;
    uint8 publishAppKeyIndex;
    
    /* List of group and virtual addresses subscribed to by the model */
    uint16 subscriptionAddress[CYMESH_MAX_SUBSCRIPTION_ADDRESSES];
    uint8 numberOfSubscribedAddresses;
    
    /* List of bound AppKeys - indexes (bitmap) and total count.
     * Because the bitmap is 32 bit - total appkeys which can be 
     * mapped is 32.
     */
    uint32 boundAppKeyMapping;
    uint8 numberOfBoundAppKeys;
    
    /* Default TTL for the model */
    uint8 modelDefaultTtl;
    
    /* Opcodes in this model */
    const CYMESH_MODEL_OPCODE_DETAIL_T * opcode;
    uint8 numberOfOpcodes;
    
    /* Pointer to the callback function */
    CYMESH_CALLBACK_T callbackPointer;
} CYMESH_MODEL_T;

typedef struct
{
	uint8 data[CYMESH_MODEL_VAR_DATA_SIZE];
	uint8 len;
}CYMESH_VARIABLE_DATA_T;

typedef struct
{
    /* Generic On Off States */
    bool isStateOnOrOff;
    uint8 defaultTransitionTime;

    /* Generic Level States */
    uint16 level;

    /* Light Lightness States */
    uint16 lightness;
	
	CYMESH_VARIABLE_DATA_T	var;
	
} CYMESH_COMPONENT_STATES_T;


typedef struct
{
    /* This must be a unicast (source) address.
     * If this is component 0, this becomes the source address for the node.
     */
    uint16 componentAddress;
    
	/* This determines which part of the node this component represents */
	uint16 loc;
	
	/* This determines the number of SIG adopted models in this component */
	uint8 numberOfSigAdoptedModels;
	
	/* This determines the number of vendor specific models */
	uint8 numberOfVendorSpecificModels;
	
    /* Non-overlapping models within this component */
    CYMESH_MODEL_T model[CYMESH_MAX_MODELS_PER_COMPONENT];

    /* Component states */
    CYMESH_COMPONENT_STATES_T componentState;
} CYMESH_COMPONENT_T;

typedef struct
{
    /* Components in this node */
    CYMESH_COMPONENT_T components[CYMESH_NUMBER_OF_COMPONENTS];
    
    /* Default TTL value */
    uint8 deviceDefaultTtl;
    
    /* Device Key */
    uint8 deviceKey[16];
    
	/*Company Identifier */
	uint16 cid;
	
	/* Assigned Product Identifier*/
	uint16 pid;
	
	/* Product Version Idnetifier */
	uint16 vid;
} CYMESH_DEVICE_INFO_T;


/************** Complete network information *****************/

typedef struct
{
    /* Identifies which application it is */
    uint8 applicationId[16];
    
    /* Application key; Index 0 is MSB */
    uint8 applicationKey[16];
    
    /* Network Key to which this application key is mapped */
    uint8 netKeyIndex;
} CYMESH_APPLICATION_KEYS_T;


typedef struct
{
    /* How many application keys are defined */
    uint8 numberOfApplicationKeys;
    
    /* Cross-table for applications vs their keys */
    CYMESH_APPLICATION_KEYS_T appKeys[CYMESH_MAX_APPLICATION_KEYS];
    
    /* Each bit to hold 1/0 indicating validity of an entry in the list.
     * This is provided so that applications can be added to or removed
     * from the list at runtime.
     */
    uint32 isApplicationValid;
} CYMESH_APPLICATION_INFO_T;


typedef struct
{
    /* All these are stored with MSB first (index 0 is MSB); 
     * except for ivIndex (which is a regular uint). */
    uint8 networkKey[16];
    uint8 encryptionKey[16];
    uint8 privacyKey[16];
    uint8 networkId[16];
    uint32 ivIndex;

    /* Which Application keys are mapped - bit mapping.
     * Every bit - 1 indicates that appKeyIndex mapped.
     * 32 bit value - indicates max of 32 AppKeys can 
     * be mapped. 
     */
    uint32 netKeyAppKeyMapping;
    
} CYMESH_NETWORK_KEYS_T;

typedef struct
{
    /* How many network keys are defined */
    uint8 numberOfNetworkKeys;

    /* List of network keys and derivatives */
    CYMESH_NETWORK_KEYS_T netKeys[CYMESH_MAX_NETWORK_KEYS];
    
    /* Each bit to hold 1/0 indicating validity of an entry in the list.
     * This is provided so that network keys can be added to or removed
     * from the list at runtime.
     */
    uint32 isNetKeyValid;
} CYMESH_NETWORK_INFO_T;


typedef struct
{
    /* Configuration validity based on checksum match */
    bool isConfigurationValid;
    
    /* Complete device information - list of components and models etc */
    CYMESH_DEVICE_INFO_T deviceInfo;
    
    /* List of application keys */
    CYMESH_APPLICATION_INFO_T appInfo;
    
    /* List of network keys */
    CYMESH_NETWORK_INFO_T netInfo;
    
    /* Role of Bearer; calculated based on role of individual mesh networks */
    CYMESH_ROLE_T bearerRole;
    
    /* Device UUID is used during provisioning and meash beacons */
	uint8 device_UUID[CYMESH_DEVICE_UUID_LENGTH];			
    
	/* Power-off recovery sequence Number of the device */
	uint32 seq_num;
	
    /* Checksum to ensure data accuracy in flash storage */
    uint32 checksum;
	
} CYMESH_DEVICE_CONFIG_T;

typedef enum
{
	CYMESH_BEARER_UNPROVISIONED_STATE_STARTBEACON,
	CYMESH_BEARER_UNPROVISIONED_STATE_PROCESS,
} CYMESH_BEARER_UNPROVISIONED_STATE_T;


/*******************************************************************************
* Globals
*******************************************************************************/
extern CYMESH_FRIENDLIST_T cyMesh_FriendListRam;
extern const CYMESH_FRIENDLIST_T cyMesh_FriendListFlash;

extern CYMESH_DEVICE_CONFIG_T cyMesh_ConfigInfoRam;
extern const CYMESH_DEVICE_CONFIG_T cyMesh_ConfigInfoFlash;
extern uint16 cyMesh_deviceCompanyID;

/*******************************************************************************
* Externed functions
*******************************************************************************/



/******************************************************************************
* Function Name: CyMesh_ProcessEvents
***************************************************************************//**
* 
*  This function is the core engine for Mesh. It is responsible for -
*  1. Pushing the ADV packets to the BLE stack. 
*  2. Listening to BLE stack's events to receive ADV packets. 
*  3. Switching the BLE state between scan and advertisement.
* 
*  \param None.
*
*  \return None.
* 
******************************************************************************/
extern void CyMesh_ProcessEvents(void);

/******************************************************************************
* Function Name: CyMesh_BearerSetGattAdvertising
***************************************************************************//**
* 
*  This function enables/disables the GATT advertising so that the node can be  
* connected and data sent over the Mesh network.
* 
*  \param state: 0 = disable GATT Advertising
*				1 = Enables GATT advertising
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if GATT advertising Enabled
*								CYMESH_ERROR_OTHER if Bearer is not started
* 
******************************************************************************/
extern CYMESH_API_RETURN_T  CyMesh_BearerSetGattAdvertising(uint8 state);


/******************************************************************************
* Function Name: CyMesh_SecurityUpdateCredentials
***************************************************************************//**
* 
*  This function updates the security credentials for Smart Mesh using the keys
* set in cyMesh_configInRam.
* 
*  \param void
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if GATT advertising Enabled
*								CYMESH_ERROR_OTHER if Bearer is not started
* 
******************************************************************************/
CYMESH_API_RETURN_T CyMesh_SecurityUpdateCredentials(void);

/******************************************************************************
* Function Name: CyMesh_ConfigInfoUpdate
***************************************************************************//**
* 
*  This function checks for difference of data, if any, between ConfigInfoRam and 
* ConfigInfoFlash. If there is, it triggers a Flash update with data in ConfigInfoRam
* 
*  \param none:
*
*  \return none:
* 
******************************************************************************/
void CyMesh_ConfigInfoUpdate(void);

void CyMesh_varInit(void);

#endif      /* #ifndef _CYMESH_COMMON_H */

/* [] END OF FILE */
