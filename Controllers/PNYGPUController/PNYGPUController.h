/*-----------------------------------------*\
|  PNYGPUController.h                       |
|                                           |
|  Definitions and types for PNY GPU RGB    |
|  (Turing) lighting controller             |
|                                           |
|  KendallMorgan   10/17/2021               |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char   pny_dev_id;

enum
{
    PNY_GPU_REG_MODE                    = 0x60,
    PNY_GPU_REG_COLOR_RED               = 0x6C,
    PNY_GPU_REG_COLOR_GREEN             = 0x6D,
    PNY_GPU_REG_COLOR_BLUE              = 0x6E,
    PNY_GPU_REG_COLOR_BRIGHTNESS        = 0x6F,
};

enum
{
    PNY_GPU_MODE_OFF                    = 0x00,
    PNY_GPU_MODE_CUSTOM                 = 0x01,
};

class PNYGPUController
{
public:
	PNYGPUController(i2c_smbus_interface* bus, pny_dev_id dev);
	~PNYGPUController();

	std::string     GetDeviceLocation();

	unsigned char   GetMode();
	unsigned char   GetRed();
	unsigned char   GetGreen();
	unsigned char   GetBlue();

	void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
	void            SetMode(unsigned char mode);

private:
	i2c_smbus_interface*    bus;
	pny_dev_id              dev;

};
