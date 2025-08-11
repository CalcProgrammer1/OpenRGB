/*---------------------------------------------------------*\
| IonicoController.cpp                                      |
|                                                           |
|   Driver for Ionico-II-17                                 |
|                                                           |
|   Lucas Strafe                                31 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string.h>
#include "IonicoController.h"

IonicoController::IonicoController(hid_device* dev_handle, const hid_device_info& info, const unsigned short pid, std::string dev_name)
{
    dev                 = dev_handle;
    location            = info.path;
    usb_pid             = pid;
    name                = dev_name;
}

IonicoController::~IonicoController()
{
    hid_close(dev);
}

std::string IonicoController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string IonicoController::GetDeviceName()
{
    return(name);
}

uint16_t IonicoController::GetUSBPID()
{
    return(usb_pid);
}

void IonicoController::TurnOff()
{
    uint8_t usb_buf[IONICO_REPORT_SIZE];
    memset(usb_buf, 0x00, IONICO_REPORT_SIZE);
    usb_buf[1] = 0x09;
    usb_buf[2] = 0x02;
    hid_send_feature_report(dev, usb_buf, IONICO_REPORT_SIZE);
}

void IonicoController::SaveBios()
{
    uint8_t usb_buf[IONICO_REPORT_SIZE];
    memset(usb_buf, 0x00, IONICO_REPORT_SIZE);
    usb_buf[1] = 0x1A;
    usb_buf[3] = 0x01;
    usb_buf[4] = 0x04;
    usb_buf[8] = 0x01;
    hid_send_feature_report(dev, usb_buf, IONICO_REPORT_SIZE);
}

void IonicoController::SetMode(uint8_t mode_value, uint8_t brightness, uint8_t speed)
{
    uint8_t usb_buf[IONICO_REPORT_SIZE];
    memset(usb_buf, 0x00, IONICO_REPORT_SIZE);
    usb_buf[1] = 0x08;
    usb_buf[2] = 0x02;
    usb_buf[3] = mode_value;
    usb_buf[4] = speed;
    usb_buf[5] = brightness;
    usb_buf[6] = 0x08;
    hid_send_feature_report(dev, usb_buf, IONICO_REPORT_SIZE);
}

void IonicoController::SetColors(int device, std::vector<RGBColor> array_colors, bool is_mode)
{
    /*---------------------------------------------------------*\
    |                   Direct mode and effects                 |
    \*---------------------------------------------------------*/
    if(device == DEVICE_TYPE_KEYBOARD || (device == DEVICE_TYPE_LEDSTRIP && is_mode))
    {
        uint8_t usb_buf[IONICO_REPORT_SIZE];
        memset(usb_buf, 0x00, IONICO_REPORT_SIZE);
        for(size_t i = 0; i < array_colors.size(); i++)
        {
            usb_buf[1] = IONICO_DIRECT_CMD;
            usb_buf[3] = (uint8_t)(i + 1);
            usb_buf[4] = RGBGetRValue(array_colors[i]);
            usb_buf[5] = RGBGetGValue(array_colors[i]);
            usb_buf[6] = RGBGetBValue(array_colors[i]);
            hid_send_feature_report(dev, usb_buf, IONICO_REPORT_SIZE);
        }
    }
    /*---------------------------------------------------------*\
    |                 LIGHT BAR LED PER LED                     |
    \*---------------------------------------------------------*/
    else if(device == DEVICE_TYPE_LEDSTRIP && !is_mode)
    {
        uint8_t usb_buf[IONICO_REPORT_SIZE];
        memset(usb_buf, 0x00, IONICO_REPORT_SIZE);
        usb_buf[1] = 0x12;
        hid_send_feature_report(dev, usb_buf, IONICO_REPORT_SIZE);

        uint8_t usb_buf_led[IONICO_DIRECT_REPORT_SIZE];
        memset(usb_buf_led, 0x00, IONICO_DIRECT_REPORT_SIZE);

        for(size_t i = 0; i < array_colors.size(); i++)
        {
           usb_buf_led[1 + 3 * i] = RGBGetRValue(array_colors[i]);
           usb_buf_led[2 + 3 * i] = RGBGetBValue(array_colors[i]);
           usb_buf_led[3 + 3 * i] = RGBGetGValue(array_colors[i]);
        }
        hid_write(dev, usb_buf_led, IONICO_DIRECT_REPORT_SIZE);

        memset(usb_buf, 0x00, IONICO_REPORT_SIZE);
        usb_buf[1] = 0x12;
        usb_buf[3] = 0x01;
        hid_send_feature_report(dev, usb_buf, IONICO_REPORT_SIZE);
    }
}
