 
/** 
  ******************************************************************************
  * @file    hardware_h7_peripheral_config.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 19, 2023
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2023 Gremsy.  
  * All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_config.hpp"

#if defined STM32U5 || defined STM32H5
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/// GPIO _______________________________________________________________________
const uint16_t HARDWARE_GPIO_PIN[16] =
{
    GPIO_PIN_0,  GPIO_PIN_1,  GPIO_PIN_2,  GPIO_PIN_3,
    GPIO_PIN_4,  GPIO_PIN_5,  GPIO_PIN_6,  GPIO_PIN_7,
    GPIO_PIN_8,  GPIO_PIN_9,  GPIO_PIN_10, GPIO_PIN_11,
    GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15
};

const GPIO_TypeDef* HARDWARE_GPIO_PORT[16] =
{
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE,
#ifdef GPIOF
    GPIOF,
#else
    nullptr,
#endif
    GPIOG, GPIOH,
#ifdef GPIOI
    GPIOI,
#else
    nullptr,
#endif
};

/// TIME _______________________________________________________________________
#ifdef HAL_TIM_MODULE_ENABLED
const TimeConfigType HARDWARE_PERIPHERAL_TIME[TIME_COUNT] =
{
        /// NONE
        {"NONE", 0, 0, 0, 0, 0, 0, 0, 0, 0},

        /// TIME1
        #if (TIME1_ENABLE == 1)
        {
            "TIME1",
            TIME1_HANDLE,
            1,
            #ifdef TIME1_CH1_Pin
            TIME1_CH1_GPIO_Port,
            TIME1_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME1_CH2_Pin
            TIME1_CH2_GPIO_Port,
            TIME1_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME1_CH3_Pin
            TIME1_CH3_GPIO_Port,
            TIME1_CH3_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME1_CH4_Pin
            TIME1_CH4_GPIO_Port,
            TIME1_CH4_Pin,
            #else
            nullptr, 0,
            #endif
        },
        #endif

        /// TIME2
        #if (TIME2_ENABLE == 1)
        {
            "TIME2",
            TIME2_HANDLE,
            1,
            #ifdef TIME2_CH1_Pin
            TIME2_CH1_GPIO_Port,
            TIME2_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME2_CH2_Pin
            TIME2_CH2_GPIO_Port,
            TIME2_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME2_CH3_Pin
            TIME2_CH3_GPIO_Port,
            TIME2_CH3_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME2_CH4_Pin
            TIME2_CH4_GPIO_Port,
            TIME2_CH4_Pin,
            #else
            nullptr, 0,
            #endif
        },
        #endif

        /// TIME3
        #if (TIME3_ENABLE == 1)
        {
            "TIME3",
            TIME3_HANDLE,
            1,
            #ifdef TIME3_CH1_Pin
            TIME3_CH1_GPIO_Port,
            TIME3_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME3_CH2_Pin
            TIME3_CH2_GPIO_Port,
            TIME3_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME3_CH3_Pin
            TIME3_CH3_GPIO_Port,
            TIME3_CH3_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME3_CH4_Pin
            TIME3_CH4_GPIO_Port,
            TIME3_CH4_Pin,
            #else
            nullptr, 0,
            #endif
        },
        #endif

        /// TIME4
        #if (TIME4_ENABLE == 1)
        {
            "TIME4",
            TIME4_HANDLE,
            1,
            #ifdef TIME4_CH1_Pin
            TIME4_CH1_GPIO_Port,
            TIME4_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME4_CH2_Pin
            TIME4_CH2_GPIO_Port,
            TIME4_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME4_CH3_Pin
            TIME4_CH3_GPIO_Port,
            TIME4_CH3_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME4_CH4_Pin
            TIME4_CH4_GPIO_Port,
            TIME4_CH4_Pin,
            #else
            nullptr, 0,
            #endif
        },
        #endif

        /// TIME5
        #if (TIME5_ENABLE == 1)
        {
            "TIME5",
            TIME5_HANDLE,
            1,
            #ifdef TIME5_CH1_Pin
            TIME5_CH1_GPIO_Port,
            TIME5_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME5_CH2_Pin
            TIME5_CH2_GPIO_Port,
            TIME5_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME5_CH3_Pin
            TIME5_CH3_GPIO_Port,
            TIME5_CH3_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME5_CH4_Pin
            TIME5_CH4_GPIO_Port,
            TIME5_CH4_Pin,
            #else
            nullptr, 0,
            #endif
        },
        #endif

        /// TIME6
        #if (TIME6_ENABLE == 1)
        {
            "TIME6",
            TIME6_HANDLE,
            1,
            nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0
        },
        #endif

        /// TIME7
        #if (TIME7_ENABLE == 1)
        {
            "TIME7",
            TIME7_HANDLE,
            1,
            nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0
        },
        #endif

        /// TIME8
        #if (TIME8_ENABLE == 1)
        {
            "TIME8",
            TIME8_HANDLE,
            1,
            #ifdef TIME8_CH1_Pin
            TIME8_CH1_GPIO_Port,
            TIME8_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME8_CH2_Pin
            TIME8_CH2_GPIO_Port,
            TIME8_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME8_CH3_Pin
            TIME8_CH3_GPIO_Port,
            TIME8_CH3_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME8_CH4_Pin
            TIME8_CH4_GPIO_Port,
            TIME8_CH4_Pin,
            #else
            nullptr, 0,
            #endif
        },
        #endif

        /// TIME9
        #if (TIME9_ENABLE == 1)
        {"TIM9_NONE", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        #endif

        /// TIME10
        #if (TIME10_ENABLE == 1)
        {"TIM10_NONE", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        #endif

        /// TIME11
        #if (TIME11_ENABLE == 1)
        {"TIM11_NONE", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        #endif

        /// TIME12
        #if (TIME12_ENABLE == 1)
        {
            "TIME12",
            TIME12_HANDLE,
            1,
            #ifdef TIME12_CH1_Pin
            TIME12_CH1_GPIO_Port,
            TIME12_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME12_CH2_Pin
            TIME12_CH2_GPIO_Port,
            TIME12_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            nullptr, 0,
            nullptr, 0
        },
        #endif

        /// TIME13
        #if (TIME13_ENABLE == 1)
        {
            "TIME13",
            TIME13_HANDLE,
            1,
            #ifdef TIME13_CH1_Pin
            TIME13_CH1_GPIO_Port,
            TIME13_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            nullptr, 0,
            nullptr, 0,
            nullptr, 0
        },
        #endif

        /// TIME14
        #if (TIME14_ENABLE == 1)
        {
            "TIME14",
            TIME14_HANDLE,
            1,
            #ifdef TIME14_CH1_Pin
            TIME14_CH1_GPIO_Port,
            TIME14_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            nullptr, 0,
            nullptr, 0,
            nullptr, 0
        },
        #endif

        /// TIME15
        #if (TIME15_ENABLE == 1)
        {
            "TIME15",
            TIME15_HANDLE,
            1,
            #ifdef TIME15_CH1_Pin
            TIME15_CH1_GPIO_Port,
            TIME15_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            #ifdef TIME15_CH2_Pin
            TIME15_CH2_GPIO_Port,
            TIME15_CH2_Pin,
            #else
            nullptr, 0,
            #endif
            nullptr, 0,
            nullptr, 0
        },
        #endif

        /// TIME16
        #if (TIME16_ENABLE == 1)
        {
            "TIME16",
            TIME16_HANDLE,
            1,
            #ifdef TIME16_CH1_Pin
            TIME16_CH1_GPIO_Port,
            TIME16_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            nullptr, 0,
            nullptr, 0,
            nullptr, 0
        },
        #endif

        /// TIME17
        #if (TIME17_ENABLE == 1)
        {
            "TIME17",
            TIME17_HANDLE,
            1,
            #ifdef TIME17_CH1_Pin
            TIME17_CH1_GPIO_Port,
            TIME17_CH1_Pin,
            #else
            nullptr, 0,
            #endif
            nullptr, 0,
            nullptr, 0,
            nullptr, 0
        },
        #endif
};

#endif

/// UART _______________________________________________________________________
#ifdef HAL_UART_MODULE_ENABLED

const UartConfigType HARDWARE_UART_CONFIG[UART_COUNT] =
{
        {"NONE",0,0,0,0,0}, /// NONE

        #if (UART1_ENABLE == 1)
        {
            "UART1",
        #ifdef UART1_HANDLE
            UART1_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART1_TX_GPIO_Port
            UART1_TX_GPIO_Port,
            UART1_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART1_RX_GPIO_Port
            UART1_RX_GPIO_Port,
            UART1_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif

        #if (UART2_ENABLE == 1)
        {
            "UART2",
        #ifdef UART2_HANDLE
            UART2_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART2_TX_GPIO_Port
            UART2_TX_GPIO_Port,
            UART2_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART2_RX_GPIO_Port
            UART2_RX_GPIO_Port,
            UART2_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif

        #if (UART3_ENABLE == 1)
        {
            "UART3",
        #ifdef UART3_HANDLE
            UART3_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART3_TX_GPIO_Port
            UART3_TX_GPIO_Port,
            UART3_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART3_RX_GPIO_Port
            UART3_RX_GPIO_Port,
            UART3_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif

        #if (UART4_ENABLE == 1)
        {
            "UART4",
        #ifdef UART4_HANDLE
            UART4_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART4_TX_GPIO_Port
            UART4_TX_GPIO_Port,
            UART4_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART4_RX_GPIO_Port
            UART4_RX_GPIO_Port,
            UART4_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif

        #if (UART5_ENABLE == 1)
        {
            "UART5",
        #ifdef UART5_HANDLE
            UART5_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART5_TX_GPIO_Port
            UART5_TX_GPIO_Port,
            UART5_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART5_RX_GPIO_Port
            UART5_RX_GPIO_Port,
            UART5_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif

        #if (UART6_ENABLE == 1)
        {
            "UART6",
        #ifdef UART6_HANDLE
            UART6_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART6_TX_GPIO_Port
            UART6_TX_GPIO_Port,
            UART6_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART6_RX_GPIO_Port
            UART6_RX_GPIO_Port,
            UART6_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif

        #if (UART7_ENABLE == 1)
        {
            "UART7",
        #ifdef UART7_HANDLE
            UART7_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART7_TX_GPIO_Port
            UART7_TX_GPIO_Port,
            UART7_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART7_RX_GPIO_Port
            UART7_RX_GPIO_Port,
            UART7_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif

        #if (UART8_ENABLE == 1)
        {
            "UART8",
        #ifdef UART8_HANDLE
            UART8_HANDLE,
        #else
            nullptr,
        #endif
        #ifdef UART8_TX_GPIO_Port
            UART8_TX_GPIO_Port,
            UART8_TX_Pin,
        #else
            nullptr, 0,
        #endif
        #ifdef UART8_RX_GPIO_Port
            UART8_RX_GPIO_Port,
            UART8_RX_Pin,
        #else
            nullptr, 0,
        #endif
        },
        #endif
};

const uint32_t            HARDWARE_UART_PARITY[PARITY_COUNT] =
{
    UART_PARITY_NONE,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
};

const uint32_t            HARDWARE_UART_STOPBIT[STOPBITS_COUNT] =
{
    UART_STOPBITS_1,
    UART_STOPBITS_2
};

const uint32_t            HARDWARE_UART_WORDLENGTH[WORDLENGTH_COUNT] =
{
    UART_WORDLENGTH_8B,
    UART_WORDLENGTH_8B,
    UART_WORDLENGTH_9B,
};

const uint32_t            HARDWARE_UART_RX_LEVEL_INV[TTL_LEVEL_INV_COUNT] =
{
    UART_ADVFEATURE_RXINV_DISABLE,
    UART_ADVFEATURE_RXINV_DISABLE,
    UART_ADVFEATURE_RXINV_ENABLE,
};

const uint32_t            HARDWARE_UART_TX_LEVEL_INV[TTL_LEVEL_INV_COUNT] =
{
    UART_ADVFEATURE_TXINV_DISABLE,
    UART_ADVFEATURE_TXINV_DISABLE,
    UART_ADVFEATURE_TXINV_ENABLE,
};

#endif

#ifdef HAL_I2C_MODULE_ENABLED
const I2cConfigType HARDWARE_PERIPHERAL_I2C[I2C_COUNT] =
{
    /// NONE
    {"I2C_NONE", nullptr, nullptr, 0, nullptr, 0},

    /// I2C1
    #if (I2C1_ENABLE == 1)
    {
        "I2C1",
        I2C1_HANDLE,
        #ifdef I2C1_SDA_Pin
        I2C1_SDA_GPIO_Port,
        I2C1_SDA_Pin,
        #else
        nullptr, 0,
        #endif
        #ifdef I2C1_SCL_Pin
        I2C1_SCL_GPIO_Port,
        I2C1_SCL_Pin,
        #else
        nullptr, 0,
        #endif
    },
    #endif

    /// I2C2
    #if (I2C2_ENABLE == 1)
    {
        "I2C2",
        I2C2_HANDLE,
        #ifdef I2C2_SDA_Pin
        I2C2_SDA_GPIO_Port,
        I2C2_SDA_Pin,
        #else
        nullptr, 0,
        #endif
        #ifdef I2C2_SCL_Pin
        I2C2_SCL_GPIO_Port,
        I2C2_SCL_Pin,
        #else
        nullptr, 0,
        #endif
    },
    #endif

    /// I2C3
    #if (I2C3_ENABLE == 1)
    {
        "I2C3",
        I2C3_HANDLE,
        #ifdef I2C3_SDA_Pin
        I2C3_SDA_GPIO_Port,
        I2C3_SDA_Pin,
        #else
        nullptr, 0,
        #endif
        #ifdef I2C3_SCL_Pin
        I2C3_SCL_GPIO_Port,
        I2C3_SCL_Pin,
        #else
        nullptr, 0,
        #endif
    },
    #endif

    /// I2C4
    #if (I2C4_ENABLE == 1)
    {
        "I2C4",
        I2C4_HANDLE,
        #ifdef I2C4_SDA_Pin
        I2C4_SDA_GPIO_Port,
        I2C4_SDA_Pin,
        #else
        nullptr, 0,
        #endif
        #ifdef I2C4_SCL_Pin
        I2C4_SCL_GPIO_Port,
        I2C4_SCL_Pin,
        #else
        nullptr, 0,
        #endif
    },
    #endif
};
#endif

/// ADC ________________________________________________________________________
#ifdef HAL_ADC_MODULE_ENABLED
const AdcConfigType HARDWARE_PERIPHERAL_ADC[ADC_COUNT] =
{
    {"ADC_NONE", nullptr},
#if (ADC1_ENABLE == 1)
    {"ADC1", ADC1_HANDLE},
#endif
#if (ADC2_ENABLE == 1)
    {"ADC2", ADC2_HANDLE},
#endif
#if (ADC4_ENABLE == 1)
    {"ADC4", ADC4_HANDLE},
#endif
};

const AdcChannelConfigType HARDWARE_PERIPERAL_ADC_CHANNEL[ADC_CHANNEL_COUNT] =
{
    {"ADC_CH_NONE", 0, nullptr, 0},
#ifdef ADC_CH0_Pin
    {"ADC_CH0", ADC_CHANNEL_0, ADC_CH0_GPIO_Port, ADC_CH0_Pin},
#endif
#ifdef ADC_CH1_Pin
    {"ADC_CH1", ADC_CHANNEL_1, ADC_CH1_GPIO_Port, ADC_CH1_Pin},
#endif
#ifdef ADC_CH2_Pin
    {"ADC_CH2", ADC_CHANNEL_2, ADC_CH2_GPIO_Port, ADC_CH2_Pin},
#endif
#ifdef ADC_CH3_Pin
    {"ADC_CH3", ADC_CHANNEL_3, ADC_CH3_GPIO_Port, ADC_CH3_Pin},
#endif
#ifdef ADC_CH4_Pin
    {"ADC_CH4", ADC_CHANNEL_4, ADC_CH4_GPIO_Port, ADC_CH4_Pin},
#endif
#ifdef ADC_CH5_Pin
    {"ADC_CH5", ADC_CHANNEL_5, ADC_CH5_GPIO_Port, ADC_CH5_Pin},
#endif
#ifdef ADC_CH6_Pin
    {"ADC_CH6", ADC_CHANNEL_6, ADC_CH6_GPIO_Port, ADC_CH6_Pin},
#endif
#ifdef ADC_CH7_Pin
    {"ADC_CH7", ADC_CHANNEL_7, ADC_CH7_GPIO_Port, ADC_CH7_Pin},
#endif
#ifdef ADC_CH8_Pin
    {"ADC_CH8", ADC_CHANNEL_8, ADC_CH8_GPIO_Port, ADC_CH8_Pin},
#endif
#ifdef ADC_CH9_Pin
    {"ADC_CH9", ADC_CHANNEL_9, ADC_CH9_GPIO_Port, ADC_CH9_Pin},
#endif
#ifdef ADC_CH10_Pin
    {"ADC_CH10", ADC_CHANNEL_10, ADC_CH10_GPIO_Port, ADC_CH10_Pin},
#endif
#ifdef ADC_CH11_Pin
    {"ADC_CH11", ADC_CHANNEL_11, ADC_CH11_GPIO_Port, ADC_CH11_Pin},
#endif
#ifdef ADC_CH12_Pin
    {"ADC_CH12", ADC_CHANNEL_12, ADC_CH12_GPIO_Port, ADC_CH12_Pin},
#endif
#ifdef ADC_CH13_Pin
    {"ADC_CH13", ADC_CHANNEL_13, ADC_CH13_GPIO_Port, ADC_CH13_Pin},
#endif
#ifdef ADC_CH14_Pin
    {"ADC_CH14", ADC_CHANNEL_14, ADC_CH14_GPIO_Port, ADC_CH14_Pin},
#endif
#ifdef ADC_CH15_Pin
    {"ADC_CH15", ADC_CHANNEL_15, ADC_CH15_GPIO_Port, ADC_CH15_Pin},
#endif
#ifdef ADC_CH_TEMPERATURE
    {"ADC_CH_TEMPERATURE", ADC_CHANNEL_TEMPERATURE, nullptr, 0},
#endif
};
#endif

/// EXTI _______________________________________________________________________
const IRQn_Type HARDWARE_PERIPHERAL_EXTI_IRQn[16] =
{
    EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, EXTI5_IRQn,
    EXTI6_IRQn, EXTI7_IRQn, EXTI8_IRQn, EXTI9_IRQn, EXTI10_IRQn,
    EXTI11_IRQn, EXTI12_IRQn, EXTI13_IRQn, EXTI14_IRQn, EXTI15_IRQn,
};

const uint32_t HARDWARE_PERIPHERAL_EXTI_MODE[EXTI_MODE_COUNT] = {
    GPIO_MODE_IT_FALLING, GPIO_MODE_IT_RISING, GPIO_MODE_IT_FALLING, GPIO_MODE_IT_RISING_FALLING,
};

/// SPI ________________________________________________________________________
#ifdef HAL_SPI_MODULE_ENABLED
const SpiConfigType HARDWARE_SPI_CONFIG[SPI_COUNT] =
{
    {"NONE", nullptr, nullptr, 0, nullptr, 0, nullptr, 0},
#if (SPI1_ENABLE == 1)
    {
        "SPI1",
        SPI1_HANDLE,
    #ifdef SPI1_SCK_Pin
        SPI1_SCK_GPIO_Port,
        SPI1_SCK_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI1_MISO_Pin
        SPI1_MISO_GPIO_Port,
        SPI1_MISO_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI1_MOSI_Pin
        SPI1_MOSI_GPIO_Port,
        SPI1_MOSI_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
#if (SPI2_ENABLE == 1)
    {
        "SPI2",
        SPI2_HANDLE,
    #ifdef SPI2_SCK_Pin
        SPI2_SCK_GPIO_Port,
        SPI2_SCK_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI2_MISO_Pin
        SPI2_MISO_GPIO_Port,
        SPI2_MISO_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI2_MOSI_Pin
        SPI2_MOSI_GPIO_Port,
        SPI2_MOSI_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
#if (SPI3_ENABLE == 1)
    {
        "SPI3",
        SPI3_HANDLE,
    #ifdef SPI3_SCK_Pin
        SPI3_SCK_GPIO_Port,
        SPI3_SCK_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI3_MISO_Pin
        SPI3_MISO_GPIO_Port,
        SPI3_MISO_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI3_MOSI_Pin
        SPI3_MOSI_GPIO_Port,
        SPI3_MOSI_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
#if (SPI4_ENABLE == 1)
    {
        "SPI4",
        SPI4_HANDLE,
    #ifdef SPI4_SCK_Pin
        SPI4_SCK_GPIO_Port,
        SPI4_SCK_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI4_MISO_Pin
        SPI4_MISO_GPIO_Port,
        SPI4_MISO_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI4_MOSI_Pin
        SPI4_MOSI_GPIO_Port,
        SPI4_MOSI_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
#if (SPI5_ENABLE == 1)
    {
        "SPI5",
        SPI5_HANDLE,
    #ifdef SPI5_SCK_Pin
        SPI5_SCK_GPIO_Port,
        SPI5_SCK_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI5_MISO_Pin
        SPI5_MISO_GPIO_Port,
        SPI5_MISO_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI5_MOSI_Pin
        SPI5_MOSI_GPIO_Port,
        SPI5_MOSI_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
#if (SPI6_ENABLE == 1)
    {
        "SPI6",
        SPI6_HANDLE,
    #ifdef SPI6_SCK_Pin
        SPI6_SCK_GPIO_Port,
        SPI6_SCK_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI6_MISO_Pin
        SPI6_MISO_GPIO_Port,
        SPI6_MISO_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef SPI6_MOSI_Pin
        SPI6_MOSI_GPIO_Port,
        SPI6_MOSI_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
};

const uint32_t HARDWARE_SPI_DATA_SIZE[DATA_SIZE_COUNT] =
{
    0,
    SPI_DATASIZE_8BIT,
    SPI_DATASIZE_16BIT,
};

const uint32_t HARDWARE_SPI_POLARITY[POLARITY_COUNT] =
{
    0,
    SPI_POLARITY_LOW,
    SPI_POLARITY_HIGH
};

const uint32_t HARDWARE_SPI_PHASE[PHASE_COUNT] =
{
    0,
    SPI_PHASE_1EDGE,
    SPI_PHASE_2EDGE
};

const uint32_t HARDWARE_SPI_BAUDRATE_PRESCALER[BAUDRATE_PRESCALER_COUNT] =
{
    0,
    SPI_BAUDRATEPRESCALER_2,
    SPI_BAUDRATEPRESCALER_4,
    SPI_BAUDRATEPRESCALER_8,
    SPI_BAUDRATEPRESCALER_16,
    SPI_BAUDRATEPRESCALER_32,
    SPI_BAUDRATEPRESCALER_64,
    SPI_BAUDRATEPRESCALER_128,
    SPI_BAUDRATEPRESCALER_256
};
#endif

/// CANBUS (FDCAN) _____________________________________________________________
#ifdef HAL_FDCAN_MODULE_ENABLED
const CanbusConfigType HARDWARE_CAN_CONFIG[CAN_COUNT] =
{
    {"NONE", nullptr, nullptr, 0, nullptr, 0},
#if (CAN1_ENABLE == 1)
    {
        "CAN1",
        CAN1_HANDLE,
    #ifdef CAN1_TX_Pin
        CAN1_TX_GPIO_Port,
        CAN1_TX_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef CAN1_RX_Pin
        CAN1_RX_GPIO_Port,
        CAN1_RX_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
#if (CAN2_ENABLE == 1)
    {
        "CAN2",
        CAN2_HANDLE,
    #ifdef CAN2_TX_Pin
        CAN2_TX_GPIO_Port,
        CAN2_TX_Pin,
    #else
        nullptr, 0,
    #endif
    #ifdef CAN2_RX_Pin
        CAN2_RX_GPIO_Port,
        CAN2_RX_Pin,
    #else
        nullptr, 0,
    #endif
    },
#endif
};
#endif

/// FLASH ______________________________________________________________________
const SectorConfigType HARDWARE_SECTOR_CONFIG[SECTOR_COUNT] =
{
    { "NONE", 0, 0, 0, 0 },
    { "PAGE0", 0, 0x08000000, 8 * KBYTE, 0 },
    { "PAGE1", 1, 0x08002000, 8 * KBYTE, 1 },
    { "PAGE2", 2, 0x08004000, 8 * KBYTE, 2 },
    { "PAGE3", 3, 0x08006000, 8 * KBYTE, 3 },
    { "PAGE4", 4, 0x08008000, 8 * KBYTE, 4 },
    { "PAGE5", 5, 0x0800A000, 8 * KBYTE, 5 },
    { "PAGE6", 6, 0x0800C000, 8 * KBYTE, 6 },
    { "PAGE7", 7, 0x0800E000, 8 * KBYTE, 7 },
    { "PAGE8", 8, 0x08010000, 8 * KBYTE, 8 },
    { "PAGE9", 9, 0x08012000, 8 * KBYTE, 9 },
    { "PAGE10", 10, 0x08014000, 8 * KBYTE, 10 },
    { "PAGE11", 11, 0x08016000, 8 * KBYTE, 11 },
};

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/// COMMON _____________________________________________________________________
/** @brief ham cai dat chung cho ngoai vi
 *  @param none
 *  @return none
 * */
