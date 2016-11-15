/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#if !defined (CYMESH_NETWORK_H)
#define CYMESH_NETWORK_H

/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include "CyMesh_Common.h"

/******************************************************************/

/***********************Macros************************************/
#define CYMESH_NET_MSG_CACHE_SIZE					(0x0A)	/* Supporting caching of 10 messages */
#define CYMESH_NET_MSG_CACHE_CONTENT_SIZE			(0x05)
	
#define CYMESH_TRANS_MIN_DATA_LEN					(0x0D)	/*PFT+SEQ+SRC+DST+1 OPCODE+4 MICapp */
#define CYMESH_TRANS_MAX_DATA_LEN					(0x1D)//(0x18)	/* Max 31 bytes ADV - 2 bytes (Len+Mesh tag) - MICnet - IVNID */

#define CYMESH_NET_MAX_NO_OF_FRIENDS				0x03
#define CYMESH_NET_FRIEND_CACHE_SIZE				0x05
	
#define CYMESH_NET_MAX_DATA_LEN						(0x1D)	/* Max 31 bytes ADV - 2 bytes (Len+Mesh tag) */
   
/*VAVC*/
//#define CYMESH_DEBUG_ENABLED_N    
/*******************************************************************************
* Structures and Enums
*******************************************************************************/	
typedef struct
{
	uint8 msg[CYMESH_NET_MSG_CACHE_SIZE][CYMESH_NET_MSG_CACHE_CONTENT_SIZE];
	uint8 access_count[CYMESH_NET_MSG_CACHE_SIZE];
	uint8 cache_filled;

} CYMESH_NET_MSG_CACHE_STRUCT;

typedef struct
{
    uint8 * data; 
    uint8 length;
	uint8 mesh_id;
    uint8 componentIndex;
    uint8 modelIndex;
} CYMESH_NETWORK_PKT_T;

typedef enum
{
	CYMESH_TRANSPORT_PKT = 0x01
} CYMESH_NETWORK_CALLBACK_T;

#if (CYMESH_ENABLE_FRIENDSHIP == 1)
typedef struct
{
    uint8 cache[CYMESH_NET_FRIEND_CACHE_SIZE][CYMESH_NET_MAX_DATA_LEN]; 
	uint8 len[CYMESH_NET_FRIEND_CACHE_SIZE];
	uint8 cache_write_pointer;
	uint8 cache_read_pointer;
	uint16 addr;
    uint8 isOccupied;
} CYMESH_NETWORK_FRIENDSHIP_CACHE_T;
#endif

/*******************************************************************************
* Externed functions
*******************************************************************************/
/******************************************************************************
* Function Name: CyMesh_NetworkStart
*******************************************************************************
* 
*  This function starts the network layer and initilizes required variables.
* 
*  \param CYMESH_CALLBACK_T: function which network layer will call to send
*							the packet to upper layer.
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if network layer started correctly
*								other values in case of failures
* 
******************************************************************************/
CYMESH_API_RETURN_T CyMesh_NetworkStart(CYMESH_CALLBACK_T);	

/******************************************************************************
* Function Name: CyMesh_ProcessNetworkPacket
*******************************************************************************
* 
*  This function processes packet sent to the network layer from bottom layer.
* This function performs network layer clarification, decryption, address 
* resolution, message caching and relaying.
* 
*  \param uint8*: pointer to the mesh packet that will be processed
*
*  \param uint8: length of the mesh packet
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if network layer started correctly
*								other values in case of failures
* 
******************************************************************************/
CYMESH_API_RETURN_T CyMesh_ProcessNetworkPacket(uint8* , uint8 );

/******************************************************************************
* Function Name: CyMesh_NetworkSendData
*******************************************************************************
* 
*  This function pushes the data from top layer to network layer to be sent as 
* a mesh packet. This function performs network layer encryption, obfuscation
* and setting correct headers.
* 
*  \param uint8*: pointer to the transport layer data
*
*  \param uint8: length of the tranpsort layer packet
*
*  \param uint8: txCount indicating the number of times bearer layer will send 
*				this same packet
*
*  \param bool: one bit value indicating whether Application key was used or not 
*
*  \param bool: future bit. Set from the application layer
*
*  \param uint8: A 6-bit value indicating the Time-To-Live in the packet
*
*  \param uint8: Mesh id containing the network and application key to be used
*
*  \param uint8: One bit flag indicating if IVINID header has to be attached or 
*					not
*
*  \return CYMESH_API_RETURN_T: CYMESH_ERROR_OK if network layer started correctly
*								other values in case of failures
* 
******************************************************************************/
CYMESH_API_RETURN_T CyMesh_NetworkSendData(uint8 *, 
											uint8 , 
											uint8 , 
											bool , 
											bool , 
											uint8 , 
											uint8,
											uint8);

#if (CYMESH_ENABLE_FRIENDSHIP == 1)
uint8 CyMesh_NetworkIsFriendshipCacheAvailable(void);
CYMESH_API_RETURN_T CyMesh_NetworkAddFriend(uint16 );
uint8 CyMesh_NetworkIsFriend(uint16);
CYMESH_API_RETURN_T CyMesh_NetNumberOfMsgInFriendCache(uint16 addr, uint8 * numOfMsg);
CYMESH_API_RETURN_T CyMesh_NetSendMsgsInFriendCache(uint16 addr);
#endif

#endif
/* [] END OF FILE */
