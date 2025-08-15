/*---------------------------------------------------------*\
| ZalmanZSyncController.cpp                                 |
|                                                           |
|   Driver for Zalman Z Sync                                |
|                                                           |
|   Based on CorsairLightingNodeConroller, the protocol is  |
|   the same as the Corsair Lighting Node except with 8     |
|   channels                                                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "StringUtils.h"
#include "ZalmanZSyncController.h"

using namespace std::chrono_literals;

ZalmanZSyncController::ZalmanZSyncController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    SendFirmwareRequest();

    /*-----------------------------------------------------*\
    | The Corsair Lighting Node Pro requires a packet within|
    | 20 seconds of sending the lighting change in order    |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&ZalmanZSyncController::KeepaliveThread, this);
}

ZalmanZSyncController::~ZalmanZSyncController()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    hid_close(dev);
}

void ZalmanZSyncController::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::seconds(1))
        {
            SendCommit();
        }
        std::this_thread::sleep_for(1s);
    }
}

std::string ZalmanZSyncController::GetFirmwareString()
{
    return(firmware_version);
}

std::string ZalmanZSyncController::GetLocationString()
{
    return("HID: " + location);
}

std::string ZalmanZSyncController::GetNameString()
{
    return(name);
}

std::string ZalmanZSyncController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void ZalmanZSyncController::SetChannelEffect(unsigned char channel,
                                                unsigned char num_leds,
                                                unsigned char mode,
                                                unsigned char speed,
                                                unsigned char direction,
                                                bool          random,
                                                unsigned char red1,
                                                unsigned char grn1,
                                                unsigned char blu1,
                                                unsigned char red2,
                                                unsigned char grn2,
                                                unsigned char blu2,
                                                unsigned char red3,
                                                unsigned char grn3,
                                                unsigned char blu3
                                               )
{
    /*-----------------------------------------------------*\
    | Send Reset packet                                     |
    \*-----------------------------------------------------*/
    SendReset(channel);

    /*-----------------------------------------------------*\
    | Send Begin packet                                     |
    \*-----------------------------------------------------*/
    SendBegin(channel);

    /*-----------------------------------------------------*\
    | Set Port State packet                                 |
    \*-----------------------------------------------------*/
    SendPortState(channel, ZALMAN_Z_SYNC_PORT_STATE_HARDWARE);

    /*-----------------------------------------------------*\
    | Set Effect Configuration packet                       |
    \*-----------------------------------------------------*/
    SendEffectConfig
    (
        channel,
        0,
        num_leds,
        mode,
        speed,
        direction,
        random,
        red1,
        grn1,
        blu1,
        red2,
        grn2,
        blu2,
        red3,
        grn3,
        blu3,
        0,
        0,
        0
    );

    /*-----------------------------------------------------*\
    | Send Commit packet                                    |
    \*-----------------------------------------------------*/
    SendCommit();
}

void ZalmanZSyncController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    unsigned char   red_color_data[50];
    unsigned char   grn_color_data[50];
    unsigned char   blu_color_data[50];
    unsigned char   pkt_offset          = 0;
    unsigned char   pkt_size            = 0;
    unsigned int    colors_remaining    = num_colors;

    /*-----------------------------------------------------*\
    | Send Port State packet                                |
    \*-----------------------------------------------------*/
    SendPortState(channel, ZALMAN_Z_SYNC_PORT_STATE_SOFTWARE);

    /*-----------------------------------------------------*\
    | Loop through colors and send 50 at a time             |
    \*-----------------------------------------------------*/
    while(colors_remaining > 0)
    {
        if(colors_remaining < 50)
        {
            pkt_size = colors_remaining;
        }
        else
        {
            pkt_size = 50;
        }

        for(int color_idx = 0; color_idx < pkt_size; color_idx++)
        {
            red_color_data[color_idx] = RGBGetRValue(colors[pkt_offset + color_idx]);
            grn_color_data[color_idx] = RGBGetGValue(colors[pkt_offset + color_idx]);
            blu_color_data[color_idx] = RGBGetBValue(colors[pkt_offset + color_idx]);
        }

        SendDirect(channel, pkt_offset, pkt_size, ZALMAN_Z_SYNC_DIRECT_CHANNEL_RED,   red_color_data);
        SendDirect(channel, pkt_offset, pkt_size, ZALMAN_Z_SYNC_DIRECT_CHANNEL_GREEN, grn_color_data);
        SendDirect(channel, pkt_offset, pkt_size, ZALMAN_Z_SYNC_DIRECT_CHANNEL_BLUE,  blu_color_data);

        colors_remaining -= pkt_size;
        pkt_offset       += pkt_size;
    }

    /*-----------------------------------------------------*\
    | Send Commit packet                                    |
    \*-----------------------------------------------------*/
    SendCommit();
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void ZalmanZSyncController::SendFirmwareRequest()
{
    int             actual;
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Firmware Version Request packet                |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = ZALMAN_Z_SYNC_PACKET_ID_FIRMWARE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    actual = hid_read(dev, usb_buf, 17);

    if(actual > 0)
    {
        if(usb_buf[0x03] < 112)
        {
            firmware_version = "0.7.1";
        }
        else
        {
            firmware_version = std::to_string(usb_buf[0x02]) + "." + std::to_string(usb_buf[0x03] >> 4) + "." + std::to_string(usb_buf[0x03] & 0x0F);
        }
    }
}

