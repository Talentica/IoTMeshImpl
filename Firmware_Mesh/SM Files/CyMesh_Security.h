/***************************************************************************//**
* \file CyMesh_Security.h
* \version 1.0
* 
* \brief
*  This is the header file for the BLE SmartMesh v1 Security module.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYMESH_SECURITY_H
#define _CYMESH_SECURITY_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include <stdio.h>
#include <stdbool.h>
#include "CyMesh_Common.h"
//#define BLE_2_2_BELOW
/*******************************************************************************
* Typedef
*******************************************************************************/

/* Callback function to indicate end of requested operation */
typedef void (* CYMESH_SECURITY_CALLBACK) (uint8 * cmac);


    
/*******************************************************************************
* Externed functions
*******************************************************************************/


/******************************************************************************
* Function Name: CyMesh_SecuritySetApplicationKey
***************************************************************************//**
* 
*  This function stores the new application key for the device. The application
*  ID is derived immediately.
*  The function is non blocking; a callback is initiated once the derivative
*  has been computed. This callback happens in the context of 
*  CyMesh_ProcessEvents().
* 
*  \param appKeyIndex: The index at which to add the app key. 
*
*  \param isAddOrUpdate: 0 to add a new key, 1 to update existing key
*
*  \param applicationKey: Pointer to the 16 byte application key (MSB first).
*
*  \param callback: Pointer to the function which is called back, once all the 
*         derivative keys have been calculated and the operation is complete.
*         This can be NULL if no callback is desired.
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function  
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_AES_CMAC_BUSY</td>
*    <td>Another AES-CMAC is already running; wait for it to finish</td>
*  </tr>
*  </table>
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecuritySetApplicationKey(uint8 appKeyIndex, 
                                                            bool isAddOrUpdate,
                                                            const uint8 * applicationKey, 
                                                            CYMESH_SECURITY_CALLBACK callback);


/*****************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityDeleteApplicationKey(uint8 appKeyIndex);



/******************************************************************************
* Function Name: CyMesh_SecuritySetNetworkKey
***************************************************************************//**
* 
*  This function stores the new Network Key for the device. All keys 
*  dependent on this Network Key are derived immediately. This includes:
*  - Encryption Key
*  - Network ID
*  - Privacy Counter
*  The function is non blocking; a callback is initiated once the derivatives
*  have been computed. This callback happens in the context of 
*  CyMesh_ProcessEvents().
* 
*  \param meshId: Identifies which mesh the packet belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param networkKey: Pointer to the 16 byte Network Key (MSB first).
*
*  \param callback: Pointer to the function which is called back, once all the 
*         derivative keys have been calculated and the operation is complete.
*         This can be NULL if no callback is desired.
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function  
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_AES_CMAC_BUSY</td>
*    <td>Another AES-CMAC is already running; wait for it to finish</td>
*  </tr>
*  </table>
* 
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecuritySetNetworkKey(uint8 meshId, const uint8 * networkKey, CYMESH_SECURITY_CALLBACK callback);



/******************************************************************************
* Function Name: CyMesh_SecurityEncryptAppData
***************************************************************************//**
* 
*  This function encrypts the incoming application data. It overwrites the 
*  input information when successful. 
* 
*  \param netKeyIndex: Identifies which network key can be used to encrypt the 
*         packet. Value is between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param appKeyIndex: Which application key to use for encrypting. Valid only
*         when the isAppKeyUsed parameter is true.
*
*  \param isAppKeyUsed: Whether the device key is used to encrypt, or whether
*         one of the application keys is used. Boolean false for device key,
*         and true for application key.
*
*  \param isOwnOrPeerDevKey: Whether own device key to use or peer's. Boolean
*         true for Peer device key, and false for own device key.
*
*  \param peerDevKey: Pointer to 128-bit Peer Dev Key. Ignored if 
*         isOwnOrPeerDevKey is false.
*
*  \param appPacket: Application packet to be encrypted. This includes the 
*         following - DFT, AID/SEQ, SRC, DST, Payload.
*         The payload field is overwritten with the encrypted data when the
*         operation is successful. The MICapp is appended at the end.
*
*  \param length: Length of the input data (payload). Must be between 0 and 27.
* 
*  \return CYMESH_API_RETURN_T: Return value indicates if the function  
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_AES_CCM_ENCRYPTION_FAILED</td>
*    <td>AES-CCM encryption failed.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_APP_KEY_INDEX</td>
*    <td>Invalid Application Key index passed to the API.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_MESH_ID</td>
*    <td>Invalid Mesh ID passed to the API.</td>
*  </tr>
*  </table>
* 
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityEncryptAppData(uint8 netKeyIndex, 
                                                         uint8 appKeyIndex, 
                                                         bool isAppKeyUsed, 
                                                         bool isOwnOrPeerDevKey,
                                                         const uint8 * peerDevKey,
                                                         uint8 * appPacket, 
                                                         uint8 length);



/******************************************************************************
* Function Name: CyMesh_SecurityDecryptAppData
***************************************************************************//**
* 
*  This function decrypts the incoming data to give the application data. It 
*  overwrites the input information when successful. 
* 
*  \param netKeyIndex: Identifies which network key can be used to decrypt the 
*         packet. Value is between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param appKeyIndex: Which application key to use for decrypting. Valid only
*         when the isAppKeyUsedisAppKeyUsed parameter is false.
*
*  \param isAppKeyUsed: Whether the device key is used to decrypt, or whether
*         one of the application keys is used. Boolean false for device key,
*         and true for application key.
*
*  \param isOwnOrPeerDevKey: Whether own device key to use or peer's. Boolean
*         true for Peer device key, and false for own device key.
*
*  \param peerDevKey: Pointer to 128-bit Peer Dev Key. Ignored if 
*         isOwnOrPeerDevKey is false.
*
*  \param inputPacket: Input packet to be decrypted. This includes the 
*         following - DFT, AID/SEQ, SRC, DST, Payload, MICapp.
*         The payload field is overwritten with the decrypted data when the
*         operation is successful.
*
*  \param length: Length of the encrypted packet. Must be between 0 and 27.
*         This includes the app payload and the MICapp.
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function  
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_AES_CCM_DECRYPTION_FAILED</td>
*    <td>AES-CCM decryption failed.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_APP_KEY_INDEX</td>
*    <td>Invalid Application Key index passed to the API.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_MESH_ID</td>
*    <td>Invalid Mesh ID passed to the API.</td>
*  </tr>
*  </table>
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityDecryptAppData(uint8 netKeyIndex, 
                                                         uint8 appKeyIndex, 
                                                         bool isAppKeyUsed, 
                                                         bool isOwnOrPeerDevKey,
                                                         const uint8 * peerDevKey,
                                                         uint8 * inputPacket, 
                                                         uint8 length);



/******************************************************************************
* Function Name: CyMesh_SecurityEncryptNetworkData
***************************************************************************//**
* 
*  This function encrypts the incoming network data. It overwrites the 
*  input information when successful. 
* 
*  \param meshId: Identifies which mesh the packet belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param netPacket: Network packet to be encrypted. This includes the 
*         following - PFT, SEQ, SRC, Payload (includes DST).
*         The payload field is overwritten with the encrypted data when the 
*         operation is successful. The MICnet is appended at the end. 
*
*  \param length: Length of the input data. This includes the actual network  
*         payload and 2 bytes for DST. 
*         Must be between 0 and 27. 
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function succeeded 
*          or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_AES_CCM_ENCRYPTION_FAILED</td>
*    <td>AES-CCM encryption failed.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_MESH_ID</td>
*    <td>Invalid Mesh ID passed to the API.</td>
*  </tr>
*  </table>
* 
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityEncryptNetworkData(uint8 meshId, uint8 * netPacket, uint8 length);

/******************************************************************************
* Function Name: CyMesh_SecurityDecryptNetworkData
***************************************************************************//**
* 
*  This function decrypts the incoming data to give the network data. It 
*  overwrites the input information when successful. 
* 
*  \param meshId: Identifies which mesh the packet belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param inputPacket: Input packet to be decrypted. This includes the 
*         following - PFT, SEQ, SRC, Payload (includes DST), MICnet.
*         The payload field is overwritten with the decrypted data when the
*         operation is successful.
*
*  \param length: Length of the encypted data. This includes the network
*         payload, 2 bytes for DST, and 4 bytes for MICnet.
*         Must be between 0 and 27. 
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function  
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_AES_CCM_DECRYPTION_FAILED</td>
*    <td>AES-CCM decryption failed.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_MESH_ID</td>
*    <td>Invalid Mesh ID passed to the API.</td>
*  </tr>
*  </table>
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityDecryptNetworkData(uint8 meshId, uint8 * inputPacket, uint8 length);



/******************************************************************************
* Function Name: CyMesh_SecurityObfuscateNetHeader
***************************************************************************//**
* 
*  This function obfuscates the given network header. 
* 
*  \param meshId: Identifies which mesh the packet belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param netPacket: Input packet to be obfuscated. This includes the 
*         following - DFT, AID/SEQ, SRC, Payload (includes DST, MICapp), MICnet.
*         DFT+AID/SEQ+SRC is overwritten with the obfuscated header if successful.
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function  
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_MESH_ID</td>
*    <td>Invalid Mesh ID passed to the API.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OTHER</td>
*    <td>Obfuscation failed due to AES operation failure</td>
*  </tr>
*  </table>
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityObfuscateNetHeader(uint8 meshId, uint8 * netPacket);



/******************************************************************************
* Function Name: CyMesh_SecurityClarifyNetHeader
***************************************************************************//**
* 
*  This function clarifies the given network header. This process is reverse of 
*  obfuscation of network header.
* 
*  \param meshId: Identifies which mesh the packet belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param netPacket: Input packet to be obfuscated. This includes the 
*         following - DFT, AID/SEQ, SRC, Payload (includes DST, MICapp), MICnet.
*         DFT+AID/SEQ+SRC is overwritten with the obfuscated header if successful.
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function 
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_MESH_ID</td>
*    <td>Invalid Mesh ID passed to the API.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OTHER</td>
*    <td>Clarification failed due to AES operation failure</td>
*  </tr>
*  </table>
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityClarifyNetHeader(uint8 meshId, uint8 * netPacket);



/******************************************************************************
* Function Name: CyMesh_SecurityFindAppKeyIndex
***************************************************************************//**
* 
*  This function finds whether a given AID value matches with any known 
*  application keys in the security database. All known application key indexes
*  for which a match is found, are provided to the caller.
* 
*  \param aid: The AID value which has to be identified. 
*
*  \param appKeyIndexes: Array of AppKey indexes which match for the AID given. 
*         This array may have zero or more entries. The caller must allocate 
*         sufficient memory for this. Ideal array size is 
*         CYMESH_MAX_APPLICATION_KEYS bytes.
*
*  \return uint8: The number of application keys matched. If zero, signifies 
*          that no match was found.
*  
******************************************************************************/
extern uint8 CyMesh_SecurityFindAppKeyIndex(uint8 aid, uint8 * appKeyIndexes);



