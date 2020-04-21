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

CMMP750Controller::CMMP750Controller(libusb_device_handle* dev_handle, unsigned int _inAddr, unsigned int _outAddr, int _interface)
{
    dev = dev_handle;
    inAddr = _inAddr;
    outAddr = _outAddr;
    interface = _interface;

    libusb_device* device = libusb_get_device(dev);
    int bus = libusb_get_bus_number(device);
    int bus_addr = libusb_get_device_address(device);
    int port = libusb_get_port_number(device);
    location = "Bus: " + std::to_string(bus) + "  Addr: " + std::to_string(bus_addr) + "  Port: " + std::to_string(port);

    strcpy(device_name, "Cooler Master MP750");

    current_mode = MP750_MODE_STATIC;
    current_speed = MP750_SPEED_NORMAL;
}

CMMP750Controller::~CMMP750Controller()
{
    libusb_release_interface(dev, interface);
    libusb_attach_kernel_driver(dev, interface);
}

char* CMMP750Controller::GetDeviceName()
{
    return device_name;
}

char* CMMP750Controller::GetSerial()
{
    return serial;
}

std::string CMMP750Controller::GetLocation()
{
    return location;
}

void CMMP750Controller::SetMode(unsigned char mode, unsigned char speed)
{
    current_mode  = mode;
    current_speed = speed;

    SendUpdate();
}

void CMMP750Controller::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red   = red;
    current_green = green;
    current_blue  = blue;

    SendUpdate();
}

void CMMP750Controller::SendUpdate()
{
    int actual = 0;
    unsigned char buffer[0x40] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    for(int i = 0; i < CM_COLOUR_MODE_DATA_SIZE; i++)
    {
        buffer[i] = colour_mode_data[current_mode][i];
    }

   if(current_mode > MP750_MODE_BREATHING)
    { 
        //If the mode is random colours set SPEED at BYTE2
        buffer[CM_RED_BYTE] = speed_mode_data[current_mode][current_speed];
    }
    else
    { 
        //Otherwise SPEED is BYTE5
        buffer[CM_RED_BYTE] = current_red;
        buffer[CM_GREEN_BYTE] = current_green;
        buffer[CM_BLUE_BYTE] = current_blue;
        buffer[CM_SPEED_BYTE] = speed_mode_data[current_mode][current_speed];
    }

    //Nothing will be done with the "actual" transfer length
    libusb_interrupt_transfer(dev, outAddr, buffer, buffer_size, nullptr, CM_INTERRUPT_TIMEOUT);
}

