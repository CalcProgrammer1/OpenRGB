/*---------------------------------------------------------*\
| HyperXMicrophoneV2Controller.cpp                          |
|                                                           |
|   Driver for HyperX QuadCast 2S microphone                |
|                                                           |
|   Morgan Guimard (morg)                                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXMicrophoneV2Controller.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

HyperXMicrophoneV2Controller::HyperXMicrophoneV2Controller(hid_device* dev_handle, std::string path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXMicrophoneV2Controller::~HyperXMicrophoneV2Controller()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string HyperXMicrophoneV2Controller::GetDeviceLocation()
{
    return(location);
}

std::string HyperXMicrophoneV2Controller::GetNameString()
{
    return(name);
}

std::string HyperXMicrophoneV2Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXMicrophoneV2Controller::SendDirect(std::vector<RGBColor> colors)
{
    lock.lock();

    uint8_t buf[HYPERX_QUADCAST_2S_PACKET_SIZE];

    unsigned int total_leds_sent = 0;

    for(unsigned int i = 0; i < 7; i++)
    {
        memset(buf, 0, HYPERX_QUADCAST_2S_PACKET_SIZE);

        buf[0] = HYPERX_QUADCAST_2S_REPORT_ID;
        buf[1] = i < 6 ? 0x02 : 0x01;
        buf[2] = i;

        unsigned int c = 0;

        while (c < HYPERX_QUADCAST_2S_LEDS_PER_PACKET && total_leds_sent < HYPERX_QUADCAST_2S_TOTAL_LEDS)
        {
            buf[4 + (3 * c)] = RGBGetRValue(colors[total_leds_sent]);
            buf[5 + (3 * c)] = RGBGetGValue(colors[total_leds_sent]);
            buf[6 + (3 * c)] = RGBGetBValue(colors[total_leds_sent]);

            c++;
            total_leds_sent++;
        }

        hid_write(dev, buf, HYPERX_QUADCAST_2S_PACKET_SIZE);
    }

    lock.unlock();
}
