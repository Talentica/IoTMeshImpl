/***************************************************************************//**
* \file CyMesh_SecurityPVT.h
* \version 1.0
* 
* \brief
*  This is the header file for the private implementation of the BLE SmartMesh v1
*  Security module.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYMESH_SECURITY_PVT_H
#define _CYMESH_SECURITY_PVT_H


/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include "CyMesh_Common.h"

#define CYMESH_SECURITY_CCM_BLOCK_SIZE          (16)
#define CYMESH_SECURITY_CCM_BLOCK_M_4            (4)
#define CYMESH_SECURITY_CCM_BLOCK_M_8            (8)
/*******************************************************************************
* Externed functions 
*******************************************************************************/
/******************************************************************************
* Function Name: CyMesh_SecurityPVTSwapMsbLsb
*******************************************************************************
* 
*  This function swaps the bytes in array between little-endian and big-endian.
* 
*  \param uint8*: pointer to the array containing the elements.
*
*  \param uint8: length of the array.
*
*  \return none:
*  
******************************************************************************/
extern void CyMesh_SecurityPVTSwapMsbLsb(uint8 *bytes, uint8 len);

/******************************************************************************
* Function Name: CyMesh_SecurityPVTAesEncrypt
*******************************************************************************
* 
*  This function triggers the hardware AES ECB block.
* 
*  \param const uint8*: Input array to the AES block. The size is assumed to be 
*						16 bytes
*
*  \param const uint8*: Key to be used in the AES block. The size is assumed to  
*						be 16 bytes.
*
*  \param uint8*: Output array from the AES block once the AES processing is done.
*					The size is assumed to be 16 bytes.
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if network layer started correctly
*								other values in case of failures
*  
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityPVTAesEncrypt(const uint8 * input, const uint8 * key, uint8 * output);

/******************************************************************************
* Function Name: CyMesh_SecurityAesCcmEncryption
*******************************************************************************
* 
*  This function starts the AES-CCM Encryption using the input data.
* 
*  \param const uint8*: pointer to a 16-byte array containing the security key
*
*  \param const uint8*: pointer to a 13-byte array conatining the nonce
*
*  \param const uint8*: A multi-byte array containing the plaintext
*
*  \param uint8: length of the plaintext
*
*  \param const uint8*: A multi-byte array conbtaining the additional data, if any
*						Else it is NULL.
*
*  \param uint8: legth of the additional data, if any. Else it is 0.
*
*  \param uint8*: Pointer to the multi-byte array cwhere the output of AES-CCM 
*					will be stored. The size is same as the plaintext length
*
*  \param uint8*: Pointer to an array where the generated MIC will be stored.
*
*  \param uint8: Length of the MIC generated.
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if network layer started correctly
*								other values in case of failures
*  
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityAesCcmEncryption(
    const uint8 * key, 
    const uint8 * nonce, 
    const uint8 * payload, 
    uint8 payloadLength, 
    const uint8 * additionalData, 
    uint8 additionalDataLength,
    uint8 * outputData,
    uint8 * mic,
	uint8 micLen);

/******************************************************************************
* Function Name: CyMesh_SecurityAesCcmDecryption
*******************************************************************************
* 
*  This function starts the AES-CCM Decryption using the input data.
* 
*  \param const uint8*: pointer to a 16-byte array containing the security key
*
*  \param const uint8*: pointer to a 13-byte array conatining the nonce
*
*  \param const uint8*: A multi-byte array containing the ciphertext
*
*  \param uint8: length of the ciphertext
*
*  \param const uint8*: A multi-byte array conbtaining the additional data, if any
*						Else it is NULL.
*
*  \param uint8: legth of the additional data, if any. Else it is 0.
*
*  \param uint8*: Pointer to the multi-byte array where the output of AES-CCM 
*					will be stored. The size is same as the ciphertext length
*
*  \param uint8*: Pointer to the array where the MIC is stored. This is used to 
*					check ifthe decryption was successfull or not.
*
*  \param uint8: Length of the MIC in the input.
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if network layer started correctly
*								other values in case of failures
*  
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_SecurityAesCcmDecryption(
    const uint8 * key,
    const uint8 * nonce,
    const uint8 * encPayload,
    uint8 encPayloadLength,
    const uint8 * additionalData,
    uint8 additionalDataLength,
    uint8 *outputData,
    const uint8 * encMic,
	uint8 micLen);


#endif      /* #ifndef _CYMESH_SECURITY_PVT_H */

/* [] END OF FILE */
