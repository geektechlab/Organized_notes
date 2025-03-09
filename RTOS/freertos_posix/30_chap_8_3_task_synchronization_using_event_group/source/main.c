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
#include "event_groups.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ          (100000000u)

/* Definitions for the event bits in the event group. */
#define mainFIRST_TASK_BIT  ( 1UL << 0UL ) /* Event bit 0, set by the first task. */
#define mainSECOND_TASK_BIT ( 1UL << 1UL ) /* Event bit 1, set by the second task. */
#define mainTHIRD_TASK_BIT  ( 1UL << 2UL ) /* Event bit 2, set by the third task. */

const uint32_t first_task_bit = mainFIRST_TASK_BIT;
const uint32_t second_task_bit = mainSECOND_TASK_BIT;
const uint32_t third_task_bit = mainTHIRD_TASK_BIT;

/* Declare the event group used to synchronize the three tasks. */
EventGroupHandle_t xEventGroup;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void vSyncingTask( void *pvParameters )
{
	const TickType_t xMaxDelay = pdMS_TO_TICKS( 4000UL );
	const TickType_t xMinDelay = pdMS_TO_TICKS( 200UL );

	TickType_t xDelayTime;
	EventBits_t uxThisTasksSyncBit;

	const EventBits_t uxAllSyncBits = ( mainFIRST_TASK_BIT | mainSECOND_TASK_BIT | mainTHIRD_TASK_BIT );

	/* Three instances of this task are created - each task uses a different event
	bit in the synchronization. The event bit to use is passed into each task
	instance using the task parameter. Store it in the uxThisTasksSyncBit
	variable. */
	uxThisTasksSyncBit = ( EventBits_t ) pvParameters;

	for( ;; )
	{
		/* Simulate this task taking some time to perform an action by delaying for a
		pseudo random time. This prevents all three instances of this task reaching
		the synchronization point at the same time, and so allows the example’s
		behavior to be observed more easily. */
		xDelayTime = ( rand() % xMaxDelay ) + xMinDelay;
		vTaskDelay( xDelayTime );

		/* Print out a message to show this task has reached its synchronization
		point. pcTaskGetTaskName() is an API function that returns the name assigned
		to the task when the task was created. */
		printf( "At time=%d %s %s\r\n",xTaskGetTickCount(), pcTaskGetName( NULL ), "reached sync point" );

		/* Wait for all the tasks to have reached their respective synchronization
		points. */
		xEventGroupSync( /* The event group used to synchronize. */
							xEventGroup,
						/* The bit set by this task to indicate it has reached the synchronization point. */
							uxThisTasksSyncBit,
						/* The bits to wait for, one bit for each task taking part in the synchronization. */
							uxAllSyncBits,
						/* Wait indefinitely for all three tasks to reach the synchronization point. */
							portMAX_DELAY );

		/* Print out a message to show this task has passed its synchronization point. As an
		 * indefinite delay was used the following line will only be executed after all the tasks
		 * reached their respective synchronization points. */
		printf( "At time=%d %s %s\r\n",xTaskGetTickCount(), pcTaskGetName( NULL ), "exited sync point" );
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
    printf("task sync using event groups, chap 8, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Sometimes the design of an application requires two or more tasks to synchronize with each
     * other. For example, consider a design where Task A receives an event, then delegates some of
     * the processing necessitated by the event to three other tasks: Task B, Task C and Task D. If
     * Task A cannot receive another event until tasks B, C and D have all completed processing the
     * previous event, then all four tasks will need to synchronize with each other. Each task’s
     * synchronization point will be after that task has completed its processing, and cannot proceed
     * further until each of the other tasks have done the same. Task A can only receive another
     * event after all four tasks have reached their synchronization point. */

    /* Listing 141. Pseudo code for two tasks that synchronize with each other to ensure a shared TCP
     * socket is no longer in use by either task before the socket is closed */

    /* An event group can be used to create a synchronization point:
     * - Each task that must participate in the synchronization is assigned a unique event bit within
     * the event group.
     * - Each task sets its own event bit when it reaches the synchronization point.
     * - Having set its own event bit, each task blocks on the event group to wait for the event bits
     * that represent all the other synchronizing tasks to also become set.
     * However, the xEventGroupSetBits() and xEventGroupWaitBits() API functions cannot be used in
     * this scenario. If they were used, then the setting of a bit (to indicate a task had reached
     * its synchronization point) and the testing of bits (to determine if the other synchronizing
     * tasks had reached their synchronization point) would be performed as two separate operations.
     * To see why that would be a problem, consider a scenario where Task A, Task B and Task C
     * attempt to synchronize using an event group:
     * 1. Task A and Task B have already reached the synchronization point, so their event bits are
     * set in the event group, and they are in the Blocked state to wait for task C’s event bit to
     * also become set.
     * 2. Task C reaches the synchronization point, and uses xEventGroupSetBits() to set its bit in
     * the event group. As soon as Task C’s bit is set, Task A and Task B leave the Blocked state,
     * and clear all three event bits.
     * 3. Task C then calls xEventGroupWaitBits() to wait for all three event bits to become set, but
     * by that time, all three event bits have already been cleared, Task A and Task B have left
     * their respective synchronization points, and so the synchronization has failed. To
     * successfully use an event group to create a synchronization point, the setting of an event bit,
     * and the subsequent testing of event bits, must be performed as a single uninterruptable
     * operation. The xEventGroupSync() API function is provided for that purpose. */

    /* xEventGroupSync() is provided to allow two or more tasks to use an event group to synchronize
     * with each other. The function allows a task to set one or more event bits in an event group,
     * then wait for a combination of event bits to become set in the same event group, as a single
     * uninterruptable operation. The xEventGroupSync() uxBitsToWaitFor parameter specifies the
     * calling task’s unblock condition. The event bits specified by uxBitsToWaitFor will be cleared
     * back to zero before xEventGroupSync() returns, if xEventGroupSync() returned because the
     * unblock condition had been met. */

    /* If xEventGroupSync() returned because the calling task’s unblock condition was met, then the
     * returned value is the value of the event group at the time the calling task’s unblock
     * condition was met (before any bits were automatically cleared back to zero). In this case the
     * returned value will also meet the calling task’s unblock condition. If xEventGroupSync()
     * returned because the block time specified by the xTicksToWait parameter expired, then the
     * returned value is the value of the event group at the time the block time expired. In this
     * case the returned value will not meet the calling task’s unblock condition. */

    /* Before an event group can be used it must first be created. */
     xEventGroup = xEventGroupCreate();

     /* Check the semaphore was created successfully before creating the tasks. */
     if( xEventGroup != NULL )
     {
    	/* Create three instances of the task. Each task is given a different name, which is later
    	 * printed out to give a visual indication of which task is executing. The event bit to use
    	 * when the task reaches its synchronization point is passed into the task using the task
    	 * parameter. */
		if ( xTaskCreate( vSyncingTask, "Task 1", 1000, first_task_bit, 1, NULL ) == pdPASS)
		{
	    	if ( xTaskCreate( vSyncingTask, "Task 2", 1000, second_task_bit, 1, NULL ) == pdPASS)
	    	{
		    	if ( xTaskCreate( vSyncingTask, "Task 3", 1000, third_task_bit, 1, NULL ) == pdPASS)
		    	{
		    		/* Start the FreeRTOS scheduler. */
		    		vTaskStartScheduler();
		    	}
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
