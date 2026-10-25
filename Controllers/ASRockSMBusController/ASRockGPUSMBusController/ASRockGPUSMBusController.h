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
| ASRock GPU Speed and Brightness Constants             |
\*-----------------------------------------------------*/
#define ASROCK_GPU_SPEED_MIN            0x00
#define ASROCK_GPU_SPEED_MAX            0xFF
#define ASROCK_GPU_SPEED_DEFAULT        0x80

#define ASROCK_GPU_BRIGHTNESS_MIN       0x00
#define ASROCK_GPU_BRIGHTNESS_MAX       0xFF
#define ASROCK_GPU_BRIGHTNESS_DEFAULT   0xFF

/*-----------------------------------------------------*\
| ASRock GPU Channel Names (from SignalRGB)             |
\*-----------------------------------------------------*/
enum
{
    ASROCK_GPU_CHANNEL_ARGB_HEADER      = 3,
    ASROCK_GPU_CHANNEL_TOP_SIDE         = 6,
    ASROCK_GPU_CHANNEL_FAN              = 7,
};

/*-----------------------------------------------------*\
| ASRock GPU Modes                                      |
\*-----------------------------------------------------*/
enum
{
    ASROCK_GPU_MODE_OFF                 = 0x00,
    ASROCK_GPU_MODE_STATIC              = 0x01,
    ASROCK_GPU_MODE_BREATHING           = 0x02,
    ASROCK_GPU_MODE_STROBE              = 0x03,
    ASROCK_GPU_MODE_SPECTRUM_CYCLE      = 0x04,
    ASROCK_GPU_MODE_RANDOM              = 0x05,
    ASROCK_GPU_MODE_WAVE                = 0x07,
    ASROCK_GPU_MODE_SPRING              = 0x09,
    ASROCK_GPU_MODE_STACK               = 0x0B,
    ASROCK_GPU_MODE_SCAN                = 0x0D,
    ASROCK_GPU_MODE_RAINBOW             = 0x0E,
    ASROCK_GPU_MODE_DIRECT              = 0xE1,
};

class ASRockGPUSMBusController
{
public:
    ASRockGPUSMBusController(i2c_smbus_interface* bus, asrock_gpu_dev_id dev, std::string name);
    ~ASRockGPUSMBusController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();

    void        SetChannelColor(uint8_t channel_idx, uint8_t red, uint8_t green, uint8_t blue);
    void        SetChannelMode(uint8_t channel_idx, uint8_t mode, uint8_t speed, uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue);
    void        SaveConfig();
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
