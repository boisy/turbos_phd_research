/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <stdio.h>

#include "riscv-virt.h"
#include "ns16550.h"

/* Priorities used by the tasks. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
 * to ticks using the pdMS_TO_TICKS() macro. */
#define mainQUEUE_SEND_FREQUENCY_MS        pdMS_TO_TICKS( 1000 )

/* The maximum number items the queue can hold.  The priority of the receiving
 * task is above the priority of the sending task, so the receiving task will
 * preempt the sending task and remove the queue items each time the sending task
 * writes to the queue.  Therefore the queue will never have more than one item in
 * it at any time, and even with a queue length of 1, the sending task will never
 * find the queue full. */
#define mainQUEUE_LENGTH                   ( 1 )

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

#define FREERTOS 1
#ifdef USE_EXEMPLARS
#include "../../../../phd/source/testbench/rlenc.c"
#include "../../../../phd/source/testbench/rldec.c"
#include "../../../../phd/source/testbench/aclamp.c"

static void prv_rlenc_task( void * pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ; ; )
    {
    	rlenc((unsigned char *)0x01, (unsigned char *)0x01, 1);
    }
}

static void prv_rldec_task( void * pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ; ; )
    {
    	rldec((unsigned char *)0x01, (unsigned char *)0x01, 1);
    }
}

static void prv_aclamp_task( void * pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ; ; )
    {
    	aclamp((unsigned char *)0x01, (unsigned char *)0x01, 1, 0x0a, 0x01);
    }
}
#endif

/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void * pvParameters )
{
    unsigned long ulReceivedValue;
    const unsigned long ulExpectedValue = 100UL;
    const char * const pcMessage1 = "Blink1";
    const char * const pcMessage2 = "Blink2";
    const char * const pcFailMessage = "Unexpected value received\r\n";
    int f = 1;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ; ; )
    {
        char buf[ 40 ];

        /* Wait until something arrives in the queue - this task will block
         * indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
         * FreeRTOSConfig.h. */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        /*  To get here something must have been received from the queue, but
         * is it the expected value?  If it is, toggle the LED. */
        if( ulReceivedValue == ulExpectedValue )
        {
            f = !f;

            ulReceivedValue = 0U;
        }
        else
        {
        }
    }
}

/*-----------------------------------------------------------*/

int main_exemplars( void )
{
    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
#ifdef USE_EXEMPLARS
        /* Start the two tasks as described in the comments at the top of this
         * file. */
        xTaskCreate( prv_rlenc_task, "rlenc", configMINIMAL_STACK_SIZE * 2U, NULL,
                     mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
        xTaskCreate( prv_rldec_task, "rldec", configMINIMAL_STACK_SIZE * 2U, NULL,
                     mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
        xTaskCreate( prv_aclamp_task, "aclamp", configMINIMAL_STACK_SIZE * 2U, NULL,
                     mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
#endif
    }

    vTaskStartScheduler();

    return 0;
}

void *memset(void *b, int c, size_t len) {
	return NULL;
}

void *memcpy(void *restrict dst, const void *restrict src, size_t n) {
}

int printf(const char * restrict format, ...) {
}

unsigned int __clzsi2(unsigned int x) {
}
