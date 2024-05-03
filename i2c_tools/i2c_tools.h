/*---------------------------------------------------------*\
| i2c_tools.h                                               |
|                                                           |
|   Utility functions for communicating with and reverse    |
|   engineering i2c devices                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#define MODE_AUTO   0
#define MODE_QUICK  1
#define MODE_READ   2
#define MODE_FUNC   3

std::string i2c_detect(i2c_smbus_interface * bus, int mode);

std::string i2c_dump(i2c_smbus_interface * bus, unsigned char address);

std::string i2c_read(i2c_smbus_interface * bus, unsigned char address, unsigned char regaddr, unsigned char size);
