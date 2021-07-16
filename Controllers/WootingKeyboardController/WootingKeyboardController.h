/*-------------------------------------------------------------------*\
|  WootingKeyboardController.h                                        |
|                                                                     |
|  OpenRGB driver for Wooting RGB keyboardlighting controller         |
|      https://github.com/WootingKb/wooting-rgb-sdk                   |
|                                                                     |
|  Chris M (Dr_No)         9th July 2021                              |
\*-------------------------------------------------------------------*/

#include "RGBController.h"
#include "LogManager.h"
#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define WOOTING_COMMAND_SIZE            8
#define WOOTING_REPORT_SIZE             129
#define WOOTING_RAW_COLORS_REPORT       11
#define WOOTING_SINGLE_COLOR_COMMAND    30
#define WOOTING_SINGLE_RESET_COMMAND    31
#define WOOTING_RESET_ALL_COMMAND       32
#define WOOTING_COLOR_INIT_COMMAND      33
#define WOOTING_ONE_KEY_CODE_LIMIT      95
#define WOOTING_TWO_KEY_CODE_LIMIT      116
#define RGB_RAW_BUFFER_SIZE             96

#define WOOTING_RGB_ROWS                6
#define WOOTING_RGB_COLUMNS             21
#define WOOTING_ONE_RGB_COLUMNS         17
#define WOOTING_TWO_RGB_COLUMNS         21

/*---------------------------------------------------------*\
| Placeholder for compilation. Redefined by each subclass   |
\*---------------------------------------------------------*/
#define WOOTING_CONTROLLER_NAME         "[Wooting] "

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
    WootingKeyboardController();
    virtual ~WootingKeyboardController();

    hid_device*         dev;
    std::string         name;
    std::string         vendor;
    std::string         description;
    std::string         location;
    std::string         serial;
    uint8_t             wooting_type;
    uint8_t             key_code_limit;

    std::string         GetName();
    std::string         GetVendor();
    std::string         GetDescription();
    std::string         GetLocation();
    std::string         GetSerial();
    uint8_t             GetWootingType();
    bool                wooting_usb_send_feature(uint8_t command, uint8_t param0,
                                                 uint8_t param1, uint8_t param2, uint8_t param3);

    virtual void        SendDirect(RGBColor* colors, uint8_t color_count)                           = 0;

private:
    virtual void        SendInitialize()                                                            = 0;
    virtual bool        wooting_usb_send_buffer(RGB_PARTS part_number, uint8_t* rgb_buffer)         = 0;
};
