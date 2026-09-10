 
/** 
  ******************************************************************************
  * @file    module_include.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 24, 2023
  * @brief   This file contains all the functions prototypes for the module_include.cpp 
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

#ifndef __MODULES_MODULE_INCLUDE_HPP__
#define __MODULES_MODULE_INCLUDE_HPP__

/* Includes ------------------------------------------------------------------*/

#include "Hardware/hardware.hpp"

/// PROTOCOL ___________________________________________________________________

/// MAVLINK
#define MAVLINK_TX_MSG_ONESHOOT_NUM     50

//#define MAV_TX_CONTINUOUS_MSG_NUM   20
//
//#define MAV_TYPE_ONBOARD_TESTER     123 /*!< Gremsy ONLY*/
//
//
// we have separate helpers disabled to make it possible
// to select MAVLink 1.0 in the arduino GUI build
//#define MAVLINK_SEPARATE_HELPERS
//#define MAVLINK_NO_CONVERSION_HELPERS 

// allow five telemetry ports
#define MAVLINK_COMM_NUM_BUFFERS                    6

#include "ThirdParty/MavlinkV2/ardupilotmega/version.h"

#define MAVLINK_MAX_PAYLOAD_LEN                     255

#include "ThirdParty/MavlinkV2/mavlink_types.h"

#include "ThirdParty/MavlinkV2/ardupilotmega/mavlink.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

namespace Module
{
    /// TYPEDEF ________________________________________________________________
    /// MAVLINK
    /** @brief trang thai qua trinh hoat dong cua mavlink */
    typedef enum
    {
        PROTOCOL_MAVLINK_STATE_IDLE,
        PROTOCOL_MAVLINK_STATE_INIT,
        PROTOCOL_MAVLINK_STATE_TRANSMIT_HEARTBEAT,
        PROTOCOL_MAVLINK_STATE_TRANSMIT,
        PROTOCOL_MAVLINK_STATE_LOSTCONNECTION,
        PROTOCOL_MAVLINK_STATE_RECONNECTION,
        PROTOCOL_MAVLINK_STATE_DONE,
        PROTOCOL_MAVLINK_STATE_ERROR,
    }
    ProtocolMavlinkStateEnum;

    /** @brief kieu hoat dong cua mavlink
     * */
    typedef enum
    {
        PROTOCOL_MAVLINK_TYPE_SLAVE,
        PROTOCOL_MAVLINK_TYPE_MASTER,
    }
    ProtocolMavlinkTypeEnum;

    /** @brief dinh nghia ham su dung de write mavlink
     *  @param[in] sender dia chi doi tuong goi mavlink
     *  @param[in] buff dia chi mang truyen tu mavlink
     *  @param[in] buffsize do lon cua mang truyen tu mavlink
     *  @return trang thai qua trinh truyen du lieu
     * */
    typedef struct
    {
        uint32_t    senderLL;
        uint32_t    buffsize;
    }
    ProtocolMavlinkWriteType;
    typedef StatusEnum (*FuncProtocolMavlinkWriteCallbackType)(ProtocolMavlinkWriteType* handle, uint8_t* buff);

    /** @brief dinh nghia ham su dung de truyen msg trong mavlink
     *  @param[in] sender dia chi doi tuong goi mavlink
     *  @param[in] ptrMsg dia chi cua bien txmsg trong mavlink
     *  @param[in] msgBuffLen do dai cua mang truyen mavlink
     *  @return trang thai qua trinh truyen msg
     * */
    typedef struct
    {
        uint32_t            sender;
        uint32_t            msgBuffLen;
    }
    ProtocolMavlinkTransmittedMessageType;
    typedef StatusEnum (*FuncProtocolMavlinkTransmittedMessageCallbackType)(ProtocolMavlinkTransmittedMessageType* handle, mavlink_message_t* ptrMsg);

    /** @brief dinh nghia ham su dung de truyen msg trong mavlink
     *  @param[in] sender dia chi doi tuong goi mavlink
     *  @param[in] ptrMsg dia chi cua bien txmsg trong mavlink
     *  @param[in] msgBuffLen do dai cua mang truyen mavlink
     *  @return trang thai qua trinh truyen msg
     * */
    typedef struct
    {
        uint32_t            sender;
        uint32_t            msgBuffLen;
    }
    ProtocolMavlinkTransmittedMessageIsNotExistType;
    typedef StatusEnum (*FuncProtocolMavlinkTransmittedMessageIsNotExistCallbackType)(ProtocolMavlinkTransmittedMessageIsNotExistType* handle, mavlink_message_t* ptrMsg);

