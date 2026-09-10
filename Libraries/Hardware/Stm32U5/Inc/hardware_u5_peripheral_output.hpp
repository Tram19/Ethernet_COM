/** ******************************************************************************
  * @file    hardware_u5_peripheral_output.hpp
  * @author  Gremsy Team
  * @version V3.0.0
  * @date    2026
  * @brief   This file contains all the functions prototypes for the hardware_u5_peripheral_output.cpp 
  * firmware library.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_OUTPUT_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_OUTPUT_HPP__

/* Includes ------------------------------------------------------------------*/
#include <Hardware/Stm32U5/Inc/hardware_u5_peripheral_config.hpp> // Đảm bảo bạn đã có file config cho U5

#if defined STM32U5 || defined STM32H5
/* Exported class ------------------------------------------------------------*/

namespace HW {
namespace Peripheral {

/// @brief class hardware output for STM32U5
class Output : public HW::Base::Output {

public:
    /// CONSTRUCTOR ____________________________________________________________

    Output(PinEnum _pin, bool _levelActive, PinSpeedEnum _speed);
    Output(PinEnum _pin, bool _levelActive);
    Output(PinEnum _pin);
    Output();

    /// FUNCTION _______________________________________________________________
    /** @brief Khởi tạo phần cứng GPIO Output */
    StatusEnum Open(void) ;

    /** @brief Hủy khởi tạo phần cứng */
    StatusEnum Close(void) ;

    /** @brief Bật output (theo levelActive) */
    StatusEnum On(void) ;

    /** @brief Tắt output */
    StatusEnum Off(void) ;

    /** @brief Đảo trạng thái output */
    StatusEnum Toggle(void) ;

private:
    /// PARAM _________________________________________________________________
    GPIO_InitTypeDef    gpioInitStruct;
    GPIO_TypeDef* portHal;
    uint16_t            pinHal;
    PinSpeedEnum        speed;
};

} // namespace Peripheral
} // namespace HW

#endif /** STM32U5  */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_OUTPUT_HPP__ */
