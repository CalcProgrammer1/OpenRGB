/*---------------------------------------------------------*\
| WootingTwoKeyboardController.cpp                          |
|                                                           |
|   Driver for Wooting Two keyboard                         |
|                                                           |
|   Chris M (Dr_No)                             09 Jul 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "WootingTwoKeyboardController.h"

#define WOOTING_TWO_REPORT_SIZE         257

#undef  WOOTING_CONTROLLER_NAME
#define WOOTING_CONTROLLER_NAME         "[WootingTWO] "

//Indicates an unused entry in matrix
#define NA 0x7D

//WootingTwo uses a 16bit color space
typedef uint16_t R5G6B5_color;
#define RGB888ToRGBcolor16(r, g, b)     ((R5G6B5_color)((red & 0xF8) << 8 | (green & 0xFC) << 3 | (b & 0xF8) >> 3))
#define RGB32ToRGBcolor16(color32)      ((R5G6B5_color)((color32 & 0xF8) << 8 | (color32 & 0xFC00) >> 5 | (color32 & 0xF80000) >> 19))

static unsigned int matrix_to_led_index_map_full[WOOTING_RGB_ROWS * WOOTING_TWO_RGB_COLUMNS] =
{
   0,  21,  42,  63,  84, 105,  1,  22,  43,  64,  85, 106,   2,  23,  44,  65,  86, 107,   3,  24,  45,
  66,  87,  NA,   4,  25,  46,  67,  88,  NA,   5,  26,  47,  68,  89,  NA,   6,  27,  48,  69,  90, 111,
   7,  28,  49,  70,  91,  NA,   8,  29,  50,  71,  92,  NA,   9,  30,  51,  72,  93,  NA,  10,  31,  52,
  73,  94, 115,  11,  32,  53,  74,  95, 116,  12,  33,  54,  75,  NA, 117,  13,  34,  55,  76,  97, 118,
  14,  35,  56,  77,  98, 119,  15,  36,  57,  NA,  99, 120,  16,  37,  58,  NA,  16, 121,  17,  38,  59,
  80, 101,  NA,  18,  39,  60,  81, 102, 123,  19,  40,  61,  82, 103, 124,  20,  41,  62,  NA, 104,  NA
};

WootingTwoKeyboardController::WootingTwoKeyboardController(hid_device* dev_handle, const char *path, uint8_t wooting_type, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    name                = dev_name;
    this->wooting_type  = wooting_type;
    key_code_limit      = (wooting_type == WOOTING_KB_TKL) ? WOOTING_ONE_KEY_CODE_LIMIT : WOOTING_TWO_KEY_CODE_LIMIT;

    /*---------------------------------------------------------*\
    | Get device HID manufacturer and product strings           |
    \*---------------------------------------------------------*/
    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    vendor = std::string(StringUtils::wstring_to_string(tmpName));

    hid_get_product_string(dev, tmpName, szTemp);
    description = std::string(StringUtils::wstring_to_string(tmpName));

    SendInitialize();
}

WootingTwoKeyboardController::~WootingTwoKeyboardController()
{

}

void WootingTwoKeyboardController::SendDirect(RGBColor* colors, uint8_t color_count)
{
    uint8_t rgb_buffer[WOOTING_TWO_REPORT_SIZE] = { 0, 0xD0, 0xDA, WOOTING_RAW_COLORS_REPORT};

    for(std::size_t index = 0; index < color_count; index++)
    {
        uint8_t buffer_index        = 4 + (matrix_to_led_index_map_full[index] * 2);
        R5G6B5_color color16        = RGB32ToRGBcolor16(colors[index]);

        LOG_TRACE("%sSetting LED %03i @ buffer %03i - RGB32 %08X RGB16 %04X", WOOTING_CONTROLLER_NAME, index, buffer_index, colors[index], color16);
        rgb_buffer[buffer_index]    = color16 & 0xFF;
        rgb_buffer[buffer_index+1]  = color16 >> 8;
    }

    wooting_usb_send_buffer(RGB_PARTS::PART0, rgb_buffer);
}

void WootingTwoKeyboardController::SendInitialize()
{
    wooting_usb_send_feature(WOOTING_COLOR_INIT_COMMAND, 0,0,0,0);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

bool WootingTwoKeyboardController::wooting_usb_send_buffer(RGB_PARTS /*part_number*/, uint8_t* report_buffer)
{
    uint16_t report_size = hid_write(dev, report_buffer, WOOTING_TWO_REPORT_SIZE);
    LOG_DEBUG("%sSend buffer returned - %04i expected %04i", WOOTING_CONTROLLER_NAME, report_size, WOOTING_TWO_REPORT_SIZE);
    return (report_size == WOOTING_TWO_REPORT_SIZE);
}
