/*---------------------------------------------------------*\
| CMARGBGen2A1Controller.cpp                                |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CMARGBGen2A1Controller.h"
#include "StringUtils.h"

CMARGBGen2A1controller::CMARGBGen2A1controller(hid_device* dev_handle, const hid_device_info& info)
{
    dev                 = dev_handle;
    location            = info.path;

    /*---------------------------------------------*\
    | Setup direct mode on start                    |
    \*---------------------------------------------*/
    SetupDirectMode();
}

CMARGBGen2A1controller::~CMARGBGen2A1controller()
{
    hid_close(dev);
}

std::string CMARGBGen2A1controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CMARGBGen2A1controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CMARGBGen2A1controller::SaveToFlash()
{
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[1] = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2] = CM_ARGB_GEN2_A1_FLASH;
    usb_buf[3] = CM_ARGB_GEN2_A1_WRITE;

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void CMARGBGen2A1controller::SetupDirectMode()
{
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    /*---------------------------------------------*\
    | Swith to direct mode                          |
    \*---------------------------------------------*/
    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[1] = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2] = CM_ARGB_GEN2_A1_LIGHTNING_CONTROL;
    usb_buf[3] = CM_ARGB_GEN2_A1_WRITE;
    usb_buf[4] = 0x01; // channel???

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    software_mode_activated = true;
}

