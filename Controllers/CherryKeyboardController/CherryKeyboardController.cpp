/*---------------------------------------------------------*\
| CherryKeyboardController.cpp                              |
|                                                           |
|   Driver for Cherry keyboard                              |
|                                                           |
|   Sebastian Kraus                             25 Dec 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CherryKeyboardController.h"
#include "StringUtils.h"

CherryKeyboardController::CherryKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

CherryKeyboardController::~CherryKeyboardController()
{
    hid_close(dev);
}

std::string CherryKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CherryKeyboardController::GetDeviceName()
{
    return(name);
}

std::string CherryKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void CherryKeyboardController::SetKeyboardColors
    (
    unsigned char *     color_data,
    unsigned int        size
    )
{
    unsigned int    packet_size = 0;
    unsigned int    packet_offset = 0;

    while(size > 0)
    {
        if(size >= CHERRY_KB_MAX_PACKET_SIZE)
        {
            packet_size     = CHERRY_KB_MAX_PACKET_SIZE;
        }
        else
        {
            packet_size     = size;
        }

        SendKeyboardData
            (
            &color_data[packet_offset],
            packet_size,
            packet_offset
            );

        size           -= packet_size;
        packet_offset  += packet_size;
    }
}

void CherryKeyboardController::SendKeyboardMode
    (
    unsigned char       mode
    )
{
    SendKeyboardParameter(CHERRY_KB_PARAMETER_MODE, 1, &mode);
}

void CherryKeyboardController::SendKeyboardModeEx
    (
    unsigned char       mode,
    unsigned char       brightness,
    unsigned char       speed,
    unsigned char       direction,
    unsigned char       random_flag,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char parameter_data[9];

    parameter_data[0]   = 0x0;
    parameter_data[1]   = mode;
    parameter_data[2]   = brightness;
    parameter_data[3]   = speed;
    parameter_data[4]   = direction;
    parameter_data[5]   = random_flag;
    parameter_data[6]   = red;
    parameter_data[7]   = green;
    parameter_data[8]   = blue;

    SendKeyboardParameter(0, 9, parameter_data);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void CherryKeyboardController::ComputeChecksum
    (
    char                usb_buf[CHERRY_KB_PACKET_SIZE]
    )
{
    unsigned short checksum = 0;

    for(unsigned int byte_idx = 0x03; byte_idx < CHERRY_KB_PACKET_SIZE; byte_idx++)
    {
        checksum += usb_buf[byte_idx];
    }

    usb_buf[0x01] = checksum & 0xFF;
    usb_buf[0x02] = checksum >> 8;
}

void CherryKeyboardController::SendKeyboardBegin()
{
    unsigned char usb_buf[CHERRY_KB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Begin (0x01) packet                   |
    |   Note: Not computing checksum as packet contents are |
    |         fixed                                         |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = CHERRY_KB_COMMAND_BEGIN;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = CHERRY_KB_COMMAND_BEGIN;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, CHERRY_KB_PACKET_SIZE);
    hid_read(dev, usb_buf, CHERRY_KB_PACKET_SIZE);
}

void CherryKeyboardController::SendKeyboardEnd()
{
    char usb_buf[CHERRY_KB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard End (0x02) packet                     |
    |   Note: Not computing checksum as packet contents are |
    |         fixed                                         |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x01]           = CHERRY_KB_COMMAND_END;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = CHERRY_KB_COMMAND_END;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, CHERRY_KB_PACKET_SIZE);
    hid_read(dev, (unsigned char *)usb_buf, CHERRY_KB_PACKET_SIZE);
}

void CherryKeyboardController::SendKeyboardData
    (
    unsigned char *     data,
    unsigned char       data_size,
    unsigned short      data_offset
    )
{
    char usb_buf[CHERRY_KB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Color Data (0x0B) packet              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x03]           = CHERRY_KB_COMMAND_WRITE_CUSTOM_COLOR_DATA;

    usb_buf[0x04]           = data_size;
    usb_buf[0x05]           = data_offset & 0x00FF;
    usb_buf[0x06]           = data_offset >> 8;

    /*-----------------------------------------------------*\
    | Copy in data bytes                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x08], data, data_size);

    /*-----------------------------------------------------*\
    | Compute Checksum                                      |
    \*-----------------------------------------------------*/
    ComputeChecksum(usb_buf);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, CHERRY_KB_PACKET_SIZE);
    hid_read(dev, (unsigned char *)usb_buf, CHERRY_KB_PACKET_SIZE);
}

void CherryKeyboardController::SendKeyboardParameter
    (
    unsigned char       parameter,
    unsigned char       parameter_size,
    unsigned char*      parameter_data
    )
{
    char usb_buf[CHERRY_KB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Parameter (0x08) packet               |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x03]           = CHERRY_KB_COMMAND_SET_PARAMETER;
    usb_buf[0x04]           = parameter_size;
    usb_buf[0x05]           = parameter;
    usb_buf[0x07]           = 0x55;

    /*-----------------------------------------------------*\
    | Copy in data bytes                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x08], parameter_data, parameter_size);

    /*-----------------------------------------------------*\
    | Compute Checksum                                      |
    \*-----------------------------------------------------*/
    ComputeChecksum(usb_buf);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, CHERRY_KB_PACKET_SIZE);
    hid_read(dev, (unsigned char *)usb_buf, CHERRY_KB_PACKET_SIZE);
}
