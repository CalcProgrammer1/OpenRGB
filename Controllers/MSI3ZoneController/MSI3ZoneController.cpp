/*---------------------------------------------------------*\
| MSI3ZoneController.cpp                                    |
|                                                           |
|   Driver for MSI/SteelSeries 3-Zone keyboard              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "MSI3ZoneController.h"
#include "StringUtils.h"

MSI3ZoneController::MSI3ZoneController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    //strcpy(device_name, "MSI 3-Zone Keyboard");
}

MSI3ZoneController::~MSI3ZoneController()
{
    hid_close(dev);
}

char* MSI3ZoneController::GetDeviceName()
{
    return device_name;
}

std::string MSI3ZoneController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSI3ZoneController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void MSI3ZoneController::SetLEDs(std::vector<RGBColor> colors)
{
    //Shout out to bparker06 for reverse engineering the MSI keyboard USB protocol!
    // https://github.com/bparker06/msi-keyboard/blob/master/keyboard.cpp for original implementation
    unsigned char buf[8] = { 0 };

    buf[0] = 1;
    buf[1] = 2;
    buf[2] = 64;
    buf[3] = 1;
    buf[4] = RGBGetRValue(colors[0]);
    buf[5] = RGBGetGValue(colors[0]);
    buf[6] = RGBGetBValue(colors[0]);
    buf[7] = 236;

    hid_send_feature_report(dev, buf, 8);

    buf[3] = 2;
    buf[4] = RGBGetRValue(colors[1]);
    buf[5] = RGBGetGValue(colors[1]);
    buf[6] = RGBGetBValue(colors[1]);

    hid_send_feature_report(dev, buf, 8);

    buf[3] = 3;
    buf[4] = RGBGetRValue(colors[2]);
    buf[5] = RGBGetGValue(colors[2]);
    buf[6] = RGBGetBValue(colors[2]);

    hid_send_feature_report(dev, buf, 8);

    buf[3] = 4;
    buf[4] = RGBGetRValue(colors[3]);
    buf[5] = RGBGetGValue(colors[3]);
    buf[6] = RGBGetBValue(colors[3]);

    hid_send_feature_report(dev, buf, 8);

    buf[3] = 5;

    hid_send_feature_report(dev, buf, 8);

    buf[3] = 6;

    hid_send_feature_report(dev, buf, 8);

    buf[3] = 7;

    hid_send_feature_report(dev, buf, 8);
}
