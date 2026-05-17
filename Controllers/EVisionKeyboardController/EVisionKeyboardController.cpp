/*---------------------------------------------------------*\
| EVisionKeyboardController.cpp                             |
|                                                           |
|   Driver for EVision keyboard (Redragon, Glorious, Ajazz, |
|   Tecware, and many other brands)                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "EVisionKeyboardController.h"
#include "StringUtils.h"

EVisionKeyboardController::EVisionKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

EVisionKeyboardController::~EVisionKeyboardController()
{
    hid_close(dev);
}

std::string EVisionKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string EVisionKeyboardController::GetNameString()
{
    return(name);
}

std::string EVisionKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void EVisionKeyboardController::SetKeyboardColors
    (
    unsigned char *     color_data,
    unsigned int        size
    )
{
    unsigned int    packet_size = 0;
    unsigned int    packet_offset = 0;

    while(size > 0)
    {
        if(size >= EVISION_KB_MAX_PACKET_SIZE)
        {
            packet_size     = EVISION_KB_MAX_PACKET_SIZE;
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

void EVisionKeyboardController::SendKeyboardMode
    (
    unsigned char       mode
    )
{
    SendKeyboardParameter(EVISION_KB_PARAMETER_MODE, 1, &mode);
}

void EVisionKeyboardController::SendKeyboardModeEx
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
    /*-----------------------------------------------------*\
    | Build 27-byte compound parameter (param_id = 0x00)   |
    | as observed in Windows firmware captures             |
    \*-----------------------------------------------------*/
    unsigned char profile[27];

    memset(profile, 0x00, sizeof(profile));
    profile[0]  = 0x00;     /* profile index                */
    profile[1]  = mode;
    profile[2]  = brightness;
    profile[3]  = speed;
    profile[4]  = direction;
    profile[5]  = random_flag;
    profile[6]  = red;
    profile[7]  = green;
    profile[8]  = blue;
    profile[18] = 0x03;
    profile[22] = 0x03;

    SendKeyboardBegin();
    SendKeyboardParameter(EVISION_KB_PARAMETER_MODE, sizeof(profile), profile);
    SendKeyboardEnd();
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void EVisionKeyboardController::ComputeChecksum
    (
    char                usb_buf[64]
    )
{
    unsigned short checksum = 0;

    for(unsigned int byte_idx = 0x03; byte_idx < 64; byte_idx++)
    {
        checksum += usb_buf[byte_idx];
    }

    usb_buf[0x01] = checksum & 0xFF;
    usb_buf[0x02] = checksum >> 8;
}

void EVisionKeyboardController::SendKeyboardBegin()
{
    char usb_buf[64];

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
    usb_buf[0x01]           = EVISION_KB_COMMAND_BEGIN;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = EVISION_KB_COMMAND_BEGIN;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read_timeout(dev, (unsigned char *)usb_buf, 64, 100);
}

void EVisionKeyboardController::SendKeyboardEnd()
{
    char usb_buf[64];

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
    usb_buf[0x01]           = EVISION_KB_COMMAND_END;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = EVISION_KB_COMMAND_END;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read_timeout(dev, (unsigned char *)usb_buf, 64, 100);
}

void EVisionKeyboardController::SendKeyboardData
    (
    unsigned char *     data,
    unsigned char       data_size,
    unsigned int        data_offset
    )
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Color Data (0x12) packet              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x03]           = 0x12;

    usb_buf[0x04]           = data_size;
    usb_buf[0x05]           = data_offset & 0xFF;
    usb_buf[0x06]           = (data_offset >> 8) & 0xFF;
    usb_buf[0x07]           = (data_offset >> 16) & 0xFF;

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
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read_timeout(dev, (unsigned char *)usb_buf, 64, 100);
}

void EVisionKeyboardController::SendKeyboardParameter
    (
    unsigned char       parameter,
    unsigned char       parameter_size,
    unsigned char*      parameter_data
    )
{
    char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Keyboard Parameter (0x06) packet               |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x04;
    usb_buf[0x03]           = EVISION_KB_COMMAND_SET_PARAMETER;
    usb_buf[0x04]           = parameter_size;
    usb_buf[0x05]           = parameter;

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
    hid_write(dev, (unsigned char *)usb_buf, 64);
    hid_read_timeout(dev, (unsigned char *)usb_buf, 64, 100);
}
