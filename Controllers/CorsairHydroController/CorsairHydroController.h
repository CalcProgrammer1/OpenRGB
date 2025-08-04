/*---------------------------------------------------------*\
| CorsairHydroController.h                                  |
|                                                           |
|   Driver for Corsair Hydro Series coolers                 |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      17 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <libusb.h>
#include "RGBController.h"

enum
{
    CORSAIR_HYDRO_CMD_READ_PUMP_SPEED   = 0x31,
    CORSAIR_HYDRO_CMD_WRITE_PUMP_MODE   = 0x32,
    CORSAIR_HYDRO_CMD_READ_PUMP_MODE    = 0x33,
    CORSAIR_HYDRO_CMD_READ_FAN_SPEED    = 0x41,
    CORSAIR_HYDRO_CMD_READ_AIO_TEMP     = 0xA9,
    CORSAIR_HYDRO_CMD_READ_FIRMWARE     = 0xAA,
};

enum
{
    CORSAIR_HYDRO_PUMP_MODE_QUIET       = 0x00,
    CORSAIR_HYDRO_PUMP_MODE_BALANCED    = 0x01,
    CORSAIR_HYDRO_PUMP_MODE_PERFORMANCE = 0x02,
};

class CorsairHydroController
{
public:
    CorsairHydroController(libusb_device_handle* dev_handle, std::string dev_name);
    ~CorsairHydroController();

    unsigned char   GetFanPercent(unsigned char fan_channel);

    unsigned short  GetFanRPM(unsigned char fan_channel);

    std::string     GetFirmwareString();
    std::string     GetLocation();
    std::string     GetNameString();

    void            SetBlink
                        (
                            std::vector<RGBColor> & colors,
                            unsigned char           speed
                        );

    void            SetFixed
                        (
                            std::vector<RGBColor> & colors
                        );

    void            SetPulse
                        (
                            std::vector<RGBColor> & colors,
                            unsigned char           speed
                        );

    void            SetShift
                        (
                            std::vector<RGBColor> & colors,
                            unsigned char           speed
                        );

private:
    libusb_device_handle*   dev;
    std::string             firmware_version;
    std::string             location;
    std::string             name;

    void            SendApplyBlink();
    void            SendApplyPulse();
    void            SendApplyShift();

    void            SendColors
                        (
                            std::vector<RGBColor> & colors
                        );

    void            SendFirmwareRequest();

    void            SendInit();

    void            SendSpeed
                        (
                            unsigned char           speed
                        );
};
