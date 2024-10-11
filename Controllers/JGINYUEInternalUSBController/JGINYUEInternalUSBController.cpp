/*---------------------------------------------------------*\
| JGINYUEInternalUSBController.cpp                          |
|                                                           |
|   Driver for JGINYUE USB motherboard                      |
|                                                           |
|   Tong R (tcr020)                             09 Aug 2023 |
|   Liu ShiMeng(Moon dream stars)               09 Aug 2023 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <string>
#include "RGBController.h"
#include "JGINYUEInternalUSBController.h"
#include "dmiinfo.h"

#define JGINYUE_USB_GENERAL_COMMAND_HEADER              0x01
#define JGINYUE_USB_LED_STRIPE_SET_COMMAND_HEADER       0x05
#define JGINYUE_USB_MODE_SET_COMMAND_HEADER             0x06
#define JGINYUE_USB_PER_LED_SET_COMMAND_HEADER          0x04

#define JGINYUE_USB_GET_FW_VERSION                      0xA0
#define JGINYUE_USB_GET_FW_REPLY                        0x5A
#define JGINYUE_RG_DEFAULT                              0x01
#define JGINYUE_RG_SWAP                                 0x00

using namespace std::chrono_literals;

JGINYUEInternalUSBController::JGINYUEInternalUSBController(hid_device* dev_handle, const char* path)
{
    DMIInfo         dmi;

    dev           = dev_handle;
    location      = path;
    device_name   = "JGINYUE " + dmi.getMainboard();

    memset(&device_config, 0x00, sizeof(device_config));

    Init_device(device_config);
}

JGINYUEInternalUSBController::~JGINYUEInternalUSBController()
{
    hid_close(dev);
}

unsigned int JGINYUEInternalUSBController::GetZoneCount()
{
    return(JGINYUE_MAX_ZONES);
}

std::string JGINYUEInternalUSBController::GetDeviceLocation()
{
    return("HID:" + location);
}

std::string JGINYUEInternalUSBController::GetDeviceName()
{
    return(device_name);
}

std::string JGINYUEInternalUSBController::GetSerialString()
{
    return("");
}

std::string JGINYUEInternalUSBController::GetDeviceFWVersion()
{
    unsigned char   usb_buf[16];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]  = JGINYUE_USB_GENERAL_COMMAND_HEADER;
    usb_buf[0x01]  = JGINYUE_USB_GET_FW_VERSION;

    hid_write(dev, usb_buf, 16);
    hid_read(dev, usb_buf, 16);

    if((usb_buf[0x00] != JGINYUE_USB_GENERAL_COMMAND_HEADER) || (usb_buf[0x01] != JGINYUE_USB_GET_FW_REPLY))
    {
        return("");
    }

    std::string Major_version = std::to_string(usb_buf[0x02]);
    std::string Minor_version = std::to_string(usb_buf[0x03]);

    return(Major_version + "." + Minor_version);
}

void JGINYUEInternalUSBController::Init_device(AreaConfiguration* ptr_device_cfg)
{
    for(int index_config = 1; index_config <= JGINYUE_MAX_ZONES; index_config++)
    {
        ptr_device_cfg[index_config].Brightness     = JGINYUE_USB_BRIGHTNESS_DEFAULT;
        ptr_device_cfg[index_config].Color_B        = 0xFF;
        ptr_device_cfg[index_config].Color_G        = 0xFF;
        ptr_device_cfg[index_config].Color_R        = 0xFF;
        ptr_device_cfg[index_config].RG_Swap        = JGINYUE_RG_DEFAULT;
        ptr_device_cfg[index_config].Speed          = JGINYUE_USB_SPEED_DEFAULT;
        ptr_device_cfg[index_config].LED_numbers    = 0;
        ptr_device_cfg[index_config].Mode_active    = JGINYUE_USB_MODE_STATIC;
    }
}

void JGINYUEInternalUSBController::WriteZoneMode
    (
    unsigned char   zone,
    unsigned char   mode,
    RGBColor        rgb,
    unsigned char   speed,
    unsigned char   brightness,
    unsigned char   direction
    )
{
    int Active_zone;
    unsigned char usb_buf[65];

    switch(zone)
    {
    case 0x01:
        Active_zone = 1;
        break;
    case 0x02:
        Active_zone = 2;
        break;
    default:
        Active_zone = 1;
        return;
        break;
    }

    device_config[Active_zone].Mode_active          = mode;
    device_config[Active_zone].Direct_Mode_control  = 0x00;
    device_config[Active_zone].Speed                = speed;
    device_config[Active_zone].Brightness           = brightness;
    device_config[Active_zone].Direction            = direction;
    device_config[Active_zone].Color_B              = RGBGetBValue(rgb);
    device_config[Active_zone].Color_G              = RGBGetGValue(rgb);
    device_config[Active_zone].Color_R              = RGBGetRValue(rgb);

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = JGINYUE_USB_LED_STRIPE_SET_COMMAND_HEADER;
    usb_buf[0x01]   = zone;
    usb_buf[0x02]   = device_config[Active_zone].LED_numbers;
    usb_buf[0x03]   = device_config[Active_zone].RG_Swap;
    usb_buf[0x04]   = device_config[Active_zone].Direction;
    usb_buf[0x05]   = device_config[Active_zone].Direct_Mode_control;

    hid_write(dev, usb_buf, 16);

    std::this_thread::sleep_for(20ms);

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = JGINYUE_USB_MODE_SET_COMMAND_HEADER;
    usb_buf[0x01]   = zone;
    usb_buf[0x02]   = device_config[Active_zone].Mode_active;
    usb_buf[0x03]   = device_config[Active_zone].Color_R;
    usb_buf[0x04]   = device_config[Active_zone].Color_G;
    usb_buf[0x05]   = device_config[Active_zone].Color_B;
    usb_buf[0x06]   = device_config[Active_zone].Brightness;
    usb_buf[0x07]   = device_config[Active_zone].Speed;

    hid_write(dev, usb_buf, 16);
}

void JGINYUEInternalUSBController::DirectLEDControl
    (
    RGBColor*              colors,
    unsigned char          zone
    )
{
    int Active_zone;
    unsigned char usb_buf[302];

    switch(zone)
    {
    case 0x01:
        Active_zone = 1;
        break;
    case 0x02:
        Active_zone = 2;
        break;
    default:
        Active_zone = 1;
        return;
        break;
    }

    device_config[Active_zone].Mode_active          =JGINYUE_USB_MODE_DIRECT;
    device_config[Active_zone].Direct_Mode_control  =0x01;

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = JGINYUE_USB_LED_STRIPE_SET_COMMAND_HEADER;
    usb_buf[0x01]   = zone;
    usb_buf[0x02]   = device_config[Active_zone].LED_numbers;
    usb_buf[0x03]   = device_config[Active_zone].RG_Swap;
    usb_buf[0x04]   = device_config[Active_zone].Direction;
    usb_buf[0x05]   = device_config[Active_zone].Direct_Mode_control;

    hid_write(dev, usb_buf, 16);

    std::this_thread::sleep_for(20ms);

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = JGINYUE_USB_PER_LED_SET_COMMAND_HEADER;
    usb_buf[0x01]   = zone;

    for(unsigned int color_idx = 0; color_idx < device_config[Active_zone].LED_numbers; color_idx++)
    {
        usb_buf[color_idx * 3 + 2]  = RGBGetRValue(colors[color_idx]);
        usb_buf[color_idx * 3 + 3]  = RGBGetGValue(colors[color_idx]);
        usb_buf[color_idx * 3 + 4]  = RGBGetBValue(colors[color_idx]);
    }

    hid_send_feature_report(dev, usb_buf, 302);
}

void JGINYUEInternalUSBController::Area_resize(unsigned char led_numbers, unsigned char zone)
{
    unsigned char usb_buf[65];
    int Active_zone;

    switch(zone)
    {
    case 0x01:
        Active_zone = 1;
        break;
    case 0x02:
        Active_zone = 2;
        break;
    default:
        Active_zone = 1;
        return;
        break;
    }

    device_config[Active_zone].LED_numbers = led_numbers;

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = JGINYUE_USB_LED_STRIPE_SET_COMMAND_HEADER;
    usb_buf[0x01]   = zone;
    usb_buf[0x02]   = device_config[Active_zone].LED_numbers;
    usb_buf[0x03]   = device_config[Active_zone].RG_Swap;
    usb_buf[0x04]   = device_config[Active_zone].Direction;
    usb_buf[0x05]   = device_config[Active_zone].Direct_Mode_control;

    hid_write(dev, usb_buf, 16);
}

void JGINYUEInternalUSBController::SetRGSwap(unsigned char RGSwap)
{
    if((RGSwap != 0x00) && (RGSwap != 0x01))
    {
        return;
    }

    for(int index_config=1; index_config <=JGINYUE_MAX_ZONES; index_config++)
    {
        device_config[index_config].RG_Swap = RGSwap;
    }
}
