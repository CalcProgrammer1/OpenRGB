/*-----------------------------------------*\
|  SinowealthController.h                   |
|                                           |
|  Definitions and types for Sinowealth     |
|  mice, including Glorious                 |
|                                           |
|  Niels Westphal (crashniels) 20/5/2020    |
\*-----------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    GLORIOUS_MODE_OFF               = 0x00, //does nothing
    GLORIOUS_MODE_RAINBOW           = 0x01,
    GLORIOUS_MODE_STATIC            = 0x02,
    GLORIOUS_MODE_SPECTRUM_BREATING = 0x03,
    GLORIOUS_MODE_TAIL              = 0x04,
    GLORIOUS_MODE_SPECTRUM_CYCLE    = 0x05,
    GLORIOUS_MODE_RAVE              = 0x07,
    GLORIOUS_MODE_EPILEPSY          = 0x08, //not in the official software
    GLORIOUS_MODE_WAVE              = 0x09,
    GLORIOUS_MODE_BREATHING         = 0x0a,
};

enum
{
    GLORIOUS_SPEED_SLOW             = 0x41,
    GLORIOUS_SPEED_NORMAL           = 0x42,
    GLORIOUS_SPEED_FAST             = 0x43,
};

enum
{
    GLORIOUS_DIRECTION_UP           = 0x00,
    GLORIOUS_DIRECTION_DOWN         = 0x01,
};

enum
{
    GLORIOUS_MODE_BREATING_SLOW     = 0x01,
    GLORIOUS_MODE_BREATING_NORMAL   = 0x02,
    GLORIOUS_MODE_BREATING_FAST     = 0x03,
};

class SinowealthController
{
public:
    SinowealthController(hid_device* dev_handle_id_4, hid_device* dev_handle_id_5, char *_path); //RGB, Command, path
    ~SinowealthController();

    unsigned int    GetLEDCount();
    std::string     GetLocation();

    void            SetLEDColor(RGBColor* color_buf);
    void            SetMode(unsigned char mode, unsigned char speed, unsigned char direction, RGBColor* color_buf);
    void            GetProfile();
private:
    hid_device*             dev_report_id_4;
    hid_device*             dev_report_id_5;

    unsigned int            led_count;

    unsigned char           current_mode;
    unsigned char           current_speed;
    unsigned char           current_direction;
    unsigned char           device_configuration[520];

    std::string             location;
};
