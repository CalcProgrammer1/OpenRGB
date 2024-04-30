/*---------------------------------------------------------------*\
|  HoltekA070Controller.h                                         |
|                                                                 |
|  Definitions and types for Holtek USB Gaming Mouse [04d9:a070]  |
|                                                                 |
|  Santeri Pikarinen (santeri3700) 8/01/2020                      |
\*---------------------------------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HOLTEK_A070_MODE_STATIC            = 0x01,
    HOLTEK_A070_MODE_BREATHING_SLOW    = 0x02,
    HOLTEK_A070_MODE_BREATHING_MEDIUM  = 0x03,
    HOLTEK_A070_MODE_BREATHING_FAST    = 0x04
};

class HoltekA070Controller
{
public:
    HoltekA070Controller(hid_device* dev_handle, const char* path);
    ~HoltekA070Controller();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void        SendCustomColor
                   (
                   unsigned char       red,
                   unsigned char       green,
                   unsigned char       blue
                   );

    void        SendMode
                   (
                   unsigned char       mode
                   );

private:
    hid_device*             dev;
    std::string             location;
};
