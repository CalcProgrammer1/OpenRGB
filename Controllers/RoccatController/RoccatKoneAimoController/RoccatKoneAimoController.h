/*-------------------------------------------------------------------*\
|  RoccatKoneAimoController.h                                         |
|                                                                     |
|  Driver for Roccat Kone Aimo Mouse                                  |
|                                                                     |
|  Thibaud M (enlight3d)          17th Nov 2020                       |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <string>
#include <array>
#include <map>
#include <vector>
#include <hidapi/hidapi.h>

#define NUM_LEDS            11

#define R_OFFSET            0
#define G_OFFSET            1
#define B_OFFSET            2

#define USB_COLOR_BUFF_LEN  46

enum ROCCAT_KONE_AIMO_CHANNEL
{
    SCROLL_WHEEL          = 0,
    STRIP_LEFT            = 1,
    STRIP_RIGHT           = 5,
    LOWER_LEFT            = 9,
    LOWER_RIGHT           = 10
};

class RoccatKoneAimoController
{
public:
    RoccatKoneAimoController(hid_device* dev_handle, char *_path);
    ~RoccatKoneAimoController();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SetChannelColors(ROCCAT_KONE_AIMO_CHANNEL channel, RGBColor * colors, unsigned int num_colors);
    void            SendUpdate();

private:
    std::string     device_name;
    std::string     serial;
    std::string     location;
    hid_device*     dev;
    unsigned char   usb_colors_buf[USB_COLOR_BUFF_LEN]; // USB buffer to be sent everytime we update mouse's LEDs

    void            SendInit();
};