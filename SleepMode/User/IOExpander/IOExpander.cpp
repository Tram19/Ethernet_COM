/**
  ******************************************************************************
  * @file    IOExpander.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    2024
  * @brief   Driver for PI4IOE5V6416 — 16-bit I2C I/O Expander
  *          Uses HW::Base::I2c::SendMem / GetMem instead of HAL directly
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "IOExpander.hpp"

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Map flat pin index 0..15 to port (0/1) and bitmask.
 *         Pin 0..7  → Port 0, bit (1 << pin)
 *         Pin 8..15 → Port 1, bit (1 << (pin - 8))
 */
void IOExpander::getPinMapping(uint8_t pin, uint8_t& port, uint8_t& bitMask)
{
    if (pin < 8)
    {
        port    = 0;
        bitMask = (uint8_t)(1U << pin);
    }
    else
    {
        port    = 1;
        bitMask = (uint8_t)(1U << (pin - 8U));
    }
}

/**
 * @brief  Read one byte from a PI4IOE register.
 *         Temporarily sets deviceAddress on the I2C instance, then restores it.
 */
StatusEnum IOExpander::readReg(PI4IOE_RegEnum reg, uint8_t* value)
{
    if (pi2c == nullptr || value == nullptr) return ERROR_NOT_EXIST;

    uint32_t savedAddr  = pi2c->deviceAddress;
    pi2c->deviceAddress = deviceAddr;

    StatusEnum ret = pi2c->GetMem((uint16_t)reg, value);
    if (ret!=OK) pi2c->Reset();
    pi2c->deviceAddress = savedAddr;
    return ret;
}

/**
 * @brief  Write one byte to a PI4IOE register.
 */
StatusEnum IOExpander::writeReg(PI4IOE_RegEnum reg, uint8_t value)
{
    if (pi2c == nullptr) return ERROR_NOT_EXIST;

    uint32_t savedAddr  = pi2c->deviceAddress;
    pi2c->deviceAddress = deviceAddr;

    StatusEnum ret = pi2c->SendMem((uint16_t)reg, value);
   if (ret!=OK) pi2c->Reset();
    pi2c->deviceAddress = savedAddr;
    return ret;
}

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Verify the device is reachable and configure output stage to push-pull.
 *         Datasheet recommends writing REG_OUT_PORT_CFG before setting outputs.
 */
StatusEnum IOExpander::Open(void)
{
    if (pi2c == nullptr) return ERROR_NOT_EXIST;

    /// Probe: attempt to read a known register (output port 0, default = 0xFF)
    uint8_t probe = 0;
    StatusEnum ret = readReg(REG_OUT_PORT0, &probe);

    if (ret != OK) return ret;

    /// Set output stage: push-pull for both ports (ODEN0=0, ODEN1=0)
    return writeReg(REG_OUT_PORT_CFG, 0x00U);
}

/* GPIO standard API ---------------------------------------------------------*/

StatusEnum IOExpander::pinMode(uint8_t pin, Mode mode)
{
    if (pin > 15U) return ERROR_NOT_EXIST;

    uint8_t port, mask, regVal;
    getPinMapping(pin, port, mask);

    PI4IOE_RegEnum regAddr = (PI4IOE_RegEnum)((uint8_t)REG_CFG_PORT0 + port);

    StatusEnum ret = readReg(regAddr, &regVal);
    if (ret != OK) return ret;

    if (mode == INPUT_MODE)
        regVal |=  mask;    ///< Config bit = 1 → input
    else
        regVal &= ~mask;    ///< Config bit = 0 → output

    return writeReg(regAddr, regVal);
}

StatusEnum IOExpander::digitalWrite(uint8_t pin, State state)
{
    if (pin > 15U) return ERROR_NOT_EXIST;

    uint8_t port, mask, regVal;
    getPinMapping(pin, port, mask);

    PI4IOE_RegEnum regAddr = (PI4IOE_RegEnum)((uint8_t)REG_OUT_PORT0 + port);

    StatusEnum ret = readReg(regAddr, &regVal);
    if (ret != OK) return ret;

    if (state == HIGH_STATE)
        regVal |=  mask;
    else
        regVal &= ~mask;

    return writeReg(regAddr, regVal);
}

