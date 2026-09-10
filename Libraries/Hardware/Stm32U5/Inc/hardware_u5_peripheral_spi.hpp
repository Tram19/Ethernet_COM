/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_spi.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   SPI Peripheral Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_SPI_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_SPI_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_u5_peripheral_config.hpp"
#include "RTOS/rtos.hpp"
#include <mutex>
#ifdef STM32U5
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class spi
class HW::Peripheral::Spi : public HW::Base::Spi {

public:

    /// TYPEDEF ________________________________________________________________
    Spi();
    Spi(SpiIdEnum _id);
    Spi(SpiIdEnum _id, SpiModeEnum _spiMode, BaudratePrescalerEnum _preScale);
    Spi(SpiIdEnum _id, SpiModeEnum _spiMode, BaudratePrescalerEnum _preScale, DataSizeEnum _dataSize);
    /// PARRAM _________________________________________________________________


    /** @brief param su dung private */
#ifdef HAL_SPI_MODULE_ENABLED
    SPI_HandleTypeDef*      __handle;
#endif /** SPI ENABLE */

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham open hardware
     *  @param none
     *  @return trang thai qua trinh open
     * */
    StatusEnum Open(void) override;

    /** @brief ham close hardware
     *  @param none
     *  @return trang thai qua trinh close
     * */
    StatusEnum Close(void) override;

    /** @brief ham send get du lieu truoc khi vao vong lap
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] rxbuff dia chi mang nhan
     *  @param[in] len do dai truyen nhan
     *  @return trang thai qua trinh truyen nhan
     * */
    StatusEnum SendGet(uint8_t* txbuff, uint8_t* rxbuff, int len) override;

    /** @brief ham send get du lieu truoc khi vao vong lap
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] len do dai truyen nhan
     *  @return trang thai qua trinh truyen nhan
     * */
    StatusEnum Send(uint8_t* txbuff, int len) override;

    StatusEnum Send(std::span<const uint8_t> data) override;
    /** @brief ham send get du lieu truoc khi vao vong lap
     *  @param[in] rxbuff dia chi mang truyen
     *  @param[in] len do dai truyen nhan
     *  @return trang thai qua trinh truyen nhan
     * */
    StatusEnum Get(uint8_t* rxbuff, int len) override;
    /** @brief ham doc du lieu trong thread
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] rxbuff dia chi mang nhan
     *  @param[in] len do dai truyen nhan
     *  return trang thai qua trinh doc
     * */
    StatusEnum Read(uint8_t* txbuff, uint8_t* rxbuff, int len) override;

    /** @brief ham add function spi
     *  @param[in] name ten cua function chay spi
     *  @param[in] func ham chay spi read
     *  @param[in] sender dia chi bien can truyen den ham
     *  @reurn trang thai qua trinh doc
     * */
    StatusEnum Add(Type::String                       name,
                   FuncPeripheralSpiWriteCallbackType func,
                   object                             sender);

//    /** @brief ham add function spi
//     *  @param[in] name ten cua function chay spi
//     *  @param[in] func ham chay spi read
//     *  @param[in] time thoi gian giua cac lan truyen
//     *  @param[in] sender dia chi bien can truyen den ham
//     *  @reurn trang thai qua trinh doc
//     * */
//    StatusEnum Add(Type::String                       name,
//                   FuncPeripheralSpiWriteCallbackType func,
//                   uint32_t                           time,       ///(ms) dinh thoi gian
//                   object                             sender);

    StatusEnum Abort();
    void Lock(void) override;
    void Unlock(void) override;
    /// PRIVATE ________________________________________________________________

    /** @brief ham xu ly khi nhan ngat thanh cong
     *  @param none
     *  @return none
     * */
    void __ITTxRxCpltProcess(void);
    void __ITTxCpltProcess(void);

    /** @brief ham xu ly task truyen nhan
     *  @param none
     *  @return none
     * */
    void __TaskProcess(void) override;

    /** @brief ham xu ly loi
     *  @param none
     *  @return none
     * */
    void __ErrorProcess(void) override;


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    Type::String        nameAdd;

    RTOS::Event         txEvent;
    RTOS::DynamicMutex  _bus_mutex;
    mutable RTOS::StaticMutex _spi_mutex;
    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /** STM32U5xx  */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_SPI_HPP__ */
