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
#define original_PERIOD    pdMS_TO_TICKS( 500 )

TimerHandle_t xBacklightTimer;

static volatile uint32_t count=0;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void prvBacklightTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow = xTaskGetTickCount();

	/* Print the time at which the backlight was turned off. */
	printf("Timer expired, turning backlight OFF at time=%d\r\n",xTimeNow);
	count=0;
}

static void vResetTask( void *pvParameters )
{
	const TickType_t xShortDelay = pdMS_TO_TICKS( 50 );
	TickType_t xTimeNow;

	while(1)
	{
		xTimeNow = xTaskGetTickCount();

		/* Reset the software timer. If the backlight was previously off, then this call
		will start the timer. If the backlight was previously on, then this call will
		restart the timer. A real application may read key presses in an interrupt. If
		this function was an interrupt service routine then xTimerResetFromISR() must be
		used instead of xTimerReset(). */

		if(count<6)
		{
			printf("Resetting timer=%d\r\n",xTimeNow);
			xTimerReset(xBacklightTimer,xShortDelay);
		}
		vTaskDelay(200);
		count++;
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

    /* Read pages 208 -  */

    /* More details:
     * - https://forums.freertos.org/t/why-do-we-really-need-to-use-safe-versions-of-apis-inside-interrupts/12506
     * - https://www.freertos.org/FreeRTOS_Support_Forum_Archive/March_2017/freertos_How_thread-safe_are_fromISR_functions_204737b7j.html
     * - https://forums.freertos.org/t/understanding-priority-levels-of-isr-and-freertos-apis/10453/5
     * - https://www.freertos.org/RTOS-Cortex-M3-M4.html?_ga=2.159453371.741289969.1699963902-2066404190.1699963902
     */

    /* Create the auto-reload timer, storing the handle to the created timer in xAutoReloadTimer. */
    xBacklightTimer = xTimerCreate(
		 /* Text name for the software timer - not used by FreeRTOS. */
		 "AutoReload",
		/* The software timer's period in ticks. */
		 original_PERIOD,
		/* Setting uxAutoRealod to pdTRUE creates an auto-reload timer. */
		 pdTRUE,
		/* This example does not use the timer id. */
		0,
		/* The callback function to be used by the software timer being created. */
		prvBacklightTimerCallback );

	 /* Check the software timers were created. */
	 if( xBacklightTimer != NULL )
	 {
		 /* Start the software timers, using a block time of 0 (no block time). The scheduler has
		 not been started yet so any block time specified here would be ignored anyway. */
		if ( xTimerStart( xBacklightTimer, 0 ) == pdPASS)
		{
	    	if (xTaskCreate( vResetTask, "Reset taks", 1000, NULL, 4, NULL ) == pdPASS)
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
