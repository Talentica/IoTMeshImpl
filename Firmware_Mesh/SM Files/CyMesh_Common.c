/***************************************************************************//**
* \file CyMesh_Common.c
* \version 1.0
* 
* \brief
*  This file contains the implementation of the common portion of the BLE
*  SmartMesh v1 solution.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include "CyMesh_Common.h"



/*******************************************************************************
* Data Structures
*******************************************************************************/

/* RAM and Flash copies for the friendship information */
CYMESH_FRIENDLIST_T cyMesh_FriendListRam;

#if defined(__ARMCC_VERSION)
    CY_ALIGN(CYDEV_FLS_ROW_SIZE) const CYMESH_FRIENDLIST_T cyMesh_FriendListFlash CY_SECTION(".cy_checksum_exclude") =
#elif defined (__GNUC__)
    const CYMESH_FRIENDLIST_T cyMesh_FriendListFlash CY_SECTION(".cy_checksum_exclude")
        CY_ALIGN(CYDEV_FLS_ROW_SIZE) =
#elif defined (__ICCARM__)
    #pragma data_alignment=CY_FLASH_SIZEOF_ROW
    #pragma location=".cy_checksum_exclude"
    const CYMESH_FRIENDLIST_T cyMesh_FriendListFlash =
#endif  /* (__ARMCC_VERSION) */
{
    /* numberOfFriends */
    0,
    
    /* friendList */
    {
        /* Array index 0 */
        {
            0, 0, 0
        },
        /* Array index 1 */
        {
            0, 0, 0
        },
        /* Array index 2 */
        {
            0, 0, 0
        },
        /* Array index 3 */
        {
            0, 0, 0
        }
    },
    
    /* isFriendValid */
    0
};

extern void CyMesh_NetworkManagementTimer(void);
extern void CyMesh_ConfigSendUnprovisionedBeacon(void);
extern void CyMesh_BearerSendGATTProxyADV(void);

/* RAM copy for the entire information */
CYMESH_DEVICE_CONFIG_T cyMesh_ConfigInfoRam;
CYMESH_BEARER_UNPROVISIONED_STATE_T	cyMesh_UnprovisionedState = CYMESH_BEARER_UNPROVISIONED_STATE_STARTBEACON;

/* Flash copy for the entire information - flash row aligned.
 * Stored at the end of the flash in case of Bootloadable projects so that 
 * this information will be excluded from the Bootloadable checksum.
 * The initialization structure depends on the macros defined in Common.h.
 */
#if defined(__ARMCC_VERSION)
    CY_ALIGN(CYDEV_FLS_ROW_SIZE) const CYMESH_DEVICE_CONFIG_T cyMesh_ConfigInfoFlash CY_SECTION(".cy_checksum_exclude") =
#elif defined (__GNUC__)
    const CYMESH_DEVICE_CONFIG_T cyMesh_ConfigInfoFlash CY_SECTION(".cy_checksum_exclude")
        CY_ALIGN(CYDEV_FLS_ROW_SIZE) =
#elif defined (__ICCARM__)
    #pragma data_alignment=CY_FLASH_SIZEOF_ROW
    #pragma location=".cy_checksum_exclude"
    const CYMESH_DEVICE_CONFIG_T cyMesh_ConfigInfoFlash =
#endif  /* (__ARMCC_VERSION) */
{
    /* isConfigurationValid */
    false,
    
    /* deviceInfo */
    {
        /* Components */
        {
            /* Index 0 */
            {
                /* componentAddress */
                0x0000,
                
				/* LOC */
				0x0000,
				
				/* numberOfSigAdoptedModels */
				0x00,
				
				/* numberOfVendorSpecificModels */
				0x00,
				
                /* model */
                {
                    /* Index 0 */
                    {
                        0x0000, 0x00, 0x0000, 0, {0x0000, 0x0000}, 0, 0, 0, 0, NULL, 0, NULL
                    },
                    
                    /* Index 1 */
                    {
                        0x0000, 0x00, 0x0000, 0, {0x0000, 0x0000}, 0, 0, 0, 0, NULL, 0, NULL
                    }
                },
                
                /* componentState */
                {false, 0, 0, 0, {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, 0x00}},
            },
            
            /* Index 1 */
            {
                /* componentAddress */
                0x0000,
                
				/* LOC */
				0x0000,
				
				/* numberOfSigAdoptedModels */
				0x00,
				
				/* numberOfVendorSpecificModels */
				0x00,
				
                /* model */
                {
                    /* Index 0 */
                    {
                        0x0000, 0x00, 0x0000, 0, {0x0000, 0x0000}, 0, 0, 0, 0, NULL, 0, NULL
                    },
                    
                    /* Index 1 */
                    {
                        0x0000, 0x00, 0x0000, 0, {0x0000, 0x0000}, 0, 0, 0, 0, NULL, 0, NULL
                    }
                },
                
                /* componentState */
                {false, 0, 0, 0, {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, 0x00}},
            },
			
			/* Index 2 */
            {
                /* componentAddress */
                0x0000,
                
				/* LOC */
				0x0000,
				
				/* numberOfSigAdoptedModels */
				0x00,
				
				/* numberOfVendorSpecificModels */
				0x00,
				
                /* model */
                {
                    /* Index 0 */
                    {
                        0x0000, 0x00, 0x0000, 0, {0x0000, 0x0000}, 0, 0, 0, 0, NULL, 0, NULL
                    },
                    
                    /* Index 1 */
                    {
                        0x0000, 0x00, 0x0000, 0, {0x0000, 0x0000}, 0, 0, 0, 0, NULL, 0, NULL
                    }
                },
                
                /* componentState */
                {false, 0, 0, 0, {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, 0x00}}
            }
        },
        
        /* deviceDefaultTtl */
        0,
        
        /* deviceKey */
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		
		/* CID */
		0x0000,
		
		/* PID */
		0x0000,
		
		/* VID */
		0x0000,
    },
    
    /* appInfo */
    {
        /* numberOfApplicationKeys */
        0,
        
        /* appKeys */
        {
            /* Array index 0 */
            {
                /* applicationId */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* applicationKey */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* netKeyIndex */
                0
            },
            
            /* Array index 1 */
            {
                /* applicationId */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* applicationKey */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* netKeyIndex */
                0
            }
        },
        
        /* isApplicationValid */
        0
    },
    
    /* netInfo */
    {
        /* numberOfNetworkKeys */
        0,
        
        /* netKeys */
        {
            /* Index 0 */
            {
                /* Network key */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* Encryption key */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* Privacy Key */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* Network ID */
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                
                /* IV index */
                0,

                /* netKeyAppKeyMapping */
                0,
            }
        },
        
        /* isNetKeyValid */
        0
    },
    
    /* bearerRole */
    CYMESH_ROLE_UNPROVISIONED,
    
    /* device_UUID */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    
	/* Sequence Number at Power off */
    0,
	
    /* Checksum */
    0
};

