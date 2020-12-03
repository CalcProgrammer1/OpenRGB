/*-----------------------------------------*\
|  HyperXKeyboardController.h               |
|                                           |
|  Definitions and types for HyperX RGB     |
|  Keyboard lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/30/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HYPERX_PACKET_ID_SET_EFFECT                 = 0x02,     /* Set profile effect packet            */
    HYPERX_PACKET_ID_SET_COLOR                  = 0x06,     /* Set profile color packet             */
    HYPERX_PACKET_ID_DIRECT                     = 0x16,     /* Direct control packet                */
};


enum
{
    HYPERX_DIRECTION_RIGHT  = 0x00,
    HYPERX_DIRECTION_LEFT   = 0x01,
    HYPERX_DIRECTION_UP     = 0x02,
    HYPERX_DIRECTION_DOWN   = 0x03,
    HYPERX_DIRECTION_IN     = 0x04,
    HYPERX_DIRECTION_OUT    = 0x05
};

enum
{
    HYPERX_MODE_WAVE                    = 0x00,
    HYPERX_MODE_STATIC                  = 0x01,
    HYPERX_MODE_BREATHING               = 0x02,
};

enum
{
    HYPERX_REACTIVE_MODE_TRIGGER        = 0x03,
    HYPERX_REACTIVE_MODE_EXPLOSION      = 0x04,
    HYPERX_REACTIVE_MODE_HYPERX_FLAME   = 0x05,
    HYPERX_REACTIVE_MODE_NONE           = 0xFF
};

enum
{
    HYPERX_COLOR_MODE_SINGLE            = 0x00,
    HYPERX_COLOR_MODE_DUAL              = 0x01,
    HYPERX_COLOR_MODE_SPECTRUM          = 0x02
};

enum
{
    HYPERX_COLOR_CHANNEL_RED                = 0x01,
    HYPERX_COLOR_CHANNEL_GREEN              = 0x02,
    HYPERX_COLOR_CHANNEL_BLUE               = 0x03,
    HYPERX_COLOR_CHANNEL_EXTENDED           = 0x04
};

class HyperXKeyboardController
{
public:
    HyperXKeyboardController(hid_device* dev_handle, const char* path);
    ~HyperXKeyboardController();

    std::string     GetDeviceLocation();
    std::string     GetSerialString();

    void SetMode
        (
        unsigned char mode,
        unsigned char direction,
        unsigned char speed,
        std::vector<RGBColor> colors
        );

    void SetLEDsDirect(std::vector<RGBColor> colors);
    void SetLEDs(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    unsigned char           active_mode;
    unsigned char           active_direction;
    unsigned char           active_speed;
    std::string             location;

    void    SelectProfile
                (
                unsigned char   profile
                );

    void    SendEffect
                (
                unsigned char   profile,
                unsigned char   mode,
                unsigned char   direction,
                unsigned char   reactive_mode,
                unsigned char   speed,
                unsigned char   color_mode,
                unsigned char   red_single,
                unsigned char   grn_single,
                unsigned char   blu_single,
                unsigned char   red_dual_1,
                unsigned char   grn_dual_1,
                unsigned char   blu_dual_1,
                unsigned char   red_dual_2,
                unsigned char   grn_dual_2,
                unsigned char   blu_dual_2
                );

    void    SendColor
                (
                unsigned char   profile,
                unsigned char   color_channel,
                unsigned char*  color_data
                );

    void    SendExtendedColor
                (
                unsigned char   profile,
                unsigned char*  color_data
                );

    void    SendDirect
                (
                unsigned char   color_channel,
                unsigned char*  color_data
                );

    void    SendDirectExtended
                (
                unsigned char*  color_data
                );
};
