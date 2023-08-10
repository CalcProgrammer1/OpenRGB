/*-------------------------------------------------------------------*\
|  RoccatSenseAimoController.cpp                                      |
|                                                                     |
|  Driver for Roccat Sense Aimo                                       |
|                                                                     |
|  Mola19 08/09/2023                                                  |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <hidapi/hidapi.h>

enum
{
    ROCCAT_SENSE_AIMO_MODE_DIRECT     = 0x0B,
    ROCCAT_SENSE_AIMO_MODE_STATIC     = 0x01,
    ROCCAT_SENSE_AIMO_MODE_BREATHING  = 0x03,
    ROCCAT_SENSE_AIMO_MODE_HEARTBEAT  = 0x04,
    ROCCAT_SENSE_AIMO_MODE_DEFAULT    = 0x09,
    ROCCAT_SENSE_AIMO_MODE_WAVE       = 0x0A
};

enum
{
    ROCCAT_SENSE_AIMO_SPEED_MIN             = 0xFF,
    ROCCAT_SENSE_AIMO_SPEED_MAX             = 0x00,
    ROCCAT_SENSE_AIMO_SPEED_DEFAULT         = 0x07,
    ROCCAT_SENSE_AIMO_BRIGHTNESS_MIN        = 0x00,
    ROCCAT_SENSE_AIMO_BRIGHTNESS_MAX        = 0xFF,
    ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT    = 0xFF
};

struct mode_struct
{
    uint8_t     profile;
    uint8_t     mode;
    uint8_t     speed;
    uint8_t     brightness;
    RGBColor    left;
    RGBColor    right;
};

class RoccatSenseAimoController
{
public:
    RoccatSenseAimoController(hid_device* dev_handle, char *path);
    ~RoccatSenseAimoController();

    std::string     GetSerial();
    std::string     GetLocation();
    std::string     GetVersion();

    mode_struct     GetMode();

    void            SendDirect(std::vector<RGBColor> colors);
    void            SetMode(uint8_t profile, uint8_t mode, uint8_t speed, uint8_t brightness, std::vector<RGBColor> colors);

private:
    hid_device*     dev;
    std::string     location;
};
