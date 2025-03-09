/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r12;
	uint32_t lr;
	uint32_t pc;
	uint32_t psr;
	#if defined(__ARM_ARCH_7EM__)
		uint32_t s[16];
	#endif
} ExceptionStackFrame;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// static FlagStatus prev_val=SET;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void GPIO_reg_analysis(void);
void dumpExceptionStack (ExceptionStackFrame* frame, uint32_t cfsr,
					     uint32_t mmfar, uint32_t bfar, uint32_t lr)
{
	printf ("Stack frame:\r\n");
	printf (" R0 = %08X\r\n", frame->r0);
	printf (" R1 = %08X\r\n", frame->r1);
	printf (" R2 = %08X\r\n", frame->r2);
	printf (" R3 = %08X\r\n", frame->r3);
	printf (" R12 = %08X\r\n", frame->r12);
	printf (" LR = %08X\r\n", frame->lr);
	printf (" PC = %08X\r\n", frame->pc);
	printf (" PSR = %08X\r\n", frame->psr);
	printf ("FSR/FAR:\r\n");
	printf (" CFSR = %08X\r\n", cfsr);
	printf (" HFSR = %08X\r\n", SCB->HFSR);
	printf (" DFSR = %08X\r\n", SCB->DFSR);
	printf (" AFSR = %08X\r\n", SCB->AFSR);

	if (cfsr & (1UL << 7))
	{
		printf (" MMFAR = %08X\r\n", mmfar);
	}
	if (cfsr & (1UL << 15))
	{
		printf (" BFAR = %08X\r\n", bfar);
	}
	printf ("Misc\r\n");
	printf (" LR/EXC_RETURN= %08X\r\n", lr);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("Interrupt analysis using GPIO push button\n\r");

  // This is wrong
  //dumpExceptionStack (frame, cfsr, mmfar, bfar, lr_reg);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void GPIO_reg_analysis(void)
{
	uint32_t mmfar = SCB->MMFAR; // MemManage Fault Address
	uint32_t bfar = SCB->BFAR;   // Bus Fault Address
	uint32_t cfsr = SCB->CFSR;   // Configurable Fault Status Registers
	uint32_t lr_reg;
	ExceptionStackFrame * frame;

	/* Store LR register which indicates possible values of EXC_RETURN in exception, not return address like normal function call */
	__asm ("MOV %[result], lr"
	  : [result] "=r" (lr_reg)
	);


	/* Based on EXC_RETURN value, decide MSP/PSP to use */
	/* In exception, MSP/PSP indicates core register content just at exception firing */
	asm volatile(
	" tst lr,#4 \r\n"
	" ite eq \r\n"
	" mrseq r0,msp \r\n"
	" mrsne r0,psp \r\n"
	);

	/* Store MSP/PSP value */
	__asm ("MOV %[result], r0"
		: [result] "=r" (frame)
	);

	dumpExceptionStack (frame, cfsr, mmfar, bfar, lr_reg);

	//if(GPIO_Pin == B1_Pin)
	//{
	//	prev_val=(prev_val==SET?RESET:SET);
	//	HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,prev_val);
	//}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


