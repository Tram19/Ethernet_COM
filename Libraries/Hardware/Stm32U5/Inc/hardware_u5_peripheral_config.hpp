 
/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_config.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   This file contains all the functions prototypes for the hardware_u5_peripheral_config.cpp 
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2018 Gremsy. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_CONFIG_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_CONFIG_HPP__
#include "Hardware/Base/Inc/hardware_base.hpp"

#if defined STM32U5 || defined STM32H5
/* Exported types ------------------------------------------------------------*/

/// TIME _______________________________________________________________________
#ifdef HAL_TIM_MODULE_ENABLED

#define GREMSY_TIME_ENABLE  1

#if (TIME1_ENABLE == 1)
extern TIM_HandleTypeDef        htim1;
#define TIME1_HANDLE            (&htim1)
#endif

#if (TIME2_ENABLE == 1)
extern TIM_HandleTypeDef        htim2;
#define TIME2_HANDLE            (&htim2)
#endif

#if (TIME3_ENABLE == 1)
extern TIM_HandleTypeDef        htim3;
#define TIME3_HANDLE            (&htim3)
#endif

#if (TIME4_ENABLE == 1)
extern TIM_HandleTypeDef        htim4;
#define TIME4_HANDLE            (&htim4)
#endif

#if (TIME5_ENABLE == 1)
extern TIM_HandleTypeDef        htim5;
#define TIME5_HANDLE            (&htim5)
#endif

#if (TIME6_ENABLE == 1)
extern TIM_HandleTypeDef        htim6;
#define TIME6_HANDLE            (&htim6)
#endif

#if (TIME7_ENABLE == 1)
extern TIM_HandleTypeDef        htim7;
#define TIME7_HANDLE            (&htim7)
#endif

#if (TIME8_ENABLE == 1)
extern TIM_HandleTypeDef        htim8;
#define TIME8_HANDLE            (&htim8)
#endif

#if (TIME9_ENABLE == 1)
extern TIM_HandleTypeDef        htim9;
#define TIME9_HANDLE            (&htim9)
#endif

#if (TIME10_ENABLE == 1)
extern TIM_HandleTypeDef        htim10;
#define TIME10_HANDLE            (&htim10)
#endif

#if (TIME11_ENABLE == 1)
extern TIM_HandleTypeDef        htim11;
#define TIME11_HANDLE            (&htim11)
#endif

#if (TIME12_ENABLE == 1)
extern TIM_HandleTypeDef        htim12;
#define TIME12_HANDLE            (&htim12)
#endif

#if (TIME13_ENABLE == 1)
extern TIM_HandleTypeDef        htim13;
#define TIME13_HANDLE            (&htim13)
#endif

#if (TIME14_ENABLE == 1)
extern TIM_HandleTypeDef        htim14;
#define TIME14_HANDLE            (&htim14)
#endif

#if (TIME15_ENABLE == 1)
extern TIM_HandleTypeDef        htim15;
#define TIME15_HANDLE            (&htim15)
#endif

/** @brief dinh ngia struct chua cac bien duoc cai dat trong stm32ide */
typedef struct
{
    const char*         name;
    TIM_HandleTypeDef*  handle;
    uint32_t            systemClockPrescaler;
    GPIO_TypeDef*       ch1Port;
    uint32_t            ch1Pin;
    GPIO_TypeDef*       ch2Port;
    uint32_t            ch2Pin;
    GPIO_TypeDef*       ch3Port;
    uint32_t            ch3Pin;
    GPIO_TypeDef*       ch4Port;
    uint32_t            ch4Pin;
}
TimeConfigType;

#endif


/// UART _______________________________________________________________________
#ifdef HAL_UART_MODULE_ENABLED

#define GREMSY_UART_ENABLE      1

#if (UART1_ENABLE == 1)
extern UART_HandleTypeDef       huart1;
#define UART1_HANDLE            (&huart1)
#endif

#if (UART2_ENABLE == 1)
extern UART_HandleTypeDef       huart2;
#define UART2_HANDLE            (&huart2)
#endif

#if (UART3_ENABLE == 1)
extern UART_HandleTypeDef       huart3;
#define UART3_HANDLE            (&huart3)
#endif

#if (UART4_ENABLE == 1)
extern UART_HandleTypeDef       huart4;
#define UART4_HANDLE            (&huart4)
#endif

#if (UART5_ENABLE == 1)
extern UART_HandleTypeDef       huart5;
#define UART5_HANDLE            (&huart5)
#endif

#if (UART6_ENABLE == 1)
extern UART_HandleTypeDef       huart6;
#define UART6_HANDLE            (&huart6)
#endif

#if (UART7_ENABLE == 1)
extern UART_HandleTypeDef       huart7;
#define UART7_HANDLE            (&huart7)
#endif

#if (UART8_ENABLE == 1)
extern UART_HandleTypeDef       huart8;
#define UART8_HANDLE            (&huart8)
#endif

