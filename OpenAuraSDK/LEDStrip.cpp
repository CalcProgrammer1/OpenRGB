/*---------------------------------------------------------*\
|  Processing Code for Generic LED Strip Interface          |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/11/2016       |
\*---------------------------------------------------------*/

#include "LEDStrip.h"

#include <fstream>
#include <iostream>
#include <string>

LEDStrip::LEDStrip()
{
    num_leds = 30;
}


LEDStrip::~LEDStrip()
{
}

void LEDStrip::Initialize(char* ledstring, int matrix_size, int matrix_pos, int sections, int rotate_x, bool mirror_x, bool mirror_y, bool single_color)
{
    InitializeSerial(ledstring , 115200);
}

void LEDStrip::InitializeSerial(char* portname, int baud)
{
    portname = strtok(portname, "\r");
    strcpy(port_name, portname);
    baud_rate = baud;
    serialport = new serial_port(port_name, baud_rate);
}

void LEDStrip::InitializeUDP(char * clientname, char * port)
{
    strcpy(client_name, clientname);
    strcpy(port_name, port);

    //udpport = new net_port(client_name, port_name);
    serialport = NULL;
}

void LEDStrip::InitializeEspurna(char * clientname, char * port, char * apikey)
{
    strcpy(client_name, clientname);
    strcpy(port_name, port);
    strcpy(espurna_apikey, apikey);
    //tcpport = new net_port;
    serialport = NULL;
}

char* LEDStrip::GetLEDString()
{
    return(led_string);
}

void LEDStrip::SetNumLEDs(int numleds)
{
    num_leds = numleds;
}

void LEDStrip::SetLEDs(std::vector<unsigned int> colors)
{
    if (serialport != NULL )
    {
        unsigned char *serial_buf;

        serial_buf = new unsigned char[(num_leds * 3) + 3];

        serial_buf[0] = 0xAA;

        for (int idx = 0; idx < (num_leds * 3); idx += 3)
        {
            int pixel_idx = idx / 3;
            unsigned int color = colors[pixel_idx];
            serial_buf[idx + 1] = GetRValue(color);
            serial_buf[idx + 2] = GetGValue(color);
            serial_buf[idx + 3] = GetBValue(color);
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