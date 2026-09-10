/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_canbus.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   This file contains all the functions prototypes for the hardware_u5_peripheral_canbus.cpp 
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_CANBUS_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_CANBUS_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_u5_peripheral_config.hpp"

#ifdef STM32U5
/* Exported class ------------------------------------------------------------*/

class HW::Peripheral::Canbus : public HW::Base::Canbus {

public:
#ifdef HAL_FDCAN_MODULE_ENABLED
    FDCAN_HandleTypeDef* __handle;
#endif

    StatusEnum Write(uint32_t msgId, uint8_t* buff, uint8_t buffsize);
    StatusEnum Open(void);
    StatusEnum Close(void);

    void __ErrorProcess(void);
    void __ITRxFIFO0Process(void);
    void __ITRxFIFO1Process(void);
    void __ITTxCpltProcess(void);

private:
#ifdef HAL_FDCAN_MODULE_ENABLED
    FDCAN_FilterTypeDef     s0FilterConfig;
    FDCAN_FilterTypeDef     s1FilterConfig;

    FDCAN_TxHeaderTypeDef   txMsg;

    FDCAN_RxHeaderTypeDef   rxMsg0;
    uint8_t                 rxData0[8];

    FDCAN_RxHeaderTypeDef   rxMsg1;
    uint8_t                 rxData1[8];
#endif
};

#endif /** STM32U5  */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_CANBUS_HPP__ */
