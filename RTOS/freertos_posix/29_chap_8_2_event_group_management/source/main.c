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
#define mainFIRST_TASK_BIT  ( 1UL << 0UL ) /* Event bit 0, which is set by a task. */
#define mainSECOND_TASK_BIT ( 1UL << 1UL ) /* Event bit 1, which is set by a task. */
#define mainISR_BIT         ( 1UL << 2UL ) /* Event bit 2, which is set by an ISR. */

EventGroupHandle_t xEventGroup;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void vEventBitSettingTask( void *pvParameters )
{
	const TickType_t xDelay200ms = pdMS_TO_TICKS( 200UL ), xDontBlock = 0;

	for( ;; )
	{
		/* Delay for a short while before starting the next loop. */
		vTaskDelay( xDelay200ms );

		/* Print out a message to say event bit 0 is about to be set by the task,
		then set event bit 0. */
		printf( "Bit setting task -\t about to set bit 0.\r\n" );

		xEventGroupSetBits( xEventGroup, mainFIRST_TASK_BIT );

		/* Delay for a short while before setting the other bit. */
		vTaskDelay( xDelay200ms );

		/* Print out a message to say event bit 1 is about to be set by the task,
		then set event bit 1. */
		printf( "Bit setting task -\t about to set bit 1.\r\n" );

		xEventGroupSetBits( xEventGroup, mainSECOND_TASK_BIT );
	}
}

/* Change vEventBitReadingTask call to wait for all bits to set and see execution */
/* Change vEventBitReadingTask call to clear bits on exit and see execution */
static void vEventBitReadingTask( void *pvParameters )
{
	EventBits_t xEventGroupValue;
	const EventBits_t xBitsToWaitFor = ( mainFIRST_TASK_BIT | mainSECOND_TASK_BIT );
	for( ;; )
	{
		/* Block to wait for event bits to become set within the event group. */
		xEventGroupValue = xEventGroupWaitBits( /* The event group to read. */
												xEventGroup,
												/* Bits to test. */
												xBitsToWaitFor,
												/* Clear bits on exit if the unblock condition is met. */
												pdTRUE,
												/* Don't wait for all bits. This parameter is set to
												 * pdTRUE for the second execution. */
												pdTRUE,
												/* Don't time out. */
												portMAX_DELAY );

		/* Print a message for each bit that was set. */
		if( ( xEventGroupValue & mainFIRST_TASK_BIT ) != 0 )
		{
			printf( "Bit reading task -\t Event bit 0 was set\r\n" );
		}
		if( ( xEventGroupValue & mainSECOND_TASK_BIT ) != 0 )
		{
			printf( "Bit reading task -\t Event bit 1 was set\r\n" );
		}
		if( ( xEventGroupValue & mainISR_BIT ) != 0 )
		{
			printf( "Bit reading task -\t Event bit 2 was set\r\n" );
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
    printf("gatekeeper tasks, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Before an event group can be used it must first be created. */
     xEventGroup = xEventGroupCreate();

     /* Check the semaphore was created successfully before creating the tasks. */
     if( xEventGroup != NULL )
     {
		if ( xTaskCreate( vEventBitSettingTask, "Bit Setter", 1000, NULL, 1, NULL ) == pdPASS)
		{
	    	if ( xTaskCreate( vEventBitReadingTask, "Bit Reader", 1000, NULL, 2, NULL ) == pdPASS)
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
