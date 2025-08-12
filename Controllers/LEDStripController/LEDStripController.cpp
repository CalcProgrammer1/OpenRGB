/*---------------------------------------------------------*\
| LEDStripController.cpp                                    |
|                                                           |
|   Driver for serial LED strips                            |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      11 Dec 2016 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <string>
#include "LEDStripController.h"
#include "ResourceManager.h"

LEDStripController::LEDStripController(std::string dev_name)
{
    name = dev_name;
}


LEDStripController::~LEDStripController()
{
}

void LEDStripController::Initialize(char* ledstring, led_protocol proto)
{
    LPSTR   numleds = NULL;
    LPSTR   source = NULL;
    LPSTR   udpport_baud = NULL;
    LPSTR   next = NULL;

    //Set the protocol
    protocol = proto;

    //Assume serial device unless a different protocol is specified
    bool    serial = TRUE;

    //Default i2c address out of range
    i2c_addr = 255;

    source = strtok_s(ledstring, ",", &next);

    //Check if we are setting up a Keyboard Visualizer UDP protocol device
    if (strncmp(source, "udp:", 4) == 0)
    {
        source = source + 4;
        serial = FALSE;
    }

    //Check for either the UDP port or the serial baud rate
    if (strlen(next))
    {
        udpport_baud = strtok_s(next, ",", &next);
    }

    //Check for the number of LEDs
    if (strlen(next))
    {
        numleds = strtok_s(next, ",", &next);
    }

    if (serial)
    {
        if (protocol == LED_PROTOCOL_BASIC_I2C)
        {
            //I2C uses the baud field for address
            i2c_addr = atoi(udpport_baud);
            InitializeI2C(source);
        }
        else if (udpport_baud == NULL)
        {
            //Initialize with default baud rate
            InitializeSerial(source, 115200);
        }
        else
        {
            //Initialize with custom baud rate
            InitializeSerial(source, atoi(udpport_baud));
        }
    }
    else
    {
        if (udpport_baud == NULL)
        {
            //Do something
        }
        else
        {
            //Initialize UDP port
            InitializeUDP(source, udpport_baud);
        }
    }

    if (numleds != NULL && strlen(numleds))
    {
        num_leds = atoi(numleds);
    }
}

void LEDStripController::InitializeI2C(char* i2cname)
{
    for(unsigned int i2c_idx = 0; i2c_idx < ResourceManager::get()->GetI2CBusses().size(); i2c_idx++)
    {
        if(ResourceManager::get()->GetI2CBusses()[i2c_idx]->device_name == std::string(i2cname))
        {
            if(i2c_addr < 128)
            {
                i2cport = ResourceManager::get()->GetI2CBusses()[i2c_idx];
                break;
            }
        }
    }

    serialport = NULL;
    udpport = NULL;
}

void LEDStripController::InitializeSerial(char* portname, int baud)
{
    portname = strtok(portname, "\r");
    port_name = portname;
    baud_rate = baud;
    serialport = new serial_port(port_name.c_str(), baud_rate);
    udpport = NULL;
    i2cport = NULL;
}

void LEDStripController::InitializeUDP(char * clientname, char * port)
{
    client_name = clientname;
    port_name   = port;

    udpport = new net_port(client_name.c_str(), port_name.c_str());
    serialport = NULL;
    i2cport = NULL;
}

char* LEDStripController::GetLEDString()
{
    return(led_string);
}

std::string LEDStripController::GetLocation()
{
    if(serialport != NULL)
    {
        return("COM: " + port_name);
    }
    else if(udpport != NULL)
    {
        return("UDP: " + client_name + ":" + port_name);
    }
    else if(i2cport != NULL)
    {
        return("I2C: " + std::string(i2cport->device_name) + ", Address " + std::to_string(i2c_addr));
    }
    else
    {
        return("");
    }
}

std::string LEDStripController::GetName()
{
    return(name);
}

void LEDStripController::SetLEDs(std::vector<RGBColor> colors)
{
    switch(protocol)
    {
        case LED_PROTOCOL_KEYBOARD_VISUALIZER:
            SetLEDsKeyboardVisualizer(colors);
            break;

        case LED_PROTOCOL_ADALIGHT:
            SetLEDsAdalight(colors);
            break;

        case LED_PROTOCOL_TPM2:
            SetLEDsTPM2(colors);
            break;

        case LED_PROTOCOL_BASIC_I2C:
            SetLEDsBasicI2C(colors);
            break;
    }
}

void LEDStripController::SetLEDsKeyboardVisualizer(std::vector<RGBColor> colors)
{
    unsigned char *serial_buf;

    /*-------------------------------------------------------------*\
    | Keyboard Visualizer Arduino Protocol                          |
    |                                                               |
    |   Packet size: Number of data bytes + 3                       |
    |                                                               |
    |   0: Packet Start Byte (0xAA)                                 |
    |   1-n: Data bytes                                             |
    |   n+1: Checksum MSB                                           |
    |   n+2: Checksum LSB                                           |
    \*-------------------------------------------------------------*/
    unsigned int payload_size   = (unsigned int)(colors.size() * 3);
    unsigned int packet_size    = payload_size + 3;

    serial_buf = new unsigned char[packet_size];

    /*-------------------------------------------------------------*\
    | Set up header                                                 |
    \*-------------------------------------------------------------*/
    serial_buf[0x00]            = 0xAA;

    /*-------------------------------------------------------------*\
    | Copy in color data in RGB order                               |
    \*-------------------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        unsigned int color_offset = color_idx * 3;

        serial_buf[0x01 + color_offset]     = RGBGetRValue(colors[color_idx]);
        serial_buf[0x02 + color_offset]     = RGBGetGValue(colors[color_idx]);
        serial_buf[0x03 + color_offset]     = RGBGetBValue(colors[color_idx]);
    }

    /*-------------------------------------------------------------*\
    | Calculate the checksum                                        |
    \*-------------------------------------------------------------*/
    unsigned short sum          = 0;

    for(unsigned int i = 0; i < (payload_size + 1); i++)
    {
        sum += serial_buf[i];
    }

    /*-------------------------------------------------------------*\
    | Fill in the checksum bytes                                    |
    \*-------------------------------------------------------------*/
    serial_buf[(num_leds * 3) + 1] = sum >> 8;
    serial_buf[(num_leds * 3) + 2] = sum & 0x00FF;

    /*-------------------------------------------------------------*\
    | Send the packet                                               |
    \*-------------------------------------------------------------*/
    if (serialport != NULL)
    {
        serialport->serial_write((char *)serial_buf, packet_size);
    }
    else if (udpport != NULL)
    {
        udpport->udp_write((char *)serial_buf, packet_size);
    }

    delete[] serial_buf;
}

