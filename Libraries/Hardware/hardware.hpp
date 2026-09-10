
/**
 ******************************************************************************
 * @file    hardware.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Apr 11, 2023
 * @brief   This file contains all the functions prototypes for the hardware.cpp
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

#ifndef __HARDWARE_HARDWARE_HPP__
# define __HARDWARE_HARDWARE_HPP__

/* Includes ------------------------------------------------------------------*/


#include "Hardware/Stm32U5/Inc/hardware_u5.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

extern HW::Debug::Log __gp_log;

/* Exported macro ------------------------------------------------------------*/

# define CMD_ADD HW::Debug::CommandLine::Add

# define LOGINS __gp_log

#define LOG                 LOGINS<<"\r\33[0m "

#define LOGCLEAR()          LOGINS<<"\r\33\143 "

/// chuyen doi SysTick->VAL thanh thoi gian thuc do cua he thong
# ifdef PERIPHERAL_TIME
#  define LOGFORM (int)HW::Peripheral::Time::GetSystemTimeMs() << "|" << __FILE__ << "|" << __LINE__ << "| "
#  define LOGFORMSHORT (int)HW::Peripheral::Time::GetSystemTimeMs() << "|" << __LINE__ << "| "
# else
#  define LOGFORM (int)SysTick->VAL << "|" << __FILE__ << "|" << __LINE__ << "| "
# endif

#define LOGI                LOGINS\
                                <<"\r\33[0m|INFO|"\
                                <<LOGFORM

#define LOGE                LOGINS\
                                <<"\r\33[91m|ERROR|"\
                                <<LOGFORM

#define LOGW                LOGINS\
                                <<"\r\33[95m|WARN|"\
                                <<LOGFORM

#define LOGS                LOGINS\
                                <<"\r\33[32m|INFO|"\
                                <<LOGFORMSHORT

#define LEND                "\33[0m\n"
/// chuyen doi SysTick->VAL thanh thoi gian thuc do cua he thong
# ifdef PERIPHERAL_TIME
#  define LOGTAGFORM (int)HW::Peripheral::Time::GetSystemTimeMs() << "| " << TAG << "| "
# else
#  define LOGTAGFORM (int)SysTick->VAL << "| " << TAG << "| "
# endif

#define LOGTAGI             LOGINS\
                                <<"\r\33[0m|INFO|"\
                                <<LOGTAGFORM

#define LOGTAGE             LOGINS\
                                <<"\r\33[91m|ERROR|"\
                                <<LOGTAGFORM

#define LOGTAGW             LOGINS\
                                <<"\r\33[95m|WARN|"\
                                <<LOGTAGFORM
/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_HARDWARE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
