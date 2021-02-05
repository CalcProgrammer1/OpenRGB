/*-----------------------------------------*\
|  SteelSeriesRivalController.h             |
|                                           |
|  Definitions and types for SteelSeries    |
|  Rival lighting controller                |
|                                           |
|  B Horn (bahorn) 13/5/2020                |
\*-----------------------------------------*/

#include "SteelSeriesRivalController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

static void send_usb_msg(hid_device* dev, char * data_pkt, unsigned int size)
{
    char* usb_pkt = new char[size + 1];
    
    usb_pkt[0] = 0x00;
    for(unsigned int i = 1; i < size + 1; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    
    hid_write(dev, (unsigned char *)usb_pkt, size + 1);
    
    delete usb_pkt;
}

SteelSeriesRivalController::SteelSeriesRivalController
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path
    )
{
    dev         = dev_handle;
    location    = path;
    proto       = proto_type;
}

SteelSeriesRivalController::~SteelSeriesRivalController()
{
    hid_close(dev);
}

std::string SteelSeriesRivalController::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesRivalController::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesRivalController::GetSerialString()
{
    wchar_t serial_string[128];
    hid_get_serial_number_string(dev, serial_string, 128);

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

steelseries_type SteelSeriesRivalController::GetMouseType()
{
    return proto;
}

/* Saves to the internal configuration */
void SteelSeriesRivalController::Save()
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]       = 0x09;
    send_usb_msg(dev, usb_buf, 9);
}


void SteelSeriesRivalController::SetLightEffect
    (
    unsigned char   zone_id,
    unsigned char   effect
    )
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    switch (proto)
    {
        case RIVAL_100:
            usb_buf[0x00]       = 0x07;
            usb_buf[0x01]       = 0x00;
            break;

        case RIVAL_300:
            usb_buf[0x00]       = 0x07;
            usb_buf[0x01]       = zone_id + 1;
            break;

        default:
            break;
    }
    usb_buf[0x02]       = effect;
    send_usb_msg(dev, usb_buf, 9);
}


void SteelSeriesRivalController::SetLightEffectAll
    (
    unsigned char   effect
    )
{
    switch(proto)
    {
        case RIVAL_100:
            SetLightEffect(0, effect);
            break;

        case RIVAL_300:
            SetLightEffect(0, effect);
            SetLightEffect(1, effect);
            break;
        
        default:
            break;
    }
}


void SteelSeriesRivalController::SetColor
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    switch (proto)
    {
        case RIVAL_100:
            usb_buf[0x00]       = 0x05;
            usb_buf[0x01]       = 0x00;
            break;
    
        case RIVAL_300:
            usb_buf[0x00]       = 0x08;
            usb_buf[0x01]       = zone_id + 1;
            break;

        default:
            break;
    }

    usb_buf[0x02]       = red;
    usb_buf[0x03]       = green;
    usb_buf[0x04]       = blue;

    send_usb_msg(dev, usb_buf, 9);
}

void SteelSeriesRivalController::SetColorAll
    (
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue
    )
{
    switch(proto)
    {
        case RIVAL_100:
            SetColor(0, red, green, blue);
            break;

        case RIVAL_300:
            SetColor(0, red, green, blue);
            SetColor(1, red, green, blue);
            break;

        default:
            break;
    }
}

