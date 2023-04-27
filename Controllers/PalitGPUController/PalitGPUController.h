/*-----------------------------------------*\
|  PalitGPUController.h                     |
|                                           |
|  Definitions and types for Palit GPU RGB  |
|  lighting controller                      |
|                                           |
|  Manatsawin Hanmongkolchai  04/11/2023    |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char   palit_dev_id;

enum
{
    PALIT_GPU_MODE_DIRECT = 0x00,
};

enum
{
    PALIT_GPU_REG_LED = 0x03,
};

class PalitGPUController
{
public:
	PalitGPUController(i2c_smbus_interface* bus, palit_dev_id dev);
	~PalitGPUController();

	std::string     GetDeviceLocation();

    void SetDirect(unsigned char red, unsigned char green, unsigned char blue);

private:
	i2c_smbus_interface*    bus;
	palit_dev_id              dev;

};
