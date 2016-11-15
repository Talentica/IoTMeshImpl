/***************************************************************************//**
* \file CyMesh_Timer.c
* \version 1.0
* 
* \brief
*  This file contains the implementation of the BLE SmartMesh v1 Timer module.
*
********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef _CYMESH_TIMER_H
#define _CYMESH_TIMER_H

/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include <stdbool.h>
#include "CyMesh_Common.h"


/*******************************************************************************
* Compile Time Options
*******************************************************************************/
#define CYMESH_TIMER_NUMBER_OF_CALLBACKS            (4)



/*******************************************************************************
* Data types
*******************************************************************************/
typedef void (* CYMESH_TIMER_CALLBACK_PTR)(void);



/*******************************************************************************
* Data Structures
*******************************************************************************/




/*******************************************************************************
* Globals
*******************************************************************************/

extern volatile uint32 cyMesh_TimerTimestamp;



/*******************************************************************************
* Externed functions
*******************************************************************************/


/******************************************************************************
* Function Name: CyMesh_TimerInit
***************************************************************************//**
* 
*  Starts the timer block running on WDT. Periodic interrupts are generated 
*  as a result, until the timer is stopped. If called more than once (without
*  stopping the timer), the timer period is changed to the new period. This 
*  affects the ongoing period.
* 
*  \param timerPeriod: Sets the period of the timer. A timer interrupt will
*         fire every time this period expires. 
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
*    <td>CYMESH_ERROR_INVALID_PARAMETER</td>
*    <td>The time period provided as parameter is invalid. Possible invalid 
*        values include ZERO. </td>
*  </tr>
*  </table>
* 
* 
******************************************************************************/
extern CYMESH_API_RETURN_T CyMesh_TimerInit(uint32 timerPeriod);



/******************************************************************************
* Function Name: CyMesh_TimerStop
***************************************************************************//**
* 
*  Stops the timer block. All future timer interrupts are removed. 
* 
*  \param None.
*
*  \return None.
*  
******************************************************************************/
extern void CyMesh_TimerStop(void);



/******************************************************************************
* Function Name: CyMesh_TimerGetTimestamp
***************************************************************************//**
* 
*  Gives the current system timestamp. Useful for applications when keeping 
*  track of long time delays (such as a minute or a day etc.).
* 
*  \param None.
*
*  \return uint32: The current system timestamp. The timestamp is reset when
*          the timer is started (not on re-init).
*  
******************************************************************************/
extern uint32 CyMesh_TimerGetTimestamp(void);



/******************************************************************************
* Function Name: CyMesh_TimerRegisterCallback
***************************************************************************//**
* 
*  Registers a callback function. This function is called from the timer ISR
*  when its corresponding time has elapsed. This emulates a configurable timed
*  interrupt for any block in general.
* 
*  \param callbackIndex: The index at which to register a callback. Note that 
*         if a callbackIndex value was already used, that timer callback will
*         be overwritten with the new one. This can also be used to un-register
*         a callback by passing NULL for callbackPtr argument.
*         Valid range is 0 to (CYMESH_TIMER_NUMBER_OF_CALLBACKS - 1).
*  
*  \param callbackPtr: The pointer to the function to be called by the timer 
*         ISR. Any non-NULL value is considered a valid callback pointer. 
*         Passing NULL to this argument results in invalidating that callback 
*         slot.
*  
*  \param periodTicks: The number of timer periods to elapse before triggering
*         this interrupt (hence this number is a multiple of the period). 
*         For example, with a timer period of 10 ms and a callback period of 
*         50 ms, this value would be 5. 
*         Any non-zero value is valid.
*
*  \return None.
*  
******************************************************************************/
extern void CyMesh_TimerRegisterCallback(uint32 callbackIndex, CYMESH_TIMER_CALLBACK_PTR callbackPtr, uint32 periodTicks);



#endif      /* #ifndef _CYMESH_TIMER_H */

/* [] END OF FILE */
