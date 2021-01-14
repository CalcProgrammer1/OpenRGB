/*---------------------------------------------------------*\
|  Processing Code for Generic LED Strip Interface          |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/11/2016       |
\*---------------------------------------------------------*/

#include "LEDStripController.h"

#include <fstream>
#include <iostream>
#include <string>

LEDStripController::LEDStripController()
{

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
        if (udpport_baud == NULL)
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

void LEDStripController::InitializeSerial(char* portname, int baud)
{
    portname = strtok(portname, "\r");
    port_name = portname;
    baud_rate = baud;
    serialport = new serial_port(port_name.c_str(), baud_rate);
    udpport = NULL;
}

void LEDStripController::InitializeUDP(char * clientname, char * port)
{
    client_name = clientname;
    port_name   = port;

    udpport = new net_port(client_name.c_str(), port_name.c_str());
    serialport = NULL;
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
    else
    {
        return("");
    }
}

char* LEDStripController::GetLEDString()
{
    return(led_string);
}

void LEDStripController::SetLEDs(std::vector<RGBColor> colors)
{
    switch(protocol)
    {
        case LED_PROTOCOL_KEYBOARD_VISUALIZER:
            SetLEDsKeyboardVisualizer(colors);
            break;

        case LED_PROTOCOL_ADALIGHT:
            break;

        case LED_PROTOCOL_TPM2:
            SetLEDsTPM2(colors);
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
    unsigned int payload_size   = (colors.size() * 3);
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

    for (int i = 0; i < (payload_size + 1); i++)
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
    unsigned int payload_size   = (colors.size() * 3);
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