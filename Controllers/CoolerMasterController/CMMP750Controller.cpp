/*-------------------------------------------------------------------*\
|  CMMP750Controller.cpp                                              |
|                                                                     |
|  Driver for Coolermaster MP750 mousepad                             |
|                                                                     |
|  Chris M (Dr_No)          16th Apr 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMMP750Controller.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

CMMP750Controller::CMMP750Controller(libusb_device_handle* dev_handle)
{
    dev = dev_handle;
    inAddr = 0x82;
    outAddr = 0x03;

    strcpy(device_name, "CoolerMaster MP750");

    current_mode = MP750_MODE_BREATHING;
    current_speed = 0x80;
}

CMMP750Controller::~CMMP750Controller()
{

}

char* CMMP750Controller::GetDeviceName()
{
    return device_name;
}

void CMMP750Controller::SetMode(unsigned char mode, unsigned char speed)
{
    int actual = 0;
    unsigned char buffer[6] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));
    for(int i = 0; i < buffer_size; i++)
    {
        buffer[i] = colour_mode_data[mode][i];
    }

    if(mode > 3)
    {   //If the mode is random colours set SPEED at BYTE2
        buffer[2] = speed;
    }
    else
    { //Otherwise SPEED is BYTE5
        buffer[2] = current_red;
        buffer[3] = current_green;
        buffer[4] = current_blue;
        buffer[5] = speed;
    }

    libusb_interrupt_transfer(dev, outAddr, buffer, buffer_size, &actual, 0);
    libusb_interrupt_transfer(dev, inAddr, buffer, buffer_size, &actual, 0);
}

void CMMP750Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    int actual = 0;
    unsigned char buffer[6] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));
    for(int i=0; i < buffer_size; i++)
    {
        buffer[i] = colour_mode_data[current_mode][i];
    }

    if(current_mode > 3)
    {   //If the mode is random colours set SPEED at BYTE2
        buffer[2] = current_speed;
    }
    else
    { //Otherwise SPEED is BYTE5
        buffer[2] = red;
        buffer[3] = green;
        buffer[4] = blue;
        buffer[5] = current_speed;
    }

    libusb_interrupt_transfer(dev, outAddr, buffer, buffer_size, &actual, 0);
    libusb_interrupt_transfer(dev, inAddr, buffer, buffer_size, &actual, 0);
}

