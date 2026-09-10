 
/** 
  ******************************************************************************
  * @file    module_base_imu.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 24, 2023
  * @brief   This file contains all the functions prototypes for the module_base_imu.cpp 
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

#ifndef __MODULES_BASE_MODULE_BASE_IMU_HPP__
#define __MODULES_BASE_MODULE_BASE_IMU_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class dung cho cac imu
class Module::Base::ImuSpi : public Module::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    HW::Base::Output*       pCsPin;
    HW::Base::Spi*          pspi;

    float                   gyroRange;
    float                   gyroODR;
    float                   accelRange;
    float                   accelODR;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    ImuSpi()
    {
        pCsPin                              = NULL;
        pspi                                = NULL;
        gyroRange                           = 0;
        gyroODR                             = 0;
        accelRange                          = 0;
        accelODR                            = 0;

        stateCommon                         = IMU_STATE_IDLE;

        gyroData.count                      = 0;
        gyroData.scaler                     = 0;
        gyroData.timeConfig                 = 0;
        gyroData.timeReal                   = 0;
        gyroData.x                          = 0;
        gyroData.y                          = 0;
        gyroData.z                          = 0;

        accelData.count                     = 0;
        accelData.scaler                    = 0;
        accelData.timeConfig                = 0;
        accelData.timeReal                  = 0;
        accelData.x                         = 0;
        accelData.y                         = 0;
        accelData.z                         = 0;

        gyroRaw.sumCount                    = 0;
        gyroRaw.timeConfig                  = 0;
        gyroRaw.timeReal                    = 0;
        gyroRaw.x                           = 0;
        gyroRaw.xSum                        = 0;
        gyroRaw.y                           = 0;
        gyroRaw.ySum                        = 0;
        gyroRaw.z                           = 0;
        gyroRaw.zSum                        = 0;

        accelRaw.sumCount                   = 0;
        accelRaw.timeConfig                 = 0;
        accelRaw.timeReal                   = 0;
        accelRaw.x                          = 0;
        accelRaw.xSum                       = 0;
        accelRaw.y                          = 0;
        accelRaw.ySum                       = 0;
        accelRaw.z                          = 0;
        accelRaw.zSum                       = 0;

        temperature                         = 0;
        chipID                              = 0;

        funcAccelDataReadyCallback          = NULL;
        funcAccelDataReadyCallbackArg       = NULL;
        funcGyroDataReadyCallback           = NULL;
        funcGyroDataReadyCallbackArg        = NULL;
        funcTemperatureDataReadyCallback    = NULL;
        funcTemperatureDataReadyCallbackArg = NULL;
        funcSampleDataReadyCallback         = NULL;
        funcSampleDataReadyCallbackArg      = NULL;

    }

    /** @breif operator copy constructor */
    ImuSpi(const ImuSpi &c)
    {
        pCsPin                              = c.pCsPin;
        pspi                                = c.pspi;
        gyroRange                           = c.gyroRange;
        gyroODR                             = c.gyroODR;
        accelRange                          = c.accelRange;
        accelODR                            = c.accelODR;

        stateCommon                         = c.stateCommon;

        gyroData.count                      = c.gyroData.count;
        gyroData.scaler                     = c.gyroData.scaler;
        gyroData.timeConfig                 = c.gyroData.timeConfig;
        gyroData.timeReal                   = c.gyroData.timeReal;
        gyroData.x                          = c.gyroData.x;
        gyroData.y                          = c.gyroData.y;
        gyroData.z                          = c.gyroData.z;

        accelData.count                     = c.accelData.count;
        accelData.scaler                    = c.accelData.scaler;
        accelData.timeConfig                = c.accelData.timeConfig;
        accelData.timeReal                  = c.accelData.timeReal;
        accelData.x                         = c.accelData.x;
        accelData.y                         = c.accelData.y;
        accelData.z                         = c.accelData.z;

        gyroRaw.sumCount                    = c.gyroRaw.sumCount;
        gyroRaw.timeConfig                  = c.gyroRaw.timeConfig;
        gyroRaw.timeReal                    = c.gyroRaw.timeReal;
        gyroRaw.x                           = c.gyroRaw.x;
        gyroRaw.xSum                        = c.gyroRaw.xSum;
        gyroRaw.y                           = c.gyroRaw.y;
        gyroRaw.ySum                        = c.gyroRaw.ySum;
        gyroRaw.z                           = c.gyroRaw.z;
        gyroRaw.zSum                        = c.gyroRaw.zSum;

        accelRaw.sumCount                   = c.accelRaw.sumCount;
        accelRaw.timeConfig                 = c.accelRaw.timeConfig;
        accelRaw.timeReal                   = c.accelRaw.timeReal;
        accelRaw.x                          = c.accelRaw.x;
        accelRaw.xSum                       = c.accelRaw.xSum;
        accelRaw.y                          = c.accelRaw.y;
        accelRaw.ySum                       = c.accelRaw.ySum;
        accelRaw.z                          = c.accelRaw.z;
        accelRaw.zSum                       = c.accelRaw.zSum;

        temperature                         = c.temperature;
        chipID                              = c.chipID;

        funcAccelDataReadyCallback          = c.funcAccelDataReadyCallback;
        funcAccelDataReadyCallbackArg       = c.funcAccelDataReadyCallbackArg;
        funcGyroDataReadyCallback           = c.funcGyroDataReadyCallback;
        funcGyroDataReadyCallbackArg        = c.funcGyroDataReadyCallbackArg;
        funcTemperatureDataReadyCallback    = c.funcTemperatureDataReadyCallback;
        funcTemperatureDataReadyCallbackArg = c.funcTemperatureDataReadyCallbackArg;
        funcSampleDataReadyCallback         = c.funcSampleDataReadyCallback;
        funcSampleDataReadyCallbackArg      = c.funcSampleDataReadyCallbackArg;
    }

    /** @brief operation delete */
    virtual ~ImuSpi()
    {
        pCsPin                              = NULL;
        pspi                                = NULL;
        gyroRange                           = 0;
        gyroODR                             = 0;
        accelRange                          = 0;
        accelODR                            = 0;

        stateCommon                         = IMU_STATE_IDLE;

        gyroData.count                      = 0;
        gyroData.scaler                     = 0;
        gyroData.timeConfig                 = 0;
        gyroData.timeReal                   = 0;
        gyroData.x                          = 0;
        gyroData.y                          = 0;
        gyroData.z                          = 0;

        accelData.count                     = 0;
        accelData.scaler                    = 0;
        accelData.timeConfig                = 0;
        accelData.timeReal                  = 0;
        accelData.x                         = 0;
        accelData.y                         = 0;
        accelData.z                         = 0;

        gyroRaw.sumCount                    = 0;
        gyroRaw.timeConfig                  = 0;
        gyroRaw.timeReal                    = 0;
        gyroRaw.x                           = 0;
        gyroRaw.xSum                        = 0;
        gyroRaw.y                           = 0;
        gyroRaw.ySum                        = 0;
        gyroRaw.z                           = 0;
        gyroRaw.zSum                        = 0;

        accelRaw.sumCount                   = 0;
        accelRaw.timeConfig                 = 0;
        accelRaw.timeReal                   = 0;
        accelRaw.x                          = 0;
        accelRaw.xSum                       = 0;
        accelRaw.y                          = 0;
        accelRaw.ySum                       = 0;
        accelRaw.z                          = 0;
        accelRaw.zSum                       = 0;

        temperature                         = 0;
        chipID                              = 0;

        funcAccelDataReadyCallback          = NULL;
        funcAccelDataReadyCallbackArg       = NULL;
        funcGyroDataReadyCallback           = NULL;
        funcGyroDataReadyCallbackArg        = NULL;
        funcTemperatureDataReadyCallback    = NULL;
        funcTemperatureDataReadyCallbackArg = NULL;
        funcSampleDataReadyCallback         = NULL;
        funcSampleDataReadyCallbackArg      = NULL;
    }

    /** @brief operator assignment */
    ImuSpi &operator = (const ImuSpi &c);

    /** @brief operator add function gyro data ready callback
     * */
    virtual ImuSpi &operator += (const FuncImuGyroDataReadyCallbackType func) {
        funcGyroDataReadyCallback = func;

        if(funcGyroDataReadyCallbackArg == 0)
        {
            funcGyroDataReadyCallbackArg = (ImuGyroDataReadyType*)g_calloc(sizeof(ImuGyroDataReadyType));
        }

        //funcGyroDataReadyCallbackArg->data = &gyroData; // cap nhat dia chi con tro chua du lieu gyro, muc dich de khoi gan lai gia tri tai ham datareadycallback
        funcGyroDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operator add function accel data ready callback
     * */
    virtual ImuSpi &operator += (const FuncImuAccelDataReadyCallbackType func) {
        funcAccelDataReadyCallback = func;

        if(funcAccelDataReadyCallbackArg == 0)
        {
            funcAccelDataReadyCallbackArg = (ImuAccelDataReadyType*)g_calloc(sizeof(ImuAccelDataReadyType));
        }

        //funcAccelDataReadyCallbackArg->data = &accelData;  // cap nhat dia chi con tro chua du lieu accel, muc dich de khoi gan lai gia tri tai ham datareadycallback
        funcAccelDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operator add function temprature data ready callback
     * */
    virtual ImuSpi &operator += (const FuncImuTemperatureDataReadyCallbackType func) {
        funcTemperatureDataReadyCallback = func;

        if(funcTemperatureDataReadyCallbackArg == 0)
        {
            funcTemperatureDataReadyCallbackArg = (ImuTemperatureDataReadyType*)g_calloc(sizeof(ImuTemperatureDataReadyType));
        }
        funcTemperatureDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operator add function sample data ready callback
     * */
    virtual ImuSpi &operator += (const FuncImuSampleDataReadyCallbackType func) {
        funcSampleDataReadyCallback = func;

        if(funcSampleDataReadyCallbackArg == 0)
        {
            funcSampleDataReadyCallbackArg = (ImuSampleDataReadyType*)g_calloc(sizeof(ImuSampleDataReadyType));
        }
        funcSampleDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham tra ve trang thai cua imu
     *  @param none
     *  @return trang thai cua imu
     * */
    virtual ImuStateEnum GetState(void){return stateCommon;}

    /** @brief ham tra ve gia tri doc duoc gyro cua imu
     *  @param none
     *  @reutrn dia chi gia tri cua gyro
     * */
    virtual ImuDataType* GetGyroData(void){return &gyroData;}

    /** @brief ham tra ve gia tri doc duoc accel cua imu
     *  @param none
     *  @reutrn dia chi gia tri cua accel
     * */
    virtual ImuDataType* GetAccelData(void){return &accelData;}

    /** @brief ham tra ve gia tri doc duoc temperature cua imu
     *  @param none
     *  @reutrn dia chi gia tri cua accel
     * */
    virtual float GetTemperature(void){return temperature;}

    /** @brief ham doc chip id
     *  @param none
     *  @return trang thai qua trinh doc
     * */
    virtual uint32_t GetID(void){return chipID;}


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    FuncImuGyroDataReadyCallbackType        funcGyroDataReadyCallback;
    ImuGyroDataReadyType*                   funcGyroDataReadyCallbackArg;

    FuncImuAccelDataReadyCallbackType       funcAccelDataReadyCallback;
    ImuAccelDataReadyType*                  funcAccelDataReadyCallbackArg;

    FuncImuTemperatureDataReadyCallbackType funcTemperatureDataReadyCallback;
    ImuTemperatureDataReadyType*            funcTemperatureDataReadyCallbackArg;

    FuncImuSampleDataReadyCallbackType      funcSampleDataReadyCallback;
    ImuSampleDataReadyType*                 funcSampleDataReadyCallbackArg;

    ImuStateEnum                            stateCommon;

    ImuDataType                             gyroData;
    ImuDataType                             accelData;

    ImuRawDataType                          gyroRaw;
    ImuRawDataType                          accelRaw;

    float                                   temperature;

    uint8_t                                 chipID;
    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham delay su dung trong cai dat
     *  @param[in] time (ms) thoi gian
     *  @return none
     * */
    static void Delay(uint32_t time)
    {
        g_hardware_delay(time);
    }

    /** @brief ham convert 2byte fifo thanh int16
        @param arg dia chi bien dua vao
        @return none
    */
    virtual int16_t U8ToI16(uint8_t* addr)
    {
        return (int16_t)((int16_t)*addr << 8) | (int16_t)*(addr + 1);
    }

    /** @brief ham read gia tri
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] rxbuff dia chi mang nhan
     *  @param[in] len do dai can truyen nhan
     *  @return trang thai truyen
     * */
    virtual StatusEnum ReadData(uint8_t* txbuff, uint8_t* rxbuff, int len)
    {
        if(pspi != nullptr && pCsPin != nullptr)
        {
            StatusEnum ret = OK;

            pspi->Lock();

            /// select
            pCsPin->On();

            /// ham truyen nhan SPI
            ret = pspi->Read(txbuff, rxbuff, len);

            /// unselect
            pCsPin->Off();

            pspi->Unlock();

            return ret;
        }

        return ERROR_HAL;
    }

    /** @brief ham send get gia tri khi cai dat
     *  @param[in] txbuff dia chi mang truyen
     *  @param[in] rxbuff dia chi mang nhan
     *  @param[in] len do dai can truyen nhan
     *  @return trang thai truyen nhan
     * */
    virtual StatusEnum SendGet(uint8_t* txbuff, uint8_t* rxbuff, int len)
    {
        if(pspi != nullptr && pCsPin != nullptr)
        {
            StatusEnum ret = OK;

            pspi->Lock();

            /// select
            pCsPin->On();

            /// ham truyen nhan SPI
            ret = pspi->SendGet(txbuff, rxbuff, len);

            /// unselect
            pCsPin->Off();

            pspi->Unlock();

            return ret;
        }

        return ERROR_HAL;
    }

    /** @brief ham read thanh ghi
     *  @param[in] reg dia chi thanh ghi
     *  @param[in] pval dia chi lay gia tri trong thanh ghi
     *  @return trang thai qua trinh doc thanh ghi
     * */
    virtual StatusEnum ReadReg(uint8_t reg, uint8_t* pval)
    {
        StatusEnum ret = OK;

        uint8_t txbuff[2] = {reg, 0x00};
        uint8_t rxbuff[2] = {0x00, 0x00};

        /// doc spi
        ret = SendGet(txbuff, rxbuff, 2);

        /// lay gia tri
        *pval = rxbuff[1];

        return ret;
    }

    /** @brief ham read thanh ghi
     *  @param[in] reg dia chi thanh ghi
     *  @param[in] pval dia chi lay gia tri trong thanh ghi
     *  @param[in] retryNum so lan doc
     *  @return trang thai qua trinh doc thanh ghi
     * */
    virtual StatusEnum ReadReg(uint8_t reg, uint8_t* pval, int retryNum)
    {
        StatusEnum ret = ERROR_NOT_EXIST;
        int i;

        /// tao vong quet kiem tra
        for(i = 0; i < retryNum; i++)
        {
            /// write thanh ghi
            ret = ReadReg(reg, pval);
            Delay(10);

            /// read thanh ghi
            if(*pval != 0)
            {
                return OK;
            }
        }

        return ret;
    }

    /** @brief ham write vao thanh ghi
     *  @param[in] reg dia chi thanh ghi
     *  @param[in] val gia tri ghi vao thanh ghi
     *  @return trang thai qua trinh ghi vao thanh ghi
     * */
    virtual StatusEnum WriteReg(uint8_t reg, uint8_t val)
    {
        StatusEnum ret = OK;
        uint8_t txbuff[2] = {reg,  val};
        uint8_t rxbuff[2] = {0x00, 0x00};

        /// doc spi
        ret = SendGet(txbuff, rxbuff, 2);

        return ret;
    }

    /** @brief ham write vao thanh ghi co doc lai de kiem tra voi gia tri set
     *  @param[in] reg dia chi thanh ghi
     *  @param[in] val gia tri ghi vao thanh ghi
     *  @param[in] retryNum so lan thu lai
     *  @return trang thai qua trinh ghi vao thanh ghi
     * */
    virtual StatusEnum WriteReg(uint8_t reg, uint8_t val, int retryNum)
    {
        StatusEnum ret = ERROR_NOT_EXIST;
        uint8_t temp;
        int i;

        /// tao vong quet kiem tra
        for(i = 0; i < retryNum; i++)
        {
            /// write thanh ghi
            ret = WriteReg(reg, val);
            Delay(10);

            /// read thanh ghi
            if(ret == OK)
            {
                ret = ReadReg(reg, &temp);
                Delay(10);

                /// kiem tra gia tri ghi vao va doc duoc
                if(temp == val)
                {
                    return OK;
                }
            }
            else
            {
                return ret;
            }
        }

        return ERROR_NOT_EXIST;
    }

    /** @brief ham write vao thanh ghi
     *  @param[in] reg dia chi thanh ghi
     *  @param[in] val gia tri ghi vao thanh ghi
     *  @param[in] mask mat na cua gia tri truyen di
     *  @param[in] retryNum so lan thu lai
     *  @return trang thai qua trinh ghi vao thanh ghi
     * */
    virtual StatusEnum WriteReg(uint8_t reg, uint8_t mask, uint8_t val, int retryNum)
    {
        StatusEnum ret = ERROR_NOT_EXIST;
        uint8_t temp;
        int i;

        /// tao vong quet kiem tra
        for(i = 0; i < retryNum; i++)
        {
            /// write thanh ghi
            ret = WriteReg(reg, val);
            Delay(10);

            /// read thanh ghi
            if(ret == OK)
            {
                ret = ReadReg(reg, &temp);
                Delay(10);

                /// kiem tra gia tri ghi vao va doc duoc
                temp = temp & mask;
                if(temp == val)
                {
                    return OK;
                }
            }
            else
            {
                return ret;
            }
        }


        return ERROR_NOT_EXIST;
    }

    /** @brief ham lay gia tri thanh ghi theo gia tri thuc
     *  @param[in] pConfigArray dia chi cua mang config
     *  @param[in] pConfig dia chi gia tri cai dat
     *  @param[in] pcmd gia tri thanh ghi can lay
     *  @param[in] val gia tri thuc
     *  @return trang thai qua trinh doc
     * */
    virtual StatusEnum GetRegistersFormValueReal(const ImuConfigRegistersType*  pConfigArray,
                                                 const ImuConfigRegistersType** pConfig,
                                                 int   configSize,
                                                 float val)
    {
        StatusEnum ret = ERROR_NOT_EXIST;
        const ImuConfigRegistersType* pconfigAfter;
        const ImuConfigRegistersType* pconfigBefor;
        float deltaAfter, deltaBefor, absDeltaAfter, absDeltaBefor;
        int i;

        ///
        for(i = 0; i < configSize - 1; i++)
        {
            pconfigAfter = &pConfigArray[i];
            pconfigBefor = &pConfigArray[i + 1];


            deltaAfter = val - pconfigAfter->real;
            deltaBefor = val - pconfigBefor->real;

            absDeltaAfter = abs(deltaAfter);
            absDeltaBefor = abs(deltaBefor);

            ///
            if(absDeltaAfter <= 0.01f)
            {
                *pConfig = pconfigAfter;
                return OK;
            }
            else if(absDeltaBefor <= 0.01f)
            {
                *pConfig = pconfigBefor;
                return OK;
            }
            else if(deltaAfter > 0.01f && deltaBefor < -0.01f)
            {
                if(absDeltaAfter < absDeltaBefor)
                {
                    *pConfig = pconfigAfter;
                    return OK;
                }
                else
                {
                    *pConfig = pconfigBefor;
                    return OK;
                }
            }
            else
            {
                if(i == 0)
                {
                    if(deltaAfter < -0.01f && deltaBefor < -0.01f)
                    {
                        *pConfig = pconfigAfter;
                        return OK;
                    }
                }
                else if(i == (configSize - 2))
                {
                    if(deltaAfter > 0.01f && deltaBefor > 0.01f)
                    {
                        *pConfig = pconfigBefor;
                        return OK;
                    }
                }
            }
        }

        return ret;
    }

private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_BASE_MODULE_BASE_IMU_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
