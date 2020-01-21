/*-----------------------------------------*\
|  RGBFusion2Controller.cpp                 |
|                                           |
|  Driver for Gigabyte Aorus RGB Fusion 2.0 |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/15/2020   |
\*-----------------------------------------*/

#include "RGBFusion2Controller.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

RGBFusion2Controller::RGBFusion2Controller(hid_device* dev_handle)
{
    dev = dev_handle;

    // Set Device name
    strcpy(device_name, "Gigabyte Motherboard");

    // Set LED count
    led_count = 8;
}

RGBFusion2Controller::~RGBFusion2Controller()
{

}

std::string RGBFusion2Controller::GetDeviceName()
{
    return(device_name);
}

std::string RGBFusion2Controller::GetDeviceLocation()
{
    return("");
}

unsigned int RGBFusion2Controller::GetLEDCount()
{
    return(led_count);
}

unsigned char RGBFusion2Controller::GetMode()
{
    return(0);
}

void RGBFusion2Controller::SetAllColors(unsigned char red, unsigned char green, unsigned char blue)
{
}

void RGBFusion2Controller::SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[] =
    {
        0xCC, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
        0x5A, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    usb_buf[0x01] = (0x20 |  led);
    usb_buf[0x02] = (0x01 << led );

    usb_buf[0x0E] = blue;
    usb_buf[0x0F] = green;
    usb_buf[0x10] = red;

    hid_send_feature_report(dev, usb_buf, 64);
    
    SendApply();
}

void RGBFusion2Controller::SendApply()
{
    unsigned char usb_buf[] =
    {
        0xCC, 0x28, 0xFF, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    hid_send_feature_report(dev, usb_buf, 64);
}

void RGBFusion2Controller::SetMode(unsigned char mode)
{
}

void RGBFusion2Controller::dump()
{

}
