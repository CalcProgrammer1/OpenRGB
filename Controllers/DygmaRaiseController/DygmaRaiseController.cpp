/*---------------------------------------------------------*\
| DygmaRaiseController.cpp                                  |
|                                                           |
|   Driver for Dygma Raise keyboard                         |
|                                                           |
|   Timo Schlegel (@eispalast)                  Dec 12 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DygmaRaiseController.h"

using namespace std::chrono_literals;

static int val_char_len(int number)
{
    if(number < 10)
    {
        return 1;
    }
    else if
    (number < 100)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

DygmaRaiseController::DygmaRaiseController()
{

}

DygmaRaiseController::~DygmaRaiseController()
{
    serialport->serial_close();
    delete serialport;
}

void DygmaRaiseController::Initialize(char* port)
{
    port_name = port;

    serialport = new serial_port(port_name.c_str(), DYGMA_RAISE_BAUD);
}

std::string DygmaRaiseController::GetDeviceLocation()
{
    return("COM: " + port_name);
}

void DygmaRaiseController::SendDirect(std::vector<RGBColor>colors, size_t led_num)
{
    char serial_buf[MAX_LEN];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(serial_buf,0x00,sizeof(serial_buf));

    /*-----------------------------------------------------*\
    | Set up led theme packet                               |
    \*-----------------------------------------------------*/
    sprintf(serial_buf,"led.theme");
    int actual_length=9;

    /*-----------------------------------------------------*\
    | Fill packet with color values                         |
    \*-----------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < led_num; led_idx++)
    {
        int r = RGBGetRValue(colors[led_idx]);
        int g = RGBGetGValue(colors[led_idx]);
        int b = RGBGetBValue(colors[led_idx]);

        sprintf(serial_buf+actual_length," %d",r);
        actual_length += val_char_len(r) + 1;

        sprintf(serial_buf+actual_length," %d",g);
        actual_length += val_char_len(g) + 1;

        sprintf(serial_buf+actual_length," %d",b);
        actual_length += val_char_len(b) + 1;
    }

    /*-----------------------------------------------------*\
    | Add the final newline                                 |
    \*-----------------------------------------------------*/
    sprintf(serial_buf+actual_length,"\n");
    actual_length++;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    serialport->serial_write(serial_buf, actual_length);
}
