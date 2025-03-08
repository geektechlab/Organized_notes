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
#include "semphr.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ          (100000000u)

/* Declare a variable of type QueueHandle_t. The queue is used to send messages from the print tasks
 * and the tick interrupt to the gatekeeper task. */
QueueHandle_t xPrintQueue;

/* Define the strings that the tasks and interrupt will print out via the gatekeeper. */
static char *pcStringsToPrint[] =
{
 "Task 1 ****************************************************\r\n",
 "Task 2 ----------------------------------------------------\r\n",
 "Message printed from the tick hook interrupt ##############\r\n"
};

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void prvStdioGatekeeperTask( void *pvParameters )
{
	char *pcMessageToPrint;

	/* This is the only task that is allowed to write to standard out. Any other
	task wanting to write a string to the output does not access standard out
	directly, but instead sends the string to this task. As only this task accesses
	standard out there are no mutual exclusion or serialization issues to consider
	within the implementation of the task itself. */
	for( ;; )
	{
		/* Wait for a message to arrive. An indefinite block time is specified so
		there is no need to check the return value – the function will only return
		when a message has been successfully received. */
		xQueueReceive( xPrintQueue, &pcMessageToPrint, portMAX_DELAY );

		/* Output the received string. */
		printf( "%s", pcMessageToPrint );
		fflush( stdout );
		/* Loop back to wait for the next message. */
	}
}

static void prvPrintTask( void *pvParameters )
{
	int iIndexToString;
	const TickType_t xMaxBlockTimeTicks = 0x20;

	/* Two instances of this task are created. The task parameter is used to pass
	an index into an array of strings into the task. Cast this to the required
	type. */
	iIndexToString = ( int ) pvParameters;

	for( ;; )
	{
		/* Print out the string, not directly, but instead by passing a pointer to
		the string to the gatekeeper task via a queue. The queue is created before
		the scheduler is started so will already exist by the time this task executes
		for the first time. A block time is not specified because there should
		always be space in the queue. */
		xQueueSendToBack( xPrintQueue, &( pcStringsToPrint[ iIndexToString ] ), 0 );

		/* Wait a pseudo random time. Note that rand() is not necessarily reentrant,
		but in this case it does not really matter as the code does not care what
		value is returned. In a more secure application a version of rand() that is
		known to be reentrant should be used - or calls to rand() should be protected
		using a critical section. */
		vTaskDelay( ( rand() % xMaxBlockTimeTicks ) );
	}
}

/* In this example, a tick hook function is used to write out a message every 200 ticks:
 * A tick hook (or tick callback) is a function that is called by the kernel during each tick
 * interrupt. To use a tick hook function:
 * 1. Set configUSE_TICK_HOOK to 1 in FreeRTOSConfig.h.
 * 2. Provide the implementation of the hook function, using the exact function name and prototype
 * shown in Listing 127. */
void vApplicationTickHook( void )
{
	static int iCount = 0;
	/* Print out a message every 200 ticks. The message is not written out directly,
	but sent to the gatekeeper task. */
	iCount++;

	if( iCount >= 200 )
	{
		/* As xQueueSendToFrontFromISR() is being called from the tick hook, it is
		not necessary to use the xHigherPriorityTaskWoken parameter (the third
		parameter), and the parameter is set to NULL. */
		xQueueSendToFrontFromISR( xPrintQueue, &( pcStringsToPrint[ 2 ] ), NULL );

		/* Reset the count ready to print out the string again in 200 ticks time. */
		iCount = 0;
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
    printf("gatekeeper tasks, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Previous chapters have described features of FreeRTOS that allow events to be communicated to
     * tasks. Examples of such features include semaphores and queues, both of which have the
     * following properties:
     * - They allow a task to wait in the Blocked state for a single event to occur.
     * - They unblock a single task when the event occurs—the task that is unblocked is the highest
     * priority task that was waiting for the event. */

    /* Event groups are another feature of FreeRTOS that allow events to be communicated to tasks.
     * Unlike queues and semaphores:
     * - Event groups allow a task to wait in the Blocked state for a combination of one of more
     * events to occur.
     * - Event groups unblock all the tasks that were waiting for the same event, or combination of
     * events, when the event occurs. */

    /* These unique properties of event groups make them useful for synchronizing multiple tasks,
     * broadcasting events to more than one task, allowing a task to wait in the Blocked state for
     * any one of a set of events to occur, and allowing a task to wait in the Blocked state for
     * multiple actions to complete. Event groups also provide the opportunity to reduce the RAM used
     * by an application, as often it is possible to replace many binary semaphores with a single
     * event group. */

    /* Event Groups, Event Flags and Event Bits */

    /* An event ‘flag’ is a Boolean (1 or 0) value used to indicate if an event has occurred or not.
     * An event ‘group’ is a set of event flags. An event flag can only be 1 or 0, allowing the state
     * of an event flag to be stored in a single bit, and the state of all the event flags in an
     * event group to be stored in a single variable; the state of each event flag in an event group
     * is represented by a single bit in a variable of type EventBits_t. For that reason, event flags
     * are also known as event ‘bits’. If a bit is set to 1 in the EventBits_t variable, then the
     * event represented by that bit has occurred. If a bit is set to 0 in the EventBits_t variable,
     * then the event represented by that bit has not occurred. */

    /* Figure 71 Event flag to bit number mapping in a variable of type EventBits_t */

    /* As an example, if the value of an event group is 0x92 (binary 1001 0010) then only event bits
     * 1, 4 and 7 are set, so only the events represented by bits 1, 4 and 7 have occurred. Figure 72
     * shows a variable of type EventBits_t that has event bits 1, 4 and 7 set, and all the other
     * event bits clear, giving the event group a value of 0x92. It is up to the application writer
     * to assign a meaning to individual bits within an event group. */

    /* Event groups are objects in their own right that can be accessed by any task or ISR that knows
     * of their existence. Any number of tasks can set bits in the same event group, and any number
     * of tasks can read bits from the same event group. */

    /* The number of event bits in an event group is dependent on the configUSE_16_BIT_TICKS compile
     * time configuration constant within FreeRTOSConfig.h:
     * - If configUSE_16_BIT_TICKS is 1, then each event group contains 8 usable event bits.
     * - If configUSE_16_BIT_TICKS is 0, then each event group contains 24 usable event bits. */

    /* The implementation of the FreeRTOS+TCP TCP/IP stack provides a practical example of how an
     * event group can be used to simultaneously simplify a design, and minimize resource usage. A
     * TCP socket must respond to many different events. Examples of events include accept events,
     * bind events, read events and close events. The events a socket can expect at any given time is
     * dependent on the state of the socket. For example, if a socket has been created, but not yet
     * bound to an address, then it can expect to receive a bind event, but would not expect to
     * receive a read event (it cannot read data if it does not have an address). The state of a
     * FreeRTOS+TCP socket is held in a structure called FreeRTOS_Socket_t. The structure contains an
     * event group that has an event bit defined for each event the socket must process. FreeRTOS+TCP
     * API calls that block to wait for an event, or group of events, simply block on the event group.
     * The event group also contains an ‘abort’ bit, allowing a TCP connection to be aborted, no
     * matter which event the socket is waiting for at the time. */

	/* /* If all is well then main() will never reach here as the scheduler will now be running the
	 * tasks. If main() does reach here then it is likely that there was insufficient FreeRTOS heap
	 * memory available for the idle task to be created. Chapter 2 provides more information on heap
	 * memory management. */

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
