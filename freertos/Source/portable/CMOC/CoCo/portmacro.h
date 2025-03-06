/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

 #define portPOINTER_SIZE_TYPE uint16_t

typedef char int8_t;
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	uint8_t
#define portBASE_TYPE	char

typedef portSTACK_TYPE StackType_t;
typedef signed char BaseType_t;
typedef unsigned char UBaseType_t;


#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

/* Hardware specifics. */
#define portBYTE_ALIGNMENT			1
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portYIELD()					asm { swi }
/*-----------------------------------------------------------*/

/* Critical section handling. */
#define portENABLE_INTERRUPTS()				asm { andcc #^$50 }
#define portDISABLE_INTERRUPTS()			asm { orcc #$50 }

/*
 * Disable interrupts before incrementing the count of critical section nesting.
 * The nesting count is maintained so we know when interrupts should be
 * re-enabled.  Once interrupts are disabled the nesting count can be accessed
 * directly.  Each task maintains its own nesting count.
 */
#define portENTER_CRITICAL()  									\
{																\
	extern volatile UBaseType_t uxCriticalNesting;	\
																\
	portDISABLE_INTERRUPTS();									\
	uxCriticalNesting++;										\
}

/*
 * Interrupts are disabled so we can access the nesting count directly.  If the
 * nesting is found to be 0 (no nesting) then we are leaving the critical
 * section and interrupts can be re-enabled.
 */
#define  portEXIT_CRITICAL()									\
{																\
	extern volatile UBaseType_t uxCriticalNesting;	\
																\
	uxCriticalNesting--;										\
	if( uxCriticalNesting == 0 )								\
	{															\
		portENABLE_INTERRUPTS();								\
	}															\
}
/*-----------------------------------------------------------*/

/* Task utilities. */

/*
 * These macros are very simple as the processor automatically saves and
 * restores its registers as interrupts are entered and exited.  In
 * addition to the (automatically stacked) registers we also stack the
 * critical nesting count.  Each task maintains its own critical nesting
 * count as it is legitimate for a task to yield from within a critical
 * section.  If the banked memory model is being used then the PPAGE
 * register is also stored as part of the tasks context.
 */

	/*
	 * These macros are as per the BANKED versions above, but without saving
	 * and restoring the PPAGE register.
	 */
    #define portRESTORE_CONTEXT()                                   \
    {                                                               \
        extern volatile void * pxCurrentTCB;                        \
        extern volatile UBaseType_t uxCriticalNesting;  \
                                                                    \
        asm { ldx pxCurrentTCB } \
        asm { lds 0,x } \
        asm { puls a } \
        asm { sta uxCriticalNesting } \
    }

    #define portSAVE_CONTEXT()                                      \
    {                                                               \
        extern volatile void * pxCurrentTCB;                        \
        extern volatile UBaseType_t uxCriticalNesting;  \
                                                                    \
        asm { lda uxCriticalNesting } \
        asm { pshs a } \
        asm { ldx pxCurrentTCB   } \
        asm { sts 0,x } \
    }

#define dumpRegs() { \
    uint16_t *t = (uint16_t *)pxCurrentTCB; \
    uint8_t *s = t[11]; \
    printf("PC = %02X%02X\n", *(s++), *(s++)); \
    }

#define dumpRegs2() { \
    uint16_t *t = (uint16_t *)pxCurrentTCB; \
    printf("TCB Address = %04X\n", t); \
    printf("S = %04X\n", t[0]); \
    uint8_t *s = t[0]; \
    printf("A   = %02X\n", *(s++)); \
    printf("CC  = %02X, DP = %02X\n", *(s++), *(s++)); \
    printf("D  = %02X%02X\n", *(s++), *(s++)); \
    printf("X  = %02X%02X\n", *(s++), *(s++)); \
    printf("Y  = %02X%02X\n", *(s++), *(s++)); \
    printf("U  = %02X%02X\n", *(s++), *(s++)); \
    char *addr = *s * 256 + *(s+1); \
    printf("PC = %02X%02X\n", *(s++), *(s++)); \
    printf("ADDR = %04X\n", addr); \
    }

/*
 * Utility macros to save/restore correct software registers for GCC. This is
 * useful when GCC does not generate appropriate ISR head/tail code.
 */
#define portISR_HEAD()									\
{											\
		asm {									\
		}									\
}

#define portISR_TAIL()									\
{											\
                    asm {	rti								\
                    }									\
}

/*
 * Utility macro to call macros above in correct order in order to perform a
 * task switch from within a standard ISR.  This macro can only be used if
 * the ISR does not use any local (stack) variables.  If the ISR uses stack
 * variables portYIELD() should be used in it's place.
 */

#define portTASK_SWITCH_FROM_ISR()								\
	portSAVE_CONTEXT();											\
	vTaskSwitchContext();										\
	portRESTORE_CONTEXT();


/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