void g_hardware_init(void)
{
    /// RCC GPIO ENABLE
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

}

/** @brief ham delay cua hardware
 *  @param[in] time thoi gian tinh theo ms
 *  @return none
 * */
void g_hardware_delay(int time)
{
    HAL_Delay(time);
}

/// GPIO _______________________________________________________________________
/** @brief ham chuyen doi port & pin sang kieu pin_t cua thu vien
 *  @param[in] port dia chi port trong Gpio cua hall
 *  @param[in] pin so chan trong gpio cua hall
 *  @return so chan trong PinEnum
 * */
PinEnum g_hardware_convert_gpio_to_pin_enum(const GPIO_TypeDef* port, uint16_t pin)
{
    uint8_t PIN  = 0xFF;
    uint8_t PORT = 0xFF;

    uint16_t i;

    /// Check
    if(port == 0)
    {
        return PIN_NONE;
    }

    /// PIN
    for(i = 0; i < 16; i++)
    {
        if(HARDWARE_GPIO_PIN[i] == pin)
        {
            PIN = i;
            i = 16;
        }
    }

    if(PIN == 0xFF)
    {
        return PIN_NONE;
    }

    /// PORT
    for(i = 0; i < 16; i++)
    {
        if(HARDWARE_GPIO_PORT[i] == port)
        {
            PORT = i;
            i = 16;
        }
    }

    if(PORT == 0xFF)
    {
        return PIN_NONE;
    }

    /// Gep Pin
    PIN = ((PORT<<4)&0xF0)|(PIN&0x0F);
    PIN += 1;

    return (PinEnum)PIN;
}

#endif /** STM32U5 */
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
