/*---------------------------------------------------------*\
| SteelSeriesSenseiController.cpp                           |
|                                                           |
|   Driver for SteelSeries Sensei                           |
|                                                           |
|   Based on SteelSeries Rival controller                   |
|   B Horn (bahorn)                             13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "SteelSeriesSenseiController.h"
#include "StringUtils.h"

static void send_usb_msg(hid_device* dev, unsigned char * data_pkt, unsigned int size)
{
    unsigned char* usb_pkt = new unsigned char[size + 1];

    usb_pkt[0] = 0x00;
    for(unsigned int i = 1; i < size + 1; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }

    hid_write(dev, usb_pkt, size + 1);

    delete[] usb_pkt;
}

SteelSeriesSenseiController::SteelSeriesSenseiController
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path,
    std::string         dev_name
    )
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    proto       = proto_type;
}

SteelSeriesSenseiController::~SteelSeriesSenseiController()
{
    hid_close(dev);
}

std::string SteelSeriesSenseiController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string SteelSeriesSenseiController::GetDeviceName()
{
    return(name);
}

std::string SteelSeriesSenseiController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

steelseries_type SteelSeriesSenseiController::GetMouseType()
{
    return proto;
}

/* Saves to the internal configuration */
void SteelSeriesSenseiController::Save()
{
    /*-----------------------------------------------------*\
    | Saves to the internal configuration                   |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[9];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Save packet                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]       = 0x59;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    send_usb_msg(dev, usb_buf, 9);
}


void SteelSeriesSenseiController::SetLightEffect
    (
    unsigned char   zone_id,
    unsigned char   effect,
    unsigned char   speed,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Light Effect packet                            |
    \*-----------------------------------------------------*/
    unsigned char dur1 = 0x27;
    unsigned char dur2 = 0x10;            	//10 sec cycle

    switch(effect)
    {
        case STEELSERIES_SENSEI_MODE_BREATHING:
            switch(speed)
            {
                case STEELSERIES_SENSEI_EFFECT_BREATHING_MIN:
                    dur1 = 0x27;
                    dur2 = 0x10;            //10 sec cycle
                    break;

                case STEELSERIES_SENSEI_EFFECT_BREATHING_MID:
                    dur1 = 0x13;
                    dur2 = 0x88;            //5 sec cycle
                    break;

                case STEELSERIES_SENSEI_EFFECT_BREATHING_MAX:
                    dur1 = 0x09;
                    dur2 = 0xc4;            //2.5 sec cycle
                    break;
            }
            usb_buf[0x00]       = 0x5B;     //command byte
            usb_buf[0x02]       = zone_id;
            usb_buf[0x04]       = dur1;     //duration in ms 1st byte
            usb_buf[0x03]       = dur2;     //duration in ms 2nd byte
            usb_buf[0x1B]       = 0x03;     //Number of colors

            /*---------------------------------------------*\
            | Original software duplicates these RGB bytes, |
            | but seems unnecessary                         |
            \*---------------------------------------------*/
            usb_buf[0x1C]       = red;
            usb_buf[0x1D]       = green;
            usb_buf[0x1E]       = blue;

            usb_buf[0x1F]       = red;
            usb_buf[0x20]       = green;
            usb_buf[0x21]       = blue;
            usb_buf[0x26]       = 0x7F;     //percent of duration out of 0xFF
            usb_buf[0x27]       = red;
            usb_buf[0x28]       = green;
            usb_buf[0x29]       = blue;
            usb_buf[0x2A]       = 0x7F;     //percent of duration out of 0xFF
            break;

        case STEELSERIES_SENSEI_MODE_RAINBOW:
            switch(speed)
            {
                case STEELSERIES_SENSEI_EFFECT_RAINBOW_MIN:
                    dur1 = 0x4E;
                    dur2 = 0x20;            //20 sec cycle
                    break;

                case STEELSERIES_SENSEI_EFFECT_RAINBOW_MID:
                    dur1 = 0x27;
                    dur2 = 0x10;            //10 sec cycle
                    break;

                case STEELSERIES_SENSEI_EFFECT_RAINBOW_MAX:
                    dur1 = 0x13;
                    dur2 = 0x88;            //5 sec cycle
                    break;
            }
            usb_buf[0x00]       = 0x5B;     //command byte
            usb_buf[0x02]       = zone_id;
            usb_buf[0x04]       = dur1;     //duration in ms 1st byte
            usb_buf[0x03]       = dur2;     //duration in ms 2nd byte
            usb_buf[0x1B]       = 0x07;     //Number of colors

            /*---------------------------------------------*\
            | Original software duplicates these RGB bytes, |
            | but seems unnecessary                         |
            \*---------------------------------------------*/
            usb_buf[0x1C]       = red;
            usb_buf[0x1D]       = green;
            usb_buf[0x1E]       = blue;

            usb_buf[0x1C]       = 0xFF;
            usb_buf[0x1F]       = 0xFF;
            usb_buf[0x22]       = 0x14;
            usb_buf[0x23]       = 0xFF;
            usb_buf[0x24]       = 0xFF;
            usb_buf[0x26]       = 0x2B;     //percent of duration out of 0xFF
            usb_buf[0x28]       = 0xFF;
            usb_buf[0x2A]       = 0x2B;
            usb_buf[0x2C]       = 0xFF;
            usb_buf[0x2D]       = 0xFF;     //percent of duration out of 0xFF
            usb_buf[0x2E]       = 0x28;
            usb_buf[0x31]       = 0xFF;
            usb_buf[0x32]       = 0x2B;
            usb_buf[0x33]       = 0xFF;
            usb_buf[0x35]       = 0xFF;
            usb_buf[0x36]       = 0x2B;
            usb_buf[0x37]       = 0xFF;
            usb_buf[0x3A]       = 0x14;
            break;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    send_usb_msg(dev, usb_buf, sizeof(usb_buf));
}


void SteelSeriesSenseiController::SetLightEffectAll
    (
    unsigned char   effect,
    unsigned char   speed,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    SetLightEffect(0, effect, speed, red, green, blue);
    SetLightEffect(1, effect, speed, red, green, blue);
}


void SteelSeriesSenseiController::SetColor
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Set Color packet                               |
    \*-----------------------------------------------------*/
    usb_buf[0x00]       = 0x5B;             //command byte
    usb_buf[0x02]       = zone_id;

    /*-----------------------------------------------------*\
    | Original software duplicates these RGB bytes,         |
    | but seems unnecessary                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x1C]       = red;
    usb_buf[0x1D]       = green;
    usb_buf[0x1E]       = blue;

    usb_buf[0x1F]       = red;
    usb_buf[0x20]       = green;
    usb_buf[0x21]       = blue;
    usb_buf[0x13]       = 0x01;             //Static color flag
    usb_buf[0x1B]       = 0x01;             //Number of colors

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    send_usb_msg(dev, usb_buf, sizeof(usb_buf));
}

void SteelSeriesSenseiController::SetColorAll
    (
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue
        )
{
    SetColor(0, red, green, blue);
    SetColor(1, red, green, blue);
}
