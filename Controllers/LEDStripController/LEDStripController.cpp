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

void LEDStripController::Initialize(char* ledstring)
{
    LPSTR   numleds = NULL;
    LPSTR   source = NULL;
    LPSTR   udpport_baud = NULL;
    LPSTR   next = NULL;
    
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
    unsigned char *serial_buf;

    serial_buf = new unsigned char[(num_leds * 3) + 3];

    serial_buf[0] = 0xAA;

    for (int idx = 0; idx < (num_leds * 3); idx += 3)
    {
        int pixel_idx = idx / 3;
        RGBColor color = colors[pixel_idx];
        serial_buf[idx + 1] = RGBGetRValue(color);
        serial_buf[idx + 2] = RGBGetGValue(color);
        serial_buf[idx + 3] = RGBGetBValue(color);
    }

    unsigned short sum = 0;

    for (int i = 0; i < (num_leds * 3) + 1; i++)
    {
        sum += serial_buf[i];
    }

    serial_buf[(num_leds * 3) + 1] = sum >> 8;
    serial_buf[(num_leds * 3) + 2] = sum & 0x00FF;

    if (serialport != NULL)
    {
        serialport->serial_write((char *)serial_buf, (num_leds * 3) + 3);
    }
    else if (udpport != NULL)
    {
        udpport->udp_write((char *)serial_buf, (num_leds * 3) + 3);
    }

    delete[] serial_buf;
}
