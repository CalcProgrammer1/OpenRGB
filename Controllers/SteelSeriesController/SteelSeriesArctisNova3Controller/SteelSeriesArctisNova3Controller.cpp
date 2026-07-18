/*---------------------------------------------------------*\
| SteelSeriesArctisNova3Controller.cpp                      |
|                                                           |
|   Driver for SteelSeries Arctis Nova 3                    |
|                                                           |
|   Tomasz Gorczyca (eldiablo123)               18 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/
#include <string.h>
#include "SteelSeriesArctisNova3Controller.h"
#include "StringUtils.h"

SteelSeriesArctisNova3Controller::SteelSeriesArctisNova3Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev         = dev_handle;
    location    = info.path;
    name        = dev_name;
}

SteelSeriesArctisNova3Controller::~SteelSeriesArctisNova3Controller()
{
    hid_close(dev);
}

std::string SteelSeriesArctisNova3Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string SteelSeriesArctisNova3Controller::GetNameString()
{
    return(name);
}

std::string SteelSeriesArctisNova3Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SteelSeriesArctisNova3Controller::SetSpeedBytes(unsigned char* usb_buf, unsigned char speed)
{
    switch(speed)
    {
        case 1:     usb_buf[0x0D] = 0x30; usb_buf[0x0E] = 0x75; break;
        case 2:     usb_buf[0x0D] = 0x2E; usb_buf[0x0E] = 0x68; break;
        case 3:     usb_buf[0x0D] = 0x2C; usb_buf[0x0E] = 0x5B; break;
        case 4:     usb_buf[0x0D] = 0x20; usb_buf[0x0E] = 0x4E; break;
        case 5:     usb_buf[0x0D] = 0x1E; usb_buf[0x0E] = 0x41; break;
        case 6:     usb_buf[0x0D] = 0x1C; usb_buf[0x0E] = 0x34; break;
        case 7:     usb_buf[0x0D] = 0x1A; usb_buf[0x0E] = 0x27; break;
        case 8:     usb_buf[0x0D] = 0x0E; usb_buf[0x0E] = 0x1A; break;
        case 9:     usb_buf[0x0D] = 0x0C; usb_buf[0x0E] = 0x0D; break;
        case 10:    usb_buf[0x0D] = 0x94; usb_buf[0x0E] = 0x02; break;
        default:    usb_buf[0x0D] = 0x0C; usb_buf[0x0E] = 0x0D; break;
    }
}

/*---------------------------------------------------------------------*\
| Select/enable packet (64 bytes): 06 A5 (01=left/02=right) (00/0A)     |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::SelectZone(unsigned char zone_id, unsigned char state)
{
    unsigned char usb_buf[ARCTIS_NOVA3_APPLY_REPORT_SIZE];
    memset(usb_buf, 0x00, ARCTIS_NOVA3_APPLY_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_NOVA3_REPORT_ID;
    usb_buf[0x01] = 0xA5;
    usb_buf[0x02] = (zone_id == 0) ? 0x02 : 0x01;
    usb_buf[0x03] = state;

    hid_write(dev, usb_buf, ARCTIS_NOVA3_APPLY_REPORT_SIZE);
}

/*---------------------------------------------------------------------*\
| Apply packet (64 bytes): 06 (A3=left/09=right)                        |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::ApplyZone(unsigned char zone_id)
{
    unsigned char usb_buf[ARCTIS_NOVA3_APPLY_REPORT_SIZE];
    memset(usb_buf, 0x00, ARCTIS_NOVA3_APPLY_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_NOVA3_REPORT_ID;
    usb_buf[0x01] = (zone_id == 0) ? 0x09 : 0xA3;

    hid_write(dev, usb_buf, ARCTIS_NOVA3_APPLY_REPORT_SIZE);
}

void SteelSeriesArctisNova3Controller::SendEffectPacket(unsigned char* usb_buf)
{
    hid_write(dev, usb_buf, ARCTIS_NOVA3_REPORT_SIZE);
}

void SteelSeriesArctisNova3Controller::SetOff(unsigned char zone_id)
{
    SelectZone(zone_id, 0x00);
    ApplyZone(zone_id);
}

/*---------------------------------------------------------------------*\
| Save                                                                  |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::Save(unsigned char zone_id)
{
    ApplyZone(zone_id);
}

/*---------------------------------------------------------------------*\
| Single Color / Direct                                                 |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::SetDirect(unsigned char zone_id, RGBColor color)
{
    unsigned char usb_buf[ARCTIS_NOVA3_REPORT_SIZE];
    memset(usb_buf, 0x00, ARCTIS_NOVA3_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_NOVA3_REPORT_ID;
    usb_buf[0x01] = 0xAA;
    usb_buf[0x02] = (zone_id == 0) ? 0x01 : 0x00;

    usb_buf[0x0D] = 0x98;
    usb_buf[0x0E] = 0x3A;

    usb_buf[0x15] = 0x55;
    usb_buf[0x17] = 0x96;

    usb_buf[0x1D] = 0x01;

    usb_buf[0x25] = 0x02;

    usb_buf[0x2D] = RGBGetRValue(color);
    usb_buf[0x2E] = RGBGetGValue(color);
    usb_buf[0x2F] = RGBGetBValue(color);
    usb_buf[0x30] = 0xFF;

    SendEffectPacket(usb_buf);

    SelectZone(zone_id, 0x0A);
    ApplyZone(zone_id);
}

/*---------------------------------------------------------------------*\
| Breathe                                                                |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::SetBreathe(unsigned char zone_id, RGBColor color, unsigned char speed)
{
    unsigned char usb_buf[ARCTIS_NOVA3_REPORT_SIZE];
    memset(usb_buf, 0x00, ARCTIS_NOVA3_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_NOVA3_REPORT_ID;
    usb_buf[0x01] = 0xAA;
    usb_buf[0x02] = (zone_id == 0) ? 0x01 : 0x00;

    SetSpeedBytes(usb_buf, speed);

    usb_buf[0x15] = 0x55;
    usb_buf[0x17] = 0x96;

    usb_buf[0x25] = 0x03;

    usb_buf[0x29] = RGBGetRValue(color);
    usb_buf[0x2A] = RGBGetGValue(color);
    usb_buf[0x2B] = RGBGetBValue(color);

    usb_buf[0x30] = 0x7F;
    usb_buf[0x31] = RGBGetRValue(color);
    usb_buf[0x32] = RGBGetGValue(color);
    usb_buf[0x33] = RGBGetBValue(color);
    usb_buf[0x34] = 0x7F;

    SendEffectPacket(usb_buf);

    SelectZone(zone_id, 0x0A);
    ApplyZone(zone_id);
}

/*---------------------------------------------------------------------*\
| Color Shift - Rainbow                                                 |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::SetColorShiftRainbow(unsigned char zone_id, unsigned char speed)
{
    unsigned char usb_buf[ARCTIS_NOVA3_REPORT_SIZE];
    memset(usb_buf, 0x00, ARCTIS_NOVA3_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_NOVA3_REPORT_ID;
    usb_buf[0x01] = 0xAA;
    usb_buf[0x02] = (zone_id == 0) ? 0x01 : 0x00;

    SetSpeedBytes(usb_buf, speed);

    usb_buf[0x15] = 0x55;
    usb_buf[0x17] = 0x96;

    usb_buf[0x25] = 0x07;

    static const unsigned char rainbow_stops[] =
    {
        0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
        0x2B, 0x00, 0xFF, 0x00, 0x2B, 0x00, 0xFF, 0xFF, 0x2B, 0x00,
        0x00, 0xFF, 0x2B, 0xFF, 0x00, 0xFF, 0x2B, 0xFF, 0x00, 0x00,
        0x26
    };

    memcpy(&usb_buf[0x26], rainbow_stops, sizeof(rainbow_stops));

    SendEffectPacket(usb_buf);

    SelectZone(zone_id, 0x0A);
    ApplyZone(zone_id);
}

/*---------------------------------------------------------------------*\
| Color Shift - Heat Orange                                             |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::SetColorShiftOrange(unsigned char zone_id, unsigned char speed)
{
    unsigned char usb_buf[ARCTIS_NOVA3_REPORT_SIZE];
    memset(usb_buf, 0x00, ARCTIS_NOVA3_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_NOVA3_REPORT_ID;
    usb_buf[0x01] = 0xAA;
    usb_buf[0x02] = (zone_id == 0) ? 0x01 : 0x00;

    SetSpeedBytes(usb_buf, speed);

    usb_buf[0x15] = 0x55;
    usb_buf[0x17] = 0x96;

    usb_buf[0x25] = 0x03;

    static const unsigned char orange_stops[] =
    {
        0xFF, 0xEA, 0x00, 0xFF, 0xEA, 0x00, 0x00, 0xFF, 0x4D, 0x00,
        0x7F, 0xFF, 0xEA, 0x00, 0x7F
    };

    memcpy(&usb_buf[0x26], orange_stops, sizeof(orange_stops));

    SendEffectPacket(usb_buf);

    SelectZone(zone_id, 0x0A);
    ApplyZone(zone_id);
}

/*---------------------------------------------------------------------*\
| Color Shift - Frost Blue                                              |
\*---------------------------------------------------------------------*/
void SteelSeriesArctisNova3Controller::SetColorShiftBlue(unsigned char zone_id, unsigned char speed)
{
    unsigned char usb_buf[ARCTIS_NOVA3_REPORT_SIZE];
    memset(usb_buf, 0x00, ARCTIS_NOVA3_REPORT_SIZE);

    usb_buf[0x00] = ARCTIS_NOVA3_REPORT_ID;
    usb_buf[0x01] = 0xAA;
    usb_buf[0x02] = (zone_id == 0) ? 0x01 : 0x00;

    SetSpeedBytes(usb_buf, speed);

    usb_buf[0x15] = 0x55;
    usb_buf[0x17] = 0x96;

    usb_buf[0x25] = 0x03;

    static const unsigned char blue_stops[] =
    {
        0x8C, 0x00, 0xFF, 0x8C, 0x00, 0xFF, 0x00, 0x3B, 0xD1, 0xFF,
        0x7F, 0x8C, 0x00, 0xFF, 0x7F
    };

    memcpy(&usb_buf[0x26], blue_stops, sizeof(blue_stops));

    SendEffectPacket(usb_buf);

    SelectZone(zone_id, 0x0A);
    ApplyZone(zone_id);
}
