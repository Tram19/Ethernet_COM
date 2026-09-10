 
/** 
  ******************************************************************************
  * @file    gremsy_include.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 11, 2023
  * @brief   This file contains all the functions prototypes for the gremsy_include.cpp 
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

#ifndef __GREMSY_INCLUDE_HPP__
#define __GREMSY_INCLUDE_HPP__

/* Includes ------------------------------------------------------------------*/
#define USING_STD

#ifdef USING_STD
#include <map>
#include <string>
using namespace std;
#endif

#include "Math/math.hpp"
#include "gremsy_component_hall_v1.hpp"
#include "gremsy_version.hpp"
#include "program.hpp"

#include "API/api_module_config.hpp"
#include "inplace_function.h"
/* Exported types ------------------------------------------------------------*/

/// STATUS _____________________________________________________________________
/** @brief dinh nghia trang thai su dung trong cac cai dat va ham tra ve trang thai
 *  @note cac phan tu trung lap voi cac status cua thu vien hall tra ve
 * */
typedef enum
{
    OK              = 0x00,
    ERROR_HAL       = 0x01,
    BUSY            = 0x02,
    ERROR_TIMEOUT   = 0x03,
    ERROR_PARAM,
    ERROR_EXIST,
    ERROR_MEMORY,
    ERROR_NOT_EXIST,
}
StatusEnum;

/// PERMISSION ________________________________________________________________________
/** @brief dinh nghia quyen doc ghi cua doi tuong
 * */
typedef enum
{
    R  = 0x01,
    W  = 0x02,
    RW = 0x03
}
WriteReadPermissionsEnum;

/** @brief dinh nghia quyen
 * */
typedef enum
{
    OWNER,
    EDITER,
    VIEWER,
    USER,
    AUTHOR_COUNT,
}
AuthorPermissionsEnum;


/// SYSTEM ____________________________________________________________________________
/** @breif dinh nghia trang thai hoat dong cua system
 * */
typedef enum
{
    OFF,
    ON,
    LOCK,
    EMERGENCY_STOP,
}
SystemStateEnum;


/// FREERTOS ___________________________________________________________________

