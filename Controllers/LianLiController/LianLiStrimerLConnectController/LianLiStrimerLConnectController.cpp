/*---------------------------------------------------------*\
| LianLiStrimerLConnectController.cpp                       |
|                                                           |
|   Driver for Lian Li Strimer L Connect                    |
|                                                           |
|   Chris M (Dr_No)                             03 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LianLiStrimerLConnectController.h"
#include "StringUtils.h"

static uint8_t speed_data[5] =
{
    0x02, 0x01, 0x00, 0xFE, 0xFF    /* Slow to fast */
};

static uint8_t brightness_data[5] =
{
    0x08, 0x03, 0x02, 0x01, 0x00    /* 0%, 25%, 50%, 75%, 100% */
};

LianLiStrimerLConnectController::LianLiStrimerLConnectController(hid_device* dev_handle, const char* path)
{
    dev                 = dev_handle;
    location            = path;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));
}

LianLiStrimerLConnectController::~LianLiStrimerLConnectController()
{
    hid_close(dev);
}

std::string LianLiStrimerLConnectController::GetDeviceName()
{
    return device_name;
}

std::string LianLiStrimerLConnectController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string LianLiStrimerLConnectController::GetLocation()
{
    return("HID: " + location);
}

void LianLiStrimerLConnectController::SendApply()
{
    uint8_t buffer[STRIMERLCONNECT_PACKET_SIZE] = { STRIMERLCONNECT_REPORT_ID, 0x2C, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00 };

    hid_write(dev, buffer, STRIMERLCONNECT_PACKET_SIZE);
}

void LianLiStrimerLConnectController::SetMode(uint8_t mode, uint8_t zone, uint8_t speed, uint8_t brightness, uint8_t direction, bool /*random_colours*/)
{
    uint8_t buffer[STRIMERLCONNECT_PACKET_SIZE] = { STRIMERLCONNECT_REPORT_ID, STRIMERLCONNECT_MODE_COMMAND, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    buffer[STRIMERLCONNECT_COMMAND_BYTE]       |= zone;

    buffer[STRIMERLCONNECT_DATA_BYTE]           = mode;
    buffer[STRIMERLCONNECT_SPEED_BYTE]          = speed_data[speed];
    buffer[STRIMERLCONNECT_DIRECTION_BYTE]      = (direction == 0) ? 1 : 0;
    buffer[STRIMERLCONNECT_BRIGHTNESS_BYTE]     = brightness_data[brightness];

    hid_write(dev, buffer, STRIMERLCONNECT_PACKET_SIZE);
}

void LianLiStrimerLConnectController::SetLedsDirect(uint8_t zone, RGBColor * led_colours, uint8_t led_count)
{
    uint8_t buffer[STRIMERLCONNECT_PACKET_SIZE] = { STRIMERLCONNECT_REPORT_ID, STRIMERLCONNECT_COLOUR_COMMAND, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    buffer[STRIMERLCONNECT_COMMAND_BYTE]       |= zone;

    for(size_t i = 0; i < led_count; i++)
    {
        uint8_t offset                          = (3 * (uint8_t)i) + STRIMERLCONNECT_DATA_BYTE;

        buffer[offset]                          = RGBGetRValue(led_colours[i]);
        buffer[offset + 1]                      = RGBGetBValue(led_colours[i]);
        buffer[offset + 2]                      = RGBGetGValue(led_colours[i]);
    }

    hid_write(dev, buffer, STRIMERLCONNECT_PACKET_SIZE);
}
