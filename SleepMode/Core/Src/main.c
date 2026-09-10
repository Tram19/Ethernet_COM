/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "cmsis_os2.h"
#include "adc.h"
#include "gpio.h"
#include "gpdma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "wwdg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>   // snprintf
#include <string.h>  // memset, strlen
#include "FreeRTOS.h"
#include "task.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void CheckResetCause(void) {
    char buf[64];
    int len = 0;

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) {
        len = snprintf(buf, sizeof(buf),
            "*** Last reset: WWDG (system hang) ***\r\n");
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        len = snprintf(buf, sizeof(buf),
            "*** Last reset: IWDG ***\r\n");
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
        len = snprintf(buf, sizeof(buf),
            "Last reset: Software\r\n");
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST)) {
        len = snprintf(buf, sizeof(buf),
            "Last reset: Brownout (power issue)\r\n");
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
        len = snprintf(buf, sizeof(buf),
            "Last reset: PIN reset\r\n");
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST)) {
        len = snprintf(buf, sizeof(buf),
            "Last reset: Option byte loader\r\n");
    }

    if (len > 0) {
        HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 500);
    }
    __HAL_RCC_CLEAR_RESET_FLAGS();
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    char buf[64];
    int len = 0;
    len = snprintf(buf, sizeof(buf),
        "*** STACK OVERFLOW in task: %s ***\r\n", pcTaskName);
    if (len > 0) {
        HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 500);
    }
    __disable_irq();
    while(1) { __NOP(); }   // dừng lại đây để attach debugger, xem call stack lúc crash
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  // Bit số 1 tương ứng với GPIOBEN
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) /* && TAMP->BKP0R == 0xAA55AA55 */)
  {
    RCC->AHB2ENR1 |= (1U << 1); 
    __DSB(); // Đợi 1-2 chu kỳ để Clock ổn định đi tới ngoại vi

  // (Tùy chọn) Bật clock cho Backup Domain nếu bạn lưu trạng thái trong Backup SRAM/Registers
  // __HAL_RCC_PWR_CLK_ENABLE(); 
  // HAL_PWR_EnableBkUpAccess();

  // Kiểm tra nếu là WWDG Reset và có cờ lưu trạng thái

      /* 2. Ghi mức logic CAO (SET) vào chân PB13 TRƯỚC KHI chuyển thành Output */
      // Sử dụng thanh ghi BSRR (Bit Set/Reset Register) để Set bit 13.
      // Thanh ghi này an toàn và thao tác nhanh hơn 1 chu kỳ so với việc dùng |= trên ODR.
      GPIOB->BSRR = (1U << 13); // Ép mức HIGH ngay

      /* 3. Cấu hình Pull-down cho PB13 */
      // Thanh ghi PUPDR cần 2 bit cho mỗi chân. PB13 nằm ở bit 27:26.
      // Giá trị cấu hình: 00 = No Pull, 01 = Pull-up, 10 = Pull-down.
      GPIOB->PUPDR = (GPIOB->PUPDR & ~(3U << (13 * 2))) | (2U << (13 * 2));

      /* 4. Cấu hình PB13 thành Output Push-Pull */
      // Thanh ghi MODER cũng cần 2 bit cho mỗi chân. PB13 nằm ở bit 27:26.
      // Giá trị cấu hình: 00 = Input, 01 = Output, 10 = Alternate Function, 11 = Analog.
      GPIOB->MODER = (GPIOB->MODER & ~(3U << (13 * 2))) | (1U << (13 * 2));
  }
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  // if (!__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))   
   MX_GPIO_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  
  MX_GPDMA1_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM5_Init();
  MX_WWDG_Init();
  /* USER CODE BEGIN 2 */

  //MX_GPIO_Init_Struct();
  CheckResetCause();

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Call init function for freertos objects (in app_freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV2;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 1;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
