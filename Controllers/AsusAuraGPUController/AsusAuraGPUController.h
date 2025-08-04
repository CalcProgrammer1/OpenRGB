/*---------------------------------------------------------*\
| AsusAuraGPUController.h                                   |
|                                                           |
|   Driver for ASUS Aura GPU                                |
|                                                           |
|   Jan Rettig (Klapstuhl)                      14 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char aura_gpu_dev_id;

#define AURA_GPU_MAGIC_VAL 0x1589                   /* This magic value is present in all Aura GPU controllers */
#define AURA_GPU_APPLY_VAL 0x01                     /* Value for Apply Changes Register     */

enum
{
    AURA_GPU_REG_RED                        = 0x04,    /* AURA GPU RED Register            */
    AURA_GPU_REG_GREEN                      = 0x05,    /* AURA GPU GREEN Register          */
    AURA_GPU_REG_BLUE                       = 0x06,    /* AURA GPU BLUE Register           */
    AURA_GPU_REG_MODE                       = 0x07,    /* AURA GPU Mode Selection Register */
    AURA_GPU_REG_SYNC                       = 0x0C,    /* AURA GPU "Sync" Register         */
    AURA_GPU_REG_APPLY                      = 0x0E,    /* AURA GPU Save or Apply Register  */
};

enum
{
    AURA_GPU_MODE_OFF                       = 0,        /* OFF mode (not a real Mode! Doesn't do anything!)    */
    AURA_GPU_MODE_STATIC                    = 1,        /* Static color mode                                   */
    AURA_GPU_MODE_BREATHING                 = 2,        /* Breathing effect mode                               */
    AURA_GPU_MODE_FLASHING                  = 3,        /* Flashing effect mode                                */
    AURA_GPU_MODE_SPECTRUM_CYCLE            = 4,        /* Spectrum Cycle mode                                 */
    AURA_GPU_MODE_DIRECT                    = 5,        /* Direct mode (not a real Mode! Doesn't do anything!) */
    AURA_GPU_NUMBER_MODES
};

class AuraGPUController
{
public:
    AuraGPUController(i2c_smbus_interface* bus, aura_gpu_dev_id, std::string dev_name);
    ~AuraGPUController();

    std::string   GetDeviceName();
    std::string   GetDeviceLocation();
    unsigned char GetLEDRed();
    unsigned char GetLEDGreen();
    unsigned char GetLEDBlue();
    void          SetLEDColors(unsigned char red, unsigned char green, unsigned char blue);
    void          SetMode(unsigned char mode);
    void          Save();

    unsigned char AuraGPURegisterRead(unsigned char reg);
    void          AuraGPURegisterWrite(unsigned char reg, unsigned char val);

    bool          SaveOnlyApplies();
    bool          direct = false;                                                // Temporary solution to check if we are in "Direct" mode

private:
    i2c_smbus_interface *   bus;
    aura_gpu_dev_id         dev;
    std::string             name;
};