    /** @brief dinh nghia ham su dung de nhan msg trong mavlink
     *  @param[in] sender dia chi doi tuong goi mavlink
     *  @param[in] ptrMsg dia chi bien rxmsg trong mavlink
     *  @param[in] time thoi gian giua cac lan truyen mavlink
     *  @return trang thai qua trinh nhan msg
     * */
    typedef struct
    {
        uint32_t            sender;
        uint32_t            time;
    }
    ProtocolMavlinkReceivedMessageType;
    typedef StatusEnum (*FuncProtocolMavlinkReceivedMessageCallbackType)(ProtocolMavlinkReceivedMessageType* handle, mavlink_message_t* ptrMsg);

    /** @brief dinh nghia ham nhan cac msg khong nam trong cai dat cua mavlink
     *  @param[in] sender dia chi cua doi tuong goi mavlink
     *  @param[in] ptrMsg dia chi bien rxmsg trong mavlink
     *  @return trang thai qua trinh nhan msg
     * */
    typedef struct
    {
        uint32_t            sender;
    }
    ProtocolMavlinkReceivedMessageIsNotExistType;
    typedef StatusEnum(*FuncProtocolMavlinkReceivedMessageIsNotExistCallbackType)(ProtocolMavlinkReceivedMessageIsNotExistType* handle, mavlink_message_t* ptrMsg);

    /** @brief dinh nghia ham msg heartbeat trong mavlink
     *  @param[in] sender dia chi cua doi tuong goi mavlink
     *  @param[in] ptrParam dia chi cua param heartbeat nhan duoc
     *  @param[in] systemIdSender dia chi system id cua doi tuong truyen den
     *  @param[in] compIdSender dia chi component id cua doi tuong truyen den
     *  @return trang thai qua trinh truyen msg heartbeat
     * */
    typedef struct
    {
        uint32_t                sender;
        uint8_t                 systemIdSender;
        uint8_t                 compIdSender;
    }
    ProtocolMavlinkReceivedMessageHeartbeatType;
    typedef StatusEnum (*FuncProtocolMavlinkReceivedMessageHeartbeatCallbackType)(ProtocolMavlinkReceivedMessageHeartbeatType* handle, mavlink_heartbeat_t* pheartbeat);

    /** @brief dinh nghia ham msg heartbeat trong mavlink
     *  @param[in] sender dia chi cua doi tuong goi mavlink
     *  @param[in] ptrParam dia chi cua param heartbeat truyen
     *  @param[in] systemId system id muon truyen qua heartbeat
     *  @param[in] compId component id muon truyen qua heartbeat
     *  @return trang thai qua trinh truyen
     * */
    typedef struct
    {
        uint32_t                sender;
        uint8_t                 systemId;
        uint8_t                 compId;
    }
    ProtocolMavlinkTransmittedMessageHeartbeatType;
    typedef StatusEnum (*FuncProtocolMavlinkTransmittedMessageHeartbeatCallbackType)(ProtocolMavlinkTransmittedMessageHeartbeatType* handle, mavlink_heartbeat_t* phearbeat);

    /// REMOTE
    /// dinh nghia trang thai hoat dong cua remote
    typedef enum
    {
        PROTOCOL_REMOTE_STATE_IDLE,
        PROTOCOL_REMOTE_STATE_INIT,
        PROTOCOL_REMOTE_STATE_PLUG,
        PROTOCOL_REMOTE_STATE_RUN,
        PROTOCOL_REMOTE_STATE_UNPLUG,
        PROTOCOL_REMOTE_STATE_DONE,
        PROTOCOL_REMOTE_STATE_ERROR,
    }
    ProtocolRemoteStateEnum;

    /// dinh nghia kieu hoat dong cua remote
    typedef enum
    {
        PROTOCOL_REMOTE_MODE_NONE       = 0x00,
        PROTOCOL_REMOTE_MODE_RX_ONLY    = 0x01,
        PROTOCOL_REMOTE_MODE_TX_ONLY    = 0x02,
        PROTOCOL_REMOTE_MODE_TX_AND_RX  = 0x03,
    }
    ProtocolRemoteModeEnum;

