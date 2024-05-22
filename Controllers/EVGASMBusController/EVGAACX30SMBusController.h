/*---------------------------------------------------------*\
| EVGAACX30SMBusController.h                                |
|                                                           |
|   Driver for SMBus EVGA ACX 30 motherboards               |
|                                                           |
|   Balázs Triszka (balika011)                  21 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

enum
{
    /*------------------------------------------------------------------------------------------*\
    | Acx30 Common Registers                                                                    |
    \*------------------------------------------------------------------------------------------*/
    ACX30_REG_01                            = 0x01,
    ACX30_REG_PTYPE                         = 0x03,
    ACX30_REG_VER_LOW                       = 0x04,
    ACX30_REG_VER_HIGH                      = 0x05,
    ACX30_REG_RED                           = 0x09,
    ACX30_REG_GREEN                         = 0x0A,
    ACX30_REG_BLUE                          = 0x0B,
    ACX30_REG_MODE                          = 0x0C,
    ACX30_REG_CONTROL                       = 0x0E,
    ACX30_REG_SPEED                         = 0x19,
    ACX30_REG_20                            = 0x20,
    ACX30_REG_21                            = 0x21,
    ACX30_REG_22                            = 0x22,
};

/*----------------------------------------------------------------------------------------------*\
| Definitions for Acx30                                                                          |
\*----------------------------------------------------------------------------------------------*/

enum
{
    ACX30_MODE_OFF                          = 0x00,     /* OFF mode                             */
    ACX30_MODE_STATIC                       = 0x01,     /* Static color mode                    */
    ACX30_MODE_SPECTRUM_CYCLE               = 0x02,     /* Spectrum Cycle effect mode           */
    ACX30_MODE_BREATHING                    = 0x05,     /* Breathing effect mode                */
};

enum
{
    ACX30_SPEED_MIN                         = 0x00,     /* Slowest speed                        */
    ACX30_SPEED_DEFAULT                     = 0x04,     /* Default speed                        */
    ACX30_SPEED_MAX                         = 0xFF,     /* Fastest speed                        */
};

class EVGAACX30SMBusController
{
public:
    EVGAACX30SMBusController(i2c_smbus_interface *bus, uint8_t dev);
    ~EVGAACX30SMBusController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();
    std::string             GetFirmwareVersion();
    uint8_t                 GetMode();
    void                    Unlock();
    void                    Lock();
    void                    SetColors(uint8_t red, uint8_t green, uint8_t blue);
    void                    SetMode(uint8_t mode);
    void                    SetSpeed(uint8_t speed);

private:
    std::string             device_name;
    i2c_smbus_interface*    bus;
    uint8_t                 dev;
};