/// Priority values.
typedef enum
{
    PRIORITY_NONE          =  0,         ///< No priority (not initialized).
    PRIORITY_IDLE          =  1,         ///< Reserved for Idle thread.
    PRIORITY_LOW           =  8,         ///< PRIORITY: low
    PRIORITY_LOW1          =  8+1,       ///< PRIORITY: low + 1
    PRIORITY_LOW2          =  8+2,       ///< PRIORITY: low + 2
    PRIORITY_LOW3          =  8+3,       ///< PRIORITY: low + 3
    PRIORITY_LOW4          =  8+4,       ///< PRIORITY: low + 4
    PRIORITY_LOW5          =  8+5,       ///< PRIORITY: low + 5
    PRIORITY_LOW6          =  8+6,       ///< PRIORITY: low + 6
    PRIORITY_LOW7          =  8+7,       ///< PRIORITY: low + 7
    PRIORITY_BELOW_NORMAL   = 16,         ///< PRIORITY: below normal
    PRIORITY_BELOW_NORMAL1  = 16+1,       ///< PRIORITY: below normal + 1
    PRIORITY_BELOW_NORMAL2  = 16+2,       ///< PRIORITY: below normal + 2
    PRIORITY_BELOW_NORMAL3  = 16+3,       ///< PRIORITY: below normal + 3
    PRIORITY_BELOW_NORMAL4  = 16+4,       ///< PRIORITY: below normal + 4
    PRIORITY_BELOW_NORMAL5  = 16+5,       ///< PRIORITY: below normal + 5
    PRIORITY_BELOW_NORMAL6  = 16+6,       ///< PRIORITY: below normal + 6
    PRIORITY_BELOW_NORMAL7  = 16+7,       ///< PRIORITY: below normal + 7
    PRIORITY_NORMAL        = 24,         ///< PRIORITY: normal
    PRIORITY_NORMAL1       = 24+1,       ///< PRIORITY: normal + 1
    PRIORITY_NORMAL2       = 24+2,       ///< PRIORITY: normal + 2
    PRIORITY_NORMAL3       = 24+3,       ///< PRIORITY: normal + 3
    PRIORITY_NORMAL4       = 24+4,       ///< PRIORITY: normal + 4
    PRIORITY_NORMAL5       = 24+5,       ///< PRIORITY: normal + 5
    PRIORITY_NORMAL6       = 24+6,       ///< PRIORITY: normal + 6
    PRIORITY_NORMAL7       = 24+7,       ///< PRIORITY: normal + 7
    PRIORITY_ABOVE_NORMAL   = 32,         ///< PRIORITY: above normal
    PRIORITY_ABOVE_NORMAL1  = 32+1,       ///< PRIORITY: above normal + 1
    PRIORITY_ABOVE_NORMAL2  = 32+2,       ///< PRIORITY: above normal + 2
    PRIORITY_ABOVE_NORMAL3  = 32+3,       ///< PRIORITY: above normal + 3
    PRIORITY_ABOVE_NORMAL4  = 32+4,       ///< PRIORITY: above normal + 4
    PRIORITY_ABOVE_NORMAL5  = 32+5,       ///< PRIORITY: above normal + 5
    PRIORITY_ABOVE_NORMAL6  = 32+6,       ///< PRIORITY: above normal + 6
    PRIORITY_ABOVE_NORMAL7  = 32+7,       ///< PRIORITY: above normal + 7
    PRIORITY_HIGH          = 40,         ///< PRIORITY: high
    PRIORITY_HIGH1         = 40+1,       ///< PRIORITY: high + 1
    PRIORITY_HIGH2         = 40+2,       ///< PRIORITY: high + 2
    PRIORITY_HIGH3         = 40+3,       ///< PRIORITY: high + 3
    PRIORITY_HIGH4         = 40+4,       ///< PRIORITY: high + 4
    PRIORITY_HIGH5         = 40+5,       ///< PRIORITY: high + 5
    PRIORITY_HIGH6         = 40+6,       ///< PRIORITY: high + 6
    PRIORITY_HIGH7         = 40+7,       ///< PRIORITY: high + 7
    PRIORITY_REALTIME      = 48,         ///< PRIORITY: realtime
    PRIORITY_REALTIME1     = 48+1,       ///< PRIORITY: realtime + 1
    PRIORITY_REALTIME2     = 48+2,       ///< PRIORITY: realtime + 2
    PRIORITY_REALTIME3     = 48+3,       ///< PRIORITY: realtime + 3
    PRIORITY_REALTIME4     = 48+4,       ///< PRIORITY: realtime + 4
    PRIORITY_REALTIME5     = 48+5,       ///< PRIORITY: realtime + 5
    PRIORITY_REALTIME6     = 48+6,       ///< PRIORITY: realtime + 6
    PRIORITY_REALTIME7     = 48+7,       ///< PRIORITY: realtime + 7
    PRIORITY_ISR           = 56,         ///< Reserved for ISR deferred thread.
    PRIORITY_ERROR         = -1,         ///< System cannot determine priority or illegal priority.
    PRIORITY_RESERVED      = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
}
PriorityEnum;


/// flag wait
typedef enum
{
    FLAGS_WAIT_ANY        = 0x00000000U,
    FLAGS_WAIT_ALL        = 0x00000001U,
    FLAGS_NO_CLEAR        = 0x00000002U,
}
FlagWaitOptionEnum;

/// COMANDLINE _________________________________________________________________
/// dinh nghia cac kieu command line
typedef enum
{
    COMMAND_LINE_FUNCTION_NO_RETURN,
    COMMAND_LINE_FUNCTION_RETURN_STRING,
    COMMAND_LINE_FUNCTION_RETURN_NUMBER,
}
CommandLineFuncTypeEnum;

/// GPIO _______________________________________________________________________
using TrigFuncCallBack = stdext::inplace_function<void(void), 128>;
// dinh nghia cho io
typedef enum
{
    NONE_PIN,
    INPUT_PIN,
    OUTPUT_PIN,
    IT_PIN,
    NUM_PIN
}
IOTypeEnum;

