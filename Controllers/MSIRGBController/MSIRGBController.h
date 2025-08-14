/*---------------------------------------------------------*\
| MSIRGBController.h                                        |
|                                                           |
|   Driver for MSI-RGB motherboard                          |
|                                                           |
|   Logic adapted from https://github.com/nagisa/msi-rgb    |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

#define MSI_SIO_LOGDEV_RGB      0x12

enum
{
    MSI_SIO_RGB_REG_ENABLE      = 0xE0,
    MSI_SIO_RGB_REG_CFG_1       = 0xE4,
    MSI_SIO_RGB_REG_RED_1_0     = 0xF0,
    MSI_SIO_RGB_REG_RED_3_2     = 0xF1,
    MSI_SIO_RGB_REG_RED_5_4     = 0xF2,
    MSI_SIO_RGB_REG_RED_7_6     = 0xF3,
    MSI_SIO_RGB_REG_GREEN_1_0   = 0xF4,
    MSI_SIO_RGB_REG_GREEN_3_2   = 0xF5,
    MSI_SIO_RGB_REG_GREEN_5_4   = 0xF6,
    MSI_SIO_RGB_REG_GREEN_7_6   = 0xF7,
    MSI_SIO_RGB_REG_BLUE_1_0    = 0xF8,
    MSI_SIO_RGB_REG_BLUE_3_2    = 0xF9,
    MSI_SIO_RGB_REG_BLUE_5_4    = 0xFA,
    MSI_SIO_RGB_REG_BLUE_7_6    = 0xFB,
    MSI_SIO_RGB_REG_CFG_2       = 0xFE,
    MSI_SIO_RGB_REG_CFG_3       = 0xFF,
};

#define MSI_SIO_RGB_ENABLE_MASK 0xE0

class MSIRGBController
{
public:
    MSIRGBController(int sioaddr, bool invert, std::string dev_name);
    ~MSIRGBController();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();

    unsigned int    GetMode();
    void            SetMode(unsigned char new_mode, unsigned char new_speed);

    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);
private:
    int             msi_sioaddr;
    std::string     name;
};
