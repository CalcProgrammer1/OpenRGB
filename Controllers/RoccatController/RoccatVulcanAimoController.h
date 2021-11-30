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

class RoccatVulcanAimoController
{
public:
    RoccatVulcanAimoController(hid_device* dev_handle, char *path);
    ~RoccatVulcanAimoController();

    std::string     GetSerial();
    std::string     GetLocation();

    void            SendColors(std::vector<RGBColor> colors);

private:
    std::string     location;
    hid_device*     dev;
};
