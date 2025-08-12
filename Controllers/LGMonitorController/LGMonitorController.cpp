/*---------------------------------------------------------*\
| LGMonitorController.cpp                                   |
|                                                           |
|   Driver for LG monitor                                   |
|                                                           |
|   Morgan Guimard (morg)                       11 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "LGMonitorController.h"
#include "StringUtils.h"

LGMonitorController::LGMonitorController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;
}

LGMonitorController::~LGMonitorController()
{
    hid_close(dev);
}

std::string LGMonitorController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string LGMonitorController::GetNameString()
{
    return(name);
}

std::string LGMonitorController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LGMonitorController::SetDirect(const std::vector<RGBColor> colors)
{
    /*---------------------------------------------------------*\
    | Make sure the device is set to on                         |
    \*---------------------------------------------------------*/
    if(!on)
    {
        TurnOn(true);
    }

    /*---------------------------------------------------------*\
    | Make sure the direct mode is enabled                      |
    \*---------------------------------------------------------*/
    if(!direct_mode_enabled)
    {
        EnableDirectMode();
    }

    /*---------------------------------------------------------*\
    | Prepare the colors data                                   |
    \*---------------------------------------------------------*/
    uint8_t data[192];
    memset(data, 0x00, 192);

    unsigned int offset = 0;

    data[offset++] = LG_MONITOR_START_CMD_1;
    data[offset++] = LG_MONITOR_START_CMD_2;
    data[offset++] = LG_MONITOR_DIRECT_CTL;
    data[offset++] = 0x02;
    data[offset++] = 0x91;
    data[offset++] = 0x00;

    for(const RGBColor color: colors)
    {
        data[offset++] = RGBGetRValue(color);
        data[offset++] = RGBGetGValue(color);
        data[offset++] = RGBGetBValue(color);
    }

    data[offset]    = crc(data, 0, offset);
    offset++;

    data[offset++]  = LG_MONITOR_END_CMD_1;
    data[offset]    = LG_MONITOR_END_CMD_2;

    /*---------------------------------------------------------*\
    | Send the data (3 packets of 64 bytes)                     |
    \*---------------------------------------------------------*/
    uint8_t buf[LG_MONITOR_PACKET_SIZE];
    memset(buf, 0x00, LG_MONITOR_PACKET_SIZE);

    for(unsigned int i = 0; i < 3; i++)
    {
        memcpy(&buf[1], &data[64 * i], 64);
        hid_write(dev, buf, LG_MONITOR_PACKET_SIZE);
    }
}

void LGMonitorController::SetMode(uint8_t mode_value, uint8_t brightness, const std::vector<RGBColor> colors)
{
    switch(mode_value)
    {
        case LG_MONITOR_OFF_MODE_VALUE:
            /*---------------------------------------------------------*\
            | Turn off lighting                                         |
            \*---------------------------------------------------------*/
            TurnOn(false);
            break;

        case LG_MONITOR_STATIC_SLOT_1_MODE_VALUE:
            /*---------------------------------------------------------*\
            | Set slot 1 active                                         |
            \*---------------------------------------------------------*/
            EnableMode(LG_MONITOR_STATIC_SLOT_1_MODE_VALUE);

            SetBrightness(brightness);
            /*---------------------------------------------------------*\
            | Send color in slot 1                                      |
            \*---------------------------------------------------------*/
            SetSlotColor(LG_MONITOR_STATIC_SLOT_1_MODE_VALUE, colors[0]);

            break;

        case LG_MONITOR_SPECTRUM_CYCLE_MODE_VALUE:
        case LG_MONITOR_RAINBOW_MODE_VALUE:
            /*---------------------------------------------------------*\
            | Enable given mode                                         |
            \*---------------------------------------------------------*/
            EnableMode(mode_value);
            SetBrightness(brightness);
            break;

        default:
            break;
    }

    direct_mode_enabled = false;
}

