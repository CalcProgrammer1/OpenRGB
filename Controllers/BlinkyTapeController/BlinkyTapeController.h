/*---------------------------------------------------------*\
|  Definitions for BlinkyTape Interface                     |
|                                                           |
|  Matt Mets (matt@blinkinlabs.com), 07/01/2021             |
\*---------------------------------------------------------*/

#ifndef BLINKYTAPE_H
#define BLINKYTAPE_H

#include "RGBController.h"
#include "serial_port.h"

struct BlinkyTapeDevice
{
    std::string     port;
    unsigned int    num_leds;
};

class BlinkyTapeController
{
public:
    BlinkyTapeController();
    ~BlinkyTapeController();

    void        Initialize(const std::string &portname, int led_count);

    char*       GetLEDString();
    std::string GetLocation();

    void        SetLEDs(std::vector<RGBColor> colors);

    int num_leds;

private:
    char led_string[1024];
    std::string port_name;
    serial_port *serialport = nullptr;
};

#endif
