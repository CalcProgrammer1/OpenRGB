/*---------------------------------------------------------*\
| CMARGBGen2A1Controller.cpp                                |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CMARGBGen2A1Controller.h"
#include "StringUtils.h"

CMARGBGen2A1controller::CMARGBGen2A1controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    name                = dev_name;

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

std::string CMARGBGen2A1controller::GetNameString()
{
    return(name);
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

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_LONG));
}

void CMARGBGen2A1controller::SetupDirectMode()
{
    ResetDevice();

    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    /*---------------------------------------------*\
    | Swith to direct mode                          |
    \*---------------------------------------------*/
    usb_buf[1]  = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2]  = CM_ARGB_GEN2_A1_HW_MODE_SETUP;
    usb_buf[3]  = CM_ARGB_GEN2_A1_WRITE;
    usb_buf[4]  = CM_ARGB_GEN2_A1_CHANNEL_ALL;    // CHANNEL
    usb_buf[5]  = CM_ARGB_GEN2_A1_SUBCHANNEL_ALL; // SUBCHANNEL
    usb_buf[6]  = CM_ARGB_GEN2_A1_CUSTOM_MODE;
    usb_buf[7]  = CM_ARGB_GEN2_A1_SPEED_HALF;
    usb_buf[8]  = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
    usb_buf[9]  = 0xFF; // R
    usb_buf[10] = 0xFF; // G
    usb_buf[11] = 0xFF; // B

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_SHORT));

    std::vector<RGBColor> colorOffChain;
    colorOffChain.push_back(0);
    for(unsigned int channel = 0; channel < CM_ARGB_GEN2_A1_CHANNEL_COUNT; channel++)
    {
        SendChannelColors(channel, CM_ARGB_GEN2_A1_SUBCHANNEL_ALL, colorOffChain);
    }

    for(unsigned int channel = 0; channel < CM_ARGB_GEN2_A1_CHANNEL_COUNT; channel++)
    {
        SetCustomSequence(channel);
    }

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

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_LONG));

    /*---------------------------------------------*\
    | Refresh direct mode to cycle the strips       |
    | with the new length                           |
    \*---------------------------------------------*/
    if(software_mode_activated)
    {
        SetupDirectMode();
    }
}

void CMARGBGen2A1controller::SendChannelColors(unsigned int zone_id, unsigned int subchannel_id, std::vector<RGBColor> colors)
{
    /*---------------------------------------------*\
    | Create the color data array                   |
    \*---------------------------------------------*/
    std::vector<unsigned char> color_data = CreateColorData(colors);

    std::vector<unsigned char>::iterator it = color_data.begin();

    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    unsigned int offset;

    /*----------------------------------------------------*\
    | Break-up color data in packet/s                      |
    | Intentionally clearing first packet only             |
    | Leaving garbage on subsequent packets                |
    | Original software appears to not clear them anyways. |
    \*----------------------------------------------------*/
    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);
    for(unsigned int p = 0; p < CM_ARGB_GEN2_A1_PACKETS_PER_CHANNEL && it != color_data.end(); p++)
    {
        offset = 1;

        usb_buf[offset++] = p;
        usb_buf[offset++] = CM_ARGB_GEN2_A1_SET_RGB_VALUES;
        usb_buf[offset++] = CM_ARGB_GEN2_A1_WRITE;
        usb_buf[offset++] = 1 << zone_id;
        usb_buf[offset++] = 1 << subchannel_id;

        while(it != color_data.end() && offset < CM_ARGB_GEN2_A1_PACKET_LENGTH)
        {
            usb_buf[offset++] = *it;
            it++;
        }

        if(p >= CM_ARGB_GEN2_A1_PACKETS_PER_CHANNEL - 1 || it == color_data.end())
        {
            /*--------------------------*\
            | Rewrite as end packet      |
            \*--------------------------*/
            usb_buf[1] = p + 0x80;
        }

        hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

        /*-----------------------------------------------*\
        | This device needs some delay before we send     |
        | any other packet :(                             |
        | This time is critical since the device is       |
        | still latching its input buffer.                |
        | Reducing this may start to introduce artifacts  |
        \*-----------------------------------------------*/
        std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_MEDIUM));
    }

    /*---------------------------------------------*\
    | Next channel needs some delay as well         |
    \*---------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_SHORT));
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

        std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_LONG));
    }

    /*---------------------------------------------*\
    | Set the mode values and write to the device   |
    \*---------------------------------------------*/
    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[1] = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2] = CM_ARGB_GEN2_A1_HW_MODE_SETUP;
    usb_buf[3] = CM_ARGB_GEN2_A1_WRITE;

    usb_buf[4] = CM_ARGB_GEN2_A1_CHANNEL_ALL;
    usb_buf[5] = CM_ARGB_GEN2_A1_SUBCHANNEL_ALL;

    usb_buf[6] = mode_value;

    bool is_custom_mode = (mode_value == CM_ARGB_GEN2_A1_CUSTOM_MODE);

    if(is_custom_mode)
    {
        usb_buf[7]  = CM_ARGB_GEN2_A1_SPEED_MAX;
        usb_buf[8]  = CM_ARGB_GEN2_A1_BRIGHTNESS_MAX;
        usb_buf[9]  = 0xFF; // R
        usb_buf[10] = 0xFF; // G
        usb_buf[11] = 0xFF; // B
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

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_LONG));

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

void CMARGBGen2A1controller::SetCustomSequence(unsigned int zone_id)
{
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    /*---------------------------------------------*\
    | Set custom speed for sequence mode            |
    \*---------------------------------------------*/
    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[1] = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2] = CM_ARGB_GEN2_A1_CUSTOM_SPEED;
    usb_buf[3] = CM_ARGB_GEN2_A1_WRITE;
    usb_buf[4] = 1 << zone_id; // CHANNEL
    usb_buf[5] = 0x32;

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_SHORT));

    SetPipelineStaticSequence(zone_id);
}

void CMARGBGen2A1controller::SetPipelineStaticSequence(unsigned int zone_id)
{
    /*------------------------------------------------*\
    | Set the mode sequence to full static             |
    | All steps on the effect pipeline to 0x01 STATIC  |
    \*------------------------------------------------*/
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];
    memset(usb_buf, CM_ARGB_GEN2_A1_STATIC_MODE, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    usb_buf[0]  = 0x00;
    usb_buf[1]  = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2]  = CM_ARGB_GEN2_A1_CUSTOM_SEQUENCES;
    usb_buf[3]  = CM_ARGB_GEN2_A1_WRITE;
    usb_buf[4]  = 1 << zone_id;

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_LONG));
}

void CMARGBGen2A1controller::ResetDevice()
{
    unsigned char usb_buf[CM_ARGB_GEN2_A1_PACKET_LENGTH];

    memset(usb_buf, 0x00, CM_ARGB_GEN2_A1_PACKET_LENGTH);
    usb_buf[1]  = CM_ARGB_GEN2_A1_COMMAND;
    usb_buf[2]  = CM_ARGB_GEN2_A1_RESET;
    usb_buf[3]  = CM_ARGB_GEN2_A1_WRITE;

    hid_write(dev, usb_buf, CM_ARGB_GEN2_A1_PACKET_LENGTH);

    std::this_thread::sleep_for(std::chrono::milliseconds(CM_ARGB_GEN2_A1_SLEEP_LONG));
}