// dinh nghia cho io
typedef enum
{
    IT_RISING           = GPIO_MODE_IT_RISING,
    IT_FALLING          = GPIO_MODE_IT_FALLING,
    IT_RISING_FALLING   = GPIO_MODE_IT_RISING_FALLING
}
IOEdgeITEnum;
/// dinh nghia pin
typedef enum
{
    PIN_NONE,
    A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
    B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
    C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15,
    D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15,
    E0, E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12, E13, E14, E15,
    F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
    G0, G1, G2, G3, G4, G5, G6, G7, G8, G9, G10, G11, G12, G13, G14, G15,
    H0, H1, H2, H3, H4, H5, H6, H7, H8, H9, H10, H11, H12, H13, H14, H15,
    I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15,
    PIN_COUNT,
}
PinEnum;

#define IS_PIN(pin)             ((pin > PIN_NONE) && (pin < PIN_COUNT))

#define GPIO_PORT(pin)          (((pin)>>4)&0x0F)
#define GPIO_PIN(pin)           ((pin)&0x0F)

/// dinh nghia dien tro treo
typedef enum
{
    PIN_NO_PULL,
    PIN_PULL,
}
PinPupdEnum;

typedef enum
{
    PIN_SPEED_LOW       = GPIO_SPEED_FREQ_LOW,
    PIN_SPEED_MEDIUM    = GPIO_SPEED_FREQ_MEDIUM,
    PIN_SPEED_HIGH      = GPIO_SPEED_FREQ_HIGH,
    PIN_SPEED_VERY_HIGH = GPIO_SPEED_FREQ_VERY_HIGH
}
PinSpeedEnum;

/// ANALOG _____________________________________________________________________
typedef enum
{
    ADC_NONE,

#if (ADC1_ENABLE == 1)
    ADC_COM1,
#endif

#if (ADC2_ENABLE == 1)
    ADC_COM2,
#endif

#if (ADC3_ENABLE == 1)
    ADC_COM3,
#endif

#if (ADC4_ENABLE == 1)
    ADC_COM4,
#endif

    ADC_COUNT,
}
AnalogIdEnum;

#define IS_ANALOG_ID(id)        ((id > ADC_NONE) && (id < ADC_COUNT))

/// EXTI _______________________________________________________________________
typedef enum
{
    EXTI_MODE_DEFAULT,
    EXTI_MODE_RISING,
    EXTI_MODE_FALING,
    EXTI_MODE_BOTH,
    EXTI_MODE_COUNT,
}
ExtiModeEnum;

/// TIMER ______________________________________________________________________
typedef enum
{
    TIME_NONE,

#if (TIME1_ENABLE == 1)
    TIME1,
#endif

#if (TIME2_ENABLE == 1)
    TIME2,
#endif

#if (TIME3_ENABLE == 1)
    TIME3,
#endif

#if (TIME4_ENABLE == 1)
    TIME4,
#endif

#if (TIME5_ENABLE == 1)
    TIME5,
#endif

#if (TIME6_ENABLE == 1)
    TIME6,
#endif

#if (TIME7_ENABLE == 1)
    TIME7,
#endif

#if (TIME8_ENABLE == 1)
    TIME8,
#endif

#if (TIME9_ENABLE == 1)
    TIME9,
#endif

#if (TIME10_ENABLE == 1)
    TIME10,
#endif

#if (TIME11_ENABLE == 1)
    TIME11,
#endif

#if (TIME12_ENABLE == 1)
    TIME12,
#endif

#if (TIME13_ENABLE == 1)
    TIME13,
#endif

#if (TIME14_ENABLE == 1)
    TIME14,
#endif

#if (TIME15_ENABLE == 1)
    TIME15,
#endif

#if (TIME16_ENABLE == 1)
    TIME16,
#endif

#if (TIME17_ENABLE == 1)
    TIME17,
#endif

    TIME_COUNT,
}
TimeIdEnum;

#define IS_TIME_ID(id)      ((id > TIME_NONE) && (id < TIME_COUNT))

/// dinh nghia cac kieu thoi gian
typedef enum
{
    TIME_MS,
    TIME_US,
    TIME_S,
    TIME_RAW,
    TIME_TYPE_COUNT,
}
TimeOptionEnum;

/// dinh nghia cac kieu xu ly cua thoi gian
typedef enum
{
    TIME_GET,
    TIME_LOOP,
}
TimeProcessEnum;

/// UART _______________________________________________________________________

