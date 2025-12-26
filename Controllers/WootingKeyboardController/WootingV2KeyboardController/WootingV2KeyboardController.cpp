/*---------------------------------------------------------*\
| WootingV2KeyboardController.cpp                           |
|                                                           |
|   Driver for Wooting keyboards with v2 firmware           |
|                                                           |
|   Chris M (Dr_No)                             09 Jul 2021 |
|   Diogo Trindade (diogotr7)                   25 Dec 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "WootingV2KeyboardController.h"

#define WOOTING_TWO_REPORT_SIZE         257

#undef  WOOTING_CONTROLLER_NAME
#define WOOTING_CONTROLLER_NAME         "[WootingTWO] "

//Indicates an unused entry in matrix
#define NA 0x7D

//WootingTwo uses a 16bit color space
typedef uint16_t R5G6B5_color;
#define RGB888ToRGBcolor16(r, g, b)     ((R5G6B5_color)((red & 0xF8) << 8 | (green & 0xFC) << 3 | (b & 0xF8) >> 3))
#define RGB32ToRGBcolor16(color32)      ((R5G6B5_color)((color32 & 0xF8) << 8 | (color32 & 0xFC00) >> 5 | (color32 & 0xF80000) >> 19))

WootingV2KeyboardController::WootingV2KeyboardController(hid_device* dev_handle, const char *path, WOOTING_DEVICE_TYPE wooting_type, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    name                = dev_name;
    this->wooting_type  = wooting_type;

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

WootingV2KeyboardController::~WootingV2KeyboardController()
{

}

void WootingV2KeyboardController::SendDirect(RGBColor* colors, uint8_t color_count)
{
    uint8_t rgb_buffer[WOOTING_TWO_REPORT_SIZE] = { 0, 0xD0, 0xDA, WOOTING_RAW_COLORS_REPORT};

    for(std::size_t index = 0; index < color_count; index++)
    {
        size_t buffer_index        = 4 + (index * 2);
        R5G6B5_color color16        = RGB32ToRGBcolor16(colors[index]);

        rgb_buffer[buffer_index]    = color16 & 0xFF;
        rgb_buffer[buffer_index+1]  = color16 >> 8;
    }

    uint16_t report_size = hid_write(dev, rgb_buffer, WOOTING_TWO_REPORT_SIZE);
    LOG_DEBUG("%sSend buffer returned - %04i expected %04i", WOOTING_CONTROLLER_NAME, report_size, WOOTING_TWO_REPORT_SIZE);
}
