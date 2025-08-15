/*---------------------------------------------------------*\
| CorsairK55RGBPROXTController.cpp                          |
|                                                           |
|   Driver for Corsair K55 RGB PRO XT keyboard              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "CorsairK55RGBPROXTController.h"
#include "LogManager.h"
#include "StringUtils.h"

#define COLOR_BANK_SIZE     137
#define HID_PACKET_LENGTH   65
#define HID_PAYLOAD_SIZE1   (HID_PACKET_LENGTH - 12)
#define HID_PAYLOAD_SIZE2   (HID_PACKET_LENGTH - 4)

static const unsigned int keys[] =
    { 127, 128, 129, 130, 131, 132,  37,  49,  39,  53, 102, 101,  26,  96, 104,  54,  27,  16,   0,  25,
      103,  55,  28,  22,  18,  23,  56,  29,   4,   3,   2,  57,  30,  17,   5,  21,  31,  58,  32,  19,
        6,   1,  40,  59,  33,  24,   7,  60,  34,  20,   9,  13,  61,  35,   8,  10,  12,  14,  11,  50,
       62,  41,  15,  47,  51, 107,  63,  42,  43,  48,  52, 118,  64,  38,  44,  46, 106,  97,  65,  36,
      105,  66,  69,  72,  76,  67,  70,  73,  78,  77,  68,  71,  74,  75,  79,  91,  88,  85,  94,  80,
       92,  89,  86,  81,  93,  90,  87,  95,  82,  83,  84 };


static unsigned char color_bank[3][COLOR_BANK_SIZE];

static const unsigned char filler[] =
    { 0x70, 0x6E, 0x4E, 0x4D, 0x4C, 0x65, 0x6F, 0x2C, 0x4B, 0x4A, 0x49, 0x48, 0x47, 0x46, 0x45, 0x05,
      0x19, 0x06, 0x1B, 0x1D, 0x64, 0x6A, 0x34, 0x6B, 0x6C, 0x69, 0x38, 0x37, 0x36, 0x10, 0x11, 0x16,
      0x04, 0x39, 0x2F, 0x13, 0x12, 0x0C, 0x18, 0x33, 0x0F, 0x0E, 0x0D, 0x0B, 0x0A, 0x09, 0x07, 0x27,
      0x7A, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20, 0x1C, 0x17, 0x15, 0x08, 0x1A, 0x14, 0x2B, 0x2D,
      0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x29, 0x1F, 0x1E, 0x35, 0x44, 0x43, 0x42, 0x41, 0x40, 0x62,
      0x00, 0x5B, 0x5A, 0x59, 0x5E, 0x5D, 0x5C, 0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0x63, 0x53, 0x4F,
      0x61, 0x60, 0x5F, 0x58, 0x57, 0x56, 0x55, 0x54, 0x2A, 0x2E, 0x28, 0x32, 0x30, 0x51, 0x50, 0x52,
      0x6D };


CorsairK55RGBPROXTController::CorsairK55RGBPROXTController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev             = dev_handle;
    location        = path;
    name            = dev_name;

    LightingControl();
}

CorsairK55RGBPROXTController::~CorsairK55RGBPROXTController()
{
    hid_close(dev);
}

std::string CorsairK55RGBPROXTController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairK55RGBPROXTController::GetNameString()
{
    return(name);
}

std::string CorsairK55RGBPROXTController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CorsairK55RGBPROXTController::LightingControl()
{
    unsigned char usb_buf[HID_PACKET_LENGTH];

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x01;
    usb_buf[0x03] = 0x03;
    usb_buf[0x05] = 0x02;
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x02;
    usb_buf[0x03] = 0x5F;
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x0D;
    usb_buf[0x04] = 0x01;
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);
}

void CorsairK55RGBPROXTController::SetLEDs(std::vector<RGBColor>colors)
{
    for(std::size_t color_idx = 0; color_idx < colors.size(); ++color_idx)
    {
        RGBColor color                 = colors[color_idx];
        color_bank[0][keys[color_idx]] = RGBGetRValue(color);
        color_bank[1][keys[color_idx]] = RGBGetGValue(color);
        color_bank[2][keys[color_idx]] = RGBGetBValue(color);
    }

    unsigned char* color_ptr = &color_bank[0][0];
    unsigned char usb_buf[HID_PACKET_LENGTH];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x06;
    usb_buf[0x04] = 0x9B;
    usb_buf[0x05] = 0x01;

    memcpy(&usb_buf[12], color_ptr, HID_PAYLOAD_SIZE1);
    color_ptr += HID_PAYLOAD_SIZE1;
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    usb_buf[0x02] = 0x07;

    for(std::size_t i = 0; i < 6; ++i)
    {
        memcpy(&usb_buf[4], color_ptr, HID_PAYLOAD_SIZE2);
        color_ptr += HID_PAYLOAD_SIZE2;
        hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);
    }
}

void CorsairK55RGBPROXTController::SetHardwareMode
     (
         int mode_value,
         unsigned int color_mode,
         std::vector<RGBColor> colors,
         unsigned int speed,
         unsigned int direction
     )
{
    LightingControl();

    unsigned char usb_buf[HID_PACKET_LENGTH];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x0D;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x61;
    usb_buf[0x05] = 0x6D;
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x09;
    usb_buf[0x03] = 0x01;
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x06;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x78;
    usb_buf[0x08] = mode_value & 0x00FF;
    usb_buf[0x09] = mode_value >> 8;

    if(color_mode != MODE_COLORS_NONE)
    {
        usb_buf[0x0A] = color_mode == MODE_COLORS_RANDOM ? CORSAIR_HW_MODE_COLOR_RANDOM : CORSAIR_HW_MODE_COLOR_PREDEF;
    }

    if(mode_value == CORSAIR_HW_MODE_WATER_COLOR_VALUE)
    {
        usb_buf[0x0A] = CORSAIR_HW_MODE_COLOR_UNKNOWN;
    }

    usb_buf[0x0B] = speed;

    if((mode_value == CORSAIR_HW_MODE_COLOR_WAVE_VALUE) ||
       (mode_value == CORSAIR_HW_MODE_RAINBOW_WAVE_VALUE) ||
       (mode_value == CORSAIR_HW_MODE_RAIN_VALUE) ||
       (mode_value == CORSAIR_HW_MODE_SPIRAL_VALUE) ||
       (mode_value == CORSAIR_HW_MODE_VISOR_VALUE))
    {
        switch(direction)
        {
            case MODE_DIRECTION_LEFT:
                if(mode_value == CORSAIR_HW_MODE_SPIRAL_VALUE)
                {
                    usb_buf[0x0C] = CORSAIE_HW_MODE_DIR_COUNTER_CLOCK_WISE;
                }
                else
                {
                    usb_buf[0x0C] = CORSAIR_HW_MODE_DIR_LEFT;
                }
                break;

            case MODE_DIRECTION_RIGHT:
                if(mode_value == CORSAIR_HW_MODE_SPIRAL_VALUE)
                {
                    usb_buf[0x0C] = CORSAIR_HW_MODE_DIR_CLOCK_WISE;
                }
                else
                {
                    usb_buf[0x0C] = CORSAIR_HW_MODE_DIR_RIGHT;
                }
                break;

            case MODE_DIRECTION_UP:
                usb_buf[0x0C] = CORSAIR_HW_MODE_DIR_UP;
                break;

            case MODE_DIRECTION_DOWN:
                usb_buf[0x0C] = CORSAIR_HW_MODE_DIR_DOWN;
                break;

            default:
               usb_buf[0x0C] = CORSAIR_HW_MODE_DIR_NONE;
               break;
        }
    }

    int fill_dest_index = 0x0F;
    int fill_src_index  = 0x00;

    if(usb_buf[0x0A] != CORSAIR_HW_MODE_COLOR_RANDOM)
    {
        usb_buf[0x0E] = (unsigned char)colors.size();

        for(size_t i = 0; i < colors.size(); ++i)
        {
            usb_buf[0x0F + i * 4] = 0xFF;
            usb_buf[0x10 + i * 4] = RGBGetBValue(colors[i]);
            usb_buf[0x11 + i * 4] = RGBGetGValue(colors[i]);
            usb_buf[0x12 + i * 4] = RGBGetRValue(colors[i]);
            usb_buf[4]      += 4;
            fill_dest_index += 4;
        }
    }

    memcpy(&usb_buf[fill_dest_index], &filler[fill_src_index], HID_PACKET_LENGTH - fill_dest_index);
    fill_src_index += (HID_PACKET_LENGTH - fill_dest_index);
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x07;
    usb_buf[0x03] = 0x01;
    memcpy(&usb_buf[4], &filler[fill_src_index], HID_PACKET_LENGTH - 4);
    fill_src_index += (HID_PACKET_LENGTH - 4);
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x07;
    usb_buf[0x03] = 0x01;
    memcpy(&usb_buf[4], &filler[fill_src_index], sizeof(filler) - fill_src_index);
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01] = 0x08;
    usb_buf[0x02] = 0x05;
    usb_buf[0x03] = 0x01;
    usb_buf[0x04] = 0x01;
    hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);
}

void CorsairK55RGBPROXTController::SwitchMode(bool software)
{
    if(software)
    {
        LightingControl();
    }
    else
    {
        unsigned char usb_buf[HID_PACKET_LENGTH];

        memset(usb_buf, 0x00, sizeof(usb_buf));
        usb_buf[0x01] = 0x00;
        usb_buf[0x02] = 0x01;
        usb_buf[0x03] = 0x03;
        usb_buf[0x05] = 0x01;
        hid_write(dev, (unsigned char *)usb_buf, HID_PACKET_LENGTH);
    }
}
