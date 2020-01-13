/*---------------------------------------------------------*\
|  Processing Code for NZXT Hue+                            |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/11/2016       |
\*---------------------------------------------------------*/

#include "HuePlusController.h"

#include <fstream>
#include <iostream>
#include <string>


#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

HuePlusController::HuePlusController()
{

}

HuePlusController::~HuePlusController()
{
}

void HuePlusController::Initialize(char* port_name)
{
    strcpy(led_string, port_name);
    
    serialport = new serial_port(port_name, HUE_PLUS_BAUD);

    channel_leds[HUE_PLUS_CHANNEL_1_IDX] = GetLEDsOnChannel(HUE_PLUS_CHANNEL_1);
    channel_leds[HUE_PLUS_CHANNEL_2_IDX] = GetLEDsOnChannel(HUE_PLUS_CHANNEL_2);
}

char* HuePlusController::GetLEDString()
{
    return(led_string);
}

unsigned int HuePlusController::GetLEDsOnChannel(unsigned int channel)
{
    unsigned char serial_buf[] =
    {
        0x8D, 0x00, 0x00, 0x00, 0x00
    };

    unsigned int ret_val = 0;

    /*-----------------------------------------------------*\
    | Set channel in serial packet                          |
    \*-----------------------------------------------------*/
    serial_buf[0x01]   = channel;

    serialport->serial_flush_rx();
    serialport->serial_write((char *)serial_buf, 2);
    serialport->serial_flush_tx();

    Sleep(50);

    int bytes_read = serialport->serial_read((char *)serial_buf, 5);

    if(bytes_read == 5)
    {
        if(serial_buf[3] == 0x01)
        {
            ret_val = serial_buf[4] * 8;
        }
        else
        {
            ret_val += serial_buf[4] * 10;
        }
    }

    return(ret_val);
}

void HuePlusController::SetChannelEffect(unsigned int channel, unsigned int mode, std::vector<RGBColor> colors)
{
    unsigned char serial_buf[] =
    {
        0x4B, 0x00, 0x00, 0x00,
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
        0x00
    };

    /*-----------------------------------------------------*\
    | Set channel in serial packet                          |
    \*-----------------------------------------------------*/
    serial_buf[0x01]   = channel;

    /*-----------------------------------------------------*\
    | Set mode in serial packet                             |
    \*-----------------------------------------------------*/
    serial_buf[0x02]   = mode;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for (std::size_t idx = 0; idx < colors.size(); idx++)
    {
        int pixel_idx = idx * 3;
        RGBColor color = colors[idx];
        serial_buf[pixel_idx + 0x05] = RGBGetGValue(color);
        serial_buf[pixel_idx + 0x06] = RGBGetRValue(color);
        serial_buf[pixel_idx + 0x07] = RGBGetBValue(color);
    }

    serialport->serial_write((char *)serial_buf, HUE_PLUS_PACKET_SIZE);
    serialport->serial_flush_tx();

    Sleep(10);
}

void HuePlusController::SetChannelLEDs(unsigned int channel, std::vector<RGBColor> colors)
{
    unsigned char serial_buf[] =
    {
        0x4B, 0x00, 0x00, 0x00,
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
        0x00
    };

    /*-----------------------------------------------------*\
    | Set channel in serial packet                          |
    \*-----------------------------------------------------*/
    serial_buf[0x01]   = channel;

    /*-----------------------------------------------------*\
    | Set mode in serial packet                             |
    \*-----------------------------------------------------*/
    serial_buf[0x02]   = HUE_PLUS_MODE_FIXED;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for (unsigned int idx = 0; idx < (colors.size() * 3); idx += 3)
    {
        int pixel_idx = idx / 3;
        RGBColor color = colors[pixel_idx];
        serial_buf[idx + 5] = RGBGetGValue(color);
        serial_buf[idx + 6] = RGBGetRValue(color);
        serial_buf[idx + 7] = RGBGetBValue(color);
    }

    serialport->serial_write((char *)serial_buf, HUE_PLUS_PACKET_SIZE);
    serialport->serial_flush_tx();

    Sleep(10);
}
