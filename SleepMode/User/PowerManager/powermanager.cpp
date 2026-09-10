/**
  ******************************************************************************
  * @file    PowerManager.cpp
  * @author  Gremsy Team
  * @version V1.0.0
  * @date    2024
  * @brief   Power sequencing manager implementation
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "powermanager.hpp"
#include "Signal_Clone/signal_clone.hpp"
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Drive the enable pin of one rail descriptor HIGH or LOW.
 *         Dispatches to GPIO or IOExpander based on pinType.
 */
StatusEnum PowerManager::__setPin(const PowerRailDescriptor& desc, bool enable)
{
    if (desc.pinType == PWR_PIN_GPIO)
    {
        if (desc.gpioPort == nullptr) return ERROR_NOT_EXIST;

        HAL_GPIO_WritePin(
            desc.gpioPort,
            desc.gpioPin,
            enable ? GPIO_PIN_SET : GPIO_PIN_RESET
        );

        return OK;
    }
    else /* PWR_PIN_EXPANDER */
    {
        if (pExpander == nullptr) return ERROR_NOT_EXIST;

        return pExpander->digitalWrite(
            desc.expanderPin,
            enable ? IOExpander::HIGH_STATE : IOExpander::LOW_STATE
        );
    }
}

/**
 * @brief  Enable or disable a rail with retry logic.
 *         Tries up to POWER_RETRY_COUNT times before returning ERROR_HAL.
 */
StatusEnum PowerManager::__setRailWithRetry(PowerRailEnum rail, bool enable)
{
    const PowerRailDescriptor& desc = POWER_RAIL_TABLE[rail];

    for (uint8_t attempt = 0; attempt < POWER_RETRY_COUNT; attempt++)
    {
        StatusEnum ret = __setPin(desc, enable);

        if (ret == OK) return OK;

        /// Wait a short time before retry
        osDelay(10U);
    }

    return ERROR_HAL;
}

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Configure all enable pins as output and drive them LOW (all off).
 *         GPIO pins: configured via HAL_GPIO_Init (assumed done by CubeMX).
 *         Expander pins: set to OUTPUT_MODE then driven LOW.
 */
StatusEnum PowerManager::Open(void)
{
    _adc_12_som.id = ADC_COM1;
    _adc_12_som.channel = ADC_CHANNEL7;
    StatusEnum adc_ret = _adc_12_som.Open();
    if (adc_ret != OK && adc_ret != ERROR_EXIST) return adc_ret;
    CalibrateCurrentOffset();

    /// --- Configure expander pins as output and drive LOW ---
    if (pExpander == nullptr) return ERROR_NOT_EXIST;

        /// P0_0..P0_3 are all expander rails — set OUTPUT then LOW
    for (int i = 0; i < PWR_RAIL_COUNT; i++)
    {
        const PowerRailDescriptor& desc = POWER_RAIL_TABLE[i];
    	if(desc.pinType == PWR_PIN_EXPANDER)
    	{
            StatusEnum ret = pExpander->pinMode(desc.expanderPin, IOExpander::OUTPUT_MODE);
            if (ret != OK) return ret;
            ret = pExpander->digitalWrite(desc.expanderPin, IOExpander::LOW_STATE);
            if (ret != OK) return ret;
        }else
        {
            /// GPIO pins

            if (desc.gpioPort == nullptr) return ERROR_NOT_EXIST;
            /// Assume CubeMX already set GPIO pins as output, just drive LOW here
            HAL_GPIO_WritePin(desc.gpioPort, desc.gpioPin, GPIO_PIN_RESET);
        }
    }

    HAL_GPIO_WritePin(PHONE_ON_N_MCU_PORT, PHONE_ON_N_MCU_PIN, GPIO_PIN_RESET);
    __state     = PWR_STATE_OFF;
    __faultRail = -1;

    return OK;
}

uint32_t PowerManager::ReadCurrent12vSys(void)
{
    const uint32_t new_sample = _adc_12_som.Get();
    adc_filter_sum -= adc_filter_samples[adc_filter_index];
    adc_filter_samples[adc_filter_index] = new_sample;
    adc_filter_sum += new_sample;
    adc_filter_index = (adc_filter_index + 1U) % ADC_FILTER_SAMPLES;

    const uint32_t filtered_raw = adc_filter_sum / ADC_FILTER_SAMPLES;
    adc_raw_12_som = filtered_raw > adc_offset_12_som
        ? filtered_raw - adc_offset_12_som
        : 0U;
    _current_12v_som_a = static_cast<uint32_t>(adc_raw_12_som*1000.0f) * ADC_TO_CURRENT_AMP;
    return adc_raw_12_som;
}

uint32_t PowerManager::CalibrateCurrentOffset(void)
{
    uint32_t adc_sum = 0U;
    for (uint32_t sample = 0U; sample < ADC_FILTER_SAMPLES; ++sample) {
        adc_sum += _adc_12_som.Get();
    }
    adc_offset_12_som = adc_sum / ADC_FILTER_SAMPLES;
    __resetCurrentFilter(adc_offset_12_som);
    adc_raw_12_som = 0U;
    _current_12v_som_a = 0.0f;
    return adc_offset_12_som;
}

