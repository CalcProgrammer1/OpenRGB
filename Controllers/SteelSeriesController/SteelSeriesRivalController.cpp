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

SteelSeriesRivalController::SteelSeriesRivalController(hid_device* dev_handle)
{
    dev = dev_handle;
}

SteelSeriesRivalController::~SteelSeriesRivalController()
{

}

char* SteelSeriesRivalController::GetDeviceName()
{
    return device_name;
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
        unsigned char effect
    )
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]       = 0x07;
    usb_buf[0x01]       = 0x00;
    usb_buf[0x02]       = effect;
    send_usb_msg(dev, usb_buf, 9);
}

void SteelSeriesRivalController::SetColor
    (
            unsigned char   red,
            unsigned char   green,
            unsigned char   blue
    )
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]       = 0x05;
    usb_buf[0x01]       = 0x00;
    usb_buf[0x02]       = red;
    usb_buf[0x03]       = green;
    usb_buf[0x04]       = blue;

    send_usb_msg(dev, usb_buf, 9);
}
