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

/* The periods assigned to the one-shot and auto-reload timers are 3.333 second and half a second
 * respectively. */
#define original_PERIOD    pdMS_TO_TICKS( 500 )

SemaphoreHandle_t xMutex;

static volatile uint32_t count=0;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void prvNewPrintString( const char *pcString )
{
	/* The mutex is created before the scheduler is started, so already exists by the
	time this task executes.

	Attempt to take the mutex, blocking indefinitely to wait for the mutex if it is
	not available straight away. The call to xSemaphoreTake() will only return when
	the mutex has been successfully obtained, so there is no need to check the
	function return value. If any other delay period was used then the code must
	check that xSemaphoreTake() returns pdTRUE before accessing the shared resource
	(which in this case is standard out). As noted earlier in this book, indefinite
	time outs are not recommended for production code. */
	xSemaphoreTake( xMutex, portMAX_DELAY );
	{
		/* The following line will only execute once the mutex has been successfully
		obtained. Standard out can be accessed freely now as only one task can have
		the mutex at any one time. */
		printf( "%s", pcString );

		fflush( stdout );
		/* The mutex MUST be given back! */
	}

	xSemaphoreGive( xMutex );
}

static void prvPrintTask( void *pvParameters )
{
	char *pcStringToPrint;
	const TickType_t xMaxBlockTimeTicks = 0x20;

	/* Two instances of this task are created. The string printed by the task is
	passed into the task using the task’s parameter. The parameter is cast to the
	required type. */
	pcStringToPrint = ( char * ) pvParameters;

	for( ;; )
	{
		/* Print out the string using the newly defined function. */
		prvNewPrintString( pcStringToPrint );

		/* Wait a pseudo random time. Note that rand() is not necessarily reentrant,
		but in this case it does not really matter as the code does not care what
		value is returned. In a more secure application a version of rand() that is
		known to be reentrant should be used - or calls to rand() should be protected
		using a critical section. */
		vTaskDelay( ( rand() % xMaxBlockTimeTicks ) );
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
    printf("software timer period change demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* A Mutex is a special type of binary semaphore that is used to control access to a resource that
     * is shared between two or more tasks. The word MUTEX originates from ‘MUTual EXclusion’.
     * configUSE_MUTEXES must be set to 1 in FreeRTOSConfig.h for mutexes to be available. When used
     * in a mutual exclusion scenario, the mutex can be thought of as a token that is associated with
     * the resource being shared. For a task to access the resource legitimately, it must first
     * successfully ‘take’ the token (be the token holder). When the token holder has finished with
     * the resource, it must ‘give’ the token back. Only when the token has been returned can another
     * task successfully take the token, and then safely access the same shared resource. A task is
     * not permitted to access the shared resource unless it holds the token.
     */

    /* Even though mutexes and binary semaphores share many characteristics, following are primary
     * differences:
     * - A semaphore that is used for mutual exclusion must always be returned.
     * - A semaphore that is used for synchronization is normally discarded and not returned.
     */

    /* A mutex is a type of semaphore. Handles to all the various types of FreeRTOS semaphore are
     * stored in a variable of type SemaphoreHandle_t. Before a mutex can be used, it must be created.
     * To create a mutex type semaphore, use the xSemaphoreCreateMutex() API function. */

    /* Before a semaphore is used it must be explicitly created. In this example a mutex type
     * semaphore is created. */
     xMutex = xSemaphoreCreateMutex();

     /* Check the semaphore was created successfully before creating the tasks. */
     if( xMutex != NULL )
     {
		if ( xTaskCreate( prvPrintTask, "Print1", 1000,
				 "Task 1 ***************************************\r\n", 1, NULL ) == pdPASS)
		{
	    	if ( xTaskCreate( prvPrintTask, "Print2", 1000,
	    			 "Task 2 ---------------------------------------\r\n", 2, NULL ) == pdPASS)
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