/******************************************************************************
* Function Name: CyMesh_SecurityFindMeshId
***************************************************************************//**
* 
*  This function finds whether a given NID value matches with any known mesh
*  network keys in the security database. All known mesh IDs for which a match
*  is found, are provided to the caller.
* 
*  \param nID: Pointer to the NID value which has to be identified. This could
*         be one octet or eight octets, depending on the numberOfBits parameter.
*         This input must be given MSB first.
*
*  \param numberOfBits: 5 or 63 least significant bits to be compared.
*
*  \param meshId: Array of mesh IDs which match for the NID given. This array 
*         may have zero or more entries. The caller must allocate sufficient 
*         memory for this. Ideal array size is CYMESH_MAX_NETWORK_KEYS 
*         bytes.
*
*  \return uint8: The number of mesh networks matched. If zero, signifies that
*          no match was found.
*  
******************************************************************************/
extern uint8 CyMesh_SecurityFindMeshId(const uint8 * nID, uint8 numberOfBits, uint8 * meshId);



/******************************************************************************
* Function Name: CyMesh_SecuritySetIVindex
***************************************************************************//**
* 
*  This function sets the new IVindex value for the mesh network. 
* 
*  \param meshId: Identifies which mesh the IVindex belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param newIVindex: The new value of the IVindex to be taken. 
*
*  \return 
*     None
*  
******************************************************************************/
extern void CyMesh_SecuritySetIVindex(uint8 meshId, uint32 newIVIndex);