/// @brief ma dinh danh cho uart
/// @note khong dat ten la UARTx la vi UART4, UART5, UART7, UART8 trung voi thu vien hall
typedef enum
{
    UART_NONE,

#ifdef UART1_ENABLE
    UART_COM1,
#endif

#ifdef UART2_ENABLE
    UART_COM2,
#endif

#ifdef UART3_ENABLE
    UART_COM3,
#endif

#ifdef UART4_ENABLE
    UART_COM4,
#endif

#ifdef UART5_ENABLE
    UART_COM5,
#endif

#ifdef UART6_ENABLE
    UART_COM6,
#endif

#ifdef UART7_ENABLE
    UART_COM7,
#endif

#ifdef UART8_ENABLE
    UART_COM8,
#endif

    UART_COUNT,
}
UartIdEnum;

#define IS_UART_ID(id)          ((id > UART_NONE) && (id < UART_COUNT))

/// stop bits
typedef enum
{
    STOPBITS_1,
    STOPBITS_2,
    STOPBITS_COUNT,
}
StopbitEnum;

/// parity bits
typedef enum
{
    PARITY_NONE,
    PARITY_EVEN,
    PARITY_ODD,
    PARITY_COUNT,
}
ParityEnum;

typedef enum
{
    WORDLENGTH_DEFAULT,
    WORDLENGTH_8B,
    WORDLENGTH_9B,
    WORDLENGTH_COUNT,
}
WordlengthEnum;

typedef enum
{
    TTL_LEVEL_INV_DEFAULT,
    TTL_LEVEL_INV_DISABLE,
    TTL_LEVEL_INV_ENABLE,
    TTL_LEVEL_INV_COUNT,
}
TTLLevelInvertEnum;

/// SPI ________________________________________________________________________
typedef enum
{
    SPI_NONE,

#if (SPI1_ENABLE == 1)
    SPI_COM1,
#endif

#if (SPI2_ENABLE == 1)
    SPI_COM2,
#endif

#if (SPI3_ENABLE == 1)
    SPI_COM3,
#endif

#if (SPI4_ENABLE == 1)
    SPI_COM4,
#endif

#if (SPI5_ENABLE == 1)
    SPI_COM5,
#endif

#if (SPI6_ENABLE == 1)
    SPI_COM6,
#endif

    SPI_COUNT,
}
SpiIdEnum;

#define IS_SPI_ID(id)   ((id > SPI_NONE) && (id < SPI_COUNT))

/// data size
typedef enum
{
    DATA_SIZE_NONE,
    DATA_SIZE_8BIT,
    DATA_SIZE_16BIT,
    DATA_SIZE_COUNT,
}
DataSizeEnum;

/// polarity
typedef enum
{
    POLARITY_NONE,
    POLARITY_LOW,
    POLARITY_HIGH,
    POLARITY_COUNT,
}
PolarityEnum;

/// phase
typedef enum
{
    PHASE_NONE,
    PHASE_1EDGE,
    PHASE_2EDGE,
    PHASE_COUNT,
}
PhaseEnum;

/// baudrate prescaler
typedef enum
{
    BAUDRATE_PRESCALER_NONE,
    BAUDRATE_PRESCALER_2,
    BAUDRATE_PRESCALER_4,
    BAUDRATE_PRESCALER_8,
    BAUDRATE_PRESCALER_16,
    BAUDRATE_PRESCALER_32,
    BAUDRATE_PRESCALER_64,
    BAUDRATE_PRESCALER_128,
    BAUDRATE_PRESCALER_256,
    BAUDRATE_PRESCALER_COUNT,
}
BaudratePrescalerEnum;
/// SPI MODE
typedef enum
{
    SPI_MODE_NONE,
    SPI_MODE_0,
    SPI_MODE_3,
    SPI_CUS1,
    SPI_CUS2,
}
SpiModeEnum;
/// CAN ________________________________________________________________________
typedef enum
{
    CAN_NONE,

#if (CAN1_ENABLE == 1)
    CAN_COM1,
#endif

#if (CAN2_ENABLE == 1)
    CAN_COM2,
#endif

    CAN_COUNT,
}
CanbusIdEnum;

#define IS_CANBUS_ID(id)        ((id > CAN_NONE) && (id < CAN_COUNT))

