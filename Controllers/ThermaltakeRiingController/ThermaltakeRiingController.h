/*-----------------------------------------*\
|  ThermaltakeRiingController.h             |
|                                           |
|  Definitions and types for Thermaltake    |
|  Riing Plus lighting controller           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/7/2020    |
\*-----------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <libusb-1.0/libusb.h>

#pragma once

enum
{
    THERMALTAKE_PORT_1              = 0x01,
    THERMALTAKE_PORT_2              = 0x02,
    THERMALTAKE_PORT_3              = 0x03,
    THERMALTAKE_PORT_4              = 0x04,
    THERMALTAKE_PORT_5              = 0x05
};

enum
{
    THERMALTAKE_MODE_FLOW           = 0x00,
    THERMALTAKE_MODE_SPECTRUM       = 0x04,
    THERMALTAKE_MODE_RIPPLE         = 0x08,
    THERMALTAKE_MODE_BLINK          = 0x0C,
    THERMALTAKE_MODE_PULSE          = 0x10,
    THERMALTAKE_MODE_WAVE           = 0x14,
    THERMALTAKE_MODE_PER_LED        = 0x18,
    THERMALTAKE_MODE_FULL           = 0x19
};

enum
{
    THERMALTAKE_SPEED_SLOW          = 0x03,
    THERMALTAKE_SPEED_NORMAL        = 0x02,
    THERMALTAKE_SPEED_FAST          = 0x01,
    THERMALTAKE_SPEED_EXTREME       = 0x00
};

class ThermaltakeRiingController
{
public:
    ThermaltakeRiingController(libusb_device_handle* dev_handle);
    ~ThermaltakeRiingController();

    void            SetChannelLEDs(unsigned char channel, std::vector<RGBColor> colors);
    void            SetMode(unsigned char mode, unsigned char speed);
    
    unsigned int    channel_leds[5];

private:
    libusb_device_handle*   dev;

    unsigned char   current_mode;
    unsigned char   current_speed;

    void    SendInit();

    void SendRGB
        (
            unsigned char       port,
            unsigned char       mode,
            unsigned char       speed,
            unsigned char       num_colors,
            unsigned char*      color_data
        );

    void    SendFan();
    void    SendSave();
};
