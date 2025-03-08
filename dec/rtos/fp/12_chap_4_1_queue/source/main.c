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
/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ               (100000000u)
/*******************************************************************************
* Function Prototypes
********************************************************************************/
TaskHandle_t RedTaskHandle;
TaskHandle_t GreenTaskHandle;

static volatile uint32_t count=0UL;
static volatile uint32_t idle_count=0UL;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void RedTask(void *argument)
{
	UBaseType_t uxPriority;
	while(1)
	{
		uxPriority = uxTaskPriorityGet(NULL);
		Cy_GPIO_Inv(LED_RGB_RED_PORT,LED_RGB_RED_PIN);

		//vTaskDelay(pdMS_TO_TICKS( 250 ));

		printf("Red Task current priority=%d\r\n", uxPriority);
		printf("Lowering Red task priority\r\n");
		vTaskPrioritySet( NULL, ( uxPriority - 2 ) );

		/* After lowering red task priority, green task will be executed immediately and this print
		 * will come after green task.
		 */
		printf("After lowering red task priority, executed green task and came back\r\n");
	}
}

void GreenTask(void *argument)
{
	UBaseType_t uxPriority;
	while(1)
	{
		uxPriority = uxTaskPriorityGet(NULL);
		Cy_GPIO_Inv(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN);

		//vTaskDelay(pdMS_TO_TICKS( 250 ));

		printf("Green Task current priority=%d\r\n", uxPriority);
		printf("Increasing red task priority\r\n");
		vTaskPrioritySet( RedTaskHandle, ( uxTaskPriorityGet(RedTaskHandle) + 2 ) );

		/* After increasing red task priority, red task will be executed immediately and this print
		 * will come after red task.
		 */
		printf("After increasing red task priority, executed red task and came back\r\n");
	}
}