    /** @brief dinh nghia ham su dung de write remote
     *  @param[in] sender dia chi doi tuong goi remote
     *  @param[in] buff dia chi mang truyen tu remote
     *  @param[in] buffsize do lon cua mang truyen tu remote
     *  @return trang thai qua trinh truyen du lieu
     * */
    typedef struct
    {
        uint32_t    sender;
        uint32_t    buffsize;
    }
    ProtocolRemoteWriteType;
    typedef StatusEnum (*FuncProtocolRemoteWriteCallbackType)(ProtocolRemoteWriteType* handle, uint8_t* buff);

    /** @brief dinh nghia ham su dung bao loi cho protcol remote
     *  @param[in] sender dia chi doi tuong goi remote
     *  @param[in] errorCode
     *  @return trang thai qua trinh truyen du lieu
     * */
    typedef struct
    {
        uint32_t sender;
        uint32_t errorCode;
    }
    ProtocolRemoteErrorType;
    typedef StatusEnum (*FuncProtocolRemoteErrorCallbackType)(ProtocolRemoteErrorType* handle);

    /** @brief dinh nghia ham tra ve cua protocol remote
     *  @param[in] sender dia chi doi tuong goi remote
     *  @param[in] channles dia chi mang gia tri channel tra ve
     *  @param[in] channelCount so channel tra ve
     *  @param[in] channelValueMin gia tri nho nhat cua channel
     *  @param[in] channelValueMax gia tri lon nhat cua channel
     *  @param[in] channelValueMid gia tri o giua cua channel
     *  @param[in] channelValueRange khoang gia tri cua channel
     *  @return trang thai qua tring
     * */
    typedef struct
    {
        uint32_t  sender;
        uint16_t  channelCount;
        uint16_t  channelValueMin;
        uint16_t  channelValueMax;
        uint16_t  channelValueMid;
        uint16_t  channelValueRange;
    }
    ProtocolRemoteDataReadyType;
    typedef StatusEnum (*FuncProtocolRemoteDataReadyCallbackType)(ProtocolRemoteDataReadyType* handle, uint16_t* channels);


    /// MOTOR __________________________________________________________________
    /// BRUSHLESS

    /** @brief dinh nghia ham duoc goi khi can update pwm cho motor */
    typedef struct
    {
        uint32_t    sender;

        int         in1;
        int         in2;
        int         in3;

        uint32_t    frequency;
    }
    MotorBrushlessUpdatePwmType;
    typedef StatusEnum (*FuncMotorBrushlessUpdatePwmCallbackType)(MotorBrushlessUpdatePwmType* handle);

    /// ENCODER ________________________________________________________________
    /// ENCODER PWM
    /** @brief dinh nghia trang thai hoat dong cua encoder pwm */
    typedef enum
    {
        ENCODER_PWM_STATE_IDLE,
        ENCODER_PWM_STATE_INIT,
        ENCODER_PWM_STATE_RUN,
        ENCODER_PWM_STATE_DONE,
        ENCODER_PWM_STATE_ERROR,
    }
    EncoderPwmStateEnum;

    /** @brief dinh nghia ham tra ve khi co du lieu cua encoder */
    typedef struct
    {
        uint32_t    sender;
        int32_t     count;
    }
    EncoderPwmDataReadyType;
    typedef StatusEnum (*FuncEncoderPwmDataReadyCallbackType)(EncoderPwmDataReadyType* handle);

    /** @brief dinh nghia ham tra ve khi co ket noi lai
     *  @param[in] sender doi tuong goi
     *  @return trang thai qua trinh ham tra ve
     * */
    typedef struct
    {
        uint32_t sender;
    }
    EncoderPwmReconnectType;
    typedef StatusEnum (*FuncEncoderPwmReconnectCallbackType)(EncoderPwmReconnectType* handle);

    /** @brief dinh nghia ham tra ve khi mat ket noi
     *  @param[in] sender doi tuong goi
     *  @return trang thai qua trinh ham tra ve
     * */
    typedef struct
    {
        uint32_t sender;
    }
    EncoderPwmLostconnectType;
    typedef StatusEnum (*FuncEncoderPwmLostconnectCallbackType)(EncoderPwmLostconnectType* handle);


