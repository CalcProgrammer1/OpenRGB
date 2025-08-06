/*---------------------------------------------------------*\
| DRGBController.cpp                                        |
|                                                           |
|   Driver for DRGBmods                                     |
|                                                           |
|   Zhi Yan                                     25 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "DRGBController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

DRGBController::DRGBController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    device_pid  = pid;

    /*-----------------------------------------------------*\
    | Exit hardware effects.  Start a thread to continuously|
    | send a keepalive packet every 500ms                   |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 1;
    keepalive_thread     = new std::thread(&DRGBController::KeepaliveThread, this);
}

DRGBController::~DRGBController()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;
    hid_close(dev);
}

void DRGBController::KeepaliveThread()
{
    unsigned char   sleep_buf[65];
    sleep_buf[0] = 0x65;
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::seconds(1))
        {
            SendPacketFS(sleep_buf, 1, 0);
        }
        std::this_thread::sleep_for(500ms);
    }
}

std::string DRGBController::GetFirmwareString()
{
    return "v"+std::to_string(version[0]) + "." + std::to_string(version[1]) + "." + std::to_string(version[2]) + "." + std::to_string(version[3]);
}

std::string DRGBController::GetLocationString()
{
    return("HID: " + location);
}

std::string DRGBController::GetNameString()
{
    return(name);
}

std::string DRGBController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned short DRGBController::GetDevicePID()
{
    return(device_pid);
}

void DRGBController::SetChannelLEDs(unsigned char /*channel*/, RGBColor* /*colors*/, unsigned int /*num_colors*/)
{

}

void DRGBController::SendPacket(unsigned char* colors, unsigned int buf_packets , unsigned int LEDtotal)
{
    unsigned char   usb_buf[1025];
    unsigned int    buf_idx = 0;
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]   = 0x00;
    unsigned int    HigCount = LEDtotal / 256 >= 1 ? 1 : 0;
    unsigned int    LowCount = LEDtotal >= DRGB_V4_ONE_PACKAGE_SIZE ? 60 : (LEDtotal % 256) ;
    LEDtotal = LEDtotal <= DRGB_V4_ONE_PACKAGE_SIZE ? 0 : (LEDtotal-DRGB_V4_ONE_PACKAGE_SIZE);
    for(unsigned int i = 0; i < buf_packets; i++)
    {
        usb_buf[1]  = i + 100 ;
        usb_buf[2]  = buf_packets + 99 ;
        usb_buf[3]  = HigCount;
        usb_buf[4]  = LowCount;
        buf_idx     = i*1020;
        for(unsigned int k=0;k<1020;k++)
        {
            usb_buf[k+5] = colors[buf_idx + k];
        }
        hid_write(dev, usb_buf, 1025);
        if(LEDtotal)
        {
            HigCount    = LEDtotal / 256 >= 1 ? 1 : 0;
            LowCount    = LEDtotal >= DRGB_V4_PACKAGE_SIZE ? 84 : (LEDtotal % 256) ;
            LEDtotal    = LEDtotal <= DRGB_V4_PACKAGE_SIZE ? 0 : (LEDtotal-DRGB_V4_PACKAGE_SIZE);
        }
    }
}

void DRGBController::SendPacketFS(unsigned char* colors, unsigned int buf_packets , unsigned int Array)
{
    unsigned char   usb_buf[65];
    unsigned int    buf_idx = 0;
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]   = 0x00;
    if(Array == 0x64)
    {
        for(unsigned int i = 0; i < buf_packets; i++)
        {
            usb_buf[1]  = i == buf_packets - 1 ? Array + 100 + i : Array + i;
            buf_idx     = i*63;
            for(unsigned int k=0;k<63;k++)
            {
                usb_buf[k+2] = colors[buf_idx + k];
            }
            hid_write(dev, usb_buf, 65);
        }
    }
    else if(Array == 0x47)
    {
        for(unsigned int i = 0; i < buf_packets; i++)
        {
            usb_buf[1]  = i == buf_packets - 1 ? Array + 92 + i : Array + i;
            buf_idx     = i*63;
            for(unsigned int k=0;k<63;k++)
            {
                usb_buf[k+2] = colors[buf_idx + k];
            }
            hid_write(dev, usb_buf, 65);
        }
    }
    else
    {
        for(unsigned int e=0;e<64;e++)
        {
            usb_buf[e+1] = colors[e];
        }
        hid_write(dev, usb_buf, 65);
    }
}
