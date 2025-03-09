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

/* Define the start address and size of the three RAM regions. */
#define RAM1_SIZE ( 122880 )
#define RAM2_SIZE ( 222880 )

/*******************************************************************************
* Function Prototypes
********************************************************************************/

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void Task1(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(LED_ORANGE_PORT,LED_ORANGE_PIN);
		printf("task-1\r\n");
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
	}
}

void Task2(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(LED_RED_PORT,LED_RED_PIN);
		printf("task-2\r\n");
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		Cy_GPIO_Inv(LED_RGB_RED_PORT,LED_RED_RGB_PIN);
		printf("idle task\r\n");
	}
}

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;

static __attribute__ ((used,section(".bss"))) uint8_t ucHeap_5_part_1[RAM1_SIZE];
static __attribute__ ((used,section(".bss"))) uint8_t ucHeap_5_part_2[RAM2_SIZE];

/* Create an array of HeapRegion_t definitions, with an index for each of the three
RAM regions, and terminating the array with a NULL address. The HeapRegion_t
structures must appear in start address order, with the structure that contains the
lowest start address appearing first. */
const HeapRegion_t xHeapRegions[] =
{
 /* memory regions should not be in sequence.
  * Heap 5 scheme is for connecting regions not in sequence.
  * For regions in sequence, we can use Heap 4 scheme anyway */
 { ucHeap_5_part_2, RAM2_SIZE },
 { ucHeap_5_part_1, RAM1_SIZE },
 { NULL, 0 } /* Marks the end of the array. */
};

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
    printf("Heap 5 mem allocation example, page 61-66, 161204.pdf \r\n");
    printf("Took reference from https://community.nxp.com/t5/MCUXpresso-SDK-Knowledge-Base/Using-FreeRTOS-heap-5-in-MCUXpresso-SDK/ta-p/1180982 \r\n");
    printf("**********************************************************\r\n");

    /* Initialize heap_5. */
     vPortDefineHeapRegions(xHeapRegions);

    /* Create the task */
	if (xTaskCreate(Task1, "task1", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
	{
		if (xTaskCreate(Task2, "task2", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* In heap scheme 5, memory gets allocated by user in structure xHeapRegions in heap_5.c
			 * and individual blocks sizes are also defined there. Total size is defined by
			 * configTOTAL_HEAP_SIZE in FreeRTOSConfig.h. This array can be allocated in any section
			 * defined by user at compile time and then heap is emulated on that and using that heap,
			 * task stacks and TCB etc. are created. Currently allocated blocks are maintained using
			 * linked list. For details, check implementation */

			/* we can inspect free heap size using xPortGetFreeHeapSize. This is applicable to all
			 * allocation schemes. */
			size_t freeHeapSize = xPortGetFreeHeapSize();
			printf("free heap size=%d\r\n",freeHeapSize);

			void* mem = pvPortMalloc((size_t)100U);

			freeHeapSize = xPortGetFreeHeapSize();
			printf("free heap size=%d\r\n",freeHeapSize);

			/* In heap scheme 5, memory free is allowed and freed memory will get merged */
			vPortFree(mem);

			/* should not use freed pointer, this just not to optimize mem variable */
			printf("%p\r\n",mem);

			freeHeapSize = xPortGetFreeHeapSize();
			printf("free heap size=%d\r\n",freeHeapSize);

			size_t minimumEverFreeHeapSize = xPortGetMinimumEverFreeHeapSize();
			printf("minimum ever free heap size=%d\r\n",minimumEverFreeHeapSize );

			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
