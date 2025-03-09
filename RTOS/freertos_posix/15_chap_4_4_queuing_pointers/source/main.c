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
/* Declare a variable of type QueueHandle_t to hold the handle of the queue being created. */
QueueHandle_t xPointerQueue;

/* Define an enumerated type used to identify the source of the data. */
typedef enum
{
	eSender1,
	eSender2
}DataSource_t;

/* Define the structure type that will be passed on the queue. */
typedef struct
{
	uint8_t ucValue;
	DataSource_t eDataSource;
}Data_t;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void vSenderTask( void *pvParameters )
{
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
	Data_t * data_ptr = NULL;

	/* This static variable gets shared between both instances of sender tasks. If we remove static,
	 * then it will not get shared.
	 */
	static uint8_t count=0;

	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		data_ptr = (Data_t*)pvPortMalloc(sizeof(Data_t));

		if((*((char *)pvParameters))=='1')
		{
			data_ptr->eDataSource=eSender1;
		}
		else
		{
			data_ptr->eDataSource=eSender2;
		}
		data_ptr->ucValue=count++;

		/* Send to the queue. */
		xStatus = xQueueSendToBack( xPointerQueue, &data_ptr, xTicksToWait );
		if( xStatus != pdPASS )
		{
			/* The send operation could not complete, even after waiting for 100ms.
			This must be an error as the receiving task should make space in the
			queue as soon as both sending tasks are in the Blocked state. */
			printf("Could not send to the queue.\r\n");
		}
	}
}

static void vReceiverTask( void *pvParameters )
{
	/* Declare the structure that will hold the values received from the queue. */
	Data_t * rcvd_data_ptr = NULL;
	BaseType_t xStatus;
	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* Because it has the lowest priority this task will only run when the
		sending tasks are in the Blocked state. The sending tasks will only enter
		the Blocked state when the queue is full so this task always expects the
		number of items in the queue to be equal to the queue length, which is 3 in
		this case. */
		if( uxQueueMessagesWaiting( xPointerQueue ) != 5 )
		{
			printf("Queue should have been full!\r\n");
		}
		/* Receive from the queue. */
		xStatus = xQueueReceive( xPointerQueue, &rcvd_data_ptr, 0 );
		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
			value and the source of the value. */
			if( rcvd_data_ptr->eDataSource == eSender1 )
			{
				printf("From Sender 1 =%d\r\n", rcvd_data_ptr->ucValue );
			}
			else
			{
				printf("From Sender 2 =%d\r\n", rcvd_data_ptr->ucValue );
			}
			vPortFree(rcvd_data_ptr);
		}
		else
		{
			/* Nothing was received from the queue. This must be an error as this
			task should only run when the queue is full. */
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

    /* Create a queue that can hold a maximum of 5 pointers, in this case character pointers. */
    xPointerQueue = xQueueCreate( 5, sizeof( Data_t * ) );

    /* If the size of the data being stored in the queue is large, then it is preferable to use the
     * queue to transfer pointers to the data, rather than copy the data itself into and out of the
     * queue byte by byte. Transferring pointers is more efficient in both processing time and the
     * amount of RAM required to create the queue. However, when queuing pointers, extreme care must
     * be taken to ensure that:
     * 1. The owner of the RAM being pointed to is clearly defined. When sharing memory between tasks
     * via a pointer, it is essential to ensure that both tasks do not modify the memory contents
     * simultaneously, or take any other action that could cause the memory contents to be invalid or
     * inconsistent. Ideally, only the sending task should be permitted to access the memory until a
     * pointer to the memory has been queued, and only the receiving task should be permitted to
     * access the memory after the pointer has been received from the queue.
     * 2. The RAM being pointed to remains valid. If the memory being pointed to was allocated
     * dynamically, or obtained from a pool of pre-allocated buffers, then exactly one task should be
     * responsible for freeing the memory. No tasks should attempt to access the memory after it has
     * been freed. A pointer should never be used to access data that has been allocated on a task
     * stack. The data will not be valid after the stack frame has changed. */

    if( xPointerQueue != NULL )
	{
    	/* Create two instances of the task that will write to the queue. The parameter is used to
    	 * pass the structure that the task will write to the queue, so one task will continuously
    	 * send xStructsToSend[ 0 ] to the queue while the other task will continuously send
    	 * xStructsToSend[ 1 ]. Both tasks are created at priority 2, which is above the priority of
    	 * the receiver. */

    	if (xTaskCreate( vSenderTask, "Sender1", 1000, "1", 2, NULL ) == pdPASS)
		{
			if (xTaskCreate( vSenderTask, "Sender2", 1000, "2", 2, NULL ) == pdPASS)
			{
				/* /* Create the task that will read from the queue. The task is created with
				 * priority 1, so below the priority of the sender tasks. */
				if (xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 1, NULL ) == pdPASS)
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
