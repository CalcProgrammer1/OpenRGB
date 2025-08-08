/*---------------------------------------------------------*\
| ATC800Controller.cpp                                      |
|                                                           |
|   Driver for Aorus ATC800 cooler                          |
|                                                           |
|   Felipe Cavalcanti                           13 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "ATC800Controller.h"
#include "StringUtils.h"

ATC800Controller::ATC800Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

ATC800Controller::~ATC800Controller()
{
    hid_close(dev);
}

std::string ATC800Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ATC800Controller::GetNameString()
{
    return(name);
}

std::string ATC800Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void ATC800Controller::DisableTempRPMIndicator()
{
    uint8_t usb_buf[9] = { 0x00, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ATC800Controller::SendMode(uint8_t mode, uint8_t brightness, uint8_t speed, uint8_t mystery_flag, uint8_t zone)
{
    uint8_t usb_buf[9] = { 0x00, 0xc9, mode, brightness, speed, mystery_flag, zone, 0x00, 0x00 };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ATC800Controller::SendOneColor(uint8_t color_flag, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t usb_buf[9] = { 0x00, color_flag, red, green, blue, 0x00, 0x00, 0x00, 0x00 };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ATC800Controller::SendMultiColor(uint8_t flag, uint8_t mode, uint8_t red1, uint8_t green1, uint8_t blue1, uint8_t red2, uint8_t green2, uint8_t blue2)
{
    uint8_t usb_buf[9] = { 0x00, flag, mode, red1, green1, blue1, red2, green2, blue2 };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ATC800Controller::SendOk()
{
    uint8_t usb_buf[9] = { 0x00, 0xb6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    hid_send_feature_report(dev, usb_buf, sizeof(usb_buf));
}

void ATC800Controller::SendCoolerMode(uint8_t zone, uint8_t mode, aorus_atc800_mode_config zone_config)
{
    switch (mode)
    {
        case AORUS_ATC800_MODE_OFF:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(0x01, 0x3c, 0x02, 0x02, 0x00);
                SendOneColor(0xbc, 0x00, 0x00, 0x00);
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(0x01, 0x3c, 0x02, 0x02, 0x01);
                for(int i = 0xb0; i <= 0xb3; i++)
                {
                    SendMultiColor(i, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                }
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_CUSTOM:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, 0x02, 0x00, 0x00);
                SendOneColor(0xbc, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, 0x02, 0x00, 0x01);
                for(int i = 0xb0; i <= 0xb3; i++)
                {
                    SendMultiColor(i, mode, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]), RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                }
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_BREATHING:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, 0x3c, zone_config.speed, 0x00, 0x00);
                SendOneColor(0xbc, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, 0x3c, zone_config.speed, 0x00, 0x01);
                for(int i = 0xb0; i <= 0xb3; i++)
                {
                    SendMultiColor(i, mode, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]), RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                }
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_SPECTRUM_CYCLE:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x00, 0x00);
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x00, 0x01);
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_FLASHING:
        case AORUS_ATC800_MODE_DOUBLE_FLASHING:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x00, 0x00);
                SendOneColor(0xbc, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x00, 0x01);
                for(int i = 0xb0; i <= 0xb3; i++)
                {
                    SendMultiColor(i, mode, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]), RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                }
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_GRADIENT:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x00, 0x00);
                SendOneColor(0xcd, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x0a, 0x00);
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x08, 0x01);
                SendOneColor(0xcd, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]));
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x0a, 0x01);
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_COLOR_SHIFT:
        {
            DisableTempRPMIndicator();
            SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, zone_config.numberOfColors, 0x02);
            SendMultiColor(0xb0, mode, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]), RGBGetRValue(zone_config.colors[1]), RGBGetGValue(zone_config.colors[1]), RGBGetBValue(zone_config.colors[1]));
            SendMultiColor(0xb1, mode, RGBGetRValue(zone_config.colors[2]), RGBGetGValue(zone_config.colors[2]), RGBGetBValue(zone_config.colors[2]), RGBGetRValue(zone_config.colors[3]), RGBGetGValue(zone_config.colors[3]), RGBGetBValue(zone_config.colors[3]));
            SendMultiColor(0xb2, mode, RGBGetRValue(zone_config.colors[4]), RGBGetGValue(zone_config.colors[4]), RGBGetBValue(zone_config.colors[4]), RGBGetRValue(zone_config.colors[5]), RGBGetGValue(zone_config.colors[5]), RGBGetBValue(zone_config.colors[5]));
            SendMultiColor(0xb3, mode, RGBGetRValue(zone_config.colors[6]), RGBGetGValue(zone_config.colors[6]), RGBGetBValue(zone_config.colors[6]), RGBGetRValue(zone_config.colors[7]), RGBGetGValue(zone_config.colors[7]), RGBGetBValue(zone_config.colors[7]));
            SendOk();
        }
            break;

        case AORUS_ATC800_MODE_RAINBOW_WAVE:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x02, 0x00);
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x02, 0x01);
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_RADIATE:
        {
            DisableTempRPMIndicator();
            SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x02, 0x02);
            SendMultiColor(0xb0, mode, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]), RGBGetRValue(zone_config.colors[1]), RGBGetGValue(zone_config.colors[1]), RGBGetBValue(zone_config.colors[1]));
            SendMultiColor(0xb1, mode, RGBGetRValue(zone_config.colors[2]), RGBGetGValue(zone_config.colors[2]), RGBGetBValue(zone_config.colors[2]), RGBGetRValue(zone_config.colors[3]), RGBGetGValue(zone_config.colors[3]), RGBGetBValue(zone_config.colors[3]));
            SendMultiColor(0xb2, mode, RGBGetRValue(zone_config.colors[4]), RGBGetGValue(zone_config.colors[4]), RGBGetBValue(zone_config.colors[4]), RGBGetRValue(zone_config.colors[5]), RGBGetGValue(zone_config.colors[5]), RGBGetBValue(zone_config.colors[5]));
            SendMultiColor(0xb3, mode, RGBGetRValue(zone_config.colors[6]), RGBGetGValue(zone_config.colors[6]), RGBGetBValue(zone_config.colors[6]), RGBGetRValue(zone_config.colors[7]), RGBGetGValue(zone_config.colors[7]), RGBGetBValue(zone_config.colors[7]));
            SendOk();
        }
            break;

        case AORUS_ATC800_MODE_RAINBOW_LOOP:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x00, zone);
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x08, zone);
                SendOk();
            }
        }
            break;

        case AORUS_ATC800_MODE_TRICOLOR:
        {
            if (zone == AORUS_ATC800_TOP_ZONE)
            {
                DisableTempRPMIndicator();
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x02, 0x00);
                SendMultiColor(0xb0, mode, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]), RGBGetRValue(zone_config.colors[1]), RGBGetGValue(zone_config.colors[1]), RGBGetBValue(zone_config.colors[1]));
                SendMultiColor(0xb1, mode, RGBGetRValue(zone_config.colors[2]), RGBGetGValue(zone_config.colors[2]), RGBGetBValue(zone_config.colors[2]), 0x00, 0x00, 0x00);
                SendMultiColor(0xb2, mode, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                SendMultiColor(0xb3, mode, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                SendOk();
            }
            else if (zone == AORUS_ATC800_FANS_ZONE)
            {
                SendMode(mode, (zone_config.brightness + 1) * 0x0a, zone_config.speed, 0x02, 0x01);
                SendMultiColor(0xb0, mode, RGBGetRValue(zone_config.colors[0]), RGBGetGValue(zone_config.colors[0]), RGBGetBValue(zone_config.colors[0]), RGBGetRValue(zone_config.colors[1]), RGBGetGValue(zone_config.colors[1]), RGBGetBValue(zone_config.colors[1]));
                SendMultiColor(0xb1, mode, RGBGetRValue(zone_config.colors[2]), RGBGetGValue(zone_config.colors[2]), RGBGetBValue(zone_config.colors[2]), 0x00, 0x00, 0x00);
                SendMultiColor(0xb2, mode, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                SendMultiColor(0xb3, mode, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                SendOk();
            }
        }
            break;

    }
}
