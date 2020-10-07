/*-----------------------------------------*\
|  WootingKeyboardController.h              |
|                                           |
|  Definitions and types for Wooting RGB    |
|  keyboard lighting controller             |
|                                           |
|  Diogo Trindade (diogotr7)    3/4/2021    |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum RGB_PARTS
{
    PART0,
    PART1,
    PART2,
    PART3,
    PART4
};

class WootingKeyboardController
{
public:
    WootingKeyboardController(hid_device* dev_handle);
    ~WootingKeyboardController();

    void SendDirect(RGBColor* colors, unsigned int num_colors);

private:
    hid_device*             dev;

    void SendInitialize();

    bool wooting_usb_send_feature
            (
            unsigned char command,
            unsigned char param0,
            unsigned char param1,
            unsigned char param2,
            unsigned char param3
            );

    bool wooting_usb_send_buffer
            (
            RGB_PARTS     part_number,
            unsigned char rgb_buffer[]
            );
};