typedef enum
{
    CAN_ID_FILTER_RANGE,
    CAN_ID_FILTER_DUAL,
    CAN_ID_FILTER_MASK,
    CAN_ID_FILTER_RANGE_NO_EIDM
}
CanbusIdFilterEnum;

/// FLASH ______________________________________________________________________

/// dinh nghia ten cua sector
typedef enum
{
    SECTOR_NONE,
    SECTOR0,
    SECTOR1,
    SECTOR2,
    SECTOR3,
    SECTOR4,
    SECTOR5,
    SECTOR6,
    SECTOR7,
    SECTOR8,
    SECTOR9,
    SECTOR10,
    SECTOR11,
    SECTOR_COUNT,
}
SectorIdEnum;

/// I2C ________________________________________________________________________
typedef enum
{
    I2C_NONE,

#if (I2C1_ENABLE == 1)
    I2C_COM1,
#endif

#if (I2C2_ENABLE == 1)
    I2C_COM2,
#endif

#if (I2C3_ENABLE == 1)
    I2C_COM3,
#endif

    I2C_COUNT,
}
I2cIdEnum;

#define IS_I2C_ID(id)       ((id > I2C_NONE) && (id < I2C_COUNT))


/// DIRECTOR DEFINE
typedef enum
{
    CW          = 1,
    CCW         = -1,

    NORMAL      = 1,
    INVERTED    = -1,
    REVERSE     = -1,
}
RotationDirectionEnum;

/// SINTABLE
typedef enum
{
    SINTABLE_TYPE_NORMAL,
    SINTABLE_TYPE_SVPWMTABLE,
}
SintableTypeEnum;

#define SINTABLE_VALUE_MAX                               60000
#define SINTABLE_SIZE                                    9000

/// dinh nghia sintable
typedef struct
{
    uint16_t valueMax;
    uint16_t count;
    uint16_t value[SINTABLE_SIZE];
}
SintableType;

/// MUSIC
/// dinh nghia node nhac
typedef enum
{
    NONE,
    DO,
    RE,
    MI,
    FA,
    SO,
    LA,
    SI,
    MUSIC_NOTE_COUNT,
}
MusicNodeEnum;

typedef struct
{
    MusicNodeEnum   node;
    uint32_t        nodePitch;
}
MusicNodeType;

/* Exported constants --------------------------------------------------------*/

/// PIN ________________________________________________________________________
extern const char* HARDWARE_PIN_NAME[PIN_COUNT];

/// SINTABLE ___________________________________________________________________
extern const uint16_t SINTABLE_NORMAL_ARRAY[SINTABLE_SIZE + 2];
extern const uint16_t SINTABLE_SVPWM_ARRAY[SINTABLE_SIZE + 2];

/// MUSIC ______________________________________________________________________
/// @brief tan so not nhac
extern const uint32_t MUSIC_FREQUENCY[MUSIC_NOTE_COUNT];
extern const uint32_t MUSIC_FREQUENCY_KC[MUSIC_NOTE_COUNT];

/* Exported macro ------------------------------------------------------------*/

/// dinh nghia kieu doi tuong
typedef void* object;
typedef void* Object;

///// dinh nghia kieu bool
//#define ON      true
//#define OFF     false

/// dinh nghia phan tu trong
#ifndef NULL
#define NULL            0
#endif

/// dinh nghia don vi tan so
#ifndef Mhz
#define Mhz             1000000
#endif

#ifndef Khz
#define Khz             1000
#endif

#ifndef Hz
#define Hz              1
#endif

/// dinh nghia kieu data bit
#define BYTE            1
#define KBYTE           (1024*BYTE)
#define MBYTE           (1024*KBYTE)
#define GBYTE           (1024*MBYTE)

/// Dinh nghia so PI
#ifndef PI
#define PI              3.141592654f
#endif

/// dinh ngia ti so chuyen doi tu goc sang pi
#ifndef ANGLE2PI
#define ANGLE2PI        (PI/180.0f)
#endif

/// dinh nghia ti so chuyen doi tu pi sang goc
#ifndef PI2ANGLE
#define PI2ANGLE        (180.0f/PI)
#endif

/// dinh nghia ti so chuyen doi tu sang frequenct
#ifndef MUSIC_K_BT
#define MUSIC_K_BT      0.943874312681694f
#endif

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#endif /* __GREMSY_INCLUDE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
