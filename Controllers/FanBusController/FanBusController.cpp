/*---------------------------------------------------------*\
| FanBusController.cpp                                      |
|                                                           |
|   Driver for FanBus devices                               |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "FanBusController.h"

FanBusController::FanBusController(FanBusInterface* bus_ptr, unsigned char dev_addr)
{
    bus = bus_ptr;
    dev = dev_addr;
}

FanBusController::~FanBusController()
{
    delete bus;
}

std::string FanBusController::GetLocation()
{
    std::string location_string;

    location_string = "FanBus: ";
    location_string.append(bus->GetPort());
    location_string.append(":");
    location_string.append(std::to_string(dev));

    return(location_string);
}

void FanBusController::SetLEDs(std::vector<RGBColor> colors)
{
    for(unsigned int led_idx = 0; led_idx < 4; led_idx++)
    {
        unsigned char red = RGBGetRValue(colors[led_idx]);
        unsigned char grn = RGBGetGValue(colors[led_idx]);
        unsigned char blu = RGBGetBValue(colors[led_idx]);

        bus->write_queue(dev, 0x10 + (led_idx * 3), red);
        bus->write_queue(dev, 0x11 + (led_idx * 3), grn);
        bus->write_queue(dev, 0x12 + (led_idx * 3), blu);
    }

    bus->write_queue(dev, 0x0C,  0x01);

    bus->process_queue();
}
