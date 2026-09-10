/** ******************************************************************************
  * @file    hardware_u5_peripheral_output.cpp
  * @author  Gremsy Team
  * @version V3.0.0
  * @date    2026
  * @brief   Implementation of GPIO Output for STM32U5 series.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_output.hpp"

#if defined STM32U5 || defined STM32H5

/* Constructors --------------------------------------------------------------*/

HW::Peripheral::Output::Output(PinEnum _pin, bool _levelActive, PinSpeedEnum _speed)
{
    this->speed = _speed;
    this->pin   = _pin;
    this->levelActive = _levelActive;
    this->isOpen = false;
}

HW::Peripheral::Output::Output(PinEnum _pin, bool _levelActive)
{
    this->speed = PIN_SPEED_LOW; // Thường U5 dùng macro HAL chuẩn hoặc PinSpeedEnum của bạn
    this->pin   = _pin;
    this->levelActive = _levelActive;
    this->isOpen = false;
}

HW::Peripheral::Output::Output(PinEnum _pin)
{
    this->speed = PIN_SPEED_LOW;
    this->pin   = _pin;
    this->levelActive = true;
    this->isOpen = false;
}

HW::Peripheral::Output::Output()
{
    this->speed = PIN_SPEED_LOW;
    this->pin   = PIN_NONE;
    this->levelActive = true;
    this->isOpen = false;
}

/* Public Functions ----------------------------------------------------------*/

StatusEnum HW::Peripheral::Output::On(void)
{
    if(isOpen)
    {
        HAL_GPIO_WritePin(portHal, pinHal, (GPIO_PinState)levelActive);
        return OK;
    }
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Output::Off(void)
{
    if(isOpen)
    {
        HAL_GPIO_WritePin(portHal, pinHal, (GPIO_PinState)(levelActive ^ 1));
        return OK;
    }
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Output::Toggle(void)
{
    if(isOpen)
    {
        HAL_GPIO_TogglePin(portHal, pinHal);
        return OK;
    }
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Output::Open(void)
{
    /// CHECK
    if(!IS_PIN(pin)) return ERROR_NOT_EXIST;
    if(isOpen) return ERROR_EXIST;

    /// PARAM MAPPING
    uint8_t pinReal = pin - 1;
    // Đảm bảo HARDWARE_PERIPHERAL_GPIO_PORT/PIN đã được định nghĩa cho U5 trong file config
    portHal = (GPIO_TypeDef*)HARDWARE_GPIO_PORT[GPIO_PORT(pinReal)];
    pinHal  = HARDWARE_GPIO_PIN[GPIO_PIN(pinReal)];

    /// Configure GPIO for STM32U5
    gpioInitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pull      = GPIO_NOPULL;
    gpioInitStruct.Speed     = (uint32_t)speed;
    gpioInitStruct.Pin       = pinHal;

    HAL_GPIO_Init(portHal, &gpioInitStruct);

    isOpen = true;
    Off(); // Trạng thái mặc định khi mở là tắt

    return OK;
}

StatusEnum HW::Peripheral::Output::Close(void)
{
    if(!isOpen) return ERROR_EXIST;

    HAL_GPIO_DeInit(portHal, pinHal);
    isOpen = false;

    return OK;
}

#endif /** STM32U5  */