void ZalmanZSyncController::SendDirect
    (
    unsigned char   channel,
    unsigned char   start,
    unsigned char   count,
    unsigned char   color_channel,
    unsigned char*  color_data
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = ZALMAN_Z_SYNC_PACKET_ID_DIRECT;
    usb_buf[0x02]   = channel;
    usb_buf[0x03]   = start;
    usb_buf[0x04]   = count;
    usb_buf[0x05]   = color_channel;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x06], color_data, count);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 17);
}

void ZalmanZSyncController::SendCommit()
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Update last commit time                               |
    \*-----------------------------------------------------*/
    last_commit_time = std::chrono::steady_clock::now();

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = ZALMAN_Z_SYNC_PACKET_ID_COMMIT;
    usb_buf[0x02]   = 0xFF;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 17);
}

void ZalmanZSyncController::SendBegin
    (
    unsigned char   channel
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Begin packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = ZALMAN_Z_SYNC_PACKET_ID_BEGIN;
    usb_buf[0x02]   = channel;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 17);
}

void ZalmanZSyncController::SendEffectConfig
    (
    unsigned char   channel,
    unsigned char   count,
    unsigned char   led_type,
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   direction,
    unsigned char   change_style,
    unsigned char   color_0_red,
    unsigned char   color_0_green,
    unsigned char   color_0_blue,
    unsigned char   color_1_red,
    unsigned char   color_1_green,
    unsigned char   color_1_blue,
    unsigned char   color_2_red,
    unsigned char   color_2_green,
    unsigned char   color_2_blue,
    unsigned short  temperature_0,
    unsigned short  temperature_1,
    unsigned short  temperature_2
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Effect Config packet                           |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = ZALMAN_Z_SYNC_PACKET_ID_EFFECT_CONFIG;
    usb_buf[0x02]   = channel;
    usb_buf[0x03]   = count;
    usb_buf[0x04]   = led_type;

    /*-----------------------------------------------------*\
    | Set up mode parameters                                |
    \*-----------------------------------------------------*/
    usb_buf[0x05]   = mode;
    usb_buf[0x06]   = speed;
    usb_buf[0x07]   = direction;
    usb_buf[0x08]   = change_style;
    usb_buf[0x09]   = 0;

    /*-----------------------------------------------------*\
    | Set up mode colors                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x0A]   = color_0_red;
    usb_buf[0x0B]   = color_0_green;
    usb_buf[0x0C]   = color_0_blue;
    usb_buf[0x0D]   = color_1_red;
    usb_buf[0x0E]   = color_1_green;
    usb_buf[0x0F]   = color_1_blue;
    usb_buf[0x10]   = color_2_red;
    usb_buf[0x11]   = color_2_green;
    usb_buf[0x12]   = color_2_blue;

    /*-----------------------------------------------------*\
    | Set up temperatures                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x13]   = (temperature_0 >> 8);
    usb_buf[0x14]   = (temperature_0 & 0xFF);
    usb_buf[0x15]   = (temperature_1 >> 8);
    usb_buf[0x16]   = (temperature_1 & 0xFF);
    usb_buf[0x17]   = (temperature_2 >> 8);
    usb_buf[0x18]   = (temperature_2 & 0xFF);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 17);
}

void ZalmanZSyncController::SendTemperature()
{

}

void ZalmanZSyncController::SendReset
    (
    unsigned char   channel
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Reset packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = ZALMAN_Z_SYNC_PACKET_ID_RESET;
    usb_buf[0x02]   = channel;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 17);
}

void ZalmanZSyncController::SendPortState
    (
    unsigned char   channel,
    unsigned char   state
    )
{
    unsigned char   usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Port State packet                              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = ZALMAN_Z_SYNC_PACKET_ID_PORT_STATE;
    usb_buf[0x02]   = channel;
    usb_buf[0x03]   = state;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 17);
}

void ZalmanZSyncController::SendBrightness()
{

}

void ZalmanZSyncController::SendLEDCount()
{

}

void ZalmanZSyncController::SendProtocol()
{

}
