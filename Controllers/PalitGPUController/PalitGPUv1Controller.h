/*---------------------------------------------------------*\
| PalitGPUv1Controller.h                                    |
|                                                           |
|   Driver for Palit v1 GPU                                 |
|                                                           |
|   TheRogueZeta                                05 Nov 2020 |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char palit_gpu_dev_id;

enum
{
    /* RGB Registers */
    PALIT_V1_RED_REGISTER                       = 0x03, /* Red Register */
};

class PalitGPUv1Controller
{
public:
    PalitGPUv1Controller(i2c_smbus_interface* bus, palit_gpu_dev_id, std::string dev_name);
    ~PalitGPUv1Controller();

    std::string   GetDeviceLocation();
    std::string   GetDeviceName();

    unsigned char GetLEDRed();
    unsigned char GetLEDGreen();
    unsigned char GetLEDBlue();
    void          SetLEDColors(unsigned char red, unsigned char green, unsigned char blue);

private:
    i2c_smbus_interface *   bus;
    palit_gpu_dev_id        dev;
    std::string             name;
};
