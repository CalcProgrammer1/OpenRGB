/*---------------------------------------------------------*\
| AnnePro2Controller.cpp                                    |
|                                                           |
|   Driver for Obins Lab AnnePro2 keyboard                  |
|                                                           |
|   Sergey Gavrilov (DrZlo13)                   06 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "AnnePro2Controller.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

AnnePro2Controller::AnnePro2Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AnnePro2Controller::~AnnePro2Controller()
{
    hid_close(dev);
}

std::string AnnePro2Controller::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AnnePro2Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void AnnePro2Controller::SendDirect(unsigned char frame_count, unsigned char * frame_data)
{
    /*-------------------------------------------------------------*\
    | Reverse engineered by https://github.com/manualmanul/Annemone |
    \*-------------------------------------------------------------*/

    const unsigned char hid_cmd_service_data_length                             = 4;
    const unsigned char hid_cmd_service_data[hid_cmd_service_data_length]       = {0, 123, 16, 65};

    const unsigned char hid_cmd_static_message_length                           = 3;
    const unsigned char hid_cmd_static_message[hid_cmd_static_message_length]   = {0, 0, 125};

    const unsigned char hid_cmd_command_info_length                             = 3;
    const unsigned char hid_cmd_command_info[hid_cmd_command_info_length]       = {32, 3, 255};

    const unsigned char real_command_info_length                                = hid_cmd_command_info_length + 1;
    const unsigned char max_hid_length                                          = 64;

    const unsigned char max_command_length                                      = max_hid_length - hid_cmd_service_data_length - 2 - hid_cmd_static_message_length;
    const unsigned char max_message_length                                      = max_command_length - real_command_info_length;
    const unsigned char messages_to_send_amount                                 = frame_count / max_message_length + 1;
    const unsigned char val_1                                                   = frame_count % max_message_length;
    const unsigned char val_2                                                   = (0 == val_1) ? max_message_length : val_1;

    unsigned char hid_command[max_hid_length];

    unsigned char led_data = 0;

    for(unsigned char p = 0; p < messages_to_send_amount; p++)
    {
        const unsigned char e = (messages_to_send_amount << 4) + p;
        const unsigned char a = ((messages_to_send_amount - 1) == p) ? val_2 + real_command_info_length : max_message_length + real_command_info_length;

        /*---------------------------------------------------------*\
        | Service data                                              |
        \*---------------------------------------------------------*/
        hid_command[0] = hid_cmd_service_data[0];
        hid_command[1] = hid_cmd_service_data[1];
        hid_command[2] = hid_cmd_service_data[2];
        hid_command[3] = hid_cmd_service_data[3];

        hid_command[4] = e;
        hid_command[5] = a;

        /*---------------------------------------------------------*\
        | Static message                                            |
        \*---------------------------------------------------------*/
        hid_command[6] = hid_cmd_static_message[0];
        hid_command[7] = hid_cmd_static_message[1];
        hid_command[8] = hid_cmd_static_message[2];

        /*---------------------------------------------------------*\
        | Command info                                              |
        \*---------------------------------------------------------*/
        hid_command[9] = hid_cmd_command_info[0];
        hid_command[10] = hid_cmd_command_info[1];
        hid_command[11] = hid_cmd_command_info[2];

        hid_command[12] = 2;

        /*---------------------------------------------------------*\
        | LED data                                                  |
        \*---------------------------------------------------------*/
        for(uint8_t i = 0; i < max_message_length; i++)
        {
            hid_command[13 + i] = frame_data[led_data];
            led_data++;
        }

        hid_write(dev, hid_command, max_hid_length);

        /*---------------------------------------------------------*\
        | Needed due to Anne Pro 2 ignoring commands when they're   |
        | sent faster than 50ms apart from each other.              |
        \*---------------------------------------------------------*/
        std::this_thread::sleep_for(50ms);
    }
}
