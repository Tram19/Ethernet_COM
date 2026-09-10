 
/** 
  ******************************************************************************
  * @file    hardware_f4_debug_command_line.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
  * @brief   This file contains all the functions prototypes for the hardware_f4_debug_command_line.cpp 
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

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_DEBUG_COMMAND_LINE_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_DEBUG_COMMAND_LINE_HPP__

/* Includes ------------------------------------------------------------------*/

#include <Hardware/Stm32U5/Inc/hardware_u5_peripheral_config.hpp>

#if defined STM32U5 || defined STM32H5
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
class HW::Debug::CommandLine : HW::Base::CommandLine {

public:

    /// TYPEDEF ________________________________________________________________

    /** @brief dinh nghia struct luu trong element */
    typedef struct
    {
        CommandLineFuncTypeEnum     type;
        int                         dirLen;
        void*                       func;
        void*                       funcArg;
        const char*                 dir;
    }
    CommandLineElementType;

    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham cai dat chung cho command line
     *  @param none
     *  @return none
     *  @note ham cai dat toan cuc cho command line vi day la su dung chung DEBUG_UART
     * */
    static StatusEnum Init(void);

    /** @brief ham xu lu data nhan duoc tu uart va tao lenh
     *  @param none
     *  @return none
     * */
    static void Process(void);

    /** @brief ham add command cho command line
     *  @param[in] dir duong dan lenh
     *  @param[in] func ham nhan lenh
     *  @param[in] funcArg dia chi bien su dung trong ham
     *  @return trang thai cai dat
     * */
    static StatusEnum Add(const char* Dir, FuncCommandLineNoReturnCallbackType     func, void* funcArg);
    static StatusEnum Add(const char* Dir, FuncCommandLineReturnNumberCallbackType func, void* funcArg);
    static StatusEnum Add(const char* Dir, FuncCommandLineReturnStringCallbackType func, void* funcArg);

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    /** @brief ham tim duong dan
     *  @param[in] dir  dia chi duong dan
     *  @return element trong node
     * */
    static CommandLineElementType* Search(Type::Node* pnode, const char* dir, int dirLen);


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /** STM32U5  */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_DEBUG_COMMAND_LINE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
