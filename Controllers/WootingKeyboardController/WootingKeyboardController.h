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

enum WOOTING_DEVICE_TYPE
{
    WOOTING_KB_TKL      = 0,
    WOOTING_KB_FULL     = 1
};

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
    WootingKeyboardController(hid_device* dev_handle, const char *path, uint8_t wooting_type);
    ~WootingKeyboardController();

    void                    SendDirect(RGBColor* colors, unsigned int num_colors);
    std::string             GetName();
    std::string             GetVendor();
    std::string             GetDescription();
    std::string             GetLocation();
    std::string             GetSerial();
    uint8_t                 GetWootingType();
private:
    hid_device*             dev;
    std::string             name;
    std::string             vendor;
    std::string             description;
    std::string             location;
    std::string             serial;
    uint8_t                 wooting_type;
    uint8_t                 key_code_limit;

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
