 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_spi.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 20, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_spi.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_SPI_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_SPI_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
class HW::Base::Spi : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________

    Spi(SpiIdEnum _id) : id(_id){};

    /// PARRAM _________________________________________________________________

    SpiIdEnum               id;
    PriorityEnum            priority;
    DataSizeEnum            dataSize;
    PolarityEnum            polarity;
    PhaseEnum               phase;
    BaudratePrescalerEnum   baudratePrescaler;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Spi()
    {
        id                          = SPI_NONE;
        priority                    = PRIORITY_NONE;
        dataSize                    = DATA_SIZE_8BIT;
        polarity                    = POLARITY_HIGH;
        phase                       = PHASE_2EDGE;
        baudratePrescaler           = BAUDRATE_PRESCALER_8;

        task.size                   = 0;

        elementContinous.ptrData    = NULL;
        ptrElementContinous         = NULL;

        funcErrorCallback           = NULL;
        ptrFuncErrorCallbackArg     = NULL;
    }

    /** @breif operator copy constructor */
    Spi(const Spi &c)
    {
        id                          = c.id;
        priority                    = c.priority;
        dataSize                    = c.dataSize;
        polarity                    = c.polarity;
        phase                       = c.phase;
        baudratePrescaler           = c.baudratePrescaler;

        task                        = c.task;

        elementContinous            = c.elementContinous;
        ptrElementContinous         = c.ptrElementContinous;

        funcErrorCallback           = c.funcErrorCallback;
        ptrFuncErrorCallbackArg     = c.ptrFuncErrorCallbackArg;
    }

    /** @brief operation delete */
    virtual ~Spi()
    {
        id                          = SPI_NONE;
        priority                    = PRIORITY_NONE;
        dataSize                    = DATA_SIZE_8BIT;
        polarity                    = POLARITY_HIGH;
        phase                       = PHASE_2EDGE;
        baudratePrescaler           = BAUDRATE_PRESCALER_8;

        task.size                   = 0;

        elementContinous.ptrData    = NULL;
        ptrElementContinous         = NULL;

        funcErrorCallback           = NULL;
        ptrFuncErrorCallbackArg     = NULL;
    }

    /** @brief operator assignment */
    Spi &operator = (const Spi &c);

    /** @brief ham add function error
     *  @param[in] func dia chi func error callback
     *  @return dia chi uart
     * */
    virtual Spi &operator +=(const FuncPeriperalSpiErrorCallbackType func){
        funcErrorCallback = func;

        if(ptrFuncErrorCallbackArg == 0)
        {
            ptrFuncErrorCallbackArg = (SpiErrorType*)g_calloc(sizeof(SpiErrorType));
        }

        ptrFuncErrorCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

//    /** @brief ham add function paket data ready
//     *  @param[in] func dia chi func paket data ready
//     *  @return dia chi uart
//     * */
//    virtual Spi &operator +=(const FuncPeriperalUartPacketDataReadyCallbackType func){
//        funcPacketDataReadyCallback = func;
//
//        if(ptrFuncPacketDataReadyCallbackArg == 0)
//        {
//            ptrFuncPacketDataReadyCallbackArg = (UartPacketType*)g_calloc(sizeof(UartPacketType));
//        }
//
//        ptrFuncPacketDataReadyCallbackArg->sender = sender;
//
//        return *this;
//    }

    virtual Spi &operator +=(LambdaReceivedPacketCallback && func){
        lambdaPacketReadyCallback = std::move(func);
        return *this;
    }
//    /** @brief ham add function byte data ready
//     *  @param[in] func dia chi func byte data ready
//     *  @return dia chi uart
//     * */
//    virtual Spi &operator +=(const FuncPeripheralUartByteDataReadyCallbackType func){
//        funcByteDataReadyCallback = func;
//
//        if(ptrFuncByteDataReadyCallbackArg == 0)
//        {
//            ptrFuncByteDataReadyCallbackArg = (UartByteType*)g_calloc(sizeof(UartByteType));
//        }
//
//        ptrFuncByteDataReadyCallbackArg->sender = sender;
//
//        return *this;
//    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham send get du lieu truoc khi vao vong lap
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] rxbuff dia chi mang nhan
     *  @param[in] len do dai truyen nhan
     *  @return trang thai qua trinh truyen nhan
     * */
    virtual StatusEnum SendGet(uint8_t* txbuff, uint8_t* rxbuff, int len){return ERROR_HAL;}

    /** @brief ham send get du lieu truoc khi vao vong lap
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] len do dai truyen nhan
     *  @return trang thai qua trinh truyen nhan
     * */
    virtual StatusEnum Send(uint8_t* txbuff, int len){return ERROR_HAL;}
    /** @brief ham doc du lieu trong thread
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] rxbuff dia chi mang nhan
     *  @param[in] len do dai truyen nhan
     *  return trang thai qua trinh doc
     * */
    virtual StatusEnum Send(std::span<const uint8_t> data){return ERROR_HAL;}
    virtual StatusEnum Get(uint8_t* rxbuff, int len){return ERROR_HAL;}
    virtual StatusEnum Read(uint8_t* txbuff, uint8_t* rxbuff, int len){return ERROR_HAL;}

    /** @brief ham add function spi
     *  @param[in] name ten cua function chay spi
     *  @param[in] func ham chay spi read
     *  @param[in] sender dia chi bien can truyen den ham
     *  @reurn trang thai qua trinh doc
     * */
    virtual StatusEnum Add(Type::String                       name,
                           FuncPeripheralSpiWriteCallbackType func,
                           object                             _sender)
    {
        return ERROR_HAL;
    }

    /** @brief ham add function spi
     *  @param[in] name ten cua function chay spi
     *  @param[in] func ham chay spi read
     *  @param[in] time thoi gian giua cac lan truyen
     *  @param[in] sender dia chi bien can truyen den ham
     *  @reurn trang thai qua trinh doc
     * */
    virtual StatusEnum Add(Type::String                       name,
                           FuncPeripheralSpiWriteCallbackType func,
                           uint32_t                           time,       ///(ms) dinh thoi gian
                           object                             _sender)
    {
        return ERROR_HAL;
    }

    virtual StatusEnum Abort(){return ERROR_HAL;}

    virtual void Lock(void){}
    virtual void Unlock(void){}

    /// PRIVATE ________________________________________________________________

    /** @brief ham xu ly khi nhan ngat thanh cong
     *  @param none
     *  @return none
     * */
    virtual void __ITTxRxCpltProcess(void){}

    /** @brief ham xu ly task truyen nhan
     *  @param none
     *  @return none
     * */
    virtual void __TaskProcess(void){}
    /** @brief ham xu ly loi
     *  @param none
     *  @return none
     * */
    virtual void __ErrorProcess(void){}

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    RTOS::TaskEvent task;

    Type::Node  elementContinous;
    Type::Node* ptrElementContinous;

//    Type::Node  elementTiming;
//    Type::Node* ptrElementTiming;

    FuncPeriperalSpiErrorCallbackType   funcErrorCallback;
    SpiErrorType*                       ptrFuncErrorCallbackArg;
    LambdaReceivedPacketCallback        lambdaPacketReadyCallback;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_SPI_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
