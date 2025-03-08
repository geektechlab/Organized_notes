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
/* Declare two variables of type QueueHandle_t. Both queues are added to the same queue set. */
static QueueHandle_t xQueue1 = NULL, xQueue2 = NULL;

/* Declare a variable of type QueueSetHandle_t. This is the queue set to which the two queues
 * are added. */
static QueueSetHandle_t xQueueSet = NULL;

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
void vSenderTask1( void *pvParameters )
{
	const TickType_t xBlockTime = pdMS_TO_TICKS( 100 );
	const char * const pcMessage = "Message from vSenderTask1\r\n";
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Block for 100ms. */
		vTaskDelay( xBlockTime );

		/* Send this task's string to xQueue1. It is not necessary to use a block
		time, even though the queue can only hold one item. This is because the
		priority of the task that reads from the queue is higher than the priority of
		this task; as soon as this task writes to the queue it will be pre-empted by
		the task that reads from the queue, so the queue will already be empty again
		by the time the call to xQueueSend() returns. The block time is set to 0. */
		xQueueSend( xQueue1, &pcMessage, 0 );
	}
}

void vSenderTask2( void *pvParameters )
{
	const TickType_t xBlockTime = pdMS_TO_TICKS( 200 );
	const char * const pcMessage = "Message from vSenderTask2\r\n";
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Block for 200ms. */
		vTaskDelay( xBlockTime );

		/* Send this task's string to xQueue2. It is not necessary to use a block
		time, even though the queue can only hold one item. This is because the
		priority of the task that reads from the queue is higher than the priority of
		this task; as soon as this task writes to the queue it will be pre-empted by
		the task that reads from the queue, so the queue will already be empty again
		by the time the call to xQueueSend() returns. The block time is set to 0. */
		xQueueSend( xQueue2, &pcMessage, 0 );
	}
}

/* If a call to xQueueSelectFromSet() times out, then it will return NULL. In this example,
 * xQueueSelectFromSet() is called with an indefinite block time, so will never time out, and can
 * only return a valid queue handle. Therefore, the receiving task does not need to check to see if
 * xQueueSelectFromSet() returned NULL before the return value is used. xQueueSelectFromSet() will
 * only return a queue handle if the queue referenced by the handle contains data, so it is not
 * necessary to use a block time when reading from the queue. */
