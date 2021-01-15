/*---------------------------------------------------------*\
|  Definitions for Generic LED Strip Interface              |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/11/2016       |
\*---------------------------------------------------------*/

#ifndef LED_STRIP_H
#define LED_STRIP_H

#include "RGBController.h"
#include "serial_port.h"
#include "net_port.h"
#include <vector>


#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
#endif

typedef unsigned int    led_protocol;

enum
{
    LED_PROTOCOL_KEYBOARD_VISUALIZER,
    LED_PROTOCOL_ADALIGHT,
    LED_PROTOCOL_TPM2
};

struct LEDStripDevice
{
    std::string     port;
    unsigned int    baud;
    unsigned int    num_leds;
    led_protocol    protocol;
};

class LEDStripController
{
public:
    LEDStripController();
    ~LEDStripController();

    void        Initialize(char* ledstring, led_protocol proto);

    void        InitializeSerial(char* portname, int baud);
    void        InitializeUDP(char* clientname, char* port);

    char*       GetLEDString();
    std::string GetLocation();

    void        SetLEDs(std::vector<RGBColor> colors);

    void        SetLEDsKeyboardVisualizer(std::vector<RGBColor> colors);
    void        SetLEDsAdalight(std::vector<RGBColor> colors);
    void        SetLEDsTPM2(std::vector<RGBColor> colors);

    int num_leds;

private:
    int baud_rate;

    char led_string[1024];
    std::string port_name;
    std::string client_name;
    serial_port *serialport;
    net_port *udpport;
    led_protocol protocol;
};

#endif