void LEDStripController::SetLEDsAdalight(std::vector<RGBColor> colors)
{
    unsigned char *serial_buf;

    /*-------------------------------------------------------------*\
    | Adalight Protocol                                             |
    |                                                               |
    |   Packet size: Number of data bytes + 6                       |
    |                                                               |
    |   0: 'A' (0x41)                                               |
    |   1: 'd' (0x64)                                               |
    |   2: 'a' (0x61)                                               |
    |   3: LED count MSB                                            |
    |   4: LED count LSB                                            |
    |   5: Checksum (MSB xor LSB xor 0x55)                          |
    |   6-n: Data Bytes                                             |
    \*-------------------------------------------------------------*/
    unsigned int led_count      = (unsigned int)colors.size();
    unsigned int payload_size   = (led_count * 3);
    unsigned int packet_size    = payload_size + 6;

    serial_buf = new unsigned char[packet_size];

    /*-------------------------------------------------------------*\
    | Set up header                                                 |
    \*-------------------------------------------------------------*/
    serial_buf[0x00]            = 0x41;
    serial_buf[0x01]            = 0x64;
    serial_buf[0x02]            = 0x61;
    serial_buf[0x03]            = (led_count >> 8);
    serial_buf[0x04]            = (led_count & 0xFF);
    serial_buf[0x05]            = (serial_buf[0x03] ^ serial_buf[0x04] ^ 0x55);

    /*-------------------------------------------------------------*\
    | Copy in color data in RGB order                               |
    \*-------------------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < led_count; color_idx++)
    {
        unsigned int color_offset = color_idx * 3;

        serial_buf[0x06 + color_offset]     = RGBGetRValue(colors[color_idx]);
        serial_buf[0x07 + color_offset]     = RGBGetGValue(colors[color_idx]);
        serial_buf[0x08 + color_offset]     = RGBGetBValue(colors[color_idx]);
    }

    /*-------------------------------------------------------------*\
    | Send the packet                                               |
    \*-------------------------------------------------------------*/
    if (serialport != NULL)
    {
        serialport->serial_write((char *)serial_buf, packet_size);
    }

    delete[] serial_buf;
}

