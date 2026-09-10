#pragma once
// gremsy_math.h
// Header-only math utility — không cần ARM CMSIS, không cần mavlink
// Chỉ cần: #include "gremsy_math.h"

#include <math.h>
#include <string.h>
#include <stdint.h>

// ─── Constants ───────────────────────────────────────────────
#ifndef M_PI
#define M_PI      3.14159265358979323846f
#endif
#define G_PI      M_PI
#define G_PI_2    (M_PI / 2.0f)
#define G_DEG2RAD (M_PI / 180.0f)
#define G_RAD2DEG (180.0f / M_PI)

// ─── Basic math ──────────────────────────────────────────────
static inline float gsq(float x)              { return x * x; }
static inline float gsqrt(float x)            { return sqrtf(x); }
static inline float gabs(float x)             { return fabsf(x); }
static inline float gatan2(float y, float x)  { return atan2f(y, x); }
static inline float gasin(float x)            { return asinf(x); }

// ─── Vector norm ─────────────────────────────────────────────
static inline float gnorm2(const float* v, uint8_t len)
{
    float s = 0.f;
    for (uint8_t i = 0; i < len; i++) s += v[i] * v[i];
    return s;
}

static inline float gnorm(const float* v, uint8_t len)
{
    return gsqrt(gnorm2(v, len));
}

// ─── Normalize in-place ──────────────────────────────────────
static inline void gnormalize(float* v, uint8_t len)
{
    float n = gnorm(v, len);
    if (n < 1e-7f) return;  // tránh chia 0
    float inv = 1.0f / n;
    for (uint8_t i = 0; i < len; i++) v[i] *= inv;
}

// ─── Quaternion to DCM ───────────────────────────────────────
// quat = [w, x, y, z]  (Hamilton convention)
// dcm[3][3] output: rotation matrix
static inline void gquaternion_to_dcm(const float quat[4], float dcm[3][3])
{
    float q[4];
    memcpy(q, quat, 4 * sizeof(float));
    gnormalize(q, 4);

    float w = q[0], x = q[1], y = q[2], z = q[3];

    float ww = gsq(w), xx = gsq(x), yy = gsq(y), zz = gsq(z);

    dcm[0][0] = ww + xx - yy - zz;
    dcm[0][1] = 2.f * (x*y - w*z);
    dcm[0][2] = 2.f * (w*y + x*z);

    dcm[1][0] = 2.f * (x*y + w*z);
    dcm[1][1] = ww - xx + yy - zz;
    dcm[1][2] = 2.f * (y*z - w*x);

    dcm[2][0] = 2.f * (x*z - w*y);
    dcm[2][1] = 2.f * (w*x + y*z);
    dcm[2][2] = ww - xx - yy + zz;
}

// ─── DCM to Euler (ZYX: yaw-pitch-roll) ──────────────────────
// output: roll(phi), pitch(theta), yaw(psi) — đơn vị radian
static inline void gdcm_to_euler(const float dcm[3][3],
                                  float* roll, float* pitch, float* yaw)
{
    float phi = gasin(dcm[2][1]);

    if (gabs(phi - G_PI_2) < 1e-3f) {
        // gimbal lock: pitch = +90°
        *pitch = 0.f;
        *yaw   = gatan2( dcm[0][2], -dcm[1][2]);
    } else if (gabs(phi + G_PI_2) < 1e-3f) {
        // gimbal lock: pitch = -90°
        *pitch = 0.f;
        *yaw   = gatan2(-dcm[0][2],  dcm[1][2]);
    } else {
        *pitch = gatan2(-dcm[2][0], dcm[2][2]);
        *yaw   = gatan2(-dcm[0][1], dcm[1][1]);
    }

    *roll = phi;
}

// ─── Quaternion to Euler (all-in-one) ────────────────────────
// quat = [w, x, y, z]
// output: roll, pitch, yaw — đơn vị radian
static inline void gquaternion_to_euler(const float quat[4],
                                         float* roll, float* pitch, float* yaw)
{
    float q[4];
    memcpy(q, quat, 4 * sizeof(float));
    gnormalize(q, 4);

    float dcm[3][3];
    gquaternion_to_dcm(q, dcm);
    gdcm_to_euler((const float(*)[3])dcm, roll, pitch, yaw);
}