    /// ENCODER NORMAL
    /**
     * */
    typedef enum
    {
        ENCODER_STATE_IDLE,
        ENCODER_STATE_INIT,
        ENCODER_STATE_RUN,
        ENCODER_STATE_DATA,
        ENCODER_STATE_DONE,
        ENCODER_STATE_ERROR,
    }
    EncoderStateEnum;

    /**
     * */
    typedef enum
    {
        ENCODER_TRIM_NONE,
        ENCODER_TRIM_X = 1,
        ENCODER_TRIM_Y,
        ENCODER_TRIM_XY,
    }
    EncoderTrimEnum;

    /**
     * */
    typedef struct
    {
        uint32_t    sender;
        int32_t     count;
    }
    EncoderDataReadyType;
    typedef StatusEnum (*FuncEncoderDataReadyCallbackType)(EncoderDataReadyType* handle);

    /** @brief dinh nghia ham tra ve khi co ket noi lai
     *  @param[in] sender doi tuong goi
     *  @return trang thai qua trinh ham tra ve
     * */
    typedef struct
    {
        uint32_t    sender;
    }
    EncoderReconnectType;
    typedef StatusEnum (*FuncEncoderReconnectCallbackType)(EncoderReconnectType* handle);

    /** @brief dinh nghia ham tra ve khi mat ket noi
     *  @param[in] sender doi tuong goi
     *  @return trang thai qua trinh ham tra ve
     * */
    typedef struct
    {
        uint32_t    sender;
    }
    EncoderLostconnectType;
    typedef StatusEnum (*FuncEncoderLostconnectCallbackType)(EncoderLostconnectType* handle);
    /// IMU ____________________________________________________________________

    /** @brief dinh nghia trang thai qua trinh hoat dong imu */
    typedef enum
    {
        IMU_STATE_IDLE,
        IMU_STATE_INIT,
        IMU_STATE_COMPARE_TIME,
        IMU_STATE_FLUSH_FIFO,
        IMU_STATE_LENGTH,
        IMU_STATE_DATA,
        IMU_STATE_DONE,
        IMU_STATE_ERROR,
    }
    ImuStateEnum;

    /** @brief dinh ngia gyro range */
    typedef enum
    {
        IMU_GYRO_RANGE_NONE,
        IMU_GYRO_RANGE_2000dps,
        IMU_GYRO_RANGE_1000dps,
        IMU_GYRO_RANGE_500dps,
        IMU_GYRO_RANGE_250dps,
        IMU_GYRO_RANGE_125dps,
        IMU_GYRO_RANGE_62_5dps,  //only ICM42688 support
        IMU_GYRO_RANGE_31_25dps, //only ICM42688 support
        IMU_GYRO_RANGE_15_625dps,//only ICM42688 support
        IMU_GYRO_RANGE_COUNT,
    }
    ImuGyroRangeEnum;

    /** @brief dinh nghia accel range */
    typedef enum
    {
        IMU_ACCEL_RANGE_NONE,
        IMU_ACCEL_RANGE_2G,
        IMU_ACCEL_RANGE_4G,
        IMU_ACCEL_RANGE_8G,
        IMU_ACCEL_RANGE_16G,
        IMU_ACCEL_RANGE_COUNT,
    }
    ImuAccelRangeEnum;

    /** @brief dinh nghia output data rate */
    typedef enum
    {
        IMU_ODR_NONE,
        IMU_ODR_0_78125,
        IMU_ODR_1_5625,
        IMU_ODR_3_125,
        IMU_ODR_6_25,
        IMU_ODR_12_5,
        IMU_ODR_25,
        IMU_ODR_50,
        IMU_ODR_100,
        IMU_ODR_200,
        IMU_ODR_400,
        IMU_ODR_500,
        IMU_ODR_800,
        IMU_ODR_1600,
        IMU_ODR_2000,
        IMU_ODR_3200,
        IMU_ODR_4000,
        IMU_ODR_8000,
        IMU_ODR_16000,
        IMU_ODR_32000,
        IMU_ODR_COUNT,
    }
    ImuOutputDataRateEnum;

    /** @brief dinh nghia cau hinh cho imu theo gia tri thuc
     *  @param[in] cmd gia tri thanh ghi
     *  @param[in] real gia tri thuc
     *  @param[in] scale he so chia khi quy doi gia tri
     * */
    typedef struct
    {
        uint8_t cmd;
        float   real;
        float   scaler;
    }
    ImuConfigRegistersType;

