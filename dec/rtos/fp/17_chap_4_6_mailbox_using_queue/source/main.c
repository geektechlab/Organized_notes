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
/* A mailbox is a queue, so its handle is stored in a variable of type QueueHandle_t. */
QueueHandle_t xMailbox;

/* A mailbox can hold a fixed size data item. The size of the data item is set when the mailbox
 * (queue) is created. In this example the mailbox is created to hold an Example_t structure.
 * Example_t includes a time stamp to allow the data held in the mailbox to note the time at which
 * the mailbox was last updated. The time stamp used in this example is for demonstration purposes
 * only - a mailbox can hold any data the application writer wants, and the data does not need to
 * include a time stamp. */
typedef struct xExampleStructure
{
 TickType_t xTimeStamp;
 uint32_t ulValue;
} Example_t;

static uint32_t count = 0;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void vUpdateMailbox(void * args)
{
	Example_t xData;

	while(1)
	{
		/* Write the new data into the Example_t structure.*/
		xData.ulValue = count++;

		/* Use the RTOS tick count as the time stamp stored in the Example_t structure. */
		xData.xTimeStamp = xTaskGetTickCount();

		/* Like the xQueueSendToBack() API function, the xQueueOverwrite() API function sends data to a
		 * queue. Unlike xQueueSendToBack(), if the queue is already full, then xQueueOverwrite() will
		 * overwrite data that is already in the queue. */

		/* xQueueOverwrite() should only be used with queues that have a length of one. That restriction
		 * avoids the need for the function’s implementation to make an arbitrary decision as to which
		 * item in the queue to overwrite, if the queue is full. */

		/* Note: Never call xQueueOverwrite() from an interrupt service routine. The interrupt-safe
		 * version xQueueOverwriteFromISR() should be used in its place. */

		/* Send the structure to the mailbox - overwriting any data that is already in the mailbox. */
		xQueueOverwrite( xMailbox, &xData );

		vTaskDelay(200);
	}
}

void vReadMailbox( void * args )
{
	volatile TickType_t xPreviousTimeStamp;
	BaseType_t xDataUpdated;
	Example_t pxData;

	while(1)
	{
		/* This function updates an Example_t structure with the latest value received
		from the mailbox. Record the time stamp already contained in *pxData before it
		gets overwritten by the new data. */
		xPreviousTimeStamp = pxData.xTimeStamp;

		/* Update the Example_t structure pointed to by pxData with the data contained in
		the mailbox. If xQueueReceive() was used here then the mailbox would be left
		empty, and the data could not then be read by any other tasks. Using
		xQueuePeek() instead of xQueueReceive() ensures the data remains in the mailbox.
		A block time is specified, so the calling task will be placed in the Blocked
		state to wait for the mailbox to contain data should the mailbox be empty. An
		infinite block time is used, so it is not necessary to check the value returned
		from xQueuePeek(), as xQueuePeek() will only return when data is available. */
		xQueuePeek( xMailbox, &pxData, portMAX_DELAY );

		/* Return pdTRUE if the value read from the mailbox has been updated since this
		function was last called. Otherwise return pdFALSE. */
		if( pxData.xTimeStamp > xPreviousTimeStamp )
		{
			printf("Data got updated, data=%d, timestamp=%d\r\n", pxData.ulValue, pxData.xTimeStamp);
		}
		else
		{
			xDataUpdated = pdFALSE;
		}
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
    printf("mailbox using queue demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* There is no consensus ( agreement ) on terminology within the embedded community, and
     * ‘mailbox’ will mean different things in different RTOSes. In this book the term mailbox is
     * used to refer to a queue that has a length of one. A queue may get described as a mailbox
     * because of the way it is used in the application, rather than because it has a functional
     * difference to a queue:
     * - A queue is used to send data from one task to another task, or from an interrupt service
     * routine to a task. The sender places an item in the queue, and the receiver removes the item
     * from the queue. The data passes through the queue from the sender to the receiver.
     * - A mailbox is used to hold data that can be read by any task, or any interrupt service
     * routine. The data does not pass through the mailbox, but instead remains in the mailbox until
     * it is overwritten. The sender overwrites the value in the mailbox. The receiver reads the
     * value from the mailbox, but does not remove the value from the mailbox. */

    xMailbox = xQueueCreate( 1, sizeof( Example_t ) );

    if( xMailbox != NULL )
	{
    	if (xTaskCreate( vUpdateMailbox, "Update mailbox", 1000, NULL, 2, NULL ) == pdPASS)
		{
			if (xTaskCreate( vReadMailbox, "Read mailbox", 1000, NULL, 1, NULL ) == pdPASS)
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
