#ifndef __STORAGE_PARAM_HPP__
#define __STORAGE_PARAM_HPP__

#include <cstdint>
#include <cstring>
#include "Hardware/hardware.hpp"
#include "wwdg.h"

/* ---------------------------------------------------------------------------
 * Exported Constants & Flash Addresses
 * --------------------------------------------------------------------------- */
#ifndef PARAM_ADDRESS_NET
#define PARAM_ADDRESS_NET   0x0807E000   // Page 31 of Bank2
#endif
#ifndef PARAM_MAGIC_NET
#define PARAM_MAGIC_NET     0xA55A1234
#endif

#ifndef PARAM_ADDRESS_IMU
#define PARAM_ADDRESS_IMU   0x0807C000   // Page 30 of Bank2
#endif
#ifndef PARAM_MAGIC_IMU
#define PARAM_MAGIC_IMU     0xA55A5678
#endif

// Backward compatibility defines
#ifndef PARAM_ADDRESS
#define PARAM_ADDRESS       PARAM_ADDRESS_NET
#endif
#ifndef PARAM_MAGIC
#define PARAM_MAGIC         PARAM_MAGIC_NET
#endif

/* ---------------------------------------------------------------------------
 * Configuration Structures
 * --------------------------------------------------------------------------- */
#pragma pack(push, 1)
typedef struct {
    uint8_t  droneIp[4];
    uint8_t  somIp[4];
    uint8_t  ownIp[4];
    uint16_t dronePort;
    uint16_t somPort;
    uint8_t  udpModeDrone;       // 0: fixed dest IP/port, 1: listen-only, 2: broadcast
    uint8_t  udpModeDevice;      // 0: fixed dest IP/port, 1: listen-only, 2: broadcast
    uint8_t  reserved[2];        // Padding to 4-byte boundary
    uint32_t magic;              // Magic number marker (Offset 20..23)
} NetworkConfig_t;

typedef struct {
    float    accBias[3];         // Accel bias X, Y, Z in g
    float    gyroBias[3];        // Gyro bias X, Y, Z in dps
    uint8_t  isCalibrated;       // 1: Calibrated, 0: Raw
    uint8_t  orientation;        // Sensor orientation detected during calib
    uint8_t  reserved[2];        // Padding to 4-byte boundary
    uint32_t magic;              // Magic number marker
} ImuCalibConfig_t;
#pragma pack(pop)

using NetworkConfig = NetworkConfig_t;

namespace Storage {

/* ---------------------------------------------------------------------------
 * Storage Base Template Class
 * --------------------------------------------------------------------------- */
template <typename T>
class ParamBase {
public:
    ParamBase(uint32_t address, uint32_t magic)
        : address_(address), magic_(magic) {}

    virtual ~ParamBase() = default;

    void Init(HW::Peripheral::Flash& flash) {
        flash_ = &flash;
        LoadFromFlash();
    }

    virtual T GetDefault() const = 0;

    bool LoadFromFlash() {
        const T* flashPtr = reinterpret_cast<const T*>(address_);

        if (flashPtr != nullptr && GetMagic(*flashPtr) == magic_) {
            config_ = *flashPtr;
        } else {
            config_ = GetDefault();
        }
        return true;
    }

    bool SaveToFlash() {
        if (!flash_) {
            return false;
        }

        alignas(16) uint8_t buffer[128];
        std::memset(buffer, 0xFF, sizeof(buffer));
        std::memcpy(buffer, &config_, sizeof(T));

        HAL_WWDG_Refresh(&hwwdg);

        bool erased = flash_->eraseFlash(sizeof(buffer), address_);
        if (!erased) {
            return false;
        }

        HAL_WWDG_Refresh(&hwwdg);

        uint32_t written = flash_->writeFlashBytestWithoutLock(address_, buffer, sizeof(buffer));

        HAL_WWDG_Refresh(&hwwdg);

        return (written == sizeof(buffer));
    }

    bool Save() {
        return SaveToFlash();
    }

    bool SaveToFlash(const T& config) {
        config_ = config;
        return SaveToFlash();
    }

    T& GetConfig() { return config_; }
    const T& GetConfig() const { return config_; }

protected:
    uint32_t GetMagic(const T& cfg) const {
        uint32_t magicVal = 0;
        std::memcpy(&magicVal, reinterpret_cast<const uint8_t*>(&cfg) + sizeof(T) - sizeof(uint32_t), sizeof(uint32_t));
        return magicVal;
    }

    uint32_t               address_;
    uint32_t               magic_;
    HW::Peripheral::Flash* flash_ = nullptr;
    T                      config_{};
};

/* ---------------------------------------------------------------------------
 * Network Parameter Storage
 * --------------------------------------------------------------------------- */
class NetworkParam : public ParamBase<NetworkConfig_t> {
public:
    static NetworkParam& GetInstance() {
        static NetworkParam instance;
        return instance;
    }

    NetworkConfig_t GetDefault() const override;

    bool SetParam(const char* param_id, const char* value);
    bool GetParamString(const char* param_id, char* value, uint32_t value_size) const;

private:
    NetworkParam() : ParamBase(PARAM_ADDRESS_NET, PARAM_MAGIC_NET) {}
};

/* ---------------------------------------------------------------------------
 * IMU Parameter Storage
 * --------------------------------------------------------------------------- */
class ImuParam : public ParamBase<ImuCalibConfig_t> {
public:
    static ImuParam& GetInstance() {
        static ImuParam instance;
        return instance;
    }

    ImuCalibConfig_t GetDefault() const override;

private:
    ImuParam() : ParamBase(PARAM_ADDRESS_IMU, PARAM_MAGIC_IMU) {}
};

/* ---------------------------------------------------------------------------
 * Central Parameter Storage Manager
 * --------------------------------------------------------------------------- */
class Manager {
public:
    enum class ParamType : uint8_t {
        NETWORK = (1 << 0),
        IMU     = (1 << 1),
        ALL     = 0xFF
    };

    static Manager& GetInstance() {
        static Manager instance;
        return instance;
    }

    void Init(HW::Peripheral::Flash& flash) {
        NetworkParam::GetInstance().Init(flash);
        ImuParam::GetInstance().Init(flash);
    }

    void RequestSave(ParamType type = ParamType::ALL) {
        dirty_flags_ |= static_cast<uint8_t>(type);
    }

    void Process();

    NetworkParam& Network() { return NetworkParam::GetInstance(); }
    ImuParam&     Imu()     { return ImuParam::GetInstance(); }

private:
    Manager() = default;

    volatile uint8_t dirty_flags_{0};
};

} // namespace Storage

/* ---------------------------------------------------------------------------
 * Global Compatibility Aliases
 * --------------------------------------------------------------------------- */
using StorageManager = Storage::Manager;
using StorageNetworkParam = Storage::NetworkParam;
using StorageImuParam = Storage::ImuParam;
using StorageParam = Storage::NetworkParam;

#endif /* __STORAGE_PARAM_HPP__ */
