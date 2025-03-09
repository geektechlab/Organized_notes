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
#define original_PERIOD    pdMS_TO_TICKS( 3333 )
#define updated_PERIOD     pdMS_TO_TICKS( 500 )

TimerHandle_t xAutoReloadTimer;

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

	/* xTimer did not equal xOneShotTimer, so it must have been the auto-reload timer that
	expired. */
	printf("Auto-reload timer callback executing=%d\r\n",xTimeNow);

	if( ulExecutionCount == 3 )
	{
		/* Update timer period after it has executed 3 times. This callback function
		executes in the context of the RTOS daemon task so must not call any functions that
		might place the daemon task into the Blocked state. Therefore a block time of 0 is
		used. */

		/*This callback function is executing in the context of the RTOS daemon task, so a block time
		 * of 0 is used to ensure the Daemon task never enters the Blocked state. */

		printf("\r\nGoing to update period\r\n");
		 xTimerChangePeriod( xTimer, /* The timer being updated. */
				 	 	 	 updated_PERIOD, /* The new period for the timer. */
							 0 );
		printf("Updated period\r\n\r\n");
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
    printf("software timer period change demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* The period of a software timer is changed using the xTimerChangePeriod() function. If
     * xTimerChangePeriod() is used to change the period of a timer that is already running, then the
     * timer will use the new period value to recalculate its expiry time. The recalculated expiry
     * time is relative to when xTimerChangePeriod() was called, not relative to when the timer was
     * originally started. */

    /* If xTimerChangePeriod() is used to change the period of a timer that is in the Dormant state
     * (a timer that is not running), then the timer will calculate an expiry time, and transition to
     * the Running state (the timer will start running). */

    /* Note: Never call xTimerChangePeriod() from an interrupt service routine. The interrupt-safe
     * version xTimerChangePeriodFromISR() should be used in its place. */

	 /* Create the auto-reload timer, storing the handle to the created timer in xAutoReloadTimer. */
	 xAutoReloadTimer = xTimerCreate(
		 /* Text name for the software timer - not used by FreeRTOS. */
		 "AutoReload",
		/* The software timer's period in ticks. */
		 original_PERIOD,
		/* Setting uxAutoRealod to pdTRUE creates an auto-reload timer. */
		 pdTRUE,
		/* This example does not use the timer id. */
		0,
		/* The callback function to be used by the software timer being created. */
		prvTimerCallback );

	 /* Check the software timers were created. */
	 if( xAutoReloadTimer != NULL )
	 {
		 /* Start the software timers, using a block time of 0 (no block time). The scheduler has
		 not been started yet so any block time specified here would be ignored anyway. */
		if ( xTimerStart( xAutoReloadTimer, 0 ) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
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
