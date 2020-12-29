/*---------------------------------------------------------------*\
|  HoltekA1FAController.h                                         |
|                                                                 |
|  Definitions and types for Holtek based Mousemat [04d9:a1fa]    |
|                                                                 |
|  Edoardo Ridolfi (edo2313) 26/12/2020                           |
\*---------------------------------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    HOLTEK_A1FA_BYTE_COMMAND    = 1,
    HOLTEK_A1FA_BYTE_MODE       = 2,
    HOLTEK_A1FA_BYTE_BRIGHTNESS = 3,
    HOLTEK_A1FA_BYTE_SPEED      = 4,
    HOLTEK_A1FA_BYTE_PRESET     = 5,
    HOLTEK_A1FA_BYTE_RED        = 6,
    HOLTEK_A1FA_BYTE_GREEN      = 7,
    HOLTEK_A1FA_BYTE_BLUE       = 8

};

enum
{
    HOLTEK_A1FA_MODE_STATIC    = 0x00,
    HOLTEK_A1FA_MODE_BREATHING = 0x01,
    HOLTEK_A1FA_MODE_NEON      = 0x02,
    HOLTEK_A1FA_MODE_RAINBOW   = 0x03
};

enum
{
    HOLTEK_A1FA_SPEED_SLOWEST = 0x00,   /* Slowest speed      */
    HOLTEK_A1FA_SPEED_SLOWER  = 0x01,   /* Slower speed       */
    HOLTEK_A1FA_SPEED_SLOW    = 0x02,   /* Slow speed         */
    HOLTEK_A1FA_SPEED_NORMAL  = 0x03,   /* Normal speed       */
    HOLTEK_A1FA_SPEED_FAST    = 0x04,   /* Fast speed         */
    HOLTEK_A1FA_SPEED_FASTEST = 0x05    /* Fastest speed      */
};

class HoltekA1FAController
{
public:
    HoltekA1FAController(hid_device *dev_handle, const char *path);
    ~HoltekA1FAController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendData(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char preset, unsigned char red, unsigned char green, unsigned char blue);

private:
    hid_device *dev;
    std::string location;
};
