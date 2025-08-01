/*---------------------------------------------------------*\
| BloodyMouseController.cpp                                 |
|                                                           |
|   Driver for A4Tech Bloody Mouse                          |
|                                                           |
|   Chris M (Dr_No)                             30 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "BloodyMouseController.h"
#include "StringUtils.h"

BloodyMouseController::BloodyMouseController(hid_device* dev_handle, const char* path, uint16_t product_id, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    pid                 = product_id;
    name                = dev_name;

    InitDevice();
}

BloodyMouseController::~BloodyMouseController()
{
    hid_close(dev);
}

uint16_t BloodyMouseController::GetPid()
{
    return(pid);
}

std::string BloodyMouseController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string BloodyMouseController::GetLocation()
{
    return("HID: " + location);
}

std::string BloodyMouseController::GetName()
{
    return(name);
}

void BloodyMouseController::InitDevice()
{
    uint8_t buffer[BLOODYMOUSE_WRITE_PACKET_SIZE]   = { 0x07, 0x03, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };

    hid_send_feature_report(dev, buffer, BLOODYMOUSE_WRITE_PACKET_SIZE);

    buffer[BLOODYMOUSE_MODE_BYTE]                   = 0;
    buffer[BLOODYMOUSE_DATA_BYTE]                   = 1;

    hid_send_feature_report(dev, buffer, BLOODYMOUSE_WRITE_PACKET_SIZE);
}

void BloodyMouseController::SetLedsDirect(std::vector<RGBColor> colors)
{
    uint8_t buffer[BLOODYMOUSE_WRITE_PACKET_SIZE]   = { 0x07, 0x03, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00 };

    for(uint8_t i = 0; i < colors.size(); i++)
    {
        uint8_t offset                              = 3 * (colors[i] >> 24) + BLOODYMOUSE_DATA_BYTE;

        buffer[offset]                              = RGBGetRValue(colors[i]);
        buffer[offset + 1]                          = RGBGetGValue(colors[i]);
        buffer[offset + 2]                          = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, buffer, BLOODYMOUSE_WRITE_PACKET_SIZE);
}
