/*---------------------------------------------------------*\
| AsusAuraMouseGen1Controller.cpp                           |
|                                                           |
|   Driver for ASUS Aura gen 1 mouse                        |
|                                                           |
|   Mola19                                      30 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <chrono>
#include <thread>
#include "AsusAuraMouseGen1Controller.h"
#include "StringUtils.h"

AsusAuraMouseGen1Controller::AsusAuraMouseGen1Controller(hid_device* dev_handle, const char* path, uint16_t pid, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    device_pid  = pid;
    name        = dev_name;
}

AsusAuraMouseGen1Controller::~AsusAuraMouseGen1Controller()
{
    hid_close(dev);
}

std::string AsusAuraMouseGen1Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AsusAuraMouseGen1Controller::GetName()
{
    return(name);
}

std::string AsusAuraMouseGen1Controller::GetSerialString()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AsusAuraMouseGen1Controller::GetVersion()
{
    unsigned char usb_buf[9] = { 0x0C, 0xC4, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    usb_buf[3] = (device_pid == 0x1824) ? 0x02 : 0x00;
    hid_send_feature_report(dev, usb_buf, 9);

    unsigned char usb_buf_out[9] = { 0x0C };
    hid_get_feature_report(dev, usb_buf_out, 9);

    return std::string("1." +  std::to_string(usb_buf_out[3]));
}

int AsusAuraMouseGen1Controller::GetActiveProfile()
{
    unsigned char profile;

    unsigned char profile_amount = 0;
    unsigned char profile_key    = 0;

    switch(device_pid)
    {
        case 0x185B:
            profile_amount = 3;
            profile_key    = 0xF0;
            break;
        case 0x181C:
        case 0x1824:
        default:
            profile_amount = 6;
            profile_key    = 0x60;
            break;
    }

    do
    {
        unsigned char usb_buf[9] = { 0x0C, 0xDF, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        hid_send_feature_report(dev, usb_buf, 9);

        unsigned char usb_buf_out[9] = { 0x0C };
        hid_get_feature_report(dev, usb_buf_out, 9);

        profile = usb_buf_out[4];
    } while(profile < profile_key || profile > profile_key + profile_amount - 1);

    return (profile % 16) + 1;
}

void AsusAuraMouseGen1Controller::SendUpdate
    (
    unsigned char   key,
    unsigned char   value
    )
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0x00, 9);

    usb_buf[0x00]   = 0x0C;
    usb_buf[0x01]   = 0xC4;
    usb_buf[0x02]   = 0x0F;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = key;
    usb_buf[0x05]   = value;
    hid_send_feature_report(dev, usb_buf, 9);

    unsigned char buf_in[9];
    buf_in[0] = 0x0C;
    hid_get_feature_report(dev, buf_in, 9);

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
}


void AsusAuraMouseGen1Controller::UpdateProfile
    (
    unsigned char   key,
    unsigned char   profile,
    unsigned char   value
    )
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0x00, 9);

    usb_buf[0x00]   = 0x0C;
    usb_buf[0x01]   = 0xDE;
    usb_buf[0x02]   = key;
    usb_buf[0x03]   = profile;
    usb_buf[0x04]   = value;
    hid_send_feature_report(dev, usb_buf, 9);

    unsigned char buf_in[9];
    buf_in[0] = 0x0C;
    hid_get_feature_report(dev, buf_in, 9);
}

void AsusAuraMouseGen1Controller::SendDirectSpatha(std::vector<RGBColor> colors)
{
    unsigned char usb_buf[33];

    memset(usb_buf, 0x00, 33);

    usb_buf[0x00]   = 0x10;
    usb_buf[0x01]   = 0x00;
    usb_buf[0x02]   = 0xF0;
    usb_buf[0x03]   = 0x00;

    for(unsigned char i = 0; i < 3; i++)
    {
        usb_buf[4 + i * 3] = RGBGetRValue(colors[i]);
        usb_buf[5 + i * 3] = RGBGetGValue(colors[i]);
        usb_buf[6 + i * 3] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, usb_buf, 33);
}

void AsusAuraMouseGen1Controller::ResetToSavedLighting()
{
    unsigned char usb_buf[9];

    memset(usb_buf, 0x00, 9);

    usb_buf[0x00]   = 0x0C;
    usb_buf[0x01]   = 0xC4;

    hid_send_feature_report(dev, usb_buf, 9);
}
