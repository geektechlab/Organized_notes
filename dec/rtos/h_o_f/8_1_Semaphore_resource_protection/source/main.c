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
/*******************************************************************************
* Function Prototypes
********************************************************************************/
//create storage for a pointer to a semaphore
SemaphoreHandle_t semPtr = NULL;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void giveSem(void *argument)
{
	volatile int count=0;
	while(1)
	{
		/* every 5 times through the loop, give the semaphore */
		if(++count >= 5)
		{
			count = 0;
			xSemaphoreGive(semPtr);
			/* 1: Indicates semaphore has been given */
			Cy_GPIO_Inv(LED_ORANGE_PORT,LED_ORANGE_PIN);
		}

		/* Since we want takeSem to run some time we should use non-blocking delay. Otherwise,
		 * this task will get prioritized over takeSem and takeSem will never run. */
		vTaskDelay(1000);
	}
}

void takeSem(void *argument)
{
	while(1)
	{
		/* 'take' the semaphore with a really long timeout */
		/* following semaphore approach will reduce a CPU load lot. Untill it gets semaphore, task
		 * remains blocked. If we would have used global flag to poll instead, then it would have
		 * used CPU a lot.
		 */
		if(xSemaphoreTake(semPtr, 500) == pdPASS)
		{
			/* 3: Indicates semaphore is taken by blinking blue RGB LED */
			Cy_GPIO_Inv(LED_RGB_BLUE_PORT,LED_RGB_BLUE_PIN);
			for(uint_fast8_t i = 0; i < 8; i++)
			{
				/* 4: Till this loop completes, control will remain here even if task gets preempted
				 * and scheduled again. It will not wait for semaphore taking till this loop gets
				 * completed. Once loop execution gets completed, then only it will start waiting
				 * again for taking semaphore. Green RGB LED should blink 4 times ( 8/2 ) once
				 * semaphore is taken. */
				Cy_GPIO_Inv(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN);
				vTaskDelay(100);
			}
		}
		else
		{
			/* In xSemaphoreTake() if we specify portMAX_DELAY instead of 500, we should not reach
			 * here, but real-time tasks should not use such infinite or maximum delay because they
			 * are meant to be deterministic in time */

			/* 2: Since we have specified 500 timeout in xSemaphoreTake(), it will fail to acquire.
			 * Because giveSem gives Semaphore after ~5000 because it gives it after 5 iterations
			 * and each iterations take 1000 time unit to complete. So, RED LED should blink 5 times
			 * before taking semaphore. */
			Cy_GPIO_Inv(LED_RED_PORT,LED_RED_PIN);
		}
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		printf("idle task\r\n");
	}
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
    printf("https://github.com/PacktPublishing/Hands-On-RTOS-with-Microcontrollers/blob/master/Chapter_8/Src/mainSemExample.c\r\n");
    printf("Give and take semaphore\r\n");
    printf("**********************************************************\r\n");

	/* create a semaphore using the FreeRTOS Heap */
    /* General rule of thumb: shared variable must be declared volatile */
    /* Always check for possible priority inversion scenarios and apply mutex to solve those */
    /* Don't think that if any scenario is not reproducible then it is not a bug */
	semPtr = xSemaphoreCreateBinary();
	CY_ASSERT(semPtr != NULL);

    /* Create the task */
	if (xTaskCreate(giveSem, "give semaphore", TASK_STACK_SIZE, NULL, TASK_PRIORITY+2, NULL) == pdPASS)
	{
		if (xTaskCreate(takeSem, "take semaphore", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
