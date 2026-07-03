/*---------------------------------------------------------*\
| MSILaptopController.cpp                                   |
|                                                           |
|   Driver for MSI laptop SteelSeries KLC/ALC RGB devices   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "MSILaptopController.h"
#include "StringUtils.h"

#define MSI_LAPTOP_REPORT_ID       0x00
#define MSI_LAPTOP_COMMAND         0x0C
#define MSI_LAPTOP_KLC_PACKET_ID   0x66
#define MSI_LAPTOP_ALC_PACKET_ID   0x06
#define MSI_LAPTOP_PACKET_SIZE     525
#define MSI_LAPTOP_PAYLOAD_OFFSET  5

MSILaptopController::MSILaptopController(hid_device* dev_handle, const char* path, std::string dev_name, msi_laptop_device device_type)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    type        = device_type;
}

MSILaptopController::~MSILaptopController()
{
    hid_close(dev);
}

std::string MSILaptopController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSILaptopController::GetDeviceName()
{
    return(name);
}

std::string MSILaptopController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

msi_laptop_device MSILaptopController::GetDeviceType()
{
    return(type);
}

void MSILaptopController::SetLEDs(std::vector<led> leds, std::vector<RGBColor> colors)
{
    unsigned char   buf[MSI_LAPTOP_PACKET_SIZE];
    std::size_t     led_count = (leds.size() < colors.size()) ? leds.size() : colors.size();

    memset(buf, 0x00, sizeof(buf));

    buf[0x00] = MSI_LAPTOP_REPORT_ID;
    buf[0x01] = MSI_LAPTOP_COMMAND;
    buf[0x03] = (type == MSI_LAPTOP_KLC) ? MSI_LAPTOP_KLC_PACKET_ID : MSI_LAPTOP_ALC_PACKET_ID;

    // Fill unused LED IDs with 0xFF so they are ignored by the controller
    for(int i = 0; i < (MSI_LAPTOP_PACKET_SIZE - MSI_LAPTOP_PAYLOAD_OFFSET) / 4; i++)
    {
        buf[MSI_LAPTOP_PAYLOAD_OFFSET + (i * 4)] = 0xFF;
    }

    for(std::size_t led_idx = 0; led_idx < led_count; led_idx++)
    {
        std::size_t offset = MSI_LAPTOP_PAYLOAD_OFFSET + (led_idx * 4);

        if((offset + 3) >= sizeof(buf))
        {
            break;
        }

        buf[offset + 0] = leds[led_idx].value;
        buf[offset + 1] = RGBGetRValue(colors[led_idx]);
        buf[offset + 2] = RGBGetGValue(colors[led_idx]);
        buf[offset + 3] = RGBGetBValue(colors[led_idx]);
    }

    hid_send_feature_report(dev, buf, sizeof(buf));
}
