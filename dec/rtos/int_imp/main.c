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
#include "queue.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ               (100000000u)
/*******************************************************************************
* Function Prototypes
********************************************************************************/
/* Declare a variable of type QueueHandle_t. This is used to store the handle
to the queue that is accessed by all three tasks. */
QueueHandle_t xQueue;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void vSenderTask( void *pvParameters )
{
	int32_t lValueToSend;
	BaseType_t xStatus;

	/* Two instances of this task are created so the value that is sent to the
	queue is passed in via the task parameter - this way each instance can use
	a different value. The queue was created to hold values of type int32_t,
	so cast the parameter to the required type. */
	lValueToSend = ( int32_t ) pvParameters;
	for( ;; )
	{
		/* Send the value to the queue.
		The first parameter is the queue to which data is being sent. The queue was created before
		the scheduler was started, so before this task started to execute. The second parameter is
		the address of the data to be sent, in this case the address of lValueToSend. The third
		parameter is the Block time – the time the task should be kept in the Blocked state to wait
		for space to become available on the queue should the queue already be full. In this case a
		block time is not specified because the queue should never contain more than one item, and
		therefore never be full. */
		xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );
		if( xStatus != pdPASS )
		{
			/* The send operation could not complete because the queue was full - this must be an
			 * error as the queue should never contain more than one item! */
			printf("Could not send to the queue.\r\n");
		}
	}
}

static void vReceiverTask( void *pvParameters )
{
	int32_t lReceivedValue;
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* This call should always find the queue empty because this task will immediately remove any
		 * data that is written to the queue. */
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
		{
			printf("Queue should have been empty!\r\n");
		}

		/* Receive data from the queue. The first parameter is the queue from which data is to be
		 * received. The queue is created before the scheduler is started, and therefore before this
		 * task runs for the first time. The second parameter is the buffer into which the received
		 * data will be placed. In this case the buffer is simply the address of a variable that has
		 * the required size to hold the received data. The last parameter is the block time – the
		 * maximum amount of time that the task will remain in the Blocked state to wait for data to
		 * be available should the queue already be empty.
		 * */
		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received value. */
			printf("Received =%d\r\n", lReceivedValue );
		}
		else
		{
			/* Data was not received from the queue even after waiting for 100ms. This must be an
			 * error as the sending tasks are free running and will be continuously writing to the
			 * queue. */
			printf("Could not receive from the queue.\r\n");
		}
	}
}

void vApplicationIdleHook(void)
{
	printf("idle task\r\n");
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
    printf("queue internal demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Queue_t structure ( it is a definition of queue used by scheduler ).
     * It creates a queue structure. Inside that, it maintains pointers 1) to indicate start of queue and 2) to point
     * to next free place in queue area.
     *
     * Then it holds union of queue pointer structure and semaphore data structure.
     * IMP: it is a good use case of union in embedded. Queue pointer holds pointer to end of queue storage area ( tail )
     * and pointer to store the last place from where queue item was read.
     *
     * Then it keeps a list of tasks witing to send in queue and another list of tasks waiting to receive from queue.
     *
     * Then it keeps a count of items currently in queue. Then it stores size of items that queue holds. Then it stores
     * length of the queue ( it is expressed as a number of items queue has to hold, not the number of bytes ).
     *
     * Then it stores the count of items removed from the queue while it was locked. Then it stores the count of items
     * added to the queue while it was locked.
     *
     * Then if queue set is used, it holds a pointer to this entire structure and later initialized to another queue's
     * QCB ( queue control block ) to create set of queues ?*/

    /* xQueueCreate() calls xQueueGenericCreate() which takes queue item size and queue length and then allocates
     * memory from FreeRTOS heap ( created from stack allocated by application developer ) for Queue_t data structure
     * and size of queue. Here, is Queue_t QCB ( queue control block ) and rest is for queue data storage ?
     *
     * Then it initializes allocated queue which sets start of queue pointer in Queue_t to beginning
     * if queue size is zero ( if it is used a mutex ) or just at the start of queue storage area. Then it stores
     * value of queue length and queue size in Queue_t. Then it calls xQueueGenericReset().
     *
     * xQueueGenericReset() first enters into critical section and then sets tail pointer inside QCB to point to end of
     * queue. Then sets pointer to read from queue at one item size less from end of queue. Then sets pointer to next
     * free place in queue area to start of queue storage. Then sets number of current items in queue to zero. Then sets
     * queue lock variable to unlocked. Then initialize tasks lists of waiting to send and receive from queue. Also
     * check if any tasks are already waiting and if yes then yield to another task using PendSV. Then it exits from
     * critical section.
     */

    /* xQueueReceive() checks if buffer and queue pointer passed is not NULL. Also, checks if scheduler suspended because
      * it can't receive if it is suspended. If it passes all these checks, then it enters critical section. Then checks
      * if any message is waiting to be received using uxMessagesWaiting variable from QCB.
      *
      * If any message is waiting, then it calls prvCopyDataFromQueue(), which copies queue data item using memcpy using
      * pcReadFrom pointer using uxItemSize from QCB. then it increments pcReadFrom pointer from QCB using uxItemSize from
      * QCB. If uxItemSize pointer becomes more than pcTail pointer from QCB then pcReadFrom pointer is updated as pcHead
      * pointer. Then prvCopyDataFromQueue() completes. Then it decrements uxMessagesWaiting by 1 from QCB. Then it calls
      * xTaskRemoveFromEventList() which moves one task waiting to send to queue into ready task list and checks if it's
      * priority is higher than current task using TCB ( task control block ). If priority is higher then it sets xYieldPending
      * global variable to true and returns true. Then xTaskRemoveFromEventList() completes. It xYieldPending is true
      * then than higher priority task is pended( started ) using pending a PendSV interrupt.
      *
      * If wait time is 0 then exit from critical section and return with failed receive error. If time is set then
      * maintain current tick count and to wait time count. Then put this task into event list and yield using PendSV.
      * In each subsequent calls check if data is received. If yes, extract it, otherwise check elapsed time using
      * tick count stored, current tick count and time to wait for. If time is expired, then return with failed receive
      * error.
      */

    /* xQueueSendToBack() checks if buffer and queue pointer passed is not NULL. Also, checks if scheduler suspended because
     * it can't receive if it is suspended. If it passes all these checks, then it enters critical section. Then checks
     * if any message is waiting to be received using uxMessagesWaiting variable from QCB.
     *
     * Then check if if there is any space using uxMessagesWaiting variable if it is less than uxLength. If we have to
     * overwrite then this check can be skipped. Then copy the data into queue. Then increment queue size if required
     * and update tail pointer. Uisng modulo operator, update tail to head if it grows beyond size of queue.
     *
     * Then notify queue set container then yield using PendSV.
     *
     * Rest blocking and time out related implementation is same as xQueueReceive(). */

    /* uxQueueMessagesWaiting() goes into critical section and checks if any message is present in queue using QCB */
     xQueue = xQueueCreate( 5, sizeof( int32_t ) );

	if( xQueue != NULL )
	{
		/* Create two instances of the task that will send to the queue. The task parameter is used
		* to pass the value that the task will write to the queue, so one task will continuously
		* write 100 to the queue while the other task will continuously write 200 to the queue.
		* Both tasks are created at priority 1. */
		if (xTaskCreate( vSenderTask, "Sender1", 1000, ( void * ) 100, 1, NULL ) == pdPASS)
		{
			if (xTaskCreate( vSenderTask, "Sender2", 1000, ( void * ) 200, 1, NULL ) == pdPASS)
			{
				if (xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 2, NULL ) == pdPASS)
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
