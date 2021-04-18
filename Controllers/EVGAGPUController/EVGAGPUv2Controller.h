/*-----------------------------------------*\
|  EVGAGPUv2Controller.h                    |
|                                           |
|  Definitions and types for EVGA GPU RGB   |
|  V2 (Turing) lighting controller          |
|                                           |
|  TheRogueZeta                 4/15/2021   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

#pragma once

typedef unsigned char	evga_dev_id;

#define SPEED_MULTIPLIER 10

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
    EVGA_GPU_V2_RGB_MODE_DIRECT             = 0x01, 
    EVGA_GPU_V2_RGB_MODE_STATIC             = 0x02,
    EVGA_GPU_V2_RGB_MODE_RAINBOW            = 0x03,
    EVGA_GPU_V2_RGB_MODE_BREATHING          = 0x04,
    EVGA_GPU_V2_RGB_MODE_PULSE              = 0x05,
};

enum
{
    EVGA_GPU_V2_MODE_OFF                    = 0x00,
    EVGA_GPU_V2_MODE_DIRECT                 = 0x01, 
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

typedef union
{
    uint16_t   u16;
    struct
    {
        uint8_t LSB;
        uint8_t MSB;
    };
} u16_to_u8;

class EVGAGPUv2Controller
{
public:
    EVGAGPUv2Controller(i2c_smbus_interface* bus, evga_dev_id dev);
    ~EVGAGPUv2Controller();

    std::string     GetDeviceLocation();

    unsigned char   GetBrightnessA();
    RGBColor        GetColorA();
    RGBColor        GetColorB();
    unsigned char   GetMode();
    unsigned char   GetSpeed();

    void            SetColor(RGBColor colorA, RGBColor colorB, bool boolSave);
    void            SetMode(unsigned char mode, RGBColor color1, RGBColor color2, unsigned int speed);

private:
    void            EnableWrite(bool enable);
    void            SaveSettings();
    void            SendBrightness(unsigned char brightness = 0x64);
    void            SendColor(unsigned char start_register, unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness = 0x64);
    void            SendMode(unsigned char mode);
    void            SendSpeed(u16_to_u8 aOnTime, u16_to_u8 bOnTime, u16_to_u8 b2a, u16_to_u8 a2b, u16_to_u8 speed_un);

    i2c_smbus_interface*    bus;
    evga_dev_id             dev;
};
