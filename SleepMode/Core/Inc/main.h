/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gremsy_main.hpp"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PERIPHERAL_TIME TIME5_HANDLE
#define TIME5_ENABLE 1
#define SEL_MAV_Pin GPIO_PIN_13
#define SEL_MAV_GPIO_Port GPIOC
#define EN_33_SYS_Pin GPIO_PIN_14
#define EN_33_SYS_GPIO_Port GPIOC
#define ETH_SPI_CS_Pin GPIO_PIN_15
#define ETH_SPI_CS_GPIO_Port GPIOC
#define ADC_12_SOM_Pin GPIO_PIN_2
#define ADC_12_SOM_GPIO_Port GPIOA
#define ADC_12_SYS_Pin GPIO_PIN_3
#define ADC_12_SYS_GPIO_Port GPIOA
#define ADC_33_SYS_Pin GPIO_PIN_4
#define ADC_33_SYS_GPIO_Port GPIOA
#define IMU_CS_PIN_Pin GPIO_PIN_8
#define IMU_CS_PIN_GPIO_Port GPIOA
#define EN_GIMBAL_Pin GPIO_PIN_12
#define EN_GIMBAL_GPIO_Port GPIOA
#define EN_12_SOM_Pin GPIO_PIN_3
#define EN_12_SOM_GPIO_Port GPIOB
#define PHONE_ON_Pin GPIO_PIN_4
#define PHONE_ON_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define SWITCH_UART_SOM 1
#define UART1_ENABLE 1
#define UART3_ENABLE 1
#define UART4_ENABLE 1
#define DEBUG_UART UART1_HANDLE
#define I2C1_ENABLE 1
#define SPI2_ENABLE 1
#define ADC1_ENABLE 1
#define CONFIG_MAVLINK 1
#define GREMSY_SPI_ENABLE 1
#define GREMSY_MODULE_ETHERNET
#define ADC_CH7_Pin ADC_12_SOM_Pin 
#define ADC_CH7_GPIO_Port ADC_12_SOM_GPIO_Port
/*SOFTWARE VERSION*/
#define SW_MAJOR 1
#define SW_MINOR 0
#define SW_PATCH 1

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
