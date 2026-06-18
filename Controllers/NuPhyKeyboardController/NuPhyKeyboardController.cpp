/*---------------------------------------------------------*\
| NuPhyKeyboardController.cpp                               |
|                                                           |
|   Driver for NuPhy QMK/VIA RGB Matrix keyboards           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "NuPhyKeyboardController.h"
#include "StringUtils.h"

#define NUPHY_VIA_MESSAGE_LENGTH              32
#define NUPHY_VIA_GET_PROTOCOL_VERSION        0x01
#define NUPHY_VIA_CUSTOM_SET_VALUE            0x07
#define NUPHY_VIA_CUSTOM_GET_VALUE            0x08
#define NUPHY_VIA_CUSTOM_SAVE                 0x09
#define NUPHY_VIA_RGB_MATRIX_CHANNEL          0x03
#define NUPHY_VIA_RGB_MATRIX_BRIGHTNESS       0x01
#define NUPHY_VIA_RGB_MATRIX_EFFECT           0x02
#define NUPHY_VIA_RGB_MATRIX_SPEED            0x03
#define NUPHY_VIA_RGB_MATRIX_COLOR            0x04

NuPhyKeyboardController::NuPhyKeyboardController(hid_device* dev_handle, const char* path, const std::string& device_name)
{
    dev                     = dev_handle;
    location                = path;
    name                    = device_name;
    supported               = false;
    via_protocol_version    = 0;

    wchar_t serial_string[256];
    if(hid_get_serial_number_string(dev, serial_string, 256) == 0)
    {
        serial = StringUtils::wstring_to_string(serial_string);
    }

    unsigned char version[2] = { 0, 0 };
    if(SendCommand(NUPHY_VIA_GET_PROTOCOL_VERSION, 0, 0, version, sizeof(version)))
    {
        via_protocol_version = (version[0] << 8) | version[1];
    }

    unsigned char brightness = 0;
    supported = (via_protocol_version >= 9)
             && SendCommand(NUPHY_VIA_CUSTOM_GET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_BRIGHTNESS, &brightness, sizeof(brightness));
}

NuPhyKeyboardController::~NuPhyKeyboardController()
{
    hid_close(dev);
}

std::string NuPhyKeyboardController::GetLocation()
{
    return("HID: " + location);
}

std::string NuPhyKeyboardController::GetName()
{
    return(name);
}

std::string NuPhyKeyboardController::GetSerial()
{
    return(serial);
}

std::string NuPhyKeyboardController::GetVersion()
{
    return("VIA: " + std::to_string(via_protocol_version));
}

bool NuPhyKeyboardController::GetSupported()
{
    return(supported);
}

unsigned char NuPhyKeyboardController::GetBrightness()
{
    unsigned char value = 0;
    SendCommand(NUPHY_VIA_CUSTOM_GET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_BRIGHTNESS, &value, sizeof(value));
    return(value);
}

unsigned char NuPhyKeyboardController::GetEffect()
{
    unsigned char value = 0;
    SendCommand(NUPHY_VIA_CUSTOM_GET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_EFFECT, &value, sizeof(value));
    return(value);
}

unsigned char NuPhyKeyboardController::GetSpeed()
{
    unsigned char value = 0;
    SendCommand(NUPHY_VIA_CUSTOM_GET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_SPEED, &value, sizeof(value));
    return(value);
}

void NuPhyKeyboardController::GetColor(unsigned char* hue, unsigned char* saturation)
{
    unsigned char value[2] = { 0, 0 };
    SendCommand(NUPHY_VIA_CUSTOM_GET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_COLOR, value, sizeof(value));
    *hue        = value[0];
    *saturation = value[1];
}

void NuPhyKeyboardController::SetBrightness(unsigned char brightness)
{
    SendCommand(NUPHY_VIA_CUSTOM_SET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_BRIGHTNESS, &brightness, sizeof(brightness));
}

void NuPhyKeyboardController::SetEffect(unsigned char effect)
{
    SendCommand(NUPHY_VIA_CUSTOM_SET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_EFFECT, &effect, sizeof(effect));
}

void NuPhyKeyboardController::SetSpeed(unsigned char speed)
{
    SendCommand(NUPHY_VIA_CUSTOM_SET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_SPEED, &speed, sizeof(speed));
}

void NuPhyKeyboardController::SetColor(unsigned char hue, unsigned char saturation)
{
    unsigned char value[2] = { hue, saturation };
    SendCommand(NUPHY_VIA_CUSTOM_SET_VALUE, NUPHY_VIA_RGB_MATRIX_CHANNEL, NUPHY_VIA_RGB_MATRIX_COLOR, value, sizeof(value));
}

void NuPhyKeyboardController::Save()
{
    SendCommand(NUPHY_VIA_CUSTOM_SAVE, NUPHY_VIA_RGB_MATRIX_CHANNEL, 0, nullptr, 0);
}

bool NuPhyKeyboardController::SendCommand(unsigned char command, unsigned char channel, unsigned char value_id, unsigned char* value, unsigned char value_size)
{
    unsigned char usb_buf[NUPHY_VIA_MESSAGE_LENGTH + 1];
    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[1] = command;
    usb_buf[2] = channel;
    usb_buf[3] = value_id;

    if(value_size > 0)
    {
        memcpy(&usb_buf[4], value, value_size);
    }

    if(hid_write(dev, usb_buf, sizeof(usb_buf)) < 0)
    {
        return(false);
    }

    int bytes_received = hid_read_timeout(dev, usb_buf, NUPHY_VIA_MESSAGE_LENGTH, 1000);
    if(bytes_received != NUPHY_VIA_MESSAGE_LENGTH || usb_buf[0] != command)
    {
        return(false);
    }

    if(value_size > 0)
    {
        unsigned char response_offset = (command == NUPHY_VIA_GET_PROTOCOL_VERSION) ? 1 : 3;
        memcpy(value, &usb_buf[response_offset], value_size);
    }

    return(true);
}
