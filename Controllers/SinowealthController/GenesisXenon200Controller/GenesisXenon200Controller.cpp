/*---------------------------------------------------------*\
| GenesisXenon200Controller.cpp                             |
|                                                           |
|   Driver for Genesis Xenon 200 mouse                      |
|                                                           |
|   chrabonszcz                                    Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "GenesisXenon200Controller.h"

GenesisXenon200Controller::GenesisXenon200Controller(hid_device* dev_handle, hid_device* cmd_dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    cmd_dev     = cmd_dev_handle;
    location    = path;
    name        = dev_name;
}

GenesisXenon200Controller::~GenesisXenon200Controller()
{

}

std::string GenesisXenon200Controller::GetLocationString()
{
    return("HID: " + location);
}

std::string GenesisXenon200Controller::GetNameString()
{
    return(name);
}

void GenesisXenon200Controller::SaveMode(unsigned char mode, unsigned char value, RGBColor color)
{
    unsigned char usb_buf[154];

    usb_buf[0] = 0x04;

    hid_get_feature_report(dev, usb_buf, 154);

    usb_buf[0x5D] = mode;
    usb_buf[0x60] = value;
    usb_buf[0x61] = RGBGetRValue(color);
    usb_buf[0x62] = RGBGetGValue(color);
    usb_buf[0x63] = RGBGetBValue(color);

    hid_send_feature_report(dev, usb_buf, 154);

    usb_buf[0] = 0x08;
    hid_get_feature_report(cmd_dev, usb_buf, 9);
    hid_send_feature_report(cmd_dev, usb_buf, 9);
}
