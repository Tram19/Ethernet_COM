/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_io.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   Flexible IO Peripheral Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_IO_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_IO_HPP__

#include "hardware_u5_peripheral_config.hpp"
#include <memory>

#if defined STM32U5 || defined STM32H5

class FlexibleIO : public HW::Base::IO
{
public:
    FlexibleIO(PinEnum _pin);
    StatusEnum ConfigAsInput(void);
    StatusEnum ConfigAsOutput(bool _levelActive = true, PinSpeedEnum _speed = PIN_SPEED_LOW);
    StatusEnum ConfigAsInterrupt(IOEdgeITEnum _edge, TrigFuncCallBack &&_func);
    StatusEnum Close(void);
    bool Get(void);
    StatusEnum On(void);
    StatusEnum Off(void);

private:
    PinSpeedEnum        speed;
    IOEdgeITEnum        edge;
    bool                levelActive;
    TrigFuncCallBack    ITcallback;

    StatusEnum Open(void);
    uint8_t GetIRQNumber(uint16_t pinMask);
};

#endif /* STM32U5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_IO_HPP__ */
