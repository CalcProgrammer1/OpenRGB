/*---------------------------------------------------------*\
| HyperXAlloyElite2Controller.cpp                           |
|                                                           |
|   Driver for HyperX Alloy Elite 2 keyboard                |
|                                                           |
|   KundaPanda (vojdo)                          02 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXAlloyElite2Controller.h"
#include "StringUtils.h"

/*-----------------------------------------*\
| Skip these indices in the color output    |
\*-----------------------------------------*/
static const unsigned int SKIP_INDICES[] = { 23, 29, 41, 47, 70, 71, 76, 77, 87, 88, 93, 99, 100, 102, 108, 113 };

HyperXAlloyElite2Controller::HyperXAlloyElite2Controller(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXAlloyElite2Controller::~HyperXAlloyElite2Controller()
{
    hid_close(dev);
}

std::string HyperXAlloyElite2Controller::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXAlloyElite2Controller::GetNameString()
{
    return(name);
}

std::string HyperXAlloyElite2Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXAlloyElite2Controller::SetLEDsDirect(const std::vector<RGBColor>& colors)
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Variables to keep track of color sending and skipping |
    \*-----------------------------------------------------*/
    size_t buf_idx                  = 1;
    size_t color_idx                = 0;
    size_t packets_sent             = 0;
    size_t skipped                  = 0;
    const unsigned int* skip_idx    = &SKIP_INDICES[0];

    /*-----------------------------------------------------*\
    | Initialize direct control                             |
    \*-----------------------------------------------------*/
    SendDirectInitialization();

    /*-----------------------------------------------------*\
    | Continue filling and sending packets while color data |
    | remains                                               |
    \*-----------------------------------------------------*/
    while(color_idx < colors.size())
    {
        /*-------------------------------------------------*\
        | Packets have colors in groups of 4 bytes, with    |
        | the first byte being 0x81 and then R, G, B.       |
        \*-------------------------------------------------*/
        buf[buf_idx] = 0x81;

        /*-------------------------------------------------*\
        | If at a skipped index, add null data to packet    |
        | and increment skipped count and index             |
        | Otherwise, copy color data to buffer and increment|
        | color index                                       |
        \*-------------------------------------------------*/
        if(*skip_idx == color_idx + skipped)
        {
            buf[buf_idx + 1]    = 0;
            buf[buf_idx + 2]    = 0;
            buf[buf_idx + 3]    = 0;

            skip_idx++;

            if(skip_idx >= SKIP_INDICES + sizeof(SKIP_INDICES) / sizeof(unsigned int))
            {
                skip_idx = SKIP_INDICES;
            }

            skipped++;
        }
        else
        {
            buf[buf_idx + 1]    = RGBGetRValue(colors[color_idx]);
            buf[buf_idx + 2]    = RGBGetGValue(colors[color_idx]);
            buf[buf_idx + 3]    = RGBGetBValue(colors[color_idx]);

            color_idx++;
        }

        /*-------------------------------------------------*\
        | Increment packet buffer index by 4 bytes          |
        \*-------------------------------------------------*/
        buf_idx += 4;

        /*-------------------------------------------------*\
        | If the packet buffer is full, send it and reset   |
        | buffer indexing                                   |
        | OR                                                |
        | If all colors have been filled into the buffer,   |
        | send the packet                                   |
        \*-------------------------------------------------*/
        if((buf_idx >= sizeof(buf)) || (color_idx == colors.size()))
        {
            /*---------------------------------------------*\
            | Send packet                                   |
            \*---------------------------------------------*/
            hid_send_feature_report(dev, buf, sizeof(buf));

            /*---------------------------------------------*\
            | Zero out buffer and reset index               |
            \*---------------------------------------------*/
            memset(buf, 0x00, sizeof(buf));
            buf_idx = 1;

            /*---------------------------------------------*\
            | Increment packet counter                      |
            \*---------------------------------------------*/
            packets_sent++;
        }
    }

    /*-----------------------------------------------------*\
    | Send empty packets until 9 total packets have been    |
    | sent                                                  |
    \*-----------------------------------------------------*/
    for(size_t remaining_packets = 0; packets_sent + remaining_packets < 9; remaining_packets++)
    {
        hid_send_feature_report(dev, buf, sizeof(buf));
    }
}

void HyperXAlloyElite2Controller::SendDirectInitialization()
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = 0x04;
    buf[0x02]   = 0xF2;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);
}