/******************************************************************************
* Function Name: CyMesh_SMHandleTimer
*****************************************************************************
* 
*  Registers a user application layer event handler for SM timing based
*  specific events.
* 
*  \param 
*   void
* 
*  \return
*   void
* 
******************************************************************************/
void CyMesh_SMHandleTimer(void)
{
	#if (CYMESH_ENABLE_FRIENDSHIP == 1)
		CyMesh_NetworkManagementTimer();
	#endif
	
	#if (CYMESH_ENABLE_PROVISIONING == 1)
	#if (CYMESH_IS_PROVISIONER_ENABLED_OR_UNPROV == 0)
		CyMesh_ConfigSendUnprovisionedBeacon();
	#endif
	#endif
	
	#if (CYMESH_BEARER_GATT_BEARER_ENABLED == 1)
		CyMesh_BearerSendGATTProxyADV();
	#endif
	
	#if (CYMESH_ENABLE_CONFIGINFO_PERIODIC_UPDATE == 1)
		CyMesh_ConfigInfoUpdate();
	#endif
}

/******************************************************************************
* Initializer for models *
*******************************************************************************/
void CyMesh_varInit(void)
{
    //Vendor Specific Model
    cyMesh_Model_VenSp_Comp0        = CYMESH_MDL_VENDOR_SPECIFIC_COMP_0;
    cyMesh_Model_VenSp_Comp0_MdlIdx = CYMESH_MDL_VENDOR_SPECIFIC_COMP_0_MDLIDX;
    cyMesh_Model_VenSp_Comp1        = CYMESH_MDL_VENDOR_SPECIFIC_COMP_1;
    cyMesh_Model_VenSp_Comp1_MdlIdx = CYMESH_MDL_VENDOR_SPECIFIC_COMP_1_MDLIDX;
    cyMesh_Model_VenSp_Comp2        = CYMESH_MDL_VENDOR_SPECIFIC_COMP_2;
    cyMesh_Model_VenSp_Comp2_MdlIdx = CYMESH_MDL_VENDOR_SPECIFIC_COMP_2_MDLIDX;
    cyMesh_Model_VenSp_Comp3        = CYMESH_MDL_VENDOR_SPECIFIC_COMP_3;
    cyMesh_Model_VenSp_Comp3_MdlIdx = CYMESH_MDL_VENDOR_SPECIFIC_COMP_3_MDLIDX;
    
    //Light Lightness model
    cyMesh_Model_LL_Comp0        = CYMESH_MDL_LL_COMP_0;
    cyMesh_Model_LL_Comp0_MdlIdx = CYMESH_MDL_LL_COMP_0_MDLIDX;
    cyMesh_Model_LL_Comp1        = CYMESH_MDL_LL_COMP_1;
    cyMesh_Model_LL_Comp1_MdlIdx = CYMESH_MDL_LL_COMP_1_MDLIDX;
    cyMesh_Model_LL_Comp2        = CYMESH_MDL_LL_COMP_2;
    cyMesh_Model_LL_Comp2_MdlIdx = CYMESH_MDL_LL_COMP_2_MDLIDX;
    cyMesh_Model_LL_Comp3        = CYMESH_MDL_LL_COMP_3;
    cyMesh_Model_LL_Comp3_MdlIdx = CYMESH_MDL_LL_COMP_3_MDLIDX;
                                      
    //Generic OnOff model
    cyMesh_Model_GenOnOff_Comp0        = CYMESH_MDL_GenOnOff_COMP_0;
    cyMesh_Model_GenOnOff_Comp0_MdlIdx = CYMESH_MDL_GenOnOff_COMP_0_MDLIDX;
    cyMesh_Model_GenOnOff_Comp1        = CYMESH_MDL_GenOnOff_COMP_1;
    cyMesh_Model_GenOnOff_Comp1_MdlIdx = CYMESH_MDL_GenOnOff_COMP_1_MDLIDX;
    cyMesh_Model_GenOnOff_Comp2        = CYMESH_MDL_GenOnOff_COMP_2;
    cyMesh_Model_GenOnOff_Comp2_MdlIdx = CYMESH_MDL_GenOnOff_COMP_2_MDLIDX;
    cyMesh_Model_GenOnOff_Comp3        = CYMESH_MDL_GenOnOff_COMP_3;
    cyMesh_Model_GenOnOff_Comp3_MdlIdx = CYMESH_MDL_GenOnOff_COMP_3_MDLIDX;
}

/* [] END OF FILE */
