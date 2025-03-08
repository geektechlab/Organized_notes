/******************************************************************************
* File Name: main.c
*
* Description: This code example demonstrates displaying graphics on an E-Ink
* display using EmWin graphics library and the AppWizard GUI design tool.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2021-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ          (100000000u)

/* The periods assigned to the one-shot and auto-reload timers are 3.333 second and half a second
 * respectively. */
#define mainONE_SHOT_TIMER_PERIOD    pdMS_TO_TICKS( 3333 )
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 500 )

TimerHandle_t xAutoReloadTimer, xOneShotTimer;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void prvTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow;
	uint32_t ulExecutionCount;

	/* A count of the number of times this software timer has expired is stored in the timer's
	ID. Obtain the ID, increment it, then save it as the new ID value. The ID is a void
	pointer, so is cast to a uint32_t. */
	ulExecutionCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
	ulExecutionCount++;
	vTimerSetTimerID( xTimer, ( void * ) ulExecutionCount );

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* The handle of the one-shot timer was stored in xOneShotTimer when the timer was created.
	Compare the handle passed into this function with xOneShotTimer to determine if it was the
	one-shot or auto-reload timer that expired, then output a string to show the time at which
	the callback was executed. */
	if( xTimer == xOneShotTimer )
	{
		printf("One-shot timer callback executing=%d\r\n",xTimeNow);
	}
	else
	{
		/* xTimer did not equal xOneShotTimer, so it must have been the auto-reload timer that
		expired. */
		printf("Auto-reload timer callback executing=%d\r\n",xTimeNow);

		if( ulExecutionCount == 3 )
		{
			/* Stop the auto-reload timer after it has executed 3 times. This callback function
			executes in the context of the RTOS daemon task so must not call any functions that
			might place the daemon task into the Blocked state. Therefore a block time of 0 is
			used. */

			/* This will stop only auto-reload timer. One shot timer will still run */
			xTimerStop( xTimer, 0 );
		}
	}
}

void vApplicationIdleHook(void)
{
	//printf("idle task\r\n");
}

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;

/*******************************************************************************
* Function Name: int main(void)
********************************************************************************
*
* Summary: This is the main for this code example.  This function initializes
*          the BSP, creates the eInk_task and starts the scheduler.
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result ;
#if defined (CY_DEVICE_SECURE)
    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* To avoid compiler warning */
    (void)result;

    /* Enable global interrupts. */
    __enable_irq();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("**********************************************************\r\n");
    printf("software timer id demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Each software timer has an ID, which is a tag value that can be used by the application writer
     * for any purpose. The ID is stored in a void pointer (void *), so can store an integer value
     * directly, point to any other object, or be used as a function pointer. An initial value is
     * assigned to the ID when the software timer is created—after which the ID can be updated using
     * the vTimerSetTimerID() API function, and queried using the pvTimerGetTimerID() API function. */

    /* Unlike other software timer API functions, vTimerSetTimerID() and pvTimerGetTimerID() access
     * the software timer directly—they do not send a command to the timer command queue. */

    /* The same callback function can be assigned to more than one software timer. When that is done,
     * the callback function parameter is used to determine which software timer expired. */

    /* Previous example used two separate callback functions; one callback function was used by the
     * oneshot timer, and the other callback function was used by the auto-reload timer. This example
     * creates similar functionality to that created by previous example, but assigns a single
     * callback function to both software timers. */

    /* The main() function used by this example is almost identical to the main() function used in
     * previous example. The only difference is where the software timers are created, where
     * prvTimerCallback() is used as the callback function for both timers. */

    /* prvTimerCallback() will execute when either timer expires. The implementation of
     * prvTimerCallback() uses the function’s parameter to determine if it was called because the
     * one-shot timer expired, or because the auto-reload timer expired. prvTimerCallback() also
     * demonstrates how to use the software timer ID as timer specific storage; each software timer
     * keeps a count of the number of times it has expired in its own ID, and the auto-reload timer
     * uses the count to stop itself the fifth time it executes. */

	BaseType_t xTimer1Started, xTimer2Started;

	/* Create the one shot timer, storing the handle to the created timer in xOneShotTimer. */
	 xOneShotTimer = xTimerCreate(
		 /* Text name for the software timer - not used by FreeRTOS. */
		 "OneShot",
		/* The software timer's period in ticks. */
		mainONE_SHOT_TIMER_PERIOD,
		 /* Setting uxAutoRealod to pdFALSE creates a one-shot software timer. */
		 pdFALSE,
		/* This example does not use the timer id. */
		0,
		/* The callback function to be used by the software timer being created. */
		prvTimerCallback );

	 /* Create the auto-reload timer, storing the handle to the created timer in xAutoReloadTimer. */
	 xAutoReloadTimer = xTimerCreate(
		 /* Text name for the software timer - not used by FreeRTOS. */
		 "AutoReload",
		/* The software timer's period in ticks. */
		mainAUTO_RELOAD_TIMER_PERIOD,
		/* Setting uxAutoRealod to pdTRUE creates an auto-reload timer. */
		 pdTRUE,
		/* This example does not use the timer id. */
		0,
		/* The callback function to be used by the software timer being created. */
		prvTimerCallback );

	 /* Check the software timers were created. */
	 if( ( xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) )
	 {
		 /* Start the software timers, using a block time of 0 (no block time). The scheduler has
		 not been started yet so any block time specified here would be ignored anyway. */
    	if (xTimerStart( xOneShotTimer, 0 ) == pdPASS)
		{
			if ( xTimerStart( xAutoReloadTimer, 0 ) == pdPASS)
			{
					/* Start the FreeRTOS scheduler. */
					vTaskStartScheduler();
			}
		}
	}

	/* /* If all is well then main() will never reach here as the scheduler will now be running the
	 * tasks. If main() does reach here then it is likely that there was insufficient FreeRTOS heap
	 * memory available for the idle task to be created. Chapter 2 provides more information on heap
	 * memory management. */

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
