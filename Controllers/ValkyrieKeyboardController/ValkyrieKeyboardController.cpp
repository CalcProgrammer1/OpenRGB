/*---------------------------------------------------------*\
| ValkyrieKeyboardController.cpp                            |
|                                                           |
|   Driver for Valkyrie keyboard                            |
|                                                           |
|   Nollie (Nuonuo)                             06 Dec 2023 |
|   Bartholomew Ho (imnotmental)                01 Feb 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "StringUtils.h"
#include "ValkyrieKeyboardController.h"

ValkyrieKeyboardController::ValkyrieKeyboardController(hid_device* dev_handle, const char* path, const unsigned short pid, const int interface, std::string dev_name)
{
    dev             = dev_handle;
    location        = path;
    name            = dev_name;
    usb_pid         = pid;
    interface_num   = interface;
}

ValkyrieKeyboardController::~ValkyrieKeyboardController()
{
    hid_close(dev);
}

std::string ValkyrieKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ValkyrieKeyboardController::GetNameString()
{
    return(name);
}

std::string ValkyrieKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned short ValkyrieKeyboardController::GetUSBPID()
{
    return(usb_pid);
}

int ValkyrieKeyboardController::GetInterfaceNum()
{
    return(interface_num);
}

void ValkyrieKeyboardController::SendColors(unsigned char* color_data, unsigned int /*color_data_size*/)
{
    unsigned char usb_buf_pro[392];
    unsigned char usb_buf_normal[408];
    int led_num = 0;

    switch(interface_num)
    {
    case 3:
        led_num = 98;
        for(int i = 0; i < led_num; i++)
        {
            usb_buf_pro[i * 4]     = key_code_99[i];
            usb_buf_pro[i * 4 + 1] = color_data[i * 3];
            usb_buf_pro[i * 4 + 2] = color_data[i * 3 + 1];
            usb_buf_pro[i * 4 + 3] = color_data[i * 3 + 2];
        }
        break;
    default:
        led_num = 102;
        for(int i = 0; i < led_num; i++)
        {
            usb_buf_normal[i * 4]     = key_code_103[i];
            usb_buf_normal[i * 4 + 1] = color_data[i * 3];
            usb_buf_normal[i * 4 + 2] = color_data[i * 3 + 1];
            usb_buf_normal[i * 4 + 3] = color_data[i * 3 + 2];
        }
    }

    SendInitializeColorPacket();

    for(int i = 0; i <= 6; i++)
    {
        unsigned int    usb_data_num = 16;
        if(i == 6)
        {
             usb_data_num = led_num - usb_data_num * 6;
        }
        char send_usb_buf[65];
        memset(send_usb_buf, 0x00, sizeof(send_usb_buf));

        switch(interface_num)
        {
            case 3:
                for(unsigned int index = 0; index < usb_data_num; index++)
                {
                    send_usb_buf[index * 4 + 1] = usb_buf_pro[index * 4 + i * 64    ];
                    send_usb_buf[index * 4 + 2] = usb_buf_pro[index * 4 + i * 64 + 1];
                    send_usb_buf[index * 4 + 3] = usb_buf_pro[index * 4 + i * 64 + 2];
                    send_usb_buf[index * 4 + 4] = usb_buf_pro[index * 4 + i * 64 + 3];
                }
                break;
            default:
                for(unsigned int index = 0; index < usb_data_num; index++)
                {
                    send_usb_buf[index * 4 + 1] = usb_buf_normal[index * 4 + i * 64    ];
                    send_usb_buf[index * 4 + 2] = usb_buf_normal[index * 4 + i * 64 + 1];
                    send_usb_buf[index * 4 + 3] = usb_buf_normal[index * 4 + i * 64 + 2];
                    send_usb_buf[index * 4 + 4] = usb_buf_normal[index * 4 + i * 64 + 3];
                }
                break;
        }

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_send_feature_report(dev, (unsigned char *)send_usb_buf, 65);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    SendTerminateColorPacket();
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
}

void ValkyrieKeyboardController::SendInitializeColorPacket()
{
    uint8_t usb_write_buf[65];
    uint8_t usb_read_buf[65];
    memset(usb_write_buf, 0x00, sizeof(usb_write_buf));
    memset(usb_read_buf, 0x00, sizeof(usb_read_buf));
    usb_write_buf[1]   = 0x04;
    usb_write_buf[2]   = 0x20;
    usb_write_buf[9]   = 0x08;
    hid_send_feature_report(dev, (unsigned char *)usb_write_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    hid_get_feature_report (dev, (unsigned char *)usb_read_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void ValkyrieKeyboardController::SendTerminateColorPacket()
{
    uint8_t usb_write_buf[65];
    uint8_t usb_read_buf[65];
    memset(usb_write_buf, 0x00, sizeof(usb_write_buf));
    memset(usb_read_buf, 0x00, sizeof(usb_read_buf));
    hid_send_feature_report(dev, (unsigned char *)usb_write_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    usb_write_buf[1]   = 0x04;
    usb_write_buf[2]   = 0x02;
    hid_send_feature_report(dev, (unsigned char *)usb_write_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    hid_get_feature_report (dev, (unsigned char *)usb_read_buf, 65);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
