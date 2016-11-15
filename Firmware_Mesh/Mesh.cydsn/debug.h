/***************************************************************************//**
* \file debug.h
* \version 1.0

********************************************************************************
* \copyright
* Copyright 2014-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#if !defined (DEBUG_H)
#define DEBUG_H

#include <project.h>
#include <stdio.h>
#include <cytypes.h>
    
#ifdef CYMESH_DEBUG_ENABLED
    #define DBG_PRINT_TEXT(a)           do\
                                        {\
                                            DBG_PRINTF((a));\
                                        } while (0)

    #define DBG_PRINT_DEC(a)         do\
                                        {\
                                           DBG_PRINTF("%02d ", a);\
                                        } while (0)


    #define DBG_PRINT_HEX(a)         do\
                                        {\
                                           DBG_PRINTF("%08X ", a);\
                                        } while (0)


    #define DBG_PRINT_ARRAY(a,b)     do\
                                        {\
                                            uint32 i;\
                                            \
                                            for(i = 0u; i < (b); i++)\
                                            {\
                                                DBG_PRINTF("%02X ", *(a+i));\
                                            }\
                                        } while (0)

    #define DBG_PRINTF(...)          (printf(__VA_ARGS__))
    
    
#else
	#ifdef CYMESH_DEBUG_ENABLED_ACK_COUNT
    #define DBG_PRINT_TEXT(a)           do\
                                        {\
                                            DBG_PRINTF((a));\
                                        } while (0)

    #define DBG_PRINT_DEC(a)         do\
                                        {\
                                           DBG_PRINTF("%02d ", a);\
                                        } while (0)


    #define DBG_PRINT_HEX(a)         do\
                                        {\
                                           DBG_PRINTF("%08X ", a);\
                                        } while (0)


    #define DBG_PRINT_ARRAY(a,b)     do\
                                        {\
                                            uint32 i;\
                                            \
                                            for(i = 0u; i < (b); i++)\
                                            {\
                                                DBG_PRINTF("%02X ", *(a+i));\
                                            }\
                                        } while (0)

    #define DBG_PRINTF(...)          (printf(__VA_ARGS__))
	#else
    #define DBG_PRINT_TEXT(a)
    #define DBG_PRINT_DEC(a)
    #define DBG_PRINT_HEX(a)
    #define DBG_PRINT_ARRAY(a,b)
    #define DBG_PRINTF(...)
	#endif
#endif /* #ifdef CYMESH_DEBUG_ENABLED */


#endif
/* [] END OF FILE */
