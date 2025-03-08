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

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void prvOneShotTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	printf("One-shot timer callback executing=%d\r\n",xTimeNow);
}

static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	printf("Auto-reload timer callback executing=%d\r\n",xTimeNow);
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
    printf("software timer demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Software timers are used to schedule the execution of a function at a set time in the future,
     * or periodically with a fixed frequency. The function executed by the software timer is called
     * the software timer’s callback function. */

    /* Software timers are implemented by, and are under the control of, the FreeRTOS kernel. They do
     * not require hardware support, and are not related to hardware timers or hardware counters. */

    /* Note that, in line with the FreeRTOS philosophy of using innovative design to ensure maximum
     * efficiency, software timers do not use any processing time unless a software timer callback
     * function is actually executing. */

    /* To include software timer functionality:
     * 1. Build the FreeRTOS source file FreeRTOS/Source/timers.c as part of your project.
     * 2. Set configUSE_TIMERS to 1 in FreeRTOSConfig.h. */

    /* Software timer callback functions are implemented as C functions. The only thing special about
     * them is their prototype, which must return void, and take a handle to a software timer as its
     * only parameter. */

    /* Software timer callback functions execute from start to finish, and exit in the normal way.
     * They should be kept short, and must not enter the Blocked state. */

    /* Note: As will be seen, software timer callback functions execute in the context of a task that
     * is created automatically when the FreeRTOS scheduler is started. Therefore, it is essential
     * that software timer callback functions never call FreeRTOS API functions that will result in
     * the calling task entering the Blocked state. It is ok to call functions such as
     * xQueueReceive(), but only if the function’s xTicksToWait parameter (which specifies the
     * function’s block time) is set to 0. It is not ok to call functions such as vTaskDelay(), as
     * calling vTaskDelay() will always place the calling task into the Blocked state. */

    /* A software timer’s ‘period’ is the time between the software timer being started, and the
     * software timer’s callback function executing. */

    /* There are two types of software timer:
     * 1. One-shot timers: Once started, a one-shot timer will execute its callback function once
     * only. A one-shot timer can be restarted manually, but will not restart itself.
     * 2. Auto-reload timers: Once started, an auto-reload timer will re-start itself each time it
     * expires, resulting in periodic execution of its callback function. */

    /* Figure 38 The difference in behavior between one-shot and auto-reload software timers */

    /* A software timer can be in one of the following two states:
     * - Dormant: A Dormant software timer exists, and can be referenced by its handle, but is not
     * running, so its callback functions will not execute.
     * - Running: A Running software timer will execute its callback function after a time equal to
     * its period has elapsed since the software timer entered the Running state, or since the
     * software timer was last reset. */

    /* Figure 39 and Figure 40 show the possible transitions between the Dormant and Running states
     * for an auto-reload timer and a one-shot timer respectively. The key difference between the two
     * diagrams is the state entered after the timer has expired; the auto-reload timer executes it
     * callback function then re-enters the Running state, the one-shot timer executes its callback
     * function then enters the Dormant state. */

    /* The xTimerDelete() API function deletes a timer. A timer can be deleted at any time. */

    /* All software timer callback functions execute in the context of the same RTOS daemon (or
     * ‘timer service’) task. The task used to be called the ‘timer service task’, because originally
     * it was only used to execute software timer callback functions. Now the same task is used for
     * other purposes too, so it is known by the more generic name of the ‘RTOS daemon task’. */

    /* The daemon task is a standard FreeRTOS task that is created automatically when the scheduler
     * is started. Its priority and stack size are set by the configTIMER_TASK_PRIORITY and
     * configTIMER_TASK_STACK_DEPTH compile time configuration constants respectively. Both constants
     * are defined within FreeRTOSConfig.h. */

    /* Software timer callback functions must not call FreeRTOS API functions that will result in the
     * calling task entering the Blocked state, as to do so will result in the daemon task entering
     * the Blocked state. */

    /* Software timer API functions send commands from the calling task to the daemon task on a queue
     * called the ‘timer command queue’. This is shown in Figure 41. Examples of commands include
     * ‘start a timer’, ‘stop a timer’ and ‘reset a timer’. The timer command queue is a standard
     * FreeRTOS queue that is created automatically when the scheduler is started. The length of the
     * timer command queue is set by the configTIMER_QUEUE_LENGTH compile time configuration
     * constant in FreeRTOSConfig.h. ( Figure 41 The timer command queue being used by a software
     * timer API function to communicate with the RTOS daemon task ) */

    /* The daemon task is scheduled like any other FreeRTOS task; it will only process commands, or
     * execute timer callback functions, when it is the highest priority task that is able to run.
     * Figure 42 and Figure 43 demonstrate how the configTIMER_TASK_PRIORITY setting affects the
     * execution pattern.
     */

    /* Important: read 181-184 */

    /* The time at which the software timer being started will expire is calculated from the time the
     * ‘start a timer’ command was sent to the timer command queue—it is not calculated from the
     * time the daemon task received the ‘start a timer’ command from the timer command queue. */

    /* Commands sent to the timer command queue contain a time stamp. The time stamp is used to
     * account for any time that passes between a command being sent by an application task, and the
     * same command being processed by the daemon task. For example, if a ‘start a timer’ command is
     * sent to start a timer that has a period of 10 ticks, the time stamp is used to ensure the
     * timer being started expires 10 ticks after the command was sent, not 10 ticks after the
     * command was processed by the daemon task. */

    /* A software timer must be explicitly created before it can be used. Software timers are
     * referenced by variables of type TimerHandle_t. xTimerCreate() is used to create a software
     * timer and returns a TimerHandle_t to reference the software timer it creates. Software timers
     * are created in the Dormant state. Software timers can be created before the scheduler is
     * running, or from a task after the scheduler has been started. */

    /* xTimerStart() is used to start a software timer that is in the Dormant state, or reset
     * (re-start) a software timer that is in the Running state. xTimerStop() is used to stop a
     * software timer that is in the Running state. Stopping a software timer is the same as
     * transitioning the timer into the Dormant state. xTimerStart() can be called before the
     * scheduler is started, but when this is done, the software timer will not actually start until
     * the time at which the scheduler starts. */

    /* Never call xTimerStart() from an interrupt service routine. The interrupt-safe version
     * xTimerStartFromISR() should be used in its place. */

	TimerHandle_t xAutoReloadTimer, xOneShotTimer;
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
		 prvOneShotTimerCallback );

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
		 prvAutoReloadTimerCallback );

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