void CMARGBGen2A1controller::SetupZoneSize(unsigned int zone_id, unsigned int size)
{
    /*---------------------------------------------*\
    | Set the mode sequence to full static          |
    | (01 for static)                               |
    |                                               |
    | This device stores 2 distinct values          |
    | - effect speed                                |
    | - approximated zone size                      |
    |                                               |
    | It's probably based on standard ARGB sizes    |
    | Still, the 06 value has some mystery.         |
    |                                               |
    | ES= effect speed                              |
    | LC= LEDs count                                |
    |                                               |
    | ES LC                                         |
    | -----                                         |
    | 0a 06                                         |
    | 09 06                                         |
    | 08 07                                         |
    | 07 08                                         |
    | 06 0a                                         |
    | 05 0c                                         |
    | 04 0f                                         |
    | 03 14                                         |
    | 02 1e                                         |
    | 01 3c                                         |
    \*---------------------------------------------*/

    const unsigned char gaps[10] =
    {
        0x05, 0x06, 0x07, 0x08, 0x0A, 0x0C, 0x0F, 0x14, 0x1E, 0x3C
    };

    unsigned char speed = 0x0A;

    for(unsigned int g = 0; g < 10; g++)
    {
        if(size <= gaps[g])
        {
            break;
        }

        speed--;
    }

    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[1] = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2] = CM_ARGB_GEN2_A1_SIZES;
    usb_buf[3] = CM_ARGB_GEN2_A1_WRITE;
    usb_buf[4] = 1 << zone_id;

    usb_buf[5] = speed;
    usb_buf[6] = size;

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void CMARGBGen2A1controller::SendDirectChannel(unsigned int zone_id, std::vector<RGBColor> colors)
{
    /*---------------------------------------------*\
    | Create the color data array                   |
    \*---------------------------------------------*/
    std::vector<unsigned char> color_data = CreateColorData(colors);

    std::vector<unsigned char>::iterator it = color_data.begin();

    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    unsigned int offset;

    unsigned char packet_start[CM_ARGB_GEN2_A1_PACKETS_PER_CHANNEL] =
    {
        0x00, 0x01, 0x82
    };

    /*---------------------------------------------*\
    | Send 3 packets for the zone                   |
    \*---------------------------------------------*/
    for(unsigned int p = 0; p < CM_ARGB_GEN2_A1_PACKETS_PER_CHANNEL; p++)
    {
        memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

        usb_buf[1] = packet_start[p];
        usb_buf[2] = 0x09;

        if(p == 0)
        {
            usb_buf[3] = 1 << zone_id;
            usb_buf[5] = 0x3C;

            offset = 6;
        }
        else
        {
            offset = 3;
        }

        while(offset < CM_ARGB_GEN2_A1_PACKET_LENGTH && it != color_data.end())
        {
            usb_buf[offset] = *it;
            offset++;
            it++;
        }

        hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

        /*---------------------------------------------*\
        | This device needs some delay before we send   |
        | any other packet                              |
        \*---------------------------------------------*/
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    /*---------------------------------------------*\
    | Next channel needs some delay as well         |
    \*---------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void CMARGBGen2A1controller::SetMode(unsigned int mode_value, unsigned char speed, unsigned char brightness, RGBColor color, bool random)
{
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    /*---------------------------------------------*\
    | Switch to hardware mode if needed             |
    \*---------------------------------------------*/
    if(software_mode_activated)
    {
        memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

        usb_buf[1] = CM_ARGB_GEN2_A1_COMMAND;
        usb_buf[2] = CM_ARGB_GEN2_A1_LIGHTNING_CONTROL;
        usb_buf[3] = CM_ARGB_GEN2_A1_WRITE;

        hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

        software_mode_activated = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    /*---------------------------------------------*\
    | Set the mode values and write to the device   |
    \*---------------------------------------------*/
    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[1] = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2] = CM_ARGB_GEN2_A1_HW_MODE_SETUP;
    usb_buf[3] = CM_ARGB_GEN2_A1_WRITE;

    usb_buf[4] = 0xFF;
    usb_buf[5] = 0xFF;

    usb_buf[6] = mode_value;

    bool is_custom_mode = mode_value == CM_ARGB_GEN2_A1_CUSTOM_MODE;

    if(is_custom_mode)
    {
        usb_buf[8] = 0xFF;
    }
    else
    {
        usb_buf[7]  = speed;
        usb_buf[8]  = brightness;
        usb_buf[9]  = RGBGetRValue(color);
        usb_buf[10] = RGBGetGValue(color);
        usb_buf[11] = RGBGetBValue(color);
        usb_buf[12] = random;
    }


    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if(is_custom_mode)
    {
        for(unsigned int channel = 0; channel < CM_ARGB_GEN2_A1_CHANNEL_COUNT; channel++)
        {
            SetCustomSequence(channel);
        }
    }
}

std::vector<unsigned char> CMARGBGen2A1controller::CreateColorData(std::vector<RGBColor> colors)
{
    std::vector<unsigned char> color_data;

    for(unsigned int c = 0; c < colors.size(); c++)
    {
        color_data.push_back(RGBGetRValue(colors[c]));
        color_data.push_back(RGBGetGValue(colors[c]));
        color_data.push_back(RGBGetBValue(colors[c]));
    }

    return(color_data);
}


void CMARGBGen2A1controller::SetCustomColors(unsigned int zone_id, std::vector<RGBColor> colors)
{
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    /*---------------------------------------------*\
    | Create the color data array                   |
    \*---------------------------------------------*/
    std::vector<unsigned char> color_data = CreateColorData(colors);

    std::vector<unsigned char>::iterator it = color_data.begin();

    unsigned char packet_start[5] =
    {
        CM_ARGB_GEN2_A1_COMMAND, 0x00, 0x01, 0x02, 0x83
    };

    /*---------------------------------------------*\
    | Send the 5 packets of colors                  |
    \*---------------------------------------------*/
    for(unsigned int p = 0; p < 5; p++)
    {
        memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

        usb_buf[1] = packet_start[p];

        /*----------------------------------------------*\
        | This part isnt well understood                 |
        | 1st packet starts with CM_ARGB_GEN2_A1_COMMAND |
        | Looks like it is a read command                |
        \*----------------------------------------------*/
        usb_buf[2] = p == 0 ? 0x06 : 0x08; // 0x08 custom data, 0x06 sizes?
        usb_buf[3] = p == 0 ? 0x01 : 0x02; // read/write has no meaning here

        usb_buf[4] = 1 << zone_id;

        unsigned int offset = 6;

        while(p > 0 && offset < CM_ARGB_GEN2_A1_PACKET_LENGTH && it != color_data.end())
        {
            usb_buf[offset] = *it;
            offset++;
            it++;
        }

        hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

}

void CMARGBGen2A1controller::SetCustomSequence(unsigned int zone_id)
{
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    const unsigned char static_seq = 0x01;

    /*---------------------------------------------*\
    | Set the mode sequence to full static          |
    | (01 for static)                               |
    \*---------------------------------------------*/
    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[1]  = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2]  = CM_ARGB_GEN2_A1_CUSTOM_SEQUENCES;
    usb_buf[3]  = CM_ARGB_GEN2_A1_WRITE;
    usb_buf[4]  = 1 << zone_id;

    usb_buf[5]  = static_seq;
    usb_buf[6]  = static_seq;
    usb_buf[7]  = static_seq;
    usb_buf[8]  = static_seq;
    usb_buf[9]  = static_seq;
    usb_buf[10] = static_seq;

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
