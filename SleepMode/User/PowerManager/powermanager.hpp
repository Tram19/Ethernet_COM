/**
  ******************************************************************************
  * @file    PowerManager.hpp
  * @author  Gremsy Team
  * @version V1.0.0
  * @date    2024
  * @brief   Power sequencing manager
  *
  *  Power-on sequence  (100 ms between each rail):
  *    12V_SOM → GIMBAL → 12V_SYS → 1V8_SYS → 5V_CAM → 1V4_CAM
  *
  *  Power-off sequence (reverse, 100 ms between each rail):
  *    1V4_CAM → 5V_CAM → 1V8_SYS → 12V_SYS → GIMBAL → 12V_SOM
  *
  *  Pin mapping:
  *    EN_12V_SOM  → PB3   (STM32 GPIO)
  *    EN_GIMBAL   → PA12  (STM32 GPIO)
  *    EN_12V_SYS  → IOExpander pin 1  (P0_1)
  *    EN_3V3_SYS  → PC14  (STM32 GPIO)
  *    EN_1V8_SYS  → IOExpander pin 0  (P0_0)
  *    EN_5V_CAM   → IOExpander pin 2  (P0_2)
  *    EN_1V4_CAM  → IOExpander pin 3  (P0_3)
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  ******************************************************************************
*/

#ifndef __POWER_MANAGER_HPP__
#define __POWER_MANAGER_HPP__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_adc.hpp"
#include "IOExpander/IOExpander.hpp"
#include "cmsis_os.h"

#include "gremsy.hpp"
//#include "gremsy_include.hpp
/* Exported define -----------------------------------------------------------*/

#define POWER_SEQ_DELAY_MS      10U    ///< Delay between each rail (ms)
#define POWER_RETRY_COUNT       3U      ///< Max retries per rail before abort

/// @brief PHONE_ON_N_MCU pin — PB4
#define PHONE_ON_N_MCU_PORT     GPIOB
#define PHONE_ON_N_MCU_PIN      GPIO_PIN_4
#define PHONE_ON_N_HOLD_MS      500U     ///< Hold HIGH duration before releasing


/// ADC channels for monitoring currents 
#define ADC_VREF_VOLT       3.3f              // VDD MCU = 3.3V
#define ADC_RESOLUTION      16384.0f          // 14-bit ADC (0..16383)
#define R_IMON_OHM          4700.0f          // 4.7k (R26, R29, R32)
#define G_IMON_RATIO        (95.3e-6f)       // 95.3 uA/A (Datasheet Typical)


#define ADC_TO_CURRENT_AMP  (ADC_VREF_VOLT / (ADC_RESOLUTION * G_IMON_RATIO * R_IMON_OHM))
#define ADC_FILTER_SAMPLES  20U
/* Exported types ------------------------------------------------------------*/

/// @brief Individual power rail ID — order matches power-on sequence
typedef enum
{   
    PWR_RAIL_GIMBAL = 0,
    PWR_RAIL_12V_SOM,
    PWR_RAIL_12V_SYS,
    PWR_RAIL_3V3_SYS,
    PWR_RAIL_5V_CAM,
    PWR_RAIL_1V4_CAM,
    PWR_RAIL_1V8_SYS,
    PWR_RAIL_COUNT

} PowerRailEnum;

/// @brief Pin backend type
typedef enum
{
    PWR_PIN_GPIO        = 0,    ///< Controlled via STM32 GPIO
    PWR_PIN_EXPANDER,           ///< Controlled via IOExpander

} PowerPinTypeEnum;

/// @brief Descriptor for one power rail
typedef struct
{
    const char*         name;           ///< Human-readable name (for logging)
    PowerPinTypeEnum    pinType;        ///< GPIO or Expander

    /// GPIO fields (valid when pinType == PWR_PIN_GPIO)
    GPIO_TypeDef*       gpioPort;
    uint16_t            gpioPin;

    /// Expander fields (valid when pinType == PWR_PIN_EXPANDER)
    uint8_t             expanderPin;    ///< Flat pin index 0..15

} PowerRailDescriptor;

/// @brief Overall power state
typedef enum
{
    PWR_STATE_OFF       = 0,
    PWR_STATE_POWERING_ON,
    PWR_STATE_ON,
    PWR_STATE_POWERING_OFF,
    PWR_STATE_FAULT,

} PowerStateEnum;

/* Exported class ------------------------------------------------------------*/

class PowerManager
{
public:

    //explicit PowerManager(HW::Peripheral::Adc& adc);
    /// PARAM __________________________________________________________________

    IOExpander*     pExpander{nullptr};     ///< Injected IOExpander instance

    /// FUNCTION _______________________________________________________________

    /** @brief Open — configure all enable pins as output, all LOW (off)
     *  @return StatusEnum
     */
    StatusEnum Open(void);

