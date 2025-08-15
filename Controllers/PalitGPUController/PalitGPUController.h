/*---------------------------------------------------------*\
| PalitGPUController.h                                      |
|                                                           |
|   Driver for Palit GPU                                    |
|                                                           |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef unsigned char       palit_dev_id;

enum
{
    PALIT_GPU_MODE_DIRECT   = 0x00,
};

enum
{
    PALIT_GPU_REG_LED       = 0x03,
};

class PalitGPUController
{
public:
	PalitGPUController(i2c_smbus_interface* bus, palit_dev_id dev, std::string dev_name);
	~PalitGPUController();

	std::string             GetDeviceLocation();
    std::string             GetName();

    void                    SetDirect(unsigned char red, unsigned char green, unsigned char blue);

private:
	i2c_smbus_interface*    bus;
	palit_dev_id            dev;
    std::string             name;
};