/** @brief dinh ngia struct chua cac bien duoc cai dat trong stm32ide */
typedef struct
{
    const char*         name;
    UART_HandleTypeDef* handle;
    GPIO_TypeDef*       txPort;
    uint32_t            txPin;
    GPIO_TypeDef*       rxPort;
    uint32_t            rxPin;
}
UartConfigType;

#endif

/* Exported constants --------------------------------------------------------*/

/// GPIO _______________________________________________________________________
extern  const uint16_t HARDWARE_GPIO_PIN[16];
extern const GPIO_TypeDef* HARDWARE_GPIO_PORT[16];

/// TIME _______________________________________________________________________
#ifdef HAL_TIM_MODULE_ENABLED
extern const TimeConfigType HARDWARE_PERIPHERAL_TIME[TIME_COUNT];
#endif

/// UART _______________________________________________________________________
#ifdef HAL_UART_MODULE_ENABLED
extern const UartConfigType HARDWARE_UART_CONFIG[UART_COUNT];
extern const uint32_t HARDWARE_UART_PARITY[PARITY_COUNT];
extern const uint32_t HARDWARE_UART_STOPBIT[STOPBITS_COUNT];
extern const uint32_t HARDWARE_UART_WORDLENGTH[WORDLENGTH_COUNT];
extern const uint32_t HARDWARE_UART_RX_LEVEL_INV[TTL_LEVEL_INV_COUNT];
extern const uint32_t HARDWARE_UART_TX_LEVEL_INV[TTL_LEVEL_INV_COUNT];
#endif
 
/* Exported handle declarations ----------------------------------------------*/
#ifdef HAL_I2C_MODULE_ENABLED

#define GREMSY_I2C_ENABLE      1

#if (I2C1_ENABLE == 1)
extern I2C_HandleTypeDef    hi2c1;
#define I2C1_HANDLE         (&hi2c1)
#endif
 
#if (I2C2_ENABLE == 1)
extern I2C_HandleTypeDef    hi2c2;
#define I2C2_HANDLE         (&hi2c2)
#endif
 
#if (I2C3_ENABLE == 1)
extern I2C_HandleTypeDef    hi2c3;
#define I2C3_HANDLE         (&hi2c3)
#endif
 
#if (I2C4_ENABLE == 1)
extern I2C_HandleTypeDef    hi2c4;
#define I2C4_HANDLE         (&hi2c4)
#endif

typedef struct
{
    const char*         name;
    I2C_HandleTypeDef*  handle;
    GPIO_TypeDef*       sdaPort;
    uint32_t            sdaPin;
    GPIO_TypeDef*       sclPort;
    uint32_t            sclPin;
} I2cConfigType;

extern const I2cConfigType HARDWARE_PERIPHERAL_I2C[I2C_COUNT];

#endif

/// ADC ________________________________________________________________________
#ifdef HAL_ADC_MODULE_ENABLED

#define GREMSY_ADC_ENABLE  1

#if (ADC1_ENABLE == 1)
extern ADC_HandleTypeDef    hadc1;
#define ADC1_HANDLE         (&hadc1)
#endif

#if (ADC2_ENABLE == 1)
extern ADC_HandleTypeDef    hadc2;
#define ADC2_HANDLE         (&hadc2)
#endif

#if (ADC4_ENABLE == 1)
extern ADC_HandleTypeDef    hadc4;
#define ADC4_HANDLE         (&hadc4)
#endif

typedef struct
{
    const char*         name;
    ADC_HandleTypeDef*  handle;
}
AdcConfigType;

typedef enum
{
    ADC_CHANNEL_NONE,

#ifdef ADC_CH0_Pin
    ADC_CHANNEL0,
#endif

#ifdef ADC_CH1_Pin
    ADC_CHANNEL1,
#endif

#ifdef ADC_CH2_Pin
    ADC_CHANNEL2,
#endif

#ifdef ADC_CH3_Pin
    ADC_CHANNEL3,
#endif

#ifdef ADC_CH4_Pin
    ADC_CHANNEL4,
#endif

#ifdef ADC_CH5_Pin
    ADC_CHANNEL5,
#endif

#ifdef ADC_CH6_Pin
    ADC_CHANNEL6,
#endif

#ifdef ADC_CH7_Pin
    ADC_CHANNEL7,
#endif

#ifdef ADC_CH8_Pin
    ADC_CHANNEL8,
#endif

#ifdef ADC_CH9_Pin
    ADC_CHANNEL9,
#endif

#ifdef ADC_CH10_Pin
    ADC_CHANNEL10,
#endif

#ifdef ADC_CH11_Pin
    ADC_CHANNEL11,
#endif

#ifdef ADC_CH12_Pin
    ADC_CHANNEL12,
#endif

#ifdef ADC_CH13_Pin
    ADC_CHANNEL13,
#endif

#ifdef ADC_CH14_Pin
    ADC_CHANNEL14,
#endif

#ifdef ADC_CH15_Pin
    ADC_CHANNEL15,
#endif

#ifdef ADC_CH_TEMPERATURE
    ADC_CHANNEL_TEMPERATURE,
#endif

    ADC_CHANNEL_COUNT,
}
AdcChannelEnum;

