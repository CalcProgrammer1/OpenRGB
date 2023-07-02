/*-----------------------------------------*\
|  SRGBmodsLEDControllerV1.h                |
|                                           |
|  Definitions and types for SRGBmods       |
|  LED Controller V1                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/30/2023   |
\*-----------------------------------------*/

#include "RGBController.h"
#include <chrono>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    SRGBMODS_LED_CONTROLLER_V1_MODE_RAINBOW                 = 0x01,     /* Rainbow wave mode            */
    SRGBMODS_LED_CONTROLLER_V1_MODE_BREATHING_RANDOM        = 0x02,     /* Breathing random mode        */
    SRGBMODS_LED_CONTROLLER_V1_MODE_STATIC                  = 0x03,     /* Static mode                  */
    SRGBMODS_LED_CONTROLLER_V1_MODE_BREATHING_MODE_SPECIFIC = 0x04,     /* Breathing mode specific mode */
    SRGBMODS_LED_CONTROLLER_V1_MODE_DIRECT                  = 0xFF,     /* Direct (SW) mode             */
};

class SRGBmodsLEDControllerV1
{
public:
    SRGBmodsLEDControllerV1(hid_device* dev_handle, const char* path);
    ~SRGBmodsLEDControllerV1();

    std::string     GetLocationString();
    std::string     GetSerialString();

    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);
    void            SetConfiguration(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor color);
    void            SetDirect();

private:
    hid_device*     dev;
    std::string     location;

    void            SendPacket
                        (
                        unsigned char   this_packet_id,
                        unsigned char   last_packet_id,
                        bool            reset,
                        RGBColor*       colors,
                        unsigned int    num_colors
                        );

    void            SendConfiguration
                        (
                        bool            reset,
                        unsigned char   hw_effect_enable,
                        unsigned char   hw_effect_mode,
                        unsigned char   hw_effect_speed,
                        unsigned char   hw_effect_brightness,
                        RGBColor        hw_effect_color,
                        unsigned char   status_led_enable,
                        unsigned char   color_compression_enable
                        );
};