void LGMonitorController::EnableDirectMode()
{
    EnableMode(LG_MONITOR_DIRECT_MODE_VALUE);
    direct_mode_enabled = true;
}

void LGMonitorController::EnableMode(uint8_t mode_value)
{
    uint8_t buf[LG_MONITOR_PACKET_SIZE];
    memset(buf, 0x00, LG_MONITOR_PACKET_SIZE);

    buf[1]  = LG_MONITOR_START_CMD_1;
    buf[2]  = LG_MONITOR_START_CMD_2;
    buf[3]  = LG_MONITOR_SET_MODE;
    buf[4]  = 0x02;
    buf[5]  = 0x02;
    buf[6]  = LG_MONITOR_MODE_CTL;
    buf[7]  = mode_value;
    buf[8]  = crc(buf, 1, 8);
    buf[9]  = LG_MONITOR_END_CMD_1;
    buf[10] = LG_MONITOR_END_CMD_2;

    hid_write(dev, buf, LG_MONITOR_PACKET_SIZE);
}

void LGMonitorController::SetBrightness(uint8_t brightness)
{
    uint8_t buf[LG_MONITOR_PACKET_SIZE];
    memset(buf, 0x00, LG_MONITOR_PACKET_SIZE);

    buf[1]  = LG_MONITOR_START_CMD_1;
    buf[2]  = LG_MONITOR_START_CMD_2;
    buf[3]  = LG_MONITOR_SET_POWER_STATE;
    buf[4]  = 0x02;
    buf[5]  = 0x02;
    buf[6]  = LG_MONITOR_BRIGHTNESS_CTL;
    buf[7]  = brightness;
    buf[8]  = crc(buf, 1, 8);
    buf[9]  = LG_MONITOR_END_CMD_1;
    buf[10] = LG_MONITOR_END_CMD_2;

    hid_write(dev, buf, LG_MONITOR_PACKET_SIZE);
}

void LGMonitorController::TurnOn(bool value)
{
    uint8_t buf[LG_MONITOR_PACKET_SIZE];
    memset(buf, 0x00, LG_MONITOR_PACKET_SIZE);

    buf[1]  = LG_MONITOR_START_CMD_1;
    buf[2]  = LG_MONITOR_START_CMD_2;
    buf[3]  = LG_MONITOR_SET_POWER_STATE;
    buf[4]  = 0x02;
    buf[5]  = 0x02;
    buf[6]  = value ? LG_MONITOR_POWER_ON : LG_MONITOR_POWER_OFF;
    buf[8]  = crc(buf, 1, 8);
    buf[9]  = LG_MONITOR_END_CMD_1;
    buf[10] = LG_MONITOR_END_CMD_2;

    hid_write(dev, buf, LG_MONITOR_PACKET_SIZE);

    on = value;
}

void LGMonitorController::SetSlotColor(uint8_t slot, const RGBColor color)
{
    uint8_t buf[LG_MONITOR_PACKET_SIZE];
    memset(buf, 0x00, LG_MONITOR_PACKET_SIZE);

    buf[1]  = LG_MONITOR_START_CMD_1;
    buf[2]  = LG_MONITOR_START_CMD_2;
    buf[3]  = LG_MONITOR_SET_COLOR;

    buf[4]  = 0x02;
    buf[5]  = 0x04;
    buf[6]  = slot;

    buf[7]  = RGBGetRValue(color);
    buf[8]  = RGBGetGValue(color);
    buf[9]  = RGBGetBValue(color);

    buf[10] = crc(buf, 1, 10);
    buf[11] = LG_MONITOR_END_CMD_1;
    buf[12] = LG_MONITOR_END_CMD_2;

    hid_write(dev, buf, LG_MONITOR_PACKET_SIZE);
}

uint8_t LGMonitorController::crc(const uint8_t data[], uint8_t start, uint8_t end)
{
    uint8_t crc = 0;

    for(unsigned int i = start; i < end; i++)
    {
        crc = crc ^ data[i];
    }

    return crc;
}
