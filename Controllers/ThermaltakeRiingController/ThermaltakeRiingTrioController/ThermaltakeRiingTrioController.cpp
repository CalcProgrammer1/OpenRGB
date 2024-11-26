/*---------------------------------------------------------*\
| ThermaltakeRiingTrioController.cpp                        |
|                                                           |
|   Driver for Thermaltake Riing Trio                       |
|                                                           |
|   Sam B (4rcheria)                            24 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "ThermaltakeRiingTrioController.h"

ThermaltakeRiingTrioController::ThermaltakeRiingTrioController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    /*---------------------------------------------------------*\
    | Get device name from HID manufacturer and product strings |
    \*---------------------------------------------------------*/
    wchar_t name_string[HID_MAX_STR];

    hid_get_manufacturer_string(dev, name_string, HID_MAX_STR);
    device_name = StringUtils::wstring_to_string(name_string);

    hid_get_product_string(dev, name_string, HID_MAX_STR);
    device_name.append(" ").append(StringUtils::wstring_to_string(name_string));

    SendInit();

    memset(tt_trio_buffer, 0x00, sizeof(tt_trio_buffer));
    unsigned char temp_buffer[3]    = { 0x00, 0x32, 0x52 };

    for(std::size_t zone_index = 0; zone_index < THERMALTAKE_TRIO_NUM_CHANNELS; zone_index++)
    {
        /*-------------------------------------------------*\
        | Add the constant bytes for the mode info buffer   |
        \*-------------------------------------------------*/
        memcpy(&tt_trio_buffer[zone_index][0], temp_buffer, 3);
    }
}

ThermaltakeRiingTrioController::~ThermaltakeRiingTrioController()
{
    hid_close(dev);
}

std::string ThermaltakeRiingTrioController::GetDeviceName()
{
    return device_name;
}

std::string ThermaltakeRiingTrioController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ThermaltakeRiingTrioController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void ThermaltakeRiingTrioController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    if(num_colors == 0) return;

    unsigned char* color_data = new unsigned char[3 * num_colors];

    for(unsigned int color = 0; color < num_colors; color++)
    {
        unsigned int color_idx = color * 3;
        color_data[color_idx + 0]   = RGBGetGValue(colors[color]);
        color_data[color_idx + 1]   = RGBGetRValue(colors[color]);
        color_data[color_idx + 2]   = RGBGetBValue(colors[color]);
    }

    tt_trio_buffer[channel][THERMALTAKE_TRIO_ZONE_BYTE]         = channel + 1;
    tt_trio_buffer[channel][THERMALTAKE_TRIO_MODE_BYTE]         = 0x24;
    tt_trio_buffer[channel][5]                                  = 0x03;
    tt_trio_buffer[channel][7]                                  = 0x00;

    /*-------------------------------------------------*\
    | create and send chunks min = 2 max = 4            |
    \*-------------------------------------------------*/
    for(unsigned int i = 0; (num_colors  > (THERMALTAKE_TRIO_CHUNK_LENGTH * i)) || i < 2; i++)
    {
        unsigned int colors_transmitted = THERMALTAKE_TRIO_CHUNK_LENGTH * i;
        unsigned int colors_to_transmit = (num_colors > colors_transmitted) ? num_colors - colors_transmitted : 0;

        memset(&tt_trio_buffer[channel][THERMALTAKE_TRIO_CHUNK_ID], 0x00, sizeof(tt_trio_buffer[channel]) - THERMALTAKE_TRIO_CHUNK_ID);
        tt_trio_buffer[channel][THERMALTAKE_TRIO_CHUNK_ID]          = i + 1;
        if(colors_to_transmit < THERMALTAKE_TRIO_CHUNK_LENGTH)
        {
            memcpy(&tt_trio_buffer[channel][THERMALTAKE_TRIO_DATA_BYTE], &color_data[colors_transmitted * 3], (colors_to_transmit * 3));
        }
        else
        {
            memcpy(&tt_trio_buffer[channel][THERMALTAKE_TRIO_DATA_BYTE], &color_data[colors_transmitted * 3], (THERMALTAKE_TRIO_CHUNK_LENGTH * 3));
        }

        hid_write(dev, tt_trio_buffer[channel], THERMALTAKE_TRIO_PACKET_SIZE);
    }

    delete[] color_data;
}

void ThermaltakeRiingTrioController::SetMode(unsigned char mode, unsigned char speed)
{
    current_mode        = mode;
    current_speed       = speed;
}

void ThermaltakeRiingTrioController::SendInit()
{
    unsigned char usb_buf[THERMALTAKE_TRIO_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Init packet                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0xFE;
    usb_buf[0x02]   = 0x33;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, THERMALTAKE_TRIO_PACKET_SIZE);
    hid_read_timeout(dev, usb_buf, THERMALTAKE_TRIO_PACKET_SIZE, THERMALTAKE_TRIO_INTERRUPT_TIMEOUT);
}