    /** @brief Power on all rails in sequence with 100 ms delay each
     *         Retries each rail up to POWER_RETRY_COUNT times before aborting
     *  @return OK if all rails came up, ERROR_HAL if any rail failed after retries
     */
    StatusEnum PowerOn(void);

    /** @brief Power off all rails in reverse sequence with 100 ms delay each
     *  @return StatusEnum
     */
    StatusEnum PowerOff(void);

    /** @brief Enable or disable a single rail immediately (no sequencing)
     *  @param rail   PowerRailEnum
     *  @param enable true = on, false = off
     */
    StatusEnum SetRail(PowerRailEnum rail, bool enable);

    /** @brief Get current overall power state */
    PowerStateEnum GetState(void) const { return __state; }

    /** @brief Boot SOM module sequence:
     *         1. Pull PHONE_ON_N_MCU (PB4) HIGH
     *         2. Enable 12V_SOM rail
     *         3. Hold HIGH for 10 ms then pull LOW
     *  @return StatusEnum
     */
    StatusEnum SleepSom(void);
    StatusEnum WakeUpSom(void);

    /** @brief Get the rail that caused a fault (-1 if no fault) */
    int GetFaultRail(void) const { return __faultRail; }

    /** @brief Read the 12V system current ADC */
    uint32_t ReadCurrent12vSys(void);
    uint32_t CalibrateCurrentOffset(void);
    uint32_t GetCurrent12vSys(void) const { return _current_12v_som_a;}

private:

    /// PARAM __________________________________________________________________

    HW::Peripheral::Adc _adc_12_som;
    PowerStateEnum  __state{PWR_STATE_OFF};
    int             __faultRail{-1};        ///< Index of rail that faulted

    uint32_t adc_raw_12_som{0};
    uint32_t adc_offset_12_som{0};
    uint32_t adc_filter_samples[ADC_FILTER_SAMPLES]{};
    uint32_t adc_filter_sum{0};
    uint32_t adc_filter_index{0};
    float _current_12v_sys_a{0.0f};
    uint32_t _current_12v_som_a{0}; //mA
    float _current_3v3_sys_a;
    float _current_gimbal_a;

    /// FUNCTION _______________________________________________________________

    /** @brief Drive one rail's enable pin HIGH or LOW */
    StatusEnum __setPin(const PowerRailDescriptor& desc, bool enable);
    void __resetCurrentFilter(uint32_t sample);

    /** @brief Drive one rail with retry logic
     *  @return OK or ERROR_HAL after POWER_RETRY_COUNT failures
     */
    StatusEnum __setRailWithRetry(PowerRailEnum rail, bool enable);

};

/* Exported constants --------------------------------------------------------*/

/// @brief Power rail descriptor table — indexed by PowerRailEnum
///        Edit GPIO ports/pins and expander pin numbers to match your schematic
static const PowerRailDescriptor POWER_RAIL_TABLE[PWR_RAIL_COUNT] =
{

    /* PWR_RAIL_GIMBAL */
    {
        .name           = "GIMBAL",
        .pinType        = PWR_PIN_GPIO,
        .gpioPort       = GPIOA,
        .gpioPin        = GPIO_PIN_12,
        .expanderPin    = 0,
    },
    /* PWR_RAIL_12V_SOM */
    {
        .name           = "12V_SOM",
        .pinType        = PWR_PIN_GPIO,
        .gpioPort       = GPIOB,
        .gpioPin        = GPIO_PIN_3,
        .expanderPin    = 0,
    },


    /* PWR_RAIL_12V_SYS */
    {
        .name           = "12V_SYS",
        .pinType        = PWR_PIN_EXPANDER,
        .gpioPort       = nullptr,
        .gpioPin        = 0,
        .expanderPin    = 1,    ///< P0_1
    },

    /* PWR_RAIL_3V3_SYS */
    {
        .name           = "3V3_SYS",
        .pinType        = PWR_PIN_GPIO,
        .gpioPort       = GPIOC,
        .gpioPin        = GPIO_PIN_14,
        .expanderPin    = 0,
    },

    /* PWR_RAIL_5V_CAM */
    {
        .name           = "5V_CAM",
        .pinType        = PWR_PIN_EXPANDER,
        .gpioPort       = nullptr,
        .gpioPin        = 0,
        .expanderPin    = 2,    ///< P0_2
    },

    /* PWR_RAIL_1V4_CAM */
    {
        .name           = "1V4_CAM",
        .pinType        = PWR_PIN_EXPANDER,
        .gpioPort       = nullptr,
        .gpioPin        = 0,
        .expanderPin    = 3,    ///< P0_3
    },

    /* PWR_RAIL_1V8_SYS */
    {
        .name           = "1V8_SYS",
        .pinType        = PWR_PIN_EXPANDER,
        .gpioPort       = nullptr,
        .gpioPin        = 0,
        .expanderPin    = 0,    ///< P0_0
    },

};

#endif /* __POWER_MANAGER_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
