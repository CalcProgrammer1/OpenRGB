/*---------------------------------------------------------*\
|  Definitions for Espurna Interface                        |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 9/11/2020        |
\*---------------------------------------------------------*/

#ifndef ESPURNA_H
#define ESPURNA_H

#include "RGBController.h"
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

class EspurnaController
{
public:
    EspurnaController();
    ~EspurnaController();

    void        Initialize(char* ledstring);
    void        InitializeEspurna(char* clientname, char* port, char * apikey);

    std::string GetLocation();

    void        SetLEDs(std::vector<RGBColor> colors);

private:
    int baud_rate;

    char led_string[1024];
    std::string port_name;
    std::string client_name;
    char espurna_apikey[128];

    net_port *tcpport;
};

#endif