void vReceiverTask( void *pvParameters )
{
	QueueHandle_t xQueueThatContainsData;
	char *pcReceivedString;
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Block on the queue set to wait for one of the queues in the set to contain data.
		Cast the QueueSetMemberHandle_t value returned from xQueueSelectFromSet() to a
		QueueHandle_t, as it is known all the members of the set are queues (the queue set
		does not contain any semaphores). */
		xQueueThatContainsData = ( QueueHandle_t ) xQueueSelectFromSet( xQueueSet, portMAX_DELAY );

		/* An indefinite block time was used when reading from the queue set, so
		xQueueSelectFromSet() will not have returned unless one of the queues in the set
		contained data, and xQueueThatContainsData cannot be NULL. Read from the queue. It
		is not necessary to specify a block time because it is known the queue contains
		data. The block time is set to 0. */
		xQueueReceive( xQueueThatContainsData, &pcReceivedString, 0 );

		/* Print the string received from the queue. */
		printf("%s\r\n",pcReceivedString);
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
    printf("queue set demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Often application designs require a single task to receive data of different sizes, data of
     * different meaning, and data from different sources. The previous section demonstrated how this
     * can be achieved in a neat and efficient way using a single queue that receives structures.
     * However, sometimes an application’s designer is working with constraints that limit their
     * design choices, necessitating the use of a separate queue for some data sources. For example,
     * third party code being integrated into a design might assume the presence of a dedicated queue.
     * In such cases a ‘queue set’ can be used. */

    /* Queue sets allow a task to receive data from more than one queue without the task polling each
     * queue in turn to determine which, if any, contains data. */

    /* Semaphores can also be added to a queue set. */

    /* When a queue that is a member of a set receives data, the handle of the receiving queue is
     * sent to the queue set, and returned when a task calls a function that reads from the queue set.
     * Therefore, if a queue handle is returned from a queue set then the queue referenced by the
     * handle is known to contain data, and the task can then read from the queue directly. */

    /* If a queue is a member of a queue set then do not read data from the queue unless the queue’s
     * handle has first been read from the queue set. */

    /* Queue set functionality is enabled by setting the configUSE_QUEUE_SETS compile time
     * configuration constant to 1 in FreeRTOSConfig.h. */

    /* A design that uses a queue set to receive data from multiple sources is less neat, and less
     * efficient, than a design that achieves the same functionality using a single queue that
     * receives structures. For that reason, it is recommended that queue sets are only used if
     * design constraints make their use absolutely necessary. */

    /* When a queue that is a member of a queue set receives data, the handle of the receiving queue
     * is sent to the queue set. uxEventQueueLength defines the maximum number of queue handles the
     * queue set being created can hold at any one time. */

    /* Queue handles are only sent to a queue set when a queue within the set receives data. A queue
     * cannot receive data if it is full, so no queue handles can be sent to the queue set if all the
     * queues in the set are full. Therefore, the maximum number of items the queue set will ever
     * have to hold at one time is the sum of the lengths of every queue in the set. */

    /* Semaphores can also be added to a queue set. Binary and counting semaphores are covered later
     * in this book. For the purposes of calculating the necessary uxEventQueueLength, the length of
     * a binary semaphore is one, and the length of a counting semaphore is given by the semaphore’s
     * maximum count value. */

    /* When a queue or semaphore that is a member of a set receives data, the handle of the receiving
     * queue or semaphore is sent to the queue set, and returned when a task calls
     * xQueueSelectFromSet(). If a handle is returned from a call to xQueueSelectFromSet() then the
     * queue or semaphore referenced by the handle is known to contain data and the calling task must
     * then read from the queue or semaphore directly. */

    /* Note: Do not read data from a queue or semaphore that is a member of a set unless the handle
     * of the queue or semaphore has first been returned from a call to xQueueSelectFromSet(). Only
     * read one item from a queue or semaphore each time the queue handle or semaphore handle is
     * returned from a call to xQueueSelectFromSet(). */

    /* This example demonstrates a very simplistic case; the queue set only contained queues, and the
     * two queues it contained were both used to send a character pointer. In a real application, a
     * queue set might contain both queues and semaphores, and the queues might not all hold the same
     * data type. When this is the case, it is necessary to test the value returned by
     * xQueueSelectFromSet(), before the returned value is used. */

    /* See page no. 169 to how to use the value returned from xQueueSelectFromSet() when the set has
     * the following members:
     * 1. A binary semaphore.
     * 2. A queue from which character pointers are read.
     * 3. A queue from which uint32_t values are read.
     * it assumes the queues and semaphore have already been created and added to the queue set. */

    /* Create the two queues, both of which send character pointers. The priority
     of the receiving task is above the priority of the sending tasks, so the queues
     will never have more than one item in them at any one time*/
     xQueue1 = xQueueCreate( 1, sizeof( char * ) );
     xQueue2 = xQueueCreate( 1, sizeof( char * ) );

	/* Create the queue set. Two queues will be added to the set, each of which can
	contain 1 item, so the maximum number of queue handles the queue set will ever
	have to hold at one time is 2 (2 queues multiplied by 1 item per queue). */
	xQueueSet = xQueueCreateSet( 1 * 2 );

	/* Add the two queues to the set. */
	xQueueAddToSet( xQueue1, xQueueSet );
	xQueueAddToSet( xQueue2, xQueueSet );

    if(( xQueue1 != NULL ) && ( xQueue2 != NULL ) && ( xQueueSet != NULL ))
	{
    	if (xTaskCreate( vSenderTask1, "Sender1", 1000, NULL, 1, NULL ) == pdPASS)
		{
			if (xTaskCreate( vSenderTask2, "Sender2", 1000, NULL, 1, NULL ) == pdPASS)
			{
				/* Create the task that reads from the queue set to determine which of the two queues
				 * contain data. */
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
