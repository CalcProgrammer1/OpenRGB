/*---------------------------------------------------------*\
|  Definitions for NZXT Hue+                                |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/11/2016       |
\*---------------------------------------------------------*/

#ifndef LED_STRIP_H
#define LED_STRIP_H

#include "RGBController.h"
#include "serial_port.h"
#include <vector>


#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
#endif

class HuePlusController
{
public:
    HuePlusController();
    ~HuePlusController();

    void Initialize(char* ledstring);
    char* GetLEDString();
    void SetLEDs(std::vector<RGBColor> colors);

    int num_leds;

private:
    int baud_rate;
    int fans;
    int channel;
    const int hueSize = 125;

    char led_string[1024];
    char port_name[128];
    serial_port *serialport;
};

#endif
