/*---------------------------------------------------------*\
| VS_XG270QC_Controller.cpp                                 |
|                                                           |
|   Driver for ViewSonic XG270QC                            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "VS_XG270QC_Controller.h"

VS_XG270QC_Controller::VS_XG270QC_Controller(hid_device* device, const char* path, std::string dev_name)
{
    dev         = device;
    location    = path;
    name        = dev_name;
}

VS_XG270QC_Controller::~VS_XG270QC_Controller()
{
    hid_close(dev);
}

std::string VS_XG270QC_Controller::GetLocation()
{
    return(location);
}

std::string VS_XG270QC_Controller::GetName()
{
    return(name);
}

std::string VS_XG270QC_Controller::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void VS_XG270QC_Controller::SetMode(uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2)
{
    // Music modes use different values for zone 2
    // Music: zone1=0x12 (VS_MODE_MUSIC), zone2=0x13 (VS_MODE_MUSIC_Z2)
    // Music Pulse: zone1=0x12 (VS_MODE_MUSIC_PULSE), zone2=0x14 (VS_MODE_MUSIC_PULSE_Z2)

    uint8_t actual_mode2 = mode2;

    // Map zone 1 music modes to their zone 2 equivalents
    if(mode1 == VS_MODE_MUSIC && mode2 == VS_MODE_MUSIC)
    {
        actual_mode2 = VS_MODE_MUSIC_Z2;  // 0x13
    }
    else if(mode1 == VS_MODE_MUSIC_PULSE && mode2 == VS_MODE_MUSIC_PULSE)
    {
        actual_mode2 = VS_MODE_MUSIC_PULSE_Z2;  // 0x14
    }

    SendModeComplete(mode1, r1, g1, b1, actual_mode2, r2, g2, b2);
}

void VS_XG270QC_Controller::SendModeComplete
    (
    uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2
    )
{
    uint8_t data[167] = {0};

    // Header byte
    data[0x00] = 0x02;

    // Zone 1 (Downward facing LEDs)
    data[0x01] = mode1;
    data[0x02] = r1;
    data[0x03] = g1;
    data[0x04] = b1;
    data[0x05] = 0x00;
    data[0x06] = 0x0A;
    data[0x07] = 0x00;

    // Zone 2 (Back facing LEDs)
    data[0x08] = mode2;
    data[0x09] = r2;
    data[0x0A] = g2;
    data[0x0B] = b2;
    data[0x0C] = 0x00;
    data[0x0D] = 0x0A;
    data[0x0E] = 0x00;

    // End marker
    data[0x0F] = 0x01;

    // Rest of the array is already zeroed by initialization

    SendCommand(data, 167);
}

void VS_XG270QC_Controller::SendCommand(uint8_t  *data, size_t length)
{
    hid_send_feature_report(dev, data, length);
}
