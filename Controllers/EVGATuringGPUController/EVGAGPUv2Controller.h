/*---------------------------------------------------------*\
| EVGAGPUv2Controller.h                                     |
|                                                           |
|   Driver for EVGA V2 (Turing) GPU                         |
|                                                           |
|   TheRogueZeta                                15 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef unsigned char	evga_dev_id;

#define SPEED_MULTIPLIER                10
#define EVGA_GPU_V2_BRIGHTNESS_MIN      0x01
#define EVGA_GPU_V2_BRIGHTNESS_DEFAULT  0x64
#define EVGA_GPU_V2_BRIGHTNESS_MAX      0x64
#define EVGAGPUV2_CONTROLLER_NAME   "EVGAv2"

union u16_to_u8
{
    uint16_t   u16;
    struct
    {
        uint8_t lsb;
        uint8_t msb;
    };
};

enum
{
    EVGA_GPU_V2_REG_MODE                    = 0x60,
    EVGA_GPU_V2_REG_A_TO_B_SPEED_LSB        = 0x62,
    EVGA_GPU_V2_REG_A_TO_B_SPEED_MSB        = 0x63,
    EVGA_GPU_V2_REG_B_TO_A_SPEED_LSB        = 0x64,
    EVGA_GPU_V2_REG_B_TO_A_SPEED_MSB        = 0x65,
    EVGA_GPU_V2_REG_COLOR_A_ONTIME_LSB      = 0x66,
    EVGA_GPU_V2_REG_COLOR_A_ONTIME_MSB      = 0x67,
    EVGA_GPU_V2_REG_COLOR_B_ONTIME_LSB      = 0x68,
    EVGA_GPU_V2_REG_COLOR_B_ONTIME_MSB      = 0x69,
    EVGA_GPU_V2_REG_COLOR_A_RED             = 0x6C,
    EVGA_GPU_V2_REG_COLOR_A_GREEN           = 0x6D,
    EVGA_GPU_V2_REG_COLOR_A_BLUE            = 0x6E,
    EVGA_GPU_V2_REG_COLOR_A_BRIGHTNESS      = 0x6F,
    EVGA_GPU_V2_REG_COLOR_B_RED             = 0x70,
    EVGA_GPU_V2_REG_COLOR_B_GREEN           = 0x71,
    EVGA_GPU_V2_REG_COLOR_B_BLUE            = 0x72,
    EVGA_GPU_V2_REG_COLOR_B_BRIGHTNESS      = 0x73,
    EVGA_GPU_V2_REG_COLOR_B_UN_LSB          = 0x74,
    EVGA_GPU_V2_REG_COLOR_B_UN_MSB          = 0x75,
};

enum
{
    EVGA_GPU_V2_RGB_MODE_OFF                = 0x00,
    EVGA_GPU_V2_RGB_MODE_STATIC             = 0x01,
    EVGA_GPU_V2_RGB_MODE_RAINBOW            = 0x02,
    EVGA_GPU_V2_RGB_MODE_BREATHING          = 0x03,
    EVGA_GPU_V2_RGB_MODE_PULSE              = 0x04,
};

enum
{
    EVGA_GPU_V2_MODE_OFF                    = 0x00,
    EVGA_GPU_V2_MODE_STATIC                 = 0x01,
    EVGA_GPU_V2_MODE_RAINBOW                = 0x0F,
    EVGA_GPU_V2_MODE_BREATHING              = 0x22,
};

enum
{
    EVGA_GPU_V2_SPEED_BREATHING_SLOWEST     = 0x7D,
    EVGA_GPU_V2_SPEED_BREATHING_NORMAL      = 0x4B,
    EVGA_GPU_V2_SPEED_BREATHING_FASTEST     = 0x19,
    EVGA_GPU_V2_SPEED_PULSE_SLOWEST         = 0xFA,
    EVGA_GPU_V2_SPEED_PULSE_NORMAL          = 0x96,
    EVGA_GPU_V2_SPEED_PULSE_FASTEST         = 0x32,
};

class EVGAGPUv2Controller
{
public:
    EVGAGPUv2Controller(i2c_smbus_interface* bus, evga_dev_id dev, std::string dev_name);
    ~EVGAGPUv2Controller();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    unsigned char   GetBrightnessA();
    RGBColor        GetColorA();
    RGBColor        GetColorB();
    unsigned char   GetMode();
    unsigned char   GetSpeed();

    void            SetColor(RGBColor colorA, RGBColor colorB, uint8_t brightness);
    void            SetMode(uint8_t mode, RGBColor color1, RGBColor color2, uint16_t speed, uint8_t brightness);
    void            SaveSettings();

private:
    void            EnableWrite(bool enable);
    void            SendBrightness(uint8_t brightness);
    void            SendColor(uint8_t start_register, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
    void            SendMode(uint8_t mode);
    void            SendSpeed(u16_to_u8 aOnTime, u16_to_u8 bOnTime, u16_to_u8 b2a, u16_to_u8 a2b, u16_to_u8 speed_un);

    i2c_smbus_interface*    bus;
    evga_dev_id             dev;
    std::string             name;
};
