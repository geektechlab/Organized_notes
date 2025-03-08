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
		 * be available should the queue already be empty. */
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
    printf("queue demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* This example demonstrates a queue being created, data being sent to the queue from multiple
     * tasks, and data being received from the queue. The queue is created to hold data items of type
     * int32_t. The tasks that send to the queue do not specify a block time, whereas the task that
     * receives from the queue does. */

     /* The priority of the tasks that send to the queue are lower than the priority of the task that
      * receives from the queue. This means the queue should never contain more than one item because,
      * as soon as data is sent to the queue the receiving task will unblock, pre-empt the sending
      * task, and remove the data—leaving the queue empty once again. */

    /* If priority settings are reversed, then always queue will be full because receiving task will
     * only get unblocked when sending task will be unable to send more items because queue has become
     * full.
     */

    /* IMP: Figure 33. The sequence of execution produced by Example 10 */

    /* The queue is created to hold a maximum of 5 values, each of which is large enough to hold a
     * variable of type int32_t. */
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