void PowerManager::__resetCurrentFilter(uint32_t sample)
{
    for (uint32_t index = 0U; index < ADC_FILTER_SAMPLES; ++index) {
        adc_filter_samples[index] = sample;
    }
    adc_filter_sum = sample * ADC_FILTER_SAMPLES;
    adc_filter_index = 0U;
}

/**
 * @brief  Power on all rails in sequence.
 *
 *  Sequence: 12V_SOM → GIMBAL → 12V_SYS → 1V8_SYS → 5V_CAM → 1V4_CAM
 *  Each rail waits POWER_SEQ_DELAY_MS before the next one is enabled.
 *  If a rail fails after POWER_RETRY_COUNT retries, sequencing stops,
 *  state is set to PWR_STATE_FAULT, and the faulty rail index is stored.
 */
StatusEnum PowerManager::PowerOn(void)
{
    if (__state == PWR_STATE_ON)    return OK;
    if (__state == PWR_STATE_FAULT) return ERROR_HAL;

    __state     = PWR_STATE_POWERING_ON;
    __faultRail = -1;

    for (int i = 0; i < PWR_RAIL_COUNT; i++)
    {
        /// --- Special handling for 12V_SOM (first rail) ---
        /// Pull PHONE_ON_N_MCU HIGH before enabling rail,
        /// hold 10 ms after rail is up, then release LOW.
        
        if (i == PWR_RAIL_12V_SOM)
        {
            HAL_GPIO_WritePin(PHONE_ON_N_MCU_PORT, PHONE_ON_N_MCU_PIN, GPIO_PIN_SET);
            osDelay(5);
        }
        
        StatusEnum ret = __setRailWithRetry((PowerRailEnum)i, true);

        if (ret != OK)
        {
            /// On failure: release PHONE_ON_N_MCU if we were doing 12V_SOM
            if (i == PWR_RAIL_12V_SOM)
            {
                HAL_GPIO_WritePin(PHONE_ON_N_MCU_PORT, PHONE_ON_N_MCU_PIN, GPIO_PIN_RESET);
            }

            __faultRail = i;
            __state     = PWR_STATE_FAULT;
            return ERROR_HAL;
        }

        /// --- After 12V_SOM is up: hold 10 ms then release PHONE_ON_N_MCU ---
        if (i == PWR_RAIL_12V_SOM)
        {
            osDelay(PHONE_ON_N_HOLD_MS);
            HAL_GPIO_WritePin(PHONE_ON_N_MCU_PORT, PHONE_ON_N_MCU_PIN, GPIO_PIN_RESET);
        }

        /// Delay between rails — skip after last rail
        if (i < (PWR_RAIL_COUNT - 1))
        {
            osDelay(POWER_SEQ_DELAY_MS);
        }
    }

    __state = PWR_STATE_ON;
    return OK;
}

/**
 * @brief  Power off all rails in reverse sequence.
 *
 *  Sequence: 1V4_CAM → 5V_CAM → 1V8_SYS → 12V_SYS → GIMBAL → 12V_SOM
 *  Each rail waits POWER_SEQ_DELAY_MS before the next one is disabled.
 */
StatusEnum PowerManager::PowerOff(void)
{
   
    if (__state == PWR_STATE_OFF) return OK;

    __state = PWR_STATE_POWERING_OFF;

    for (int i = (PWR_RAIL_COUNT - 1); i >= 0; i--)
    {
        /// Best-effort — continue even if one rail fails
        __setRailWithRetry((PowerRailEnum)i, false);

        if (i > 0)
        {
            osDelay(POWER_SEQ_DELAY_MS);
        }
    }

    __state     = PWR_STATE_OFF;
    __faultRail = -1;

    return OK;
}
StatusEnum PowerManager::SleepSom(void)
{
   
    if (__state == PWR_STATE_OFF) return OK;

    __state = PWR_STATE_POWERING_OFF;

    for (int i = (PWR_RAIL_COUNT - 1); i >= 0; i--)
    {
        if (i == PWR_RAIL_12V_SOM) continue;
        /// Best-effort — continue even if one rail fails
        __setRailWithRetry((PowerRailEnum)i, false);

        if (i > 0)
        {
            osDelay(POWER_SEQ_DELAY_MS);
        }
    }

    __state     = PWR_STATE_OFF;
    __faultRail = -1;

    return OK;
}

StatusEnum PowerManager::WakeUpSom(void){
    // const StatusEnum rail_result = __setRailWithRetry(PWR_RAIL_12V_SOM, true);
    // if (rail_result != OK) {
    //     return rail_result;
    // }
    HAL_GPIO_WritePin(PHONE_ON_N_MCU_PORT, PHONE_ON_N_MCU_PIN, GPIO_PIN_SET);
    osDelay(5);
    const StatusEnum rail_result = __setRailWithRetry(PWR_RAIL_12V_SOM, true);

    osDelay(1000U);
    HAL_GPIO_WritePin(PHONE_ON_N_MCU_PORT, PHONE_ON_N_MCU_PIN, GPIO_PIN_RESET);
    return OK;
}
/**
 * @brief  Immediately enable or disable a single rail (no sequencing, no delay).
 *         Useful for emergency shutdown or individual rail control during debug.
 */
StatusEnum PowerManager::SetRail(PowerRailEnum rail, bool enable)
{
    if (rail >= PWR_RAIL_COUNT) return ERROR_NOT_EXIST;

    return __setRailWithRetry(rail, enable);
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
