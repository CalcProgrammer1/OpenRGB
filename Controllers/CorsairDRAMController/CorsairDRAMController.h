/*---------------------------------------------------------*\
| CorsairDRAMController.h                                   |
|                                                           |
|   Driver for Corsair DRAM RGB controllers                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "CorsairDRAMDevices.h"
#include "RGBController.h"

typedef unsigned char corsair_dev_id;

enum
{   /* (*) indicates deprecated registers no longer used by iCue                                            */
    CORSAIR_DRAM_REG_RESET_BUFFER           = 0x0B,     /* Reset buffer by writing 0x00                     */
    CORSAIR_DRAM_REG_SET_BINARY_DATA        = 0x20,     /* Write byte to active binary data buffer          */
    CORSAIR_DRAM_REG_BINARY_START           = 0x21,     /* Start binary data transfer by writing 0x00       */
    CORSAIR_DRAM_REG_SWITCH_MODE            = 0x23,     /* Switch between Bootloader(0x00) and Normal(0x01) */
    CORSAIR_DRAM_REG_SET_BUFFER             = 0x26,     /* Select configuration buffer to write by ID (*)   */
    CORSAIR_DRAM_REG_STATUS                 = 0x30,     /* Status register                                  */
    CORSAIR_DRAM_REG_COLOR_BUFFER_BLOCK_1   = 0x31,     /* Direct color buffer block register 1             */
    CORSAIR_DRAM_REG_COLOR_BUFFER_BLOCK_2   = 0x32,     /* Direct color buffer block register 2             */
    CORSAIR_DRAM_REG_GET_BINARY_DATA        = 0x40,     /* Read byte from active binary data buffer         */
    CORSAIR_DRAM_REG_BUSY_STATUS            = 0x41,     /* Reads nonzero while busy, zero when ready(*)     */
    CORSAIR_DRAM_REG_GET_CHECKSUM           = 0x42,     /* Get checksum (CRC8) of active binary data buffer */
    CORSAIR_DRAM_REG_GET_DEVICE_INFO        = 0x61,     /* Select device info buffer                        */
    CORSAIR_DRAM_REG_GET_CONFIGURATION      = 0x63,     /* Select configuration buffer to read by ID        */
    CORSAIR_DRAM_REG_WRITE_CONFIGURATION    = 0x82,     /* Write/Apply configuration by writing buffer ID   */
};

enum
{
    CORSAIR_DRAM_ID_COMMAND_LIST            = 0,        /* Command list                                     */
    CORSAIR_DRAM_ID_EFFECT_CONFIGURATION    = 1,        /* Effect configuration                             */
    CORSAIR_DRAM_ID_COLOR_DATA              = 2,        /* Color data                                       */
};

enum
{
    CORSAIR_DRAM_MODE_DIRECT                = 0xDD,     /* Arbitrary value to compare against later. Not the actual packet  */
    CORSAIR_DRAM_MODE_COLOR_SHIFT           = 0x00,     /* Color Shift mode                                                 */
    CORSAIR_DRAM_MODE_COLOR_PULSE           = 0x01,     /* Color Pulse mode                                                 */
    CORSAIR_DRAM_MODE_RAINBOW_WAVE          = 0x03,     /* Rainbow Wave mode                                                */
    CORSAIR_DRAM_MODE_COLOR_WAVE            = 0x04,     /* Color Wave mode                                                  */
    CORSAIR_DRAM_MODE_VISOR                 = 0x05,     /* Visor mode                                                       */
    CORSAIR_DRAM_MODE_RAIN                  = 0x06,     /* Rain mode                                                        */
    CORSAIR_DRAM_MODE_MARQUEE               = 0x07,     /* Marquee mode                                                     */
    CORSAIR_DRAM_MODE_RAINBOW               = 0x08,     /* Rainbow mode                                                     */
    CORSAIR_DRAM_MODE_SEQUENTIAL            = 0x09,     /* Sequential mode                                                  */
    CORSAIR_DRAM_MODE_STATIC                = 0x10,     /* Static mode                                                      */

    CORSAIR_DRAM_NUMBER_MODES               = 10,       /* Number of Corsair Pro modes          */
};

enum
{
    CORSAIR_DRAM_SPEED_SLOW                 = 0x00,     /* Slow speed                           */
    CORSAIR_DRAM_SPEED_MEDIUM               = 0x01,     /* Medium speed                         */
    CORSAIR_DRAM_SPEED_FAST                 = 0x02,     /* Fast speed                           */
};

enum
{
    CORSAIR_DRAM_EFFECT_RANDOM_COLORS       = 0x00,     /* Random colors                        */
    CORSAIR_DRAM_EFFECT_CUSTOM_COLORS       = 0x01,     /* Custom colors                        */
};

enum
{
    CORSAIR_DRAM_DIRECTION_UP               = 0x00,     /* Up direction                         */
    CORSAIR_DRAM_DIRECTION_DOWN             = 0x01,     /* Down direction                       */
    CORSAIR_DRAM_DIRECTION_LEFT             = 0x02,     /* Left direction                       */
    CORSAIR_DRAM_DIRECTION_RIGHT            = 0x03,     /* Right direction                      */
    CORSAIR_DRAM_DIRECTION_VERTICAL         = 0x01,     /* Vertical direction                   */
    CORSAIR_DRAM_DIRECTION_HORIZONTAL       = 0x03,     /* Horizontal direction                 */
};

enum
{
    CORSAIR_DRAM_BRIGHTNESS_MIN             = 0,        /* Minimum brightness                   */
    CORSAIR_DRAM_BRIGHTNESS_MAX             = 255,      /* Maximum brightness                   */
    CORSAIR_DRAM_BRIGHTNESS_DEFAULT         = 255,      /* Default brightness                   */
};

class CorsairDRAMController
{
public:
    CorsairDRAMController(i2c_smbus_interface *bus, corsair_dev_id dev);
    ~CorsairDRAMController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();
    std::string             GetDeviceVersion();

    unsigned int            GetLEDCount();
    unsigned char           GetProtocolVersion();

    void                    SetColorsPerLED(RGBColor* colors);
    void                    SetDirect(bool direct);
    void                    SetEffect(unsigned char mode,
                                      unsigned char speed,
                                      unsigned char direction,
                                      bool          random,
                                      unsigned char brightness,
                                      unsigned char red1,
                                      unsigned char grn1,
                                      unsigned char blu1,
                                      unsigned char red2,
                                      unsigned char grn2,
                                      unsigned char blu2);

    bool                    WaitReady();

private:
    /*-----------------------------------------------------*\
    | I2C                                                   |
    \*-----------------------------------------------------*/
    i2c_smbus_interface*    bus;
    corsair_dev_id          dev;

    /*-----------------------------------------------------*\
    | State tracking                                        |
    \*-----------------------------------------------------*/
    bool                    direct_mode;

    /*-----------------------------------------------------*\
    | Corsair DRAM information                              |
    \*-----------------------------------------------------*/
    unsigned short          vid;
    unsigned short          pid;
    std::string             firmware_version;
    unsigned char           protocol_version;

    unsigned int            device_index;

    /*-----------------------------------------------------*\
    | Private functions                                     |
    \*-----------------------------------------------------*/
    void                    ReadDeviceInfo();
};
