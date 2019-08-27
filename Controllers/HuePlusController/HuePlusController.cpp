/*---------------------------------------------------------*\
|  Processing Code for NZXT Hue+                            |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/11/2016       |
\*---------------------------------------------------------*/

#include "HuePlusController.h"

#include <fstream>
#include <iostream>
#include <string>

HuePlusController::HuePlusController()
{

}


HuePlusController::~HuePlusController()
{
}

void HuePlusController::Initialize(char* ledstring)
{
    strcpy(led_string, ledstring);

    LPSTR   source = NULL;
    LPSTR   channels = NULL;
    LPSTR   numleds = NULL;
    LPSTR   next = NULL;

    source = strtok_s(ledstring, ",", &next);

    //Check for selected channel 0=both, 1= Ch.1, 2= Ch.2
    if (strlen(next))
    {
        channels = strtok_s(next, ",", &next);
    }

    switch (atoi(channels))
    {
        case 0:
            channel = 0x00;
            break;

        case 1:
            channel = 0x01;
            break;

        case 2:
            channel = 0x02;
            break;
    }

    //Check for the number of LEDs, sets the corresponding variable with the counter for the fans
    if (strlen(next))
    {
        numleds = strtok_s(next, ",", &next);
    }

    switch (atoi(numleds) / 8)
    {
        case 1:
            fans = 0x00;
            break;

        case 2:
            fans = 0x01;
            break;

        case 3:
            fans = 0x02;
            break;

        case 4:
            fans = 0x03;
            break;

        case 5:
            fans = 0x04;
            break;
    }

    //Initialize with default baud rate
    source = strtok(source, "\r");
    strcpy(port_name, source);
    baud_rate = 256000;
    serialport = new serial_port(port_name, baud_rate);

    if (numleds != NULL && strlen(numleds))
    {
        num_leds = atoi(numleds);
    }
}

char* HuePlusController::GetLEDString()
{
    return(led_string);
}

void HuePlusController::SetLEDs(std::vector<RGBColor> colors)
{
    if (serialport != NULL)
    {
        unsigned char *serial_buf;

        serial_buf = new unsigned char[hueSize];    //Size of Message always 5 XX Blocks (Mode Selection) +  3 XX for each LED (1 color) 
                                                    //-> max of 40 LEDs per Channel (or 5 Fans a 8 LEDs) -> 125 Blocks (empty LEDs are written, too
        serial_buf[0] = 0x4b;
        serial_buf[1] = channel;
        serial_buf[2] = 0x0e;				
        serial_buf[3] = fans; 
        serial_buf[4] = 0x00;

        for (int i = 5; i < hueSize; i++)
        {
            //clearing the buf otherwise sometimes strange things are written to the COM Port
            serial_buf[i] = 0x00;
        }

        for (int idx = 0; idx < (num_leds * 3); idx += 3)
        {
            int pixel_idx = idx / 3;
            RGBColor color = colors[pixel_idx];
            serial_buf[idx + 5] = RGBGetGValue(color);
            serial_buf[idx + 6] = RGBGetRValue(color);
            serial_buf[idx + 7] = RGBGetBValue(color);
        }

        serialport->serial_write((char *)serial_buf, hueSize);
        serialport->serial_flush_tx();

        delete[] serial_buf;
    }

}