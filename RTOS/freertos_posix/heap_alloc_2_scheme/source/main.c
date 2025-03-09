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

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void Task1(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(ioss_0_port_1_pin_5_PORT,ioss_0_port_1_pin_5_PIN);
		printf("task-1\r\n");
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
	}
}

void Task2(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(ioss_0_port_1_pin_5_PORT,ioss_0_port_1_pin_5_PIN);
		printf("task-2\r\n");
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		Cy_GPIO_Inv(ioss_0_port_1_pin_5_PORT,ioss_0_port_1_pin_5_PIN);
		printf("idle task\r\n");
		Cy_SysLib_Delay(1000U);
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
    printf("Heap 2 mem allocation example, page 56-58, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Create the task */
	if (xTaskCreate(Task1, "task1", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
	{
		if (xTaskCreate(Task2, "task2", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* In heap scheme 2, memory gets allocated by array ucHeap in heap_2.c
			 * and size is defined by configTOTAL_HEAP_SIZE in FreeRTOSConfig.h. This
			 * array gets allocated in .bss at compile time and then heap is emulated on
			 * that and using that heap, task stacks and TCB etc. are created. Currently
			 * allocated blocks are maintained using linked list. For details, check
			 * implementation */

			/* It uses a best fit algorithm to allocate memory and, unlike heap_1, it does allow
			 * memory to be freed. Again, the array is statically declared, so will make the
			 * application appear to consume a lot of RAM, even before any memory from the array
			 * has been assigned. The best fit algorithm ensures that pvPortMalloc() uses the free
			 * block of memory that is closest in size to the number of bytes requested. */

			/* Unlike heap_4, Heap_2 does not combine adjacent free blocks into a single larger
			 * block, so it is more susceptible to fragmentation. However, fragmentation is not
			 * an issue if the blocks being allocated and subsequently freed are always the same
			 * size. Heap_2 is suitable for an application that creates and deletes tasks repeatedly,
			 * provided the size of the stack allocated to the created tasks does not change. */

			/* Heap_2 is not deterministic, but is faster than most standard library implementations
			 * of malloc() and free(). */

			/* It maintains xStart and xEnd static linked lists within heap_2.c and allocates memory
			 * blocks using those. Informations about allocated and free memory blocks is maintained
			 * in xStart linked list. xEnd is just used to mark/store end of heap memory ? Also
			 * first bit of xStart->xBlockSize indicates if that memory block is free or not. Is set
			 * to 0 then it is free otherwise allocated to application. */
			void* mem = pvPortMalloc((size_t)100U);

			/* In heap scheme 2, memory free is allowed but freed memory do not get merged */
			vPortFree(mem);

			/* should not use freed pointer, this just not to optimize mem variable */
			printf("%p\r\n",mem);

			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