void LEDStripController::SetLEDsTPM2(std::vector<RGBColor> colors)
{
    unsigned char *serial_buf;

    /*-------------------------------------------------------------*\
    | TPM2 Protocol                                                 |
    |                                                               |
    |   Packet size: Number of data bytes + 5                       |
    |                                                               |
    |   0: Packet Start Byte (0xC9)                                 |
    |   1: Packet Type (0xDA - Data, 0xC0 - Command, 0xAA - Read)   |
    |   2: Payload Size MSB                                         |
    |   3: Payload Size LSB                                         |
    |   4-n: Data Bytes                                             |
    |   n+1: Packet End Byte (0x36)                                 |
    \*-------------------------------------------------------------*/
    unsigned int payload_size   = (unsigned int)(colors.size() * 3);
    unsigned int packet_size    = payload_size + 5;

    serial_buf = new unsigned char[packet_size];

    /*-------------------------------------------------------------*\
    | Set up header and end byte                                    |
    \*-------------------------------------------------------------*/
    serial_buf[0x00]            = 0xC9;
    serial_buf[0x01]            = 0xDA;
    serial_buf[0x02]            = (payload_size >> 8);
    serial_buf[0x03]            = (payload_size & 0xFF);
    serial_buf[packet_size - 1] = 0x36;

    /*-------------------------------------------------------------*\
    | Copy in color data in RGB order                               |
    \*-------------------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        unsigned int color_offset = color_idx * 3;

        serial_buf[0x04 + color_offset]     = RGBGetRValue(colors[color_idx]);
        serial_buf[0x05 + color_offset]     = RGBGetGValue(colors[color_idx]);
        serial_buf[0x06 + color_offset]     = RGBGetBValue(colors[color_idx]);
    }

    /*-------------------------------------------------------------*\
    | Send the packet                                               |
    \*-------------------------------------------------------------*/
    if (serialport != NULL)
    {
        serialport->serial_write((char *)serial_buf, packet_size);
    }

    delete[] serial_buf;
}

void LEDStripController::SetLEDsBasicI2C(std::vector<RGBColor> colors)
{
    unsigned char serial_buf[30];

    /*-------------------------------------------------------------*\
    | Basic I2C Protocol                                            |
    |                                                               |
    |   Packet size: At most 32 bytes (SMBus block size)            |
    |                                                               |
    |   Packet is in RGBRGBRGB... format, also provide start index  |
    \*-------------------------------------------------------------*/

    unsigned char index  = 0;
    unsigned char offset = 0;

    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        serial_buf[index + 0] = RGBGetRValue(colors[color_idx]);
        serial_buf[index + 1] = RGBGetGValue(colors[color_idx]);
        serial_buf[index + 2] = RGBGetBValue(colors[color_idx]);

        index += 3;

        if(index >= 30)
        {
            if(i2cport != NULL)
            {
                i2cport->i2c_smbus_write_i2c_block_data(i2c_addr, offset, 30, serial_buf);
                offset += 30;
                index = 0;
            }
        }
    }

    if(index > 0)
    {
        if(i2cport != NULL)
        {
            i2cport->i2c_smbus_write_i2c_block_data(i2c_addr, offset, index, serial_buf);
        }
    }

    if(i2cport != NULL)
    {
        i2cport->i2c_smbus_write_byte(i2c_addr, 0xFF);
    }
}
