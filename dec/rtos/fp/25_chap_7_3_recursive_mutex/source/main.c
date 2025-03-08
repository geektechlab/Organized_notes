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

SemaphoreHandle_t xRecursiveMutex;

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

	/* Read teh associated descr: Figure 68 A possible sequence of execution when tasks that have the
	 * same priority use the same mutex */

	xSemaphoreTakeRecursive( xRecursiveMutex, portMAX_DELAY );
	{
		xSemaphoreTakeRecursive( xRecursiveMutex, portMAX_DELAY );
		/* The following line will only execute once the mutex has been successfully
		obtained. Standard out can be accessed freely now as only one task can have
		the mutex at any one time. */
		printf( "%s", pcString );

		fflush( stdout );
		/* The mutex MUST be given back! */
		xSemaphoreGiveRecursive( xRecursiveMutex );
	}

	xSemaphoreGiveRecursive( xRecursiveMutex );
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
    printf("recursive mutex demo, chap 4, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* A higher priority task being delayed by a lower priority task in this manner is called
     * ‘priority inversion’. This undesirable behavior would be exaggerated further if a medium
     * priority task started to execute while the high priority task was waiting for the
     * semaphore—the result would be a high priority task waiting for a low priority task—without the
     * low priority task even being able to execute. */

    /* FreeRTOS mutexes and binary semaphores are very similar—the difference being that mutexes
     * include a basic ‘priority inheritance’ mechanism, whereas binary semaphores do not. Priority
     * inheritance is a scheme that minimizes the negative effects of priority inversion. It does not
     * ‘fix’ priority inversion, but merely lessens its impact by ensuring that the inversion is
     * always time bounded. However, priority inheritance complicates system timing analysis, and it
     * is not good practice to rely on it for correct system operation. */

    /* Priority inheritance works by temporarily raising the priority of the mutex holder to the
     * priority of the highest priority task that is attempting to obtain the same mutex. The low
     * priority task that holds the mutex ‘inherits’ the priority of the task waiting for the mutex.
     * This is demonstrated by Figure 67. The priority of the mutex holder is reset automatically to
     * its original value when it gives the mutex back. */

    /* As just seen, priority inheritance functionality effects the priority of tasks that are using
     * the mutex. For that reason, mutexes must not be used from an interrupt service routines. Why ?
     * maybe because task holding mutex might want to execute first but interrupt using freertos API
     * may use protected resource and cause mutex failure ? */

    /* ‘Deadlock’ is another potential pitfall of using mutexes for mutual exclusion. Deadlock is
     * sometimes also known by the more dramatic name ‘deadly embrace’. Deadlock occurs when two
     * tasks cannot proceed because they are both waiting for a resource that is held by the other.
     * Watch Chester Rebeiro notes for deadlock. */

    /* As with priority inversion, the best method of avoiding deadlock is to consider its potential
     * at design time, and design the system to ensure that deadlock cannot occur. In particular, and
     * as previously stated in this book, it is normally bad practice for a task to wait indefinitely
     * (without a time out) to obtain a mutex. Instead, use a time out that is a little longer than
     * the maximum time it is expected to have to wait for the mutex—then failure to obtain the mutex
     * within that time will be a symptom of a design error, which might be a deadlock. In practice,
     * deadlock is not a big problem in small embedded systems, because the system designers can have
     * a good understanding of the entire application, and so can identify and remove the areas where
     * it could occur. */

    /* Recursive Mutexes */

    /* It is also possible for a task to deadlock with itself. This will happen if a task attempts to take
     * the same mutex more than once, without first returning the mutex. Consider the following
     * scenario:
     * 1. A task successfully obtains a mutex.
     * 2. While holding the mutex, the task calls a library function.
     * 3. The implementation of the library function attempts to take the same mutex, and enters the
     * Blocked state to wait for the mutex to become available.
     *
     * At the end of this scenario the task is in the Blocked state to wait for the mutex to be
     * returned, but the task is already the mutex holder. A deadlock has occurred because the task
     * is in the Blocked state to wait for itself. This type of deadlock can be avoided by using a
     * recursive mutex in place of a standard mutex. A recursive mutex can be ‘taken’ more than once
     * by the same task, and will be returned only after one call to ‘give’ the recursive mutex has
     * been executed for every preceding call to ‘take’ the recursive mutex. */

    /* Before a recursive mutex is used it must be explicitly created. */
    xRecursiveMutex = xSemaphoreCreateRecursiveMutex();

     /* Check the semaphore was created successfully before creating the tasks. */
     if( xRecursiveMutex != NULL )
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
