/*-------------------------------------------------------------------*\
|  RoccatVulcanAimoController.h                                       |
|                                                                     |
|  Driver for Roccat Vulcan Aimo Keyboard                             |
|                                                                     |
|  Mola19 17/12/2021                                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

enum
{
    ROCCAT_VULCAN_MODE_DIRECT   = 0x0B,
    ROCCAT_VULCAN_MODE_STATIC   = 0x01,
    ROCCAT_VULCAN_MODE_WAVE     = 0x0A,
};

class RoccatVulcanAimoController
{
public:
    RoccatVulcanAimoController(hid_device* dev_ctrl_handle, hid_device* dev_led_handle, char *path);
    ~RoccatVulcanAimoController();

    std::string     GetSerial();
    std::string     GetLocation();
    std::string     GetVersion();

    void            SendColors(std::vector<RGBColor> colors);
    void            SendMode(unsigned int mode, unsigned int speed, unsigned int brightness, std::vector<RGBColor> colors);

private:
    std::string     location;
    hid_device*     dev_ctrl;
    hid_device*     dev_led;
};