    /** @brief dinh nghia gia tri cua imu
     *  @param[in] x gia tri theo raw truc x cua imu (theo datasheet)
     *  @param[in] y gia tri theo raw truc y cua imu (theo datasheet)
     *  @param[in] z gia tri theo raw truc z cua imu (theo datasheet)
     *  @param[in] time thoi gian doc
     *  @param[in] scaler ti so qu doi ra van toc goc (do/s)
     *  @param[in] count so mau da lay
     * */
    typedef struct
    {
        float           x;
        float           y;
        float           z;
        float           timeConfig;     /// s
        float           timeReal;       /// s
        float           scaler;
        uint32_t        count;
    }
    ImuDataType;

    /** @brief dinh nghia gia tri cua imu raw
     *  @param[in] timeReal     gia tri dung de do thoi gian thuc của imu tra ve
     *  @param[in] timeConfig   gia tri thoi gian
     * */
    typedef struct
    {
        HW::Peripheral::Time    timeReal;          /// dung de doc thoi gian trong su kien doc data raw
        float                   timeConfig;        /// thoi gian khi cai dat toc do tra ve cua data raw

        int32_t                 x, y, z;
        int32_t                 xSum, ySum, zSum;
        int32_t                 sumCount;

    }
    ImuRawDataType;

    /** @brief dinh nghia ham tra gia tri ve cua imu
     *  @param[in] sender dia chi doi tuong goi ham
     *  @param[in] data gia tri tra ve
     *  @return trang thai qua trinh tra ve data
     * */
    typedef struct
    {
        uint32_t        sender;
    }
    ImuGyroDataReadyType;
    typedef StatusEnum (*FuncImuGyroDataReadyCallbackType)(ImuGyroDataReadyType* handle, ImuDataType* data);

    /** @brief dinh nghia ham tra gia tri ve cua imu
     *  @param[in] sender dia chi doi tuong goi ham
     *  @param[in] data gia tri tra ve
     *  @return trang thai qua trinh tra ve data
     * */
    typedef struct
    {
        uint32_t        sender;
    }
    ImuAccelDataReadyType;
    typedef StatusEnum (*FuncImuAccelDataReadyCallbackType)(ImuAccelDataReadyType* handle, ImuDataType* data);

    /** @brief dinh nghia ham tra gia tri ve cua imu
     *  @param[in] sender dia chi doi tuong goi ham
     *  @param[in] data gia tri tra ve
     *  @return trang thai qua trinh tra ve data
     * */
    typedef struct
    {
        uint32_t        sender;
        float           val;
    }
    ImuTemperatureDataReadyType;
    typedef StatusEnum (*FuncImuTemperatureDataReadyCallbackType)(ImuTemperatureDataReadyType* handle);

    typedef struct
    {
        uint32_t        sender;
        int16_t         rawAccel[3];
        int16_t         rawGyro[3];
        int8_t          temp;
        uint8_t         header;
        uint16_t        timeStamp;
        bool            isFsync;
    }
    ImuSampleDataReadyType;
    typedef StatusEnum (*FuncImuSampleDataReadyCallbackType)(ImuSampleDataReadyType* handle, const ImuDataType* accel, const ImuDataType* gyro);


    /// BASE ___________________________________________________________________
    namespace Base
    {
        class Common;
        class ImuSpi;
        class StorageEeprom;
        class ProtocolMavlink;
        class ProtocolRemote;
        class MotorBrushless;
        class EncoderPwm;
        class Encoder;
    }

    /// NAMESPACE ______________________________________________________________
    /// IMU
    namespace Imu
    {
        class Bmi160;
        class Icm42688;
        class Adxl355bez;
        class Icm45686;
    }

    /// ENCODER
    namespace Encoder
    {
        class Ma730;
        class Mlx90316;
        class Ma600Pwm;
        class Ma600Spi;
    }

    /// STORAGE
    namespace Storage
    {
        class EepromFlash;
    }

    /// PROTOCOL
    namespace Protocol
    {

        /// CLASS
        class Mavlink;
        class Sbus;
        class Jr;
        class PPM;
        class Lb;
    }

    /// MOTOR
    namespace Motor
    {
        /// CLASS
        class Brushless;
    }
}

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_MODULE_INCLUDE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
