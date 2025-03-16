
/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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


/*
 * main() creates all the application tasks, then starts the scheduler.
 */

/* Kernel includes. */
#include <cmoc.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cpu.h"

/*-----------------------------------------------------------
	Definitions.
-----------------------------------------------------------*/

/* Exemplars to launch */
#define LAUNCH_RLENC
#define LAUNCH_RLDEC
#define LAUNCH_ACLAMP

/*-----------------------------------------------------------*/

void rlenc_freertos(void *parameters);
void rldec_freertos(void *parameters);
void aclamp_freertos(void *parameters);

/* This is called from startup. */
int ATTR_BANK0 main ( void )
{
    BaseType_t ret;

#ifdef LAUNCH_RLENC
	TaskHandle_t t1;

	/* Start some of the standard demo tasks. */
    ret = xTaskCreate(   rlenc_freertos,
                                    "RLENC_TASK",
                                    128,
                                    NULL,
                                    4,
                                    ( TaskHandle_t * ) &t1 );
#endif
                                    
#ifdef LAUNCH_RLDEC
	TaskHandle_t t2;

    ret = xTaskCreate(   rldec_freertos,
                                    "RLDEC_TASK",
                                    128,
                                    NULL,
                                    4,
                                    ( TaskHandle_t * ) &t2 );
#endif
                                    
#ifdef LAUNCH_ACLAMP
	TaskHandle_t t3;

    ret = xTaskCreate(   aclamp_freertos,
                                    "ACLAMP_TASK",
                                    128,
                                    NULL,
                                    4,
                                    ( TaskHandle_t * ) &t3 );
#endif                                    

	vTaskStartScheduler();
          
	/* Should not reach here! */
	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/
