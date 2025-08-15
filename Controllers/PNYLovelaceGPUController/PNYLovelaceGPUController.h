/*---------------------------------------------------------*\
| PNYLovelaceGPUController.h                                |
|                                                           |
|   Driver for PNY Lovelace GPU                             |
|                                                           |
|   yufan                                       01 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char   pny_dev_id;

enum
{
    PNY_GPU_MODE_OFF                    = 0x00,
    PNY_GPU_MODE_BREATH                 = 0x02,
    PNY_GPU_MODE_CYCLE                  = 0x03,
    PNY_GPU_MODE_WAVE                   = 0x04,
    PNY_GPU_MODE_FLASH                  = 0x05,
    PNY_GPU_MODE_STATIC                 = 0x06,
};

enum
{
    PNY_GPU_REG_LIGHTING                    = 0x02,
};

class PNYLovelaceGPUController
{
public:
    PNYLovelaceGPUController(i2c_smbus_interface* bus, pny_dev_id dev, std::string dev_name);
    ~PNYLovelaceGPUController();

	std::string GetDeviceLocation();
    std::string GetDeviceName();

    void        SetOff();
    void        SetBreath(unsigned char speed, unsigned char red, unsigned char green, unsigned char blue);
    void        SetCycle(unsigned char speed, unsigned char brightness);
    void        SetWave(unsigned char speed, unsigned char brightness);
    void        SetFlash(unsigned char speed, unsigned char brightness, unsigned char red, unsigned char green, unsigned char blue);
    void        SetDirect(unsigned char led, unsigned char red, unsigned char green, unsigned char blue);

private:
	i2c_smbus_interface*    bus;
	pny_dev_id              dev;
    std::string             name;
};