#define IS_ANALOG_CHANNEL(ch)        ((ch > ADC_CHANNEL_NONE) && (ch < ADC_CHANNEL_COUNT))

typedef struct
{
    const char*     name;
    uint32_t        halChannel;
    GPIO_TypeDef*   halPort;
    uint32_t        halPin;
}
AdcChannelConfigType;

extern const AdcConfigType HARDWARE_PERIPHERAL_ADC[ADC_COUNT];
extern const AdcChannelConfigType HARDWARE_PERIPERAL_ADC_CHANNEL[ADC_CHANNEL_COUNT];

#endif

/// EXTI _______________________________________________________________________
extern const IRQn_Type HARDWARE_PERIPHERAL_EXTI_IRQn[16];
extern const uint32_t HARDWARE_PERIPHERAL_EXTI_MODE[EXTI_MODE_COUNT];

/// SPI ________________________________________________________________________
#ifdef HAL_SPI_MODULE_ENABLED

#define GREMSY_SPI_ENABLE      1

#if (SPI1_ENABLE == 1)
extern SPI_HandleTypeDef       hspi1;
#define SPI1_HANDLE            (&hspi1)
#endif

#if (SPI2_ENABLE == 1)
extern SPI_HandleTypeDef       hspi2;
#define SPI2_HANDLE            (&hspi2)
#endif

#if (SPI3_ENABLE == 1)
extern SPI_HandleTypeDef       hspi3;
#define SPI3_HANDLE            (&hspi3)
#endif

#if (SPI4_ENABLE == 1)
extern SPI_HandleTypeDef       hspi4;
#define SPI4_HANDLE            (&hspi4)
#endif

#if (SPI5_ENABLE == 1)
extern SPI_HandleTypeDef       hspi5;
#define SPI5_HANDLE            (&hspi5)
#endif

#if (SPI6_ENABLE == 1)
extern SPI_HandleTypeDef       hspi6;
#define SPI6_HANDLE            (&hspi6)
#endif

typedef struct
{
    const char*         name;
    SPI_HandleTypeDef*  handle;
    GPIO_TypeDef*       sckPort;
    uint32_t            sckPin;
    GPIO_TypeDef*       misoPort;
    uint32_t            misoPin;
    GPIO_TypeDef*       mosiPort;
    uint32_t            mosiPin;
}
SpiConfigType;

extern const SpiConfigType HARDWARE_SPI_CONFIG[SPI_COUNT];
extern const uint32_t HARDWARE_SPI_DATA_SIZE[DATA_SIZE_COUNT];
extern const uint32_t HARDWARE_SPI_POLARITY[POLARITY_COUNT];
extern const uint32_t HARDWARE_SPI_PHASE[PHASE_COUNT];
extern const uint32_t HARDWARE_SPI_BAUDRATE_PRESCALER[BAUDRATE_PRESCALER_COUNT];

#endif

/// CANBUS (FDCAN for Stm32U5) _________________________________________________
#ifdef HAL_FDCAN_MODULE_ENABLED

#define GREMSY_CAN_ENABLE      1

#if (CAN1_ENABLE == 1)
extern FDCAN_HandleTypeDef     hfdcan1;
#define CAN1_HANDLE            (&hfdcan1)
#endif

#if (CAN2_ENABLE == 1)
extern FDCAN_HandleTypeDef     hfdcan2;
#define CAN2_HANDLE            (&hfdcan2)
#endif

typedef struct
{
    const char*          name;
    FDCAN_HandleTypeDef* handle;
    GPIO_TypeDef*        txPort;
    uint32_t             txPin;
    GPIO_TypeDef*        rxPort;
    uint32_t             rxPin;
}
CanbusConfigType;

extern const CanbusConfigType HARDWARE_CAN_CONFIG[CAN_COUNT];

#endif

/// FLASH ______________________________________________________________________
typedef struct
{
    const char* name;
    uint8_t     id;
    uint32_t    add;
    uint32_t    size;
    uint32_t    sector; // Maps to Page number on Stm32U5
}
SectorConfigType;

#define IS_SECTOR_ID(sector)    ((sector > SECTOR0) && (sector < SECTOR_COUNT))
extern const SectorConfigType HARDWARE_SECTOR_CONFIG[SECTOR_COUNT];

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/// COMMON _____________________________________________________________________
/** @brief ham cai dat chung cho ngoai vi
 *  @param none
 *  @return none
 * */
void g_hardware_init(void);

/** @brief ham delay cua hardware
 *  @param[in] time thoi gian tinh theo ms
 *  @return none
 * */
void g_hardware_delay(int time);

/// GPIO _______________________________________________________________________
/** @brief ham chuyen doi port & pin sang kieu pin_t cua thu vien
 *  @param[in] port dia chi port trong Gpio cua hall
 *  @param[in] pin so chan trong gpio cua hall
 *  @return so chan trong PinEnum
 * */
PinEnum g_hardware_convert_gpio_to_pin_enum(const GPIO_TypeDef* port, uint16_t pin);

#endif /** STM32U5  */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_CONFIG_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
