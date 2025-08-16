/*---------------------------------------------------------*\
| LogitechG915Controller.cpp                                |
|                                                           |
|   Driver for Logitech G915                                |
|                                                           |
|   Cheerpipe                                   20 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechG915Controller.h"
#include "StringUtils.h"

const size_t MIN_DATA_FRAME_SIZE    = 4;
const size_t MAX_DATA_FRAME_SIZE    = 16;
const size_t HEADER_SIZE            = 4;
const size_t MESSAGE_LEN            = 20;
const size_t RESPONSE_LEN           = 20;

LogitechG915Controller::LogitechG915Controller(hid_device* dev_handle, bool wired, std::string dev_name)
{
    this->dev_handle        = dev_handle;
    this->name              = dev_name;

    if(wired)
    {
        device_index        = 0xFF;
        feature_4522_idx    = 0x0E;
        feature_8040_idx    = 0x13;
        feature_8071_idx    = 0x09;
        feature_8081_idx    = 0x0A;
    }
    else
    {
        device_index        = 0x01;
        feature_4522_idx    = 0x0F;
        feature_8040_idx    = 0x14;
        feature_8071_idx    = 0x0A;
        feature_8081_idx    = 0x0B;
    }
}

LogitechG915Controller::~LogitechG915Controller()
{

}

std::string LogitechG915Controller::GetNameString()
{
    return(name);
}

std::string LogitechG915Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_handle, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LogitechG915Controller::Commit()
{
    SendCommit();
}

void LogitechG915Controller::SetDirect
    (
    unsigned char       frame_type,
    unsigned char *     frame_data,
    size_t              length
    )
{
    SendDirectFrame(frame_type, frame_data, length);
}

void LogitechG915Controller::SetMode
    (
    unsigned char       mode,
    unsigned short      speed,
    unsigned short      brightness,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    BeginModeSet();
    uint8_t logo_mode = mode;
    switch(mode)
    {
        case LOGITECH_G915_MODE_OFF:
        case LOGITECH_G915_MODE_STATIC:
        {
            logo_mode = mode; // static and off match
            break;
        }
        case LOGITECH_G915_MODE_BREATHING:
        {
            logo_mode = LOGITECH_G915_LOGO_MODE_BREATHING; //0x03
            break;
        }
        case LOGITECH_G915_MODE_CYCLE:
        case LOGITECH_G915_MODE_WAVE:
        {
            logo_mode = LOGITECH_G915_LOGO_MODE_CYCLE; //0x02
            break;
        }
        case LOGITECH_G915_MODE_RIPPLE:
        {
            logo_mode = LOGITECH_G915_LOGO_MODE_STATIC;
            break;
        }
    }

    SendMode(LOGITECH_G915_ZONE_MODE_KEYBOARD,  mode, speed, brightness, red, green, blue);
    SendMode(LOGITECH_G915_ZONE_MODE_LOGO, logo_mode, speed, brightness, red, green, blue);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void LogitechG915Controller::SendCommit()
{
    unsigned char usb_buf[MESSAGE_LEN];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_8081_idx;
    usb_buf[0x03]           = LOGITECH_G915_COMMIT_BYTE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 4);
    hid_read_timeout(dev_handle, usb_buf, RESPONSE_LEN, LOGITECH_READ_TIMEOUT);
}

void LogitechG915Controller::BeginModeSet()
{
    unsigned char usb_buf[MESSAGE_LEN];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_4522_idx;
    usb_buf[0x03]           = 0x3E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 4);
    hid_read(dev_handle,  usb_buf, RESPONSE_LEN);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_4522_idx;
    usb_buf[0x03]           = 0x1E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 4);
    hid_read(dev_handle,  usb_buf, RESPONSE_LEN);
}

void LogitechG915Controller::InitializeModeSet()
{
    unsigned char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_8071_idx;
    usb_buf[0x03]           = 0x5E;
    usb_buf[0x04]           = 0x01;
    usb_buf[0x05]           = 0x03;
    usb_buf[0x06]           = 0x07;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 7);
    hid_read(dev_handle,  usb_buf, RESPONSE_LEN);
}

void LogitechG915Controller::InitializeDirect()
{
    unsigned char usb_buf[MESSAGE_LEN];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_4522_idx;
    usb_buf[0x03]           = 0x3E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 4);
    hid_read(dev_handle,  usb_buf, RESPONSE_LEN);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_4522_idx;
    usb_buf[0x03]           = 0x1E;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 4);
    hid_read(dev_handle,  usb_buf, RESPONSE_LEN);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_8071_idx;
    usb_buf[0x03]           = 0x1E;
    usb_buf[0x10]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 17);
    hid_read(dev_handle,  usb_buf, MESSAGE_LEN);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_8071_idx;
    usb_buf[0x03]           = 0x1E;
    usb_buf[0x04]           = 0x01;
    usb_buf[0x10]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, 17);
    hid_read(dev_handle,  usb_buf, RESPONSE_LEN);
}

void LogitechG915Controller::SendSingleLed
    (
    unsigned char       keyCode,
    unsigned char       r,
    unsigned char       g,
    unsigned char       b
    )
{
    unsigned char little_frame[4] = { keyCode, r, g, b };
    SendDirectFrame(LOGITECH_G915_ZONE_FRAME_TYPE_LITTLE, little_frame, 4);
}

void LogitechG915Controller::SendDirectFrame
    (
    unsigned char       frame_type,
    unsigned char *     frame_data,
    size_t              length
    )
{
    if(length < MIN_DATA_FRAME_SIZE)
    {
        return;
    }
    else if(length > MAX_DATA_FRAME_SIZE)
    {
        length = MAX_DATA_FRAME_SIZE;
    }

    unsigned char usb_buf[MESSAGE_LEN];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_8081_idx;
    usb_buf[0x03]           = frame_type;

    /*-----------------------------------------------------*\
    | Copy in frame data                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x04], frame_data, length);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, HEADER_SIZE + length);
    hid_read_timeout(dev_handle, usb_buf, RESPONSE_LEN, LOGITECH_READ_TIMEOUT);
}

void LogitechG915Controller::SendMode
    (
    unsigned char       zone,
    unsigned char       mode,
    unsigned short      speed,
    unsigned short      brightness,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char usb_buf[HEADER_SIZE + 13];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/

    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = device_index;
    usb_buf[0x02]           = feature_8071_idx;
    usb_buf[0x03]           = LOGITECH_G915_ZONE_FRAME_TYPE_MODE;

    usb_buf[0x04]           = zone;
    usb_buf[0x05]           = mode;

    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    if(mode != LOGITECH_G915_MODE_RIPPLE)
    {
        speed = 100 * speed;
    }

    // mode == LOGITECH_G915_MODE_OFF; No data to set
    if(mode == LOGITECH_G915_MODE_STATIC)
    {
        usb_buf[0x09] = 0x02;
    }
    else if((mode == LOGITECH_G915_MODE_BREATHING && zone == LOGITECH_G915_ZONE_MODE_KEYBOARD) \
            || (mode == LOGITECH_G915_LOGO_MODE_BREATHING && zone == LOGITECH_G915_ZONE_MODE_LOGO))
    {
        usb_buf[0x09] = speed >> 8;
        usb_buf[0x0A] = speed & 0xFF;
        usb_buf[0x0C] = brightness & 0xFF;
    }
    else if((mode == LOGITECH_G915_MODE_CYCLE && zone == LOGITECH_G915_ZONE_MODE_KEYBOARD) \
            || (mode == LOGITECH_G915_LOGO_MODE_CYCLE && zone == LOGITECH_G915_ZONE_MODE_LOGO))
    {
        usb_buf[0x0B] = speed >> 8;
        usb_buf[0x0C] = speed & 0xFF;
        usb_buf[0x0D] = brightness & 0xFF;
    }
    else if(mode == LOGITECH_G915_MODE_WAVE)
    {
        usb_buf[0x0C] = speed & 0xFF;
        usb_buf[0x0D] = 0x01; // Direction control 0x01 is horizontal
        usb_buf[0x0E] = brightness & 0xFF;
        usb_buf[0x0F] = speed >> 8;
    }
    else if(mode == LOGITECH_G915_MODE_RIPPLE)
    {
        usb_buf[0x0B] = speed & 0xFF;
    }
    usb_buf[0x10] = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_handle, usb_buf, HEADER_SIZE + 13);
    hid_read(dev_handle, usb_buf, RESPONSE_LEN);
}
