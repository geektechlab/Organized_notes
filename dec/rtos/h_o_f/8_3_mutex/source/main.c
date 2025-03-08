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

typedef struct LED_Vals
{
 volatile bool RGB_RED;
 volatile bool RGB_GREEN;
 volatile bool RGB_BLUE;
}LED_Vals_t;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
//create storage for a pointer to a mutex
SemaphoreHandle_t mutexPtr = NULL;

LED_Vals_t LED_stat;
static volatile uint8_t state_counter=0;
/*******************************************************************************
* Function Prototypes
********************************************************************************/
void takeMutex1(void *argument)
{
	while(1)
	{
		if(xSemaphoreTake(mutexPtr, 500) == pdPASS)
		{
			Cy_GPIO_Write(LED_RGB_RED_PORT,LED_RGB_RED_PIN, LED_stat.RGB_RED);
			Cy_GPIO_Write(LED_RGB_GREEN_PORT,LED_RGB_GREEN_PIN, LED_stat.RGB_GREEN);
			Cy_GPIO_Write(LED_RGB_BLUE_PORT,LED_RGB_BLUE_PIN, LED_stat.RGB_BLUE);

			xSemaphoreGive(mutexPtr);
			vTaskDelay(1000);
		}
		else
		{
			Cy_GPIO_Inv(LED_RED_PORT,LED_RED_PIN);
		}
	}
}

void takeMutex2(void *argument)
{
	while(1)
	{
		if(xSemaphoreTake(mutexPtr, 500) == pdPASS)
		{
			state_counter++;
			state_counter%=8;

			LED_stat.RGB_RED=(state_counter & 0x1U);
			LED_stat.RGB_GREEN=(state_counter & 0x2U);
			LED_stat.RGB_BLUE=(state_counter & 0x4U);

			xSemaphoreGive(mutexPtr);

			/* If this delay wouldn't have been there then this tasks itself may acquire mutex again.
			 * Also, even if that doesn't happen, next scenario is highly likely. Since task1 has a
			 * delay of 1000, this tasks may acquire mutex, update value, release mutex and can then
			 * again acquire mutex and overwrite value without taking effect into task1. This is not
			 * a wanted situation.
			 */
			vTaskDelay(1000);
		}
		else
		{
			Cy_GPIO_Inv(LED_ORANGE_PORT,LED_ORANGE_PIN);
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

    /* create a mutex - note this is just a special case of a binary semaphore */
    mutexPtr = xSemaphoreCreateMutex();
    CY_ASSERT(mutexPtr != NULL);

    /* Create the task */
	if (xTaskCreate(takeMutex1, "mutex 1", TASK_STACK_SIZE, NULL, TASK_PRIORITY+2, NULL) == pdPASS)
	{
		if (xTaskCreate(takeMutex2, "mutex 2", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
