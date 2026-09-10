 
/** 
  ******************************************************************************
  * @file    module_base_motor_brushless.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 11, 2023
  * @brief   This file contains all the functions prototypes for the module_base_motor_brushless.cpp 
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

#ifndef __MODULES_BASE_MODULE_BASE_MOTOR_BRUSHLESS_HPP__
#define __MODULES_BASE_MODULE_BASE_MOTOR_BRUSHLESS_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung cho cho cac kieu motor bldc
class Module::Base::MotorBrushless : public Module::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    uint32_t                frequency;
    uint32_t                dutyMax;
    float                   powerMax;
    uint32_t                poleNumber;

    RotationDirectionEnum   direction;
    SintableTypeEnum        sintableType;


    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    MotorBrushless()
    {
        frequency                   = 0;
        dutyMax                     = 0;
        powerMax                    = 0;
        poleNumber                  = 0;
        direction                   = NORMAL;
        sintableType                = SINTABLE_TYPE_NORMAL;

        SINTABLE                    = NULL;
        sintableValueMaxDiv2        = 0;

        isTurn                      = false;

        polePairsNumber             = 0;
        countPerPolePairs           = 0;
        countPerRound               = 0;

        stepA                       = 0;
        stepB                       = 0;
        stepC                       = 0;

        stepBOffset                 = 0;
        stepCOffset                 = 0;

        pwmRatio                    = 0;

        stepPolePairs               = 0;

        funcUpdatePwmCallback       = NULL;
        funcUpdatePwmCallbackArg    = NULL;
    }

    /** @breif operator copy constructor */
    MotorBrushless(const MotorBrushless &c)
    {
        frequency                   = c.frequency;
        dutyMax                     = c.dutyMax;
        powerMax                    = c.powerMax;
        poleNumber                  = c.poleNumber;
        direction                   = c.direction;
        sintableType                = c.sintableType;

        SINTABLE                    = c.SINTABLE;
        sintableValueMaxDiv2        = c.sintableValueMaxDiv2;

        isTurn                      = c.isTurn;

        polePairsNumber             = c.polePairsNumber;
        countPerPolePairs           = c.countPerPolePairs;
        countPerRound               = c.countPerRound;

        stepA                       = c.stepA;
        stepB                       = c.stepB;
        stepC                       = c.stepC;

        stepBOffset                 = c.stepBOffset;
        stepCOffset                 = c.stepCOffset;

        pwmRatio                    = c.pwmRatio;

        stepPolePairs               = c.stepPolePairs;

        funcUpdatePwmCallback       = c.funcUpdatePwmCallback;
        funcUpdatePwmCallbackArg    = c.funcUpdatePwmCallbackArg;
    }

    /** @brief operation delete */
    virtual ~MotorBrushless()
    {
        frequency                   = 0;
        dutyMax                     = 0;
        powerMax                    = 0;
        poleNumber                  = 0;
        direction                   = NORMAL;
        sintableType                = SINTABLE_TYPE_NORMAL;

        SINTABLE                    = NULL;
        sintableValueMaxDiv2        = 0;

        isTurn                      = false;

        polePairsNumber             = 0;
        countPerPolePairs           = 0;
        countPerRound               = 0;

        stepA                       = 0;
        stepB                       = 0;
        stepC                       = 0;

        stepBOffset                 = 0;
        stepCOffset                 = 0;

        pwmRatio                    = 0;

        stepPolePairs               = 0;

        funcUpdatePwmCallback       = NULL;
        funcUpdatePwmCallbackArg    = NULL;
    }

    /** @brief operator assignment */
    MotorBrushless &operator = (const MotorBrushless &c);

    /** @brief operation add ham xuat pwm
     *  @param[in]
     *  @return
     * */
    virtual MotorBrushless &operator += (const FuncMotorBrushlessUpdatePwmCallbackType func){
        funcUpdatePwmCallback = func;

        if(funcUpdatePwmCallbackArg == 0)
        {
            funcUpdatePwmCallbackArg = (MotorBrushlessUpdatePwmType*)g_calloc(sizeof(MotorBrushlessUpdatePwmType));
        }

        funcUpdatePwmCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief function motor on
     *  @param none
     *  @return trang thai qua trinh on
     * */
    virtual StatusEnum On(void){return ERROR_NOT_EXIST;}

    /** @brief function motor on
     *  @param none
     *  @return trang thai qua trinh off
     * */
    virtual StatusEnum Off(void){return ERROR_NOT_EXIST;}

    /** @brief ham di chuyen theo buoc(tuyet doi) cua dong co
     *  @param[in] freq (Hz) bo chia tan so hoat dong cua pwm
     *  @param[in] step buoc di chuyen toi
     *  @param[in] power [0 : 1] cong suat
     *  @return trang thai qua trinh di chuyen
     * */
    virtual StatusEnum MoveAbs(uint32_t freq_prescaler, int32_t step, float power){return ERROR_NOT_EXIST;}

    /** @brief ham di chuyen theo buoc tuyet doi cua dong co
     *  @param[in] step buoc di chuyen toi
     *  @param[in] power [0 : 1] cong suat
     *  @return trang thai qua trinh di chuyen
     * */
    virtual StatusEnum MoveAbs(int32_t step, float power){return ERROR_NOT_EXIST;}

    /** @brief ham di chuyen theo buoc tuong doi cua dong co
     *  @param[in] freq_prescaler bo chia tan so hoat dong cua pwm
     *  @param[in] step buoc di chuyen toi
     *  @param[in] power [0 : 1] cong suat
     *  @return trang thai qua trinh di chuyen
     * */
    virtual StatusEnum MoveInc(uint32_t _frequency, int32_t step, float power){return ERROR_NOT_EXIST;}

    /** @brief ham di chuyen theo buoc tuong doi cua dong co
     *  @param[in] step buoc di chuyen toi
     *  @param[in] power [0 : 1] cong suat
     *  @return trang thai qua trinh di chuyen
     * */
    virtual StatusEnum MoveInc(int32_t step, float power){return ERROR_NOT_EXIST;}

    /** @brief ham lock dong co tai vi tri
     *  @param[in] step vi tri mong muon
     *  @param[in] power cong suat dong co
     *  @return trang thai qua trinh lock
     * */
    virtual StatusEnum Lock(int32_t step, float power){return ERROR_NOT_EXIST;}

    /** @brief ham lock dong co tai vi tri hien tai
     *  @param[in] power cong suat dong co
     *  @return trang thai qua trinh lock
     * */
    virtual StatusEnum Lock(float power){return ERROR_NOT_EXIST;}

    /** @brief ham tra ve gia tri count tren mot vong cua dong co
     *  @param none
     *  @return gia tri count
     * */
    virtual int32_t GetCountPerRound(void){return countPerRound;}

    /** @brief ham tra ve gia tri count tren mot cap cuc cua dong co
     *  @param none
     *  @return gia tri count
     * */
    virtual int32_t GetCountPerPolePairs(void){return countPerPolePairs;}

    /** @brief ham tra ve gia tri set max cua mot cap cuc
     *  @param none
     *  @return gia tri count
     * */
    virtual int32_t GetCountMaxMovePolePairs(void){return stepBOffset;}

    /** @breif ham tra ve trang thai dang hoat dong cua cua dong co
     *  @param none
     *  @return trang thai hoat dong
     * */
    virtual bool GetTurn(void){return isTurn;}

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    SintableType*   SINTABLE;
    int32_t         sintableValueMaxDiv2;

    bool            isTurn;

    int32_t         polePairsNumber;
    int32_t         countPerPolePairs;
    int32_t         countPerRound;

    int32_t         stepA;
    int32_t         stepB;
    int32_t         stepC;

    int32_t         stepBOffset;
    int32_t         stepCOffset;

    float           pwmRatio;

    int32_t         stepPolePairs;

    FuncMotorBrushlessUpdatePwmCallbackType     funcUpdatePwmCallback;
    MotorBrushlessUpdatePwmType*                funcUpdatePwmCallbackArg;


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_BASE_MODULE_BASE_MOTOR_BRUSHLESS_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
