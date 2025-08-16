/*---------------------------------------------------------*\
| LogitechGLightsyncController.cpp                          |
|                                                           |
|   Driver for Logitech Lightsync                           |
|                                                           |
|   TheRogueZeta                                21 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechGLightsyncController.h"
#include "StringUtils.h"

LogitechGLightsyncController::LogitechGLightsyncController(hid_device* dev_cmd_handle, hid_device *dev_handle, const char *path, unsigned char hid_dev_index, unsigned char hid_feature_index, unsigned char hid_fctn_ase_id, std::string dev_name)
{
    dev             = dev_handle;
    cmd_dev         = dev_cmd_handle;
    location        = path;
    dev_index       = hid_dev_index;
    feature_index   = hid_feature_index;
    fctn_ase_id     = hid_fctn_ase_id;
    mutex           = nullptr;
    name            = dev_name;
}

LogitechGLightsyncController::LogitechGLightsyncController(hid_device* dev_cmd_handle, hid_device *dev_handle, const char *path, unsigned char hid_dev_index, unsigned char hid_feature_index, unsigned char hid_fctn_ase_id, std::shared_ptr<std::mutex> mutex_ptr, std::string dev_name)
{
    dev             = dev_handle;
    cmd_dev         = dev_cmd_handle;
    location        = path;
    dev_index       = hid_dev_index;
    feature_index   = hid_feature_index;
    fctn_ase_id     = hid_fctn_ase_id;
    mutex           = mutex_ptr;
    name            = dev_name;
}

LogitechGLightsyncController::~LogitechGLightsyncController()
{
    hid_close(dev);
}

std::string LogitechGLightsyncController::GetDeviceLocation()
{
    return ("HID: " + location);
}

std::string LogitechGLightsyncController::GetNameString()
{
    return(name);
}

std::string LogitechGLightsyncController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LogitechGLightsyncController::UpdateMouseLED(
    unsigned char mode,
    std::uint16_t speed,
    unsigned char zone,
    unsigned char red,
    unsigned char green,
    unsigned char blue,
    unsigned char brightness
)
{
    unsigned char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x11;
    usb_buf[0x01] = dev_index;
    usb_buf[0x02] = feature_index;
    usb_buf[0x03] = fctn_ase_id;

    usb_buf[0x04] = zone;
    usb_buf[0x05] = mode;

    usb_buf[0x06] = red;
    usb_buf[0x07] = green;
    usb_buf[0x08] = blue;

    speed = 100 * speed;
    if (mode == LOGITECH_G_LIGHTSYNC_MODE_STATIC)
    {
        usb_buf[0x09] = 0x02;
    }
    if (mode == LOGITECH_G_LIGHTSYNC_MODE_CYCLE)
    {
        usb_buf[0x0B] = speed >> 8;
        usb_buf[0x0C] = speed & 0xFF;
        usb_buf[0x0D] = brightness;
    }
    else if (mode == LOGITECH_G_LIGHTSYNC_MODE_BREATHING)
    {
        usb_buf[0x09] = speed >> 8;
        usb_buf[0x0A] = speed & 0xFF;
        usb_buf[0x0C] = brightness;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    | This code has to be protected to avoid crashes when   |
    | this is called at the same time to change a powerplay |
    | mat and its paired wireless mouse leds. It will       |
    | happen when using effects engines with high framerate |
    \*-----------------------------------------------------*/
    if(mutex)
    {
        std::lock_guard<std::mutex> guard(*mutex);

        hid_write(dev, usb_buf, 20);
        hid_read(dev, usb_buf, 20);
    }
    else
    {
        hid_write(dev, usb_buf, 20);
        hid_read(dev, usb_buf, 20);
    }
}

void LogitechGLightsyncController::SetDirectMode(bool direct)
{
    unsigned char cmd_buf[7];
    unsigned char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(cmd_buf, 0x00, sizeof(cmd_buf));

    /*-----------------------------------------------------*\
    | Set up Command Control packet                         |
    \*-----------------------------------------------------*/
    cmd_buf[0x00]       = 0x10;
    cmd_buf[0x01]       = dev_index;
    cmd_buf[0x02]       = feature_index;
    cmd_buf[0x03]       = 0x8A;
    cmd_buf[0x04]       = 0x00;
    cmd_buf[0x05]       = 0x00;

    /*-----------------------------------------------------*\
    | If direct, disable save to flash                      |
    \*-----------------------------------------------------*/
    if(direct)
    {
        cmd_buf[0x04]       = 0x01;
        cmd_buf[0x05]       = 0x01;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    | This code has to be protected to avoid crashes when   |
    | this is called at the same time to change a powerplay |
    | mat and its paired wireless mouse leds. It will       |
    | happen when using effects engines with high framerate |
    \*-----------------------------------------------------*/
    if(mutex)
    {
        std::lock_guard<std::mutex> guard(*mutex);

        hid_write(cmd_dev, cmd_buf, 7);
        hid_read(dev, usb_buf, 20);
    }
    else
    {
        hid_write(cmd_dev, cmd_buf, 7);
        hid_read(dev, usb_buf, 20);
    }
}