IOExpander::State IOExpander::digitalRead(uint8_t pin)
{
    if (pin > 15U) return LOW_STATE;

    uint8_t port, mask, regVal = 0;
    getPinMapping(pin, port, mask);

    PI4IOE_RegEnum regAddr = (PI4IOE_RegEnum)((uint8_t)REG_IN_PORT0 + port);

    if (readReg(regAddr, &regVal) != OK) return LOW_STATE;

    return (regVal & mask) ? HIGH_STATE : LOW_STATE;
}

/* Advanced API --------------------------------------------------------------*/

StatusEnum IOExpander::setPull(uint8_t pin, Pull pull)
{
    if (pin > 15U) return ERROR_NOT_EXIST;

    uint8_t port, mask, enVal, selVal;
    getPinMapping(pin, port, mask);

    PI4IOE_RegEnum regEn  = (PI4IOE_RegEnum)((uint8_t)REG_PULL_EN0  + port);
    PI4IOE_RegEnum regSel = (PI4IOE_RegEnum)((uint8_t)REG_PULL_SEL0 + port);

    StatusEnum ret;

    ret = readReg(regEn,  &enVal);  if (ret != OK) return ret;
    ret = readReg(regSel, &selVal); if (ret != OK) return ret;

    if (pull == PULL_NONE)
    {
        enVal &= ~mask;             ///< Disable pull resistor
    }
    else
    {
        enVal |= mask;              ///< Enable pull resistor

        if (pull == PULL_UP)
            selVal |=  mask;        ///< 1 = pull-up  (100 kΩ)
        else
            selVal &= ~mask;        ///< 0 = pull-down (100 kΩ)
    }

    /// Write selection before enable to avoid voltage glitch
    ret = writeReg(regSel, selVal); if (ret != OK) return ret;
    return writeReg(regEn,  enVal);
}

/**
 * @brief  Set pull resistor for all 16 pins at once.
 *
 *  PULL_UP   → enable pull (EN=0xFF) + select pull-up   (SEL=0xFF) for both ports
 *  PULL_DOWN → enable pull (EN=0xFF) + select pull-down (SEL=0x00) for both ports
 *  PULL_NONE → disable pull (EN=0x00) for both ports, SEL untouched
 *
 *  Write order per port: SEL first, EN second (avoids voltage glitch — same as setPull())
 */
StatusEnum IOExpander::setPullAll(Pull pull)
{
    for (uint8_t port = 0; port <= 1U; port++)
    {
        StatusEnum ret = setPullPort(port, pull);
        if (ret != OK) return ret;
    }
    return OK;
}

/**
 * @brief  Set pull resistor for all 8 pins of one port.
 * @param  port  0 = Port0, 1 = Port1
 * @param  pull  PULL_UP | PULL_DOWN | PULL_NONE
 */
StatusEnum IOExpander::setPullPort(uint8_t port, Pull pull)
{
    if (port > 1U) return ERROR_NOT_EXIST;

    PI4IOE_RegEnum regEn  = (PI4IOE_RegEnum)((uint8_t)REG_PULL_EN0  + port);
    PI4IOE_RegEnum regSel = (PI4IOE_RegEnum)((uint8_t)REG_PULL_SEL0 + port);

    StatusEnum ret;

    if (pull == PULL_NONE)
    {
        /// Disable all pull resistors — SEL register untouched
        return writeReg(regEn, 0x00U);
    }

    /// Select direction first (before enabling) to avoid glitch
    uint8_t selMask = (pull == PULL_UP) ? 0xFFU : 0x00U;

    ret = writeReg(regSel, selMask);    if (ret != OK) return ret;
    return writeReg(regEn,  0xFFU);     ///< Enable all 8 pull resistors
}

