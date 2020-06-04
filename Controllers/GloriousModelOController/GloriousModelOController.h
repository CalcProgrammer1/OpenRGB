/*-----------------------------------------*\
|  GloriousModelOController.h               |
|                                           |
|  Definitions and types for Glorious       |
|  and other Mice                           |
|                                           |
|  Niels Westphal (crashniels) 20/5/2020    |
\*-----------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    GLORIOUS_MODE_OFF = 0x00, //does nothing
    GLORIOUS_MODE_RAINBOW = 0x01,
    GLORIOUS_MODE_STATIC = 0x02,
    GLORIOUS_MODE_SPECTRUM_BREATING = 0x03,
    GLORIOUS_MODE_TAIL = 0x04,
    GLORIOUS_MODE_SPECTRUM_CYCLE = 0x05,
    GLORIOUS_MODE_RAVE = 0x07,
    GLORIOUS_MODE_EPILEPSY = 0x08, //not in the official software
    GLORIOUS_MODE_WAVE = 0x09,
    GLORIOUS_MODE_BREATHING = 0x0a,
};

enum
{
    GLORIOUS_SPEED_SLOW = 0x41,
    GLORIOUS_SPEED_NORMAL = 0x42,
    GLORIOUS_SPEED_FAST = 0x43,
};

enum
{
    GLORIOUS_DIRECTION_UP = 0x00,
    GLORIOUS_DIRECTION_DOWN = 0x01,
};

enum
{
    GLORIOUS_MODE_BREATING_SLOW = 0x01,
    GLORIOUS_MODE_BREATING_NORMAL = 0x02,
    GLORIOUS_MODE_BREATING_FAST = 0x03,
};

class GloriousModelOController
{
public:
    GloriousModelOController(hid_device* dev_handle);
    ~GloriousModelOController();

    std::string     GetDeviceName();
    unsigned int    GetLEDCount();

    void            SetLEDColor(RGBColor* color_buf);
    void            SetMode(unsigned char mode, unsigned char speed, unsigned char direction, RGBColor* color_buf);
private:
    hid_device*             dev;

    char                    device_name[32];
    unsigned int            led_count;

    unsigned char           current_mode;
    unsigned char           current_speed;
    unsigned char           current_direction;

};