/******************************************************************************
* Function Name: CyMesh_SecurityGetIVindex
***************************************************************************//**
* 
*  This function returns the current IVindex value for a mesh network.
* 
*  \param meshId: Identifies which mesh the IVindex belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1). Error check is 
*         not done for a valid mesh network.
*
*  \return uint32: Current IVindex for that mesh network.
*  
******************************************************************************/
extern uint32 CyMesh_SecurityGetIVindex(uint8 meshId);



/******************************************************************************
* Function Name: CyMesh_SecurityGetNetworkId
***************************************************************************//**
* 
*  This function returns the current Network ID value for a mesh network.
*  The value is MSB first (i.e. NetworkID[0] is MSB).
* 
*  \param meshId: Identifies which mesh the IVindex belongs to. The value must 
*         be between 0 and (CYMESH_MAX_NETWORK_KEYS - 1).
*
*  \param netId: Pointer to the network ID which is updated by this function 
*         for the actual value. Must be 128-bit.
*
*  \return CYMESH_API_RETURN_T: Return value indicates if the function 
*          succeeded or failed. Following are the possible error codes.
*  
*  <table>
*  <tr>
*    <th>API Return Type</th>
*    <th>Description</th>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_OK</td>
*    <td>Operation completed successfully.</td>
*  </tr>
*  <tr>
*    <td>CYMESH_ERROR_INVALID_MESH_ID</td>
*    <td>Invalid Mesh ID passed to the API.</td>
*  </tr>
*  </table>
*  
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityGetNetworkId(uint8 meshId, uint8 * netId);


/******************************************************************************
* Function Name: CyMesh_SecurityAesCmacProcess
***************************************************************************//**
* 
*  This function runs the state machine for AES-CMAC operations happening in 
*  the Security module. The state machine is required to make these operations
*  non-blocking.
* 
*  \param None.
*
*  \return None.
*  
******************************************************************************/
extern void CyMesh_SecurityAesCmacProcess(void);

/************* Definition pending ***************/
extern CYMESH_API_RETURN_T CyMesh_SecurityCalculateBeaconAuthValue(uint8 meshId, uint8 krBit, uint32 ivIndex, CYMESH_SECURITY_CALLBACK callback);
extern CYMESH_API_RETURN_T CyMesh_ConfigProvCalcConfirmation(CYMESH_SECURITY_CALLBACK callback);
extern CYMESH_API_RETURN_T CyMesh_ConfigProvCalcRandConfirmation(CYMESH_SECURITY_CALLBACK callback);


#endif      /* #ifndef _CYMESH_SECURITY_H */

/* [] END OF FILE */
