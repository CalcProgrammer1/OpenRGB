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

    source = strtok_s(ledstring, ",", &next);

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

    //Initialize with custom baud rate
    InitializeSerial(source, atoi(udpport_baud));

    if (numleds != NULL && strlen(numleds))
    {
        num_leds = atoi(numleds);
    }
}

void LEDStripController::InitializeSerial(char* portname, int baud)
{
    portname = strtok(portname, "\r");
    strcpy(port_name, portname);
    baud_rate = baud;
    serialport = new serial_port(port_name, baud_rate);
}

void LEDStripController::InitializeUDP(char * clientname, char * port)
{
    strcpy(client_name, clientname);
    strcpy(port_name, port);

    //udpport = new net_port(client_name, port_name);
    serialport = NULL;
}

void LEDStripController::InitializeEspurna(char * clientname, char * port, char * apikey)
{
    strcpy(client_name, clientname);
    strcpy(port_name, port);
    strcpy(espurna_apikey, apikey);
    //tcpport = new net_port;
    serialport = NULL;
}

char* LEDStripController::GetLEDString()
{
    return(led_string);
}

void LEDStripController::SetLEDs(std::vector<RGBColor> colors)
{
    if (serialport != NULL )
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
            serialport->serial_flush_tx();
        }

        delete[] serial_buf;
    }
    else
    {
        //SetLEDsEspurna(pixels);
    }
}