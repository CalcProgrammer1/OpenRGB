/*---------------------------------------------------------*\
| HyperXMicrophoneController.cpp                            |
|                                                           |
|   Driver for HyperX microphone                            |
|                                                           |
|   Matt Silva (thesilvanator)                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXMicrophoneController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

HyperXMicrophoneController::HyperXMicrophoneController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, std::string path, std::string dev_name)
{
    wrapper     = hid_wrapper;
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HyperXMicrophoneController::~HyperXMicrophoneController()
{
    lock.lock();

    if(dev)
    {
        wrapper.hid_close(dev);
    }

    lock.unlock();
}

std::string HyperXMicrophoneController::GetDeviceLocation()
{
    return(location);
}

std::string HyperXMicrophoneController::GetNameString()
{
    return(name);
}

std::string HyperXMicrophoneController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = wrapper.hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void HyperXMicrophoneController::SaveColors(std::vector<RGBColor> colors, unsigned int num_frames)
{
    unsigned int  num_color_packets                     = 0;
    unsigned int  frame                                 = 0;
    unsigned char color[HYPERX_QUADCAST_S_PACKET_SIZE]  = {0};

    num_color_packets = num_frames/8;
    if(num_frames % 8)
    {
        num_color_packets++;
    }

    lock.lock();

    /*---------------------------------------------------------*\
    | Start Save Transaction                                    |
    |   0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x53, (uint8_t)num_color_packets, 0);

    while(frame < num_frames)
    {
        memset(color, 0, HYPERX_QUADCAST_S_PACKET_SIZE);

        unsigned int i = 0;
        while(i < 8 && frame < num_frames)
        {
            int index           = HYPERX_QUADCAST_S_FRAME_SIZE * i;
            RGBColor top        = colors[frame*2];
            RGBColor bot        = colors[frame*2 + 1];

            color[index + 1]    = 0x81;
            color[index + 2]    = RGBGetRValue(top);
            color[index + 3]    = RGBGetGValue(top);
            color[index + 4]    = RGBGetBValue(top);
            color[index + 5]    = 0x81;
            color[index + 6]    = RGBGetRValue(bot);
            color[index + 7]    = RGBGetGValue(bot);
            color[index + 8]    = RGBGetBValue(bot);

            i++;
            frame++;
        }

        std::this_thread::sleep_for(15ms);
        wrapper.hid_send_feature_report(dev, color, HYPERX_QUADCAST_S_PACKET_SIZE);
    }

    /*---------------------------------------------------------*\
    | Post Save Transaction                                     |
    |   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x02, 0, 0);

    /*---------------------------------------------------------*\
    | Stop Save Transaction                                     |
    |   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x23, 1, 0);

    SendEOT((uint8_t)num_frames);

    /*---------------------------------------------------------*\
    | Post Save Transaction                                     |
    |   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x02, 0, 0);

    lock.unlock();

    /*---------------------------------------------------------*\
    | Likes to have one temporary direct packet after the save  |
    | for some reason                                           |
    \*---------------------------------------------------------*/
    SendDirect(colors);
}

void HyperXMicrophoneController::SendDirect(std::vector<RGBColor> colors)
{
    /*---------------------------------------------------------*\
    | Verify colors size                                        |
    \*---------------------------------------------------------*/
    if(colors.size() != 2)
    {
        return;
    }

    RGBColor c1 = colors[0];
    RGBColor c2 = colors[1];
    uint8_t  buffer[HYPERX_QUADCAST_S_PACKET_SIZE];

    /*---------------------------------------------------------*\
    | Colour packet                                             |
    \*---------------------------------------------------------*/
    memset(buffer, 0, HYPERX_QUADCAST_S_PACKET_SIZE);

    buffer[0x01]    = 0x81;
    buffer[0x02]    = RGBGetRValue(c1);
    buffer[0x03]    = RGBGetGValue(c1);
    buffer[0x04]    = RGBGetBValue(c1);
    buffer[0x05]    = 0x81;
    buffer[0x06]    = RGBGetRValue(c2);
    buffer[0x07]    = RGBGetGValue(c2);
    buffer[0x08]    = RGBGetBValue(c2);

    lock.lock();

    wrapper.hid_send_feature_report(dev, buffer, HYPERX_QUADCAST_S_PACKET_SIZE);
    std::this_thread::sleep_for(15ms);

    SendToRegister(0xF2, 0, 1);

    lock.unlock();
}

void HyperXMicrophoneController::SendEOT(uint8_t frame_count)
{
    uint8_t buffer[HYPERX_QUADCAST_S_PACKET_SIZE];

    memset(buffer, 0, HYPERX_QUADCAST_S_PACKET_SIZE);

    buffer[0x01]    = 0x08;
    buffer[0x3C]    = 0x28;
    buffer[0x3D]    = frame_count;
    buffer[0x3E]    = 0x00;
    buffer[0x3F]    = 0xAA;
    buffer[0x40]    = 0x55;

    wrapper.hid_send_feature_report(dev, buffer, HYPERX_QUADCAST_S_PACKET_SIZE);
    std::this_thread::sleep_for(15ms);
}

void HyperXMicrophoneController::SendToRegister(uint8_t reg, uint8_t param1, uint8_t param2)
{
    uint8_t buffer[HYPERX_QUADCAST_S_PACKET_SIZE];

    memset(buffer, 0, HYPERX_QUADCAST_S_PACKET_SIZE);

    buffer[0x01]    = 0x04;
    buffer[0x02]    = reg;         // 0xF2 Apply, 0x53 Save
    buffer[0x08]    = param1;
    buffer[0x09]    = param2;

    wrapper.hid_send_feature_report(dev, buffer, HYPERX_QUADCAST_S_PACKET_SIZE);
    std::this_thread::sleep_for(15ms);
}
