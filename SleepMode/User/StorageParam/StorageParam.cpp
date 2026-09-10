#include "StorageParam.hpp"
#include "Modules/Ethernet_COM/Ethernet_COM.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Storage {

/* ---------------------------------------------------------------------------
 * Network Parameter Defaults
 * --------------------------------------------------------------------------- */
NetworkConfig_t NetworkParam::GetDefault() const {
    NetworkConfig_t cfg = {};
    cfg.droneIp[0]      = 192; cfg.droneIp[1]   = 168;
    cfg.droneIp[2]      = 16;  cfg.droneIp[3]   = 10;
    cfg.somIp[0]        = 192; cfg.somIp[1]     = 168;
    cfg.somIp[2]        = 16;  cfg.somIp[3]     = 20;
    cfg.ownIp[0]        = 192; cfg.ownIp[1]     = 168;
    cfg.ownIp[2]        = 16;  cfg.ownIp[3]     = 227;
    cfg.dronePort       = 14550;
    cfg.somPort         = 14560;
    cfg.udpModeDrone    = 1;
    cfg.udpModeDevice   = 1;
    cfg.magic           = PARAM_MAGIC_NET;
    return cfg;
}

/* ---------------------------------------------------------------------------
 * Network Parameter Parsing & Serialization
 * --------------------------------------------------------------------------- */
bool NetworkParam::SetParam(const char* param_id, const char* value) {
    if (param_id == nullptr || value == nullptr) return false;

    bool changed = false;
    if (strcmp(param_id, "DRONE_IP") == 0 ||
        strcmp(param_id, "DEVICE_IP") == 0 ||
        strcmp(param_id, "OWN_IP") == 0) {
        uint8_t ip[4]{};
        if (!_ParseIpString(value, ip)) return false;
        uint8_t* destination = strcmp(param_id, "DRONE_IP") == 0
            ? config_.droneIp
            : (strcmp(param_id, "DEVICE_IP") == 0 ? config_.somIp : config_.ownIp);
        memcpy(destination, ip, sizeof(ip));
        changed = true;
    } else {
        char* end = nullptr;
        const long number = strtol(value, &end, 10);
        if (end == value || *end != '\0') return false;
        if (strcmp(param_id, "DRONE_PORT") == 0 && number >= 1024 && number <= 65535) {
            config_.dronePort = static_cast<uint16_t>(number);
            changed = true;
        } else if (strcmp(param_id, "DEVICE_PORT") == 0 && number >= 1024 && number <= 65535) {
            config_.somPort = static_cast<uint16_t>(number);
            changed = true;
        } else if (strcmp(param_id, "UDP_MODE_DR") == 0 && number >= 0 && number <= 2) {
            config_.udpModeDrone = static_cast<uint8_t>(number);
            changed = true;
        } else if (strcmp(param_id, "UDP_MODE_DV") == 0 && number >= 0 && number <= 2) {
            config_.udpModeDevice = static_cast<uint8_t>(number);
            changed = true;
        }
    }
    LOGI << "SetParam: " << param_id << " = " << value << (changed ? " (changed)" : " (no change)") << LEND;

    return changed;
}

bool NetworkParam::GetParamString(const char* param_id, char* value, uint32_t value_size) const {
    if (param_id == nullptr || value == nullptr || value_size == 0U) return false;
    value[0] = '\0';
    if (strcmp(param_id, "DRONE_IP") == 0) {
        snprintf(value, value_size, "%u.%u.%u.%u", config_.droneIp[0], config_.droneIp[1], config_.droneIp[2], config_.droneIp[3]);
    } else if (strcmp(param_id, "DEVICE_IP") == 0) {
        snprintf(value, value_size, "%u.%u.%u.%u", config_.somIp[0], config_.somIp[1], config_.somIp[2], config_.somIp[3]);
    } else if (strcmp(param_id, "OWN_IP") == 0) {
        snprintf(value, value_size, "%u.%u.%u.%u", config_.ownIp[0], config_.ownIp[1], config_.ownIp[2], config_.ownIp[3]);
    } else if (strcmp(param_id, "DRONE_PORT") == 0) {
        snprintf(value, value_size, "%u", config_.dronePort);
    } else if (strcmp(param_id, "DEVICE_PORT") == 0) {
        snprintf(value, value_size, "%u", config_.somPort);
    } else if (strcmp(param_id, "UDP_MODE_DR") == 0) {
        snprintf(value, value_size, "%u", config_.udpModeDrone);
    } else if (strcmp(param_id, "UDP_MODE_DV") == 0) {
        snprintf(value, value_size, "%u", config_.udpModeDevice);
    } else {
        return false;
    }
    return true;
}

/* ---------------------------------------------------------------------------
 * IMU Parameter Defaults
 * --------------------------------------------------------------------------- */
ImuCalibConfig_t ImuParam::GetDefault() const {
    ImuCalibConfig_t cfg = {};
    cfg.accBias[0]       = 0.0f; cfg.accBias[1]  = 0.0f; cfg.accBias[2]  = 0.0f;
    cfg.gyroBias[0]      = 0.0f; cfg.gyroBias[1] = 0.0f; cfg.gyroBias[2] = 0.0f;
    cfg.isCalibrated     = 0;
    cfg.orientation      = 0;
    cfg.magic            = PARAM_MAGIC_IMU;
    return cfg;
}

/* ---------------------------------------------------------------------------
 * Central Storage Manager Deferred Processing
 * --------------------------------------------------------------------------- */
void Manager::Process() {
    if (dirty_flags_ & static_cast<uint8_t>(ParamType::NETWORK)) {
        dirty_flags_ &= ~static_cast<uint8_t>(ParamType::NETWORK);
        HAL_WWDG_Refresh(&hwwdg);
        NetworkParam::GetInstance().Save();
        HAL_WWDG_Refresh(&hwwdg);
    }

    if (dirty_flags_ & static_cast<uint8_t>(ParamType::IMU)) {
        dirty_flags_ &= ~static_cast<uint8_t>(ParamType::IMU);
        HAL_WWDG_Refresh(&hwwdg);
        ImuParam::GetInstance().Save();
        HAL_WWDG_Refresh(&hwwdg);
    }
}

} // namespace Storage
