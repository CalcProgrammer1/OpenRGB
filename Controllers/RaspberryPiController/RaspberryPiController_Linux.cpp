/*---------------------------------------------------------*\
| RaspberryPiController_Linux.cpp                           |
|                                                           |
|   Driver for Raspberry Pi GPIO ARGB                       |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "RaspberryPiController_Linux.h"

RaspberryPiController::RaspberryPiController()
{
    memset(&rpi_ws2811, 0x00, sizeof(rpi_ws2811));

    rpi_ws2811.freq                     = WS2811_TARGET_FREQ;
    rpi_ws2811.dmanum                   = RPI_WS2811_DMA;
    rpi_ws2811.render_wait_time         = 0;

    rpi_ws2811.channel[0].gpionum       = RPI_WS2811_GPIO;
    rpi_ws2811.channel[0].invert        = 0;
    rpi_ws2811.channel[0].count         = 0;
    rpi_ws2811.channel[0].strip_type    = WS2811_STRIP_GBR;
    rpi_ws2811.channel[0].brightness    = 255;

    rpi_ws2811.channel[1].gpionum       = 0;
    rpi_ws2811.channel[1].invert        = 0;
    rpi_ws2811.channel[1].count         = 0;
    rpi_ws2811.channel[1].strip_type    = 0;
    rpi_ws2811.channel[1].brightness    = 0;
}

RaspberryPiController::~RaspberryPiController()
{
    ws2811_fini(&rpi_ws2811);
}

std::string RaspberryPiController::GetDeviceLocation()
{
    return("GPIO: 13, DMA: 10");
}

bool RaspberryPiController::Initialize()
{
    if(ws2811_init(&rpi_ws2811) != WS2811_SUCCESS)
    {
        LOG_ERROR( "RaspberryPi: ws2811_init() failure");
        return(false);
    }

    return(true);
}

void RaspberryPiController::SetColors(RGBColor * colors)
{
    for(int i = 0; i < 2; i++)
    {
        rpi_ws2811.channel[0].leds[i] = colors[i];
    }

    ws2811_render(&rpi_ws2811);
}

void RaspberryPiController::SetSize(unsigned int new_size)
{
    rpi_ws2811.channel[0].count = new_size;

    ws2811_fini(&rpi_ws2811);
    Initialize();
}
