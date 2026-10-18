/*---------------------------------------------------------*\
| MSIClutchGM11Controller.cpp                               |
|                                                           |
|   Driver for MSI Clutch GM11 Gaming Mouse                 |
|                                                           |
|   Anatoly U. (TTolikoLL)                      20 SEP 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cmath>
#include <cstring>
#include <chrono>
#include <thread>
#include "MSIClutchGM11Controller.h"
#include "StringUtils.h"

static unsigned char colour_index_data[2][2][2] =
{
    {
        { 0x00, 0x04 },
        { 0x02, 0x03 }
    },
    {
        { 0x00, 0x05 },
        { 0x01, 0x06 }
    }
};

MSIClutchGM11Controller::MSIClutchGM11Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

MSIClutchGM11Controller::~MSIClutchGM11Controller()
{
    hid_close(dev);
}

std::string MSIClutchGM11Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string MSIClutchGM11Controller::GetNameString()
{
    return(name);
}

std::string MSIClutchGM11Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned char MSIClutchGM11Controller::GetColourIndex(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char largest = red;

    if(green > largest)
    {
        largest = green;
    }

    if(blue > largest)
    {
        largest = blue;
    }

    if(largest == 0)
    {
        return(0x00);
    }

    return colour_index_data[red == largest][green == largest][blue == largest];
}

void MSIClutchGM11Controller::SendPacket(unsigned char* data)
{
    unsigned char packet[MSI_CLUTCH_GM11_REPORT_SIZE];
    unsigned char report_buf[MSI_CLUTCH_GM11_REPORT_SIZE + 1];
    unsigned char sum = 0;

    memcpy(packet, data, MSI_CLUTCH_GM11_REPORT_SIZE);

    for(int i = 0; i < MSI_CLUTCH_GM11_REPORT_SIZE - 1; i++)
    {
        sum += packet[i];
    }

    packet[MSI_CLUTCH_GM11_REPORT_SIZE - 1] = 0xFF - sum;

    report_buf[0] = 0x00;
    memcpy(&report_buf[1], packet, MSI_CLUTCH_GM11_REPORT_SIZE);

    if(hid_send_feature_report(dev, report_buf, MSI_CLUTCH_GM11_REPORT_SIZE + 1) < 0)
    {
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(MSI_CLUTCH_GM11_PACKET_DELAY));
}

void MSIClutchGM11Controller::SendHandshake()
{
    unsigned char handshake_pkt[MSI_CLUTCH_GM11_REPORT_SIZE] =
    {
        MSI_CLUTCH_GM11_CMD_HANDSHAKE, 0x03, 0x0F, 0xF2, 0x40, 0x00, 0x00, 0x00
    };

    SendPacket(handshake_pkt);
}

void MSIClutchGM11Controller::SendModeTrigger(unsigned char mode_value)
{
    unsigned char trig_pkt[MSI_CLUTCH_GM11_REPORT_SIZE] =
    {
        MSI_CLUTCH_GM11_CMD_MODE_TRIGGER, mode_value, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    SendPacket(trig_pkt);
}

void MSIClutchGM11Controller::SendBrightness(unsigned char mode_value, unsigned char brightness)
{
    unsigned char br_pkt[MSI_CLUTCH_GM11_REPORT_SIZE] =
    {
        MSI_CLUTCH_GM11_CMD_LED_CONTROL, mode_value, MSI_CLUTCH_GM11_REG_BRIGHTNESS, brightness, 0x00, 0x00, 0x00, 0x00
    };

    SendPacket(br_pkt);
}

void MSIClutchGM11Controller::SendModeParameters(unsigned char mode_value, unsigned char param)
{
    unsigned char apply_pkt[MSI_CLUTCH_GM11_REPORT_SIZE] =
    {
        MSI_CLUTCH_GM11_CMD_LED_CONTROL, mode_value, MSI_CLUTCH_GM11_REG_PARAMS, param, 0x00, 0x00, 0x00, 0x00
    };

    SendPacket(apply_pkt);
}

void MSIClutchGM11Controller::SendCustomPalette(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char prep_pkt[MSI_CLUTCH_GM11_REPORT_SIZE] =
    {
        MSI_CLUTCH_GM11_CMD_PALETTE_PREP, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    SendPacket(prep_pkt);

    unsigned char palette_buf[MSI_CLUTCH_GM11_PALETTE_SIZE];

    memset(palette_buf, 0x00, sizeof(palette_buf));

    palette_buf[0]  = 0x00;

    palette_buf[1]  = 0xFF;
    palette_buf[2]  = 0x00;
    palette_buf[3]  = 0x00;

    palette_buf[4]  = 0xFF;
    palette_buf[5]  = 0xFF;
    palette_buf[6]  = 0x00;

    palette_buf[7]  = 0x00;
    palette_buf[8]  = 0xFF;
    palette_buf[9]  = 0x00;

    palette_buf[10] = 0x00;
    palette_buf[11] = 0xFF;
    palette_buf[12] = 0xFF;

    palette_buf[13] = 0x00;
    palette_buf[14] = 0x00;
    palette_buf[15] = 0xFF;

    palette_buf[16] = 0xFF;
    palette_buf[17] = 0x00;
    palette_buf[18] = 0xFF;

    palette_buf[19] = 0xFF;
    palette_buf[20] = 0xFF;
    palette_buf[21] = 0xFF;

    palette_buf[22] = red;
    palette_buf[23] = green;
    palette_buf[24] = blue;

    palette_buf[32] = MSI_CLUTCH_GM11_PALETTE_TERMINATOR;

    if(hid_write(dev, palette_buf, sizeof(palette_buf)) < 0)
    {
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(MSI_CLUTCH_GM11_PACKET_DELAY));
}

void MSIClutchGM11Controller::SetMode(RGBColor color,
                                     unsigned char mode_value,
                                     unsigned char brightness,
                                     unsigned char speed,
                                     unsigned char direction,
                                     unsigned int color_mode)
{
    SendHandshake();

    if(mode_value == MSI_CLUTCH_GM11_MODE_OFF)
    {
        unsigned char off_pkt[MSI_CLUTCH_GM11_REPORT_SIZE] =
        {
            MSI_CLUTCH_GM11_CMD_LED_CONTROL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        SendPacket(off_pkt);
        return;
    }

    SendModeTrigger(mode_value);
    SendBrightness(mode_value, brightness);

    unsigned char spd_val = speed;

    if(spd_val < MSI_CLUTCH_GM11_SPEED_MIN)
    {
        spd_val = MSI_CLUTCH_GM11_SPEED_MIN;
    }
    else if(spd_val > MSI_CLUTCH_GM11_SPEED_MAX)
    {
        spd_val = MSI_CLUTCH_GM11_SPEED_MAX;
    }

    if(mode_value == MSI_CLUTCH_GM11_MODE_STATIC)
    {
        SendCustomPalette(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
        SendModeParameters(mode_value, MSI_CLUTCH_GM11_SLOT_CUSTOM);
    }
    else if(mode_value == MSI_CLUTCH_GM11_MODE_BREATHING ||
            mode_value == MSI_CLUTCH_GM11_MODE_REACTIVE)
    {
        unsigned char color_val;

        if(color_mode == MODE_COLORS_RANDOM)
        {
            color_val = MSI_CLUTCH_GM11_COLOR_RANDOM;
        }
        else
        {
            color_val = (GetColourIndex(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color)) + 1) & 0x0F;
        }

        SendModeParameters(mode_value, (spd_val << 4) | color_val);
    }
    else
    {
        unsigned char dir_val = (direction == MODE_DIRECTION_DOWN) ? MSI_CLUTCH_GM11_DIR_DOWN : MSI_CLUTCH_GM11_DIR_UP;
        SendModeParameters(mode_value, (spd_val << 4) | dir_val);
    }
}