void vApplicationIdleHook(void)
{
	idle_count++;
	printf("idle task, idle count=%d\r\n",idle_count);
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
    printf("task priority, chap 3, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* ‘Queues’ provide a task-to-task, task-to-interrupt, and interrupt-to-task communication
     * mechanism.
     */

    /* A queue can hold a finite number of fixed size data items. The maximum number of items a queue
     * can hold is called its ‘length’. Both the length and the size of each data item are set when
     * the queue is created. Queues are normally used as First In First Out (FIFO) buffers, where
     * data is written to the end (tail) of the queue and removed from the front (head) of the queue.
     * Figure 31 demonstrates data being written to and read from a queue that is being used as a
     * FIFO. It is also possible to write to the front of a queue, and to overwrite data that is
     * already at the front of a queue. */

    /* There are two ways in which queue behavior could have been implemented:
     * 1. Queue by copy
     * 	Queuing by copy means the data sent to the queue is copied byte for byte into the queue.
     * 2. Queue by reference
     * 	Queuing by reference means the queue only holds pointers to the data sent to the queue, not
     * 	the data itself. */

    /* FreeRTOS uses the queue by copy method. Queuing by copy is considered to be simultaneously
     * more powerful and simpler to use than queueing by reference because:
     * - Stack variable can be sent directly to a queue, even though the variable will not exist
     * after the function in which it is declared has exited.
     * - Data can be sent to a queue without first allocating a buffer to hold the data, and then
     * copying the data into the allocated buffer.
     * - The sending task can immediately re-use the variable or buffer that was sent to the queue.
     * - The sending task and the receiving task are completely de-coupled—the application designer
     * does not need to concern themselves with which task ‘owns’ the data, or which task is
     * responsible for releasing the data.
     * - Queuing by copy does not prevent the queue from also being used to queue by reference. For
     * example, when the size of the data being queued makes it impractical to copy the data into the
     * queue, then a pointer to the data can be copied into the queue instead.
     * - The RTOS takes complete responsibility for allocating the memory used to store data.
     * - In a memory protected system, the RAM that a task can access will be restricted. In that
     * case queueing by reference could only be used if the sending and receiving task could both
     * access the RAM in which the data was stored. Queuing by copy does not impose that restriction;
     * the kernel always runs with full privileges, allowing a queue to be used to pass data across
     * memory protection boundaries. */

    /* Queues are objects in their own right that can be accessed by any task or ISR that knows of
     * their existence. Any number of tasks can write to the same queue, and any number of tasks can
     * read from the same queue. In practice it is very common for a queue to have multiple writers,
     * but much less common for a queue to have multiple readers. */

    /* When a task attempts to read from a queue, it can optionally specify a ‘block’ time. This is
     * the time the task will be kept in the Blocked state to wait for data to be available from the
     * queue, should the queue already be empty. A task that is in the Blocked state, waiting for data
     * to become available from a queue, is automatically moved to the Ready state when another task
     * or interrupt places data into the queue. The task will also be moved automatically from the
     * Blocked state to the Ready state if the specified block time expires before data becomes
     * available. */

    /* Queues can have multiple readers, so it is possible for a single queue to have more than one
     * task blocked on it waiting for data. When this is the case, only one task will be unblocked
     * when data becomes available. The task that is unblocked will always be the highest priority
     * task that is waiting for data. If the blocked tasks have equal priority, then the task that
     * has been waiting for data the longest will be unblocked.
     */

    /* Just as when reading from a queue, a task can optionally specify a block time when writing to
     * a queue. In this case, the block time is the maximum time the task should be held in the
     * Blocked state to wait for space to become available on the queue, should the queue already be
     * full. */

    /* Queues can have multiple writers, so it is possible for a full queue to have more than one
     * task blocked on it waiting to complete a send operation. When this is the case, only one task
     * will be unblocked when space on the queue becomes available. The task that is unblocked will
     * always be the highest priority task that is waiting for space. If the blocked tasks have
     * equal priority, then the task that has been waiting for space the longest will be unblocked. */

    /* Queues can be grouped into sets, allowing a task to enter the Blocked state to wait for data
     * to become available on any of the queues in the set. */

    /* A queue must be explicitly created before it can be used. Queues are referenced by handles,
     * which are variables of type QueueHandle_t. The xQueueCreate() API function creates a queue and
     * returns a QueueHandle_t that references the queue it created. */

    /* FreeRTOS allocates RAM from the FreeRTOS heap when a queue is created. The RAM is used to hold
     * both the queue data structures and the items that are contained in the queue. xQueueCreate()
     * will return NULL if there is insufficient heap RAM available for the queue to be created. */

    /* After a queue has been created the xQueueReset() API function can be used to return the queue
     * to its original empty state. */

    /* As might be expected, xQueueSendToBack() is used to send data to the back (tail) of a queue,
     * and xQueueSendToFront() is used to send data to the front (head) of a queue. xQueueSend() is
     * equivalent to, and exactly the same as, xQueueSendToBack(). */

    /* Never call xQueueSendToFront() or xQueueSendToBack() from an interrupt service routine. The
     * interrupt-safe versions xQueueSendToFrontFromISR() and xQueueSendToBackFromISR() should be
     * used in their place. */

    /* xQueueReceive() is used to receive (read) an item from a queue. The item that is received is
     * removed from the queue. */

    /* Never call xQueueReceive() from an interrupt service routine. The interrupt-safe
     * xQueueReceiveFromISR() API function is described in Chapter 6. */

    /* uxQueueMessagesWaiting() is used to query the number of items that are currently in a queue.
     * Note: Never call uxQueueMessagesWaiting() from an interrupt service routine. The interrupt
     * safe uxQueueMessagesWaitingFromISR() should be used in its place. */

    if (xTaskCreate(GreenTask, "GreenTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY+1, &GreenTaskHandle) == pdPASS)
	{
		if (xTaskCreate(RedTask, "RedTask", TASK_STACK_SIZE, NULL, TASK_PRIORITY+2, &RedTaskHandle) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
