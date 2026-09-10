/**
  ******************************************************************************
  * @file    IOExpander.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    2024
  * @brief   Driver for PI4IOE5V6416 — 16-bit I2C I/O Expander
  *          Refactored to use HW::Base::I2c interface instead of HAL directly
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  ******************************************************************************
*/

#ifndef __IO_EXPANDER_HPP__
#define __IO_EXPANDER_HPP__

/* Includes ------------------------------------------------------------------*/
#include "Hardware/Base/Inc/hardware_base_peripheral_i2c.hpp"

/* Exported define -----------------------------------------------------------*/

/// @brief 8-bit I2C slave addresses (R/W bit included)
#define PI4IOE_ADDR_LOW     0x40U   ///< ADDR = GND
#define PI4IOE_ADDR_HIGH    0x42U   ///< ADDR = VDD

/* Exported types ------------------------------------------------------------*/

/// @brief Register map (Table 3 — datasheet)
typedef enum
{
    REG_IN_PORT0        = 0x00U,    ///< Input port 0           [RO]
    REG_IN_PORT1        = 0x01U,    ///< Input port 1           [RO]
    REG_OUT_PORT0       = 0x02U,    ///< Output port 0          [R/W] default 0xFF
    REG_OUT_PORT1       = 0x03U,    ///< Output port 1          [R/W] default 0xFF
    REG_POL_INV0        = 0x04U,    ///< Polarity inversion P0  [R/W] default 0x00
    REG_POL_INV1        = 0x05U,    ///< Polarity inversion P1  [R/W] default 0x00
    REG_CFG_PORT0       = 0x06U,    ///< Direction config P0    [R/W] default 0xFF (input)
    REG_CFG_PORT1       = 0x07U,    ///< Direction config P1    [R/W] default 0xFF (input)
    REG_DRV_STR0_L      = 0x40U,    ///< Drive strength P0[3:0] [R/W] default 0xFF
    REG_DRV_STR0_H      = 0x41U,    ///< Drive strength P0[7:4] [R/W] default 0xFF
    REG_DRV_STR1_L      = 0x42U,    ///< Drive strength P1[3:0] [R/W] default 0xFF
    REG_DRV_STR1_H      = 0x43U,    ///< Drive strength P1[7:4] [R/W] default 0xFF
    REG_LATCH_PORT0     = 0x44U,    ///< Input latch P0         [R/W] default 0x00
    REG_LATCH_PORT1     = 0x45U,    ///< Input latch P1         [R/W] default 0x00
    REG_PULL_EN0        = 0x46U,    ///< Pull enable P0         [R/W] default 0x00
    REG_PULL_EN1        = 0x47U,    ///< Pull enable P1         [R/W] default 0x00
    REG_PULL_SEL0       = 0x48U,    ///< Pull up/down select P0 [R/W] default 0xFF (pull-up)
    REG_PULL_SEL1       = 0x49U,    ///< Pull up/down select P1 [R/W] default 0xFF (pull-up)
    REG_INT_MASK0       = 0x4AU,    ///< Interrupt mask P0      [R/W] default 0xFF (all masked)
    REG_INT_MASK1       = 0x4BU,    ///< Interrupt mask P1      [R/W] default 0xFF (all masked)
    REG_INT_STAT0       = 0x4CU,    ///< Interrupt status P0    [RO]  default 0x00
    REG_INT_STAT1       = 0x4DU,    ///< Interrupt status P1    [RO]  default 0x00
    REG_OUT_PORT_CFG    = 0x4FU,    ///< Output port config     [R/W] default 0x00 (push-pull)

} PI4IOE_RegEnum;

/* Exported class ------------------------------------------------------------*/

class IOExpander
{
public:

    /// TYPEDEF ________________________________________________________________

    typedef enum { INPUT_MODE = 0, OUTPUT_MODE } Mode;
    typedef enum { LOW_STATE  = 0, HIGH_STATE  } State;
    typedef enum { PULL_NONE  = 0, PULL_UP, PULL_DOWN } Pull;
    typedef enum
    {
        DRIVE_25PCT  = 0x00U,   ///< 2.5 mA
        DRIVE_50PCT  = 0x01U,   ///< 5   mA
        DRIVE_75PCT  = 0x02U,   ///< 7.5 mA
        DRIVE_100PCT = 0x03U,   ///< 10  mA

    } DriveStrength;

    /// PARAM __________________________________________________________________

    HW::Base::I2c*  pi2c{nullptr};              ///< I2C instance (injected)
    uint8_t         deviceAddr{PI4IOE_ADDR_LOW}; ///< 8-bit I2C address

    /// FUNCTION _______________________________________________________________

    /** @brief Verify device on bus and set push-pull output stage */
    StatusEnum Open(void);

    /** @brief Set direction of a single pin (0..15 across both ports) */
    StatusEnum pinMode(uint8_t pin, Mode mode);

    /** @brief Set direction of all 16 pins at once
     *  @param mode  INPUT_MODE → all pins input | OUTPUT_MODE → all pins output
     */
    StatusEnum pinModeAll(Mode mode);

    /** @brief Set direction of all 8 pins of one port
     *  @param port  0 = Port0, 1 = Port1
     *  @param mode  INPUT_MODE or OUTPUT_MODE
     */
    StatusEnum pinModePort(uint8_t port, Mode mode);

    /** @brief Write a single output pin */
    StatusEnum digitalWrite(uint8_t pin, State state);

    /** @brief Read a single input pin */
    State digitalRead(uint8_t pin);

    /** @brief Configure internal pull resistor for a single pin */
    StatusEnum setPull(uint8_t pin, Pull pull);

    /** @brief Set pull resistor for all 16 pins at once
     *  @param pull  PULL_UP | PULL_DOWN | PULL_NONE
     */
    StatusEnum setPullAll(Pull pull);

    /** @brief Set pull resistor for all 8 pins of one port
     *  @param port  0 = Port0, 1 = Port1
     *  @param pull  PULL_UP | PULL_DOWN | PULL_NONE
     */
    StatusEnum setPullPort(uint8_t port, Pull pull);

    /** @brief Set output drive strength for a single pin */
    StatusEnum setDriveStrength(uint8_t pin, DriveStrength strength);

    /** @brief Enable / disable input latch for a single pin */
    StatusEnum setInputLatch(uint8_t pin, bool enable);

    /** @brief Configure interrupt mask for a single pin
     *  @param maskConfig  true = masked (disabled), false = enabled
     */
    StatusEnum setInterruptMask(uint8_t pin, bool maskConfig);

    /** @brief Read interrupt status byte for a port (0 or 1) */
    StatusEnum readInterruptStatus(uint8_t port, uint8_t* status);

private:

    /// FUNCTION _______________________________________________________________

    /** @brief Decompose flat pin index (0..15) into port (0/1) and bitmask */
    static void getPinMapping(uint8_t pin, uint8_t& port, uint8_t& bitMask);

    /** @brief Read one byte from a device register */
    StatusEnum readReg(PI4IOE_RegEnum reg, uint8_t* value);

    /** @brief Write one byte to a device register */
    StatusEnum writeReg(PI4IOE_RegEnum reg, uint8_t value);

};

#endif /* __IO_EXPANDER_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
