/*---------------------------------------------------------*\
| ASRockGPUSMBusController.h                                |
|                                                           |
|   Driver for ASRock GPU RGB controllers                   |
|                                                           |
|   Based on SignalRGB ASRock GPU.js plugin                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"

typedef uint8_t asrock_gpu_dev_id;

#define ASROCK_GPU_CONTROLLER_NAME   "ASRock GPU SMBus Controller"
#define ASROCK_GPU_SMBUS_ADDRESS     0x36

/*-----------------------------------------------------*\
| ASRock GPU Channel Names (from SignalRGB)             |
\*-----------------------------------------------------*/
enum
{
    ASROCK_GPU_CHANNEL_ARGB_HEADER  = 3,
    ASROCK_GPU_CHANNEL_TOP_SIDE     = 6,
    ASROCK_GPU_CHANNEL_FAN          = 7,
};

class ASRockGPUSMBusController
{
public:
    ASRockGPUSMBusController(i2c_smbus_interface* bus, asrock_gpu_dev_id dev, std::string name);
    ~ASRockGPUSMBusController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();

    void        SetChannelColor(uint8_t channel_idx, uint8_t red, uint8_t green, uint8_t blue);
    bool        DetectChannels();

    uint8_t     enabled_channels[16];
    uint8_t     led_counts[16];
    uint8_t     num_channels;

private:
    i2c_smbus_interface* bus;
    asrock_gpu_dev_id    dev;
    std::string          device_name;

    bool        WriteBlock(uint8_t command, uint8_t subcommand, const uint8_t* data, uint8_t len);
    bool        ReadBlock(uint8_t* response, uint8_t len);
};