StatusEnum IOExpander::setDriveStrength(uint8_t pin, DriveStrength strength)
{
    if (pin > 15U) return ERROR_NOT_EXIST;

    /// Each register covers 4 pins (2 bits per pin)
    /// Registers: 0x40 (P0[3:0]), 0x41 (P0[7:4]), 0x42 (P1[3:0]), 0x43 (P1[7:4])
    PI4IOE_RegEnum regAddr = (PI4IOE_RegEnum)((uint8_t)REG_DRV_STR0_L + (pin / 4U));
    uint8_t bitShift       = (uint8_t)((pin % 4U) * 2U);

    uint8_t regVal;
    StatusEnum ret = readReg(regAddr, &regVal);
    if (ret != OK) return ret;

    regVal &= ~(uint8_t)(0x03U << bitShift);            ///< Clear old 2-bit field
    regVal |=  (uint8_t)((uint8_t)strength << bitShift); ///< Insert new value

    return writeReg(regAddr, regVal);
}

StatusEnum IOExpander::setInputLatch(uint8_t pin, bool enable)
{
    if (pin > 15U) return ERROR_NOT_EXIST;

    uint8_t port, mask, regVal;
    getPinMapping(pin, port, mask);

    PI4IOE_RegEnum regAddr = (PI4IOE_RegEnum)((uint8_t)REG_LATCH_PORT0 + port);

    StatusEnum ret = readReg(regAddr, &regVal);
    if (ret != OK) return ret;

    if (enable)
        regVal |=  mask;    ///< 1 = latch input state
    else
        regVal &= ~mask;    ///< 0 = no latch

    return writeReg(regAddr, regVal);
}

StatusEnum IOExpander::setInterruptMask(uint8_t pin, bool maskConfig)
{
    if (pin > 15U) return ERROR_NOT_EXIST;

    uint8_t port, maskBit, regVal;
    getPinMapping(pin, port, maskBit);

    PI4IOE_RegEnum regAddr = (PI4IOE_RegEnum)((uint8_t)REG_INT_MASK0 + port);

    StatusEnum ret = readReg(regAddr, &regVal);
    if (ret != OK) return ret;

    if (maskConfig)
        regVal |=  maskBit; ///< 1 = interrupt masked (disabled)
    else
        regVal &= ~maskBit; ///< 0 = interrupt enabled

    return writeReg(regAddr, regVal);
}

StatusEnum IOExpander::readInterruptStatus(uint8_t port, uint8_t* status)
{
    if (port > 1U || status == nullptr) return ERROR_NOT_EXIST;

    PI4IOE_RegEnum regAddr = (PI4IOE_RegEnum)((uint8_t)REG_INT_STAT0 + port);

    return readReg(regAddr, status);
}

/**
 * @brief  Set direction of all 16 pins at once.
 *         INPUT_MODE  → writes 0xFF to both config registers (all input,  power-on default)
 *         OUTPUT_MODE → writes 0x00 to both config registers (all output)
 */
StatusEnum IOExpander::pinModeAll(Mode mode)
{
    uint8_t mask = (mode == INPUT_MODE) ? 0xFFU : 0x00U;

    StatusEnum ret = writeReg(REG_CFG_PORT0, mask);
    if (ret != OK) return ret;

    return writeReg(REG_CFG_PORT1, mask);
}

/**
 * @brief  Set direction of all 8 pins of one port at once.
 *         INPUT_MODE  → writes 0xFF to the port's config register
 *         OUTPUT_MODE → writes 0x00 to the port's config register
 * @param  port  0 = Port0, 1 = Port1
 */
StatusEnum IOExpander::pinModePort(uint8_t port, Mode mode)
{
    if (port > 1U) return ERROR_NOT_EXIST;

    uint8_t mask   = (mode == INPUT_MODE) ? 0xFFU : 0x00U;
    PI4IOE_RegEnum reg = (PI4IOE_RegEnum)((uint8_t)REG_CFG_PORT0 + port);

    return writeReg(reg, mask);
}
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
