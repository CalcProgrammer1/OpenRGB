/*---------------------------------------------------------*\
| ThermaltakeRiingQuadController.cpp                        |
|                                                           |
|   Driver for Thermaltake Riing Quad                       |
|                                                           |
|   Chris M (Dr_No)                             15 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "ThermaltakeRiingQuadController.h"

ThermaltakeRiingQuadController::ThermaltakeRiingQuadController(hid_device* dev_handle, const char* path)
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

    /*-----------------------------------------------------*\
    | The Riing Quad only seems to run in direct mode and   |
    | requires a packet within seconds to remain in the     |
    | set mode (similar to Corsair Node Pro). Start a thread|
    | to send a packet every TT_QUAD_KEEPALIVE seconds      |
    \*-----------------------------------------------------*/
    memset(tt_quad_buffer, 0x00, sizeof(tt_quad_buffer));
    unsigned char temp_buffer[3]    = { 0x00, 0x32, 0x52 };

    for(std::size_t zone_index = 0; zone_index < THERMALTAKE_QUAD_NUM_CHANNELS; zone_index++)
    {
        /*-------------------------------------------------*\
        | Add the constant bytes for the mode info buffer   |
        \*-------------------------------------------------*/
        memcpy(&tt_quad_buffer[zone_index][0], temp_buffer, 3);
    }

    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&ThermaltakeRiingQuadController::KeepaliveThread, this);
}

ThermaltakeRiingQuadController::~ThermaltakeRiingQuadController()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    hid_close(dev);
}

void ThermaltakeRiingQuadController::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::seconds(THERMALTAKE_QUAD_KEEPALIVE))
        {
            SendBuffer();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::string ThermaltakeRiingQuadController::GetDeviceName()
{
    return device_name;
}

std::string ThermaltakeRiingQuadController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ThermaltakeRiingQuadController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void ThermaltakeRiingQuadController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    unsigned char* color_data = new unsigned char[3 * num_colors];

    for(unsigned int color = 0; color < num_colors; color++)
    {
        unsigned int color_idx = color * 3;
        color_data[color_idx + 0]   = RGBGetGValue(colors[color]);
        color_data[color_idx + 1]   = RGBGetRValue(colors[color]);
        color_data[color_idx + 2]   = RGBGetBValue(colors[color]);
    }

    tt_quad_buffer[channel][THERMALTAKE_QUAD_ZONE_BYTE]     = channel + 1;
    tt_quad_buffer[channel][THERMALTAKE_QUAD_MODE_BYTE]     = current_mode + ( current_speed & 0x03 );
    memcpy(&tt_quad_buffer[channel][THERMALTAKE_QUAD_DATA_BYTE], color_data, (num_colors * 3));

    hid_write(dev, tt_quad_buffer[channel], THERMALTAKE_QUAD_PACKET_SIZE);

    delete[] color_data;
}

void ThermaltakeRiingQuadController::SetMode(unsigned char mode, unsigned char speed)
{
    current_mode        = mode;
    current_speed       = speed;
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void ThermaltakeRiingQuadController::SendInit()
{
    unsigned char usb_buf[THERMALTAKE_QUAD_PACKET_SIZE];

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
    hid_write(dev, usb_buf, THERMALTAKE_QUAD_PACKET_SIZE);
    hid_read_timeout(dev, usb_buf, THERMALTAKE_QUAD_PACKET_SIZE, THERMALTAKE_QUAD_INTERRUPT_TIMEOUT);
}

void ThermaltakeRiingQuadController::SendBuffer()
{
    for(std::size_t channel_index = 0; channel_index < THERMALTAKE_QUAD_NUM_CHANNELS; channel_index++)
    {
        hid_write(dev, tt_quad_buffer[channel_index], THERMALTAKE_QUAD_PACKET_SIZE);
    }

    /*-------------------------------------*\
    | Update the last commit time           |
    \*-------------------------------------*/
    last_commit_time = std::chrono::steady_clock::now();
}
