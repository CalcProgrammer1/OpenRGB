/*---------------------------------------------------------*\
| EVGAGP102Controller.h                                     |
|                                                           |
|   Driver for EVGA GP102 GPU                               |
|                                                           |
|   Fabricio Murta (avengerx)                   31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include "i2c_smbus.h"

#define EVGA_GP102_CONTROLLER_NAME "EVGA GP102 Nvidia GPU"

enum
{
    EVGA_GP102_REG_MODE  = 0x0C,
    EVGA_GP102_REG_CMD   = 0x0E,
    EVGA_GP102_REG_VALID = 0x04
};

enum
{
    EVGA_GP102_MODE_OFF    = 0x00,
    EVGA_GP102_MODE_CUSTOM = 0x01
    // TODO: Other LEDSync modes (rainbow, breath, pulse)
};

enum
{
    EVGA_GP102_CIDX_RED = 0,
    EVGA_GP102_CIDX_GREEN = 1,
    EVGA_GP102_CIDX_BLUE = 2
};

enum
{
    EVGA_GP102_CMD_BEGIN = 0xE5,
    EVGA_GP102_CMD_COLOR = 0xE9,
    EVGA_GP102_CMD_END = 0xE0
};

typedef struct
{
    std::string zone_name;
    s32 dev_addr;
    s32 color_addrs[3];
    s32 resp_ready;
    s32 resp_clear;
} zoneinfo;

const static zoneinfo gpuzoneinfos[]
{
    {
        "Nameplate",
        0x4A,
        {0x09, 0x0A, 0x0B},
        0x03,
        0x00
    },
    {
        "Backplate", // for 1080Ti K|NGP|N
        0x2A,
        {0x30, 0x31, 0x32},
        0xE9,
        0xE0
    },
    {
        "Backplate", // for 1080Ti FTW3
        0x4F,
        {0x30, 0x31, 0x32},
        0x03,
        0x00
    },
};

class EVGAGP102Controller
{
public:
    EVGAGP102Controller(i2c_smbus_interface* bus, zoneinfo info, std::string dev_name);
    ~EVGAGP102Controller();

    bool                            IsValid();
    std::string                     GetDeviceLocation();
    std::string                     GetDeviceName();
    std::string                     GetZoneName();
    unsigned char                   GetMode();

    void                            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    std::array<unsigned char, 3>    GetColor();
    void                            SetMode(unsigned char mode);
    void                            SaveSettings();

private:
    i2c_smbus_interface*            bus;
    zoneinfo                        zi;
    std::string                     name;

    bool                            CommandAcknowledged();
    bool                            CommandCompleted();
    s32                             QueryCommand(s32 command);
    void                            SendCommand(s32 command);
    unsigned char                   GetRed();
    unsigned char                   GetGreen();
    unsigned char                   GetBlue();
};
