/*---------------------------------------------------------*\
| JGINYUEInternalUSBV2Controller.cpp                        |
|                                                           |
|   Driver for JGINYUE USB motherboard V2                   |
|                                                           |
|   Tong R (tcr020)                             08 Aug 2024 |
|   Liu ShiMeng(Moon dream stars)               09 Aug 2023 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <string>
#include <stdio.h>
#include "RGBController.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "JGINYUEInternalUSBV2Controller.h"

#include "dmiinfo.h"

#define JGINYUE_V2_HID_GENERAL_COMMAND_HEADER              0x01
#define JGINYUE_V2_CDC_COMMAND_HEADER                  0x10

#define JGINYUE_V2_HID_REQUEST_MCU_STATUS 0x21
#define JGINYUE_V2_HID_REQUEST_MCUID 0x22
#define JGINYUE_V2_HID_REQUEST_UNLOCK 0x23
#define JGINYUE_V2_HID_DOWNLOAD_SKU 0x24
#define JGINYUE_V2_HID_REQUEST_MCU_STATUS_EX 0x0F
#define JGINYUE_V2_HID_DOWNLOAD_ARGB_SETTING 0x01
#define JGINYUE_V2_HID_DOWNLOAD_GLOBAL_SETTING 0x02
#define JGINYUE_V2_HID_REQUEST_ARGB_SETTING 0x11
#define JGINYUE_V2_HID_REQUEST_GLOBAL_SETTING 0x12



#define FUNCTION_ID_CDC_ARGB 0x01

using namespace std::chrono_literals;

JGINYUEInternalUSBV2Controller::JGINYUEInternalUSBV2Controller(hid_device* jy_hid_device, const char* path,serial_port* jy_cdc_device)
{
    DMIInfo                     dmi;
    jy_hid_interface            = jy_hid_device;
    jy_cdc_interface            = jy_cdc_device;
    location                    = path;
    device_name                 = "JGINYUE " + dmi.getMainboard() + " Internal USB Controller V2";
    ZoneCount                   = 0;
    support_Global_zone         = false;
    Init_device();
}

JGINYUEInternalUSBV2Controller::~JGINYUEInternalUSBV2Controller()
{
    hid_close(jy_hid_interface);
    delete jy_cdc_interface;

}

unsigned int JGINYUEInternalUSBV2Controller::GetZoneCount()
{
    return(ZoneCount);
}

std::string JGINYUEInternalUSBV2Controller::GetDeviceLocation()
{
    return("HID:" + location);
}

std::string JGINYUEInternalUSBV2Controller::GetDeviceName()
{
    return(device_name);
}

std::string JGINYUEInternalUSBV2Controller::GetSerialString()
{
    return("");
}

std::string JGINYUEInternalUSBV2Controller::GetDeviceFWVersion()
{
    return("");
}

void JGINYUEInternalUSBV2Controller::Init_device()
{
    unsigned char   usb_buf[64];

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0] = JGINYUE_V2_HID_GENERAL_COMMAND_HEADER;
    usb_buf[1] = 0x0F;
    hid_write(jy_hid_interface, usb_buf, 64);
    std::this_thread::sleep_for(20ms);
    hid_read(jy_hid_interface, usb_buf, 64);
    if(usb_buf[1] != 0x0F)
    {
        ZoneCount = 0x00;
        memset(device_config, 0x00, sizeof(device_config));
        memset (&device_config_Global, 0x00, sizeof(device_config_Global));
        return;
    }
    unsigned char Zone_Info = usb_buf[4];
    for(unsigned char i = 0; i < 8; i ++)
    {
        if(Zone_Info & (1<<i))
        {
            device_config[ZoneCount].Area_ID = (1<<i);
            device_config[ZoneCount].Max_LED_numbers = usb_buf[5+i];
            Init_Zone(ZoneCount);
            ZoneCount++;
        }
    }
    if(ZoneCount == 0)
    {
        return;
    }
    if(support_Global_zone == true)
    {
        ZoneCount++;
        //TODO: Get Global zone information
    }

}

void JGINYUEInternalUSBV2Controller::Init_Zone(int zone)
{
    unsigned char   usb_buf[64];
    unsigned char   Area_ID = device_config[zone].Area_ID;

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0] = JGINYUE_V2_HID_GENERAL_COMMAND_HEADER;
    usb_buf[1] = JGINYUE_V2_HID_REQUEST_ARGB_SETTING;
    usb_buf[2] = Area_ID;

    hid_write(jy_hid_interface, usb_buf, 64);
    std::this_thread::sleep_for(20ms);

    hid_read(jy_hid_interface, usb_buf, 64);

    if((usb_buf[1] != JGINYUE_V2_HID_REQUEST_ARGB_SETTING)|(usb_buf[2] != Area_ID))
    {
        return;
    }

    device_config[zone].Mode_active = usb_buf[3];
    device_config[zone].User_LED_numbers = usb_buf[4];
    unsigned char Color_num = ((usb_buf[5] &0x0F)<8 ? (usb_buf[5] &0x0F): 8);
    device_config[zone].Color_num = Color_num;
    device_config[zone].Direction = (usb_buf[5]&0x10)>>4;
    device_config[zone].Speed = usb_buf[6];
    device_config[zone].Brightness = usb_buf[7];
    memcpy(&(device_config[zone].Color_Array[0]),&(usb_buf[8]),Color_num*3);
    return;
}

void JGINYUEInternalUSBV2Controller::WriteZoneMode
    (
    unsigned char   Area,
    unsigned char   Mode,
    unsigned char   Num_LED,
    std::vector<RGBColor> rgb,
    unsigned char   Speed,
    unsigned char   Brightness,
    unsigned char   Direction
    )
{
    unsigned char   usb_buf[64];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    unsigned char   num_color = (unsigned char)rgb.size();
    num_color = (num_color < 8) ? num_color : 8;

    usb_buf[0] = JGINYUE_V2_HID_GENERAL_COMMAND_HEADER;
    usb_buf[1] = JGINYUE_V2_HID_DOWNLOAD_ARGB_SETTING;
    usb_buf[2] = Area;
    usb_buf[3] = Mode;
    usb_buf[4] = Num_LED;
    usb_buf[5] = (num_color&0x0F)|((Direction&0x01)<<4);
    usb_buf[6] = Speed;
    usb_buf[7] = Brightness;

    for(unsigned char i = 0; i < num_color; i++)
    {
        RGBColor color = rgb[i];
        usb_buf[8+i*3] = RGBGetRValue(color);
        usb_buf[9+i*3] = RGBGetGValue(color);
        usb_buf[10+i*3] = RGBGetBValue(color);
    }

    hid_write(jy_hid_interface, usb_buf, 64);
    std::this_thread::sleep_for(10ms);
}

void JGINYUEInternalUSBV2Controller::DirectLEDControl
    (
    RGBColor*              colors,
    unsigned char          num_LEDs,
    unsigned char          Area
    )
{
    unsigned char cdc_buf[512];
    memset(cdc_buf, 0x00, sizeof(cdc_buf));
    cdc_buf[0] = JGINYUE_V2_CDC_COMMAND_HEADER;
    cdc_buf[1] = FUNCTION_ID_CDC_ARGB;
    cdc_buf[2] = Area;
    cdc_buf[3] = 0x20;
    cdc_buf[4] = num_LEDs;

    for(unsigned char i = 0; i < num_LEDs; i++)
    {
        cdc_buf[8+i*3] = RGBGetRValue(colors[i]);
        cdc_buf[9+i*3] = RGBGetGValue(colors[i]);
        cdc_buf[10+i*3] = RGBGetBValue(colors[i]);
    }
    int TX_len = 10 + num_LEDs*3;
    if(TX_len%64 == 0)
    {
        TX_len = TX_len+2;
    }

    jy_cdc_interface->serial_write((char*)cdc_buf,TX_len);
}
