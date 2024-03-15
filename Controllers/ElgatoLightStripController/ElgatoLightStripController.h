/*--------------------------------------------------------*\
|  Definitions for Elgato Light Strip                      |
|                                                          |
|  Monks (imtherealestmonkey@gmail.com),        11/03/2021 |
|  Edit by DomePlaysHD                          12/03/2024 |
\*--------------------------------------------------------*/

#include "net_port.h"
#include "hsv.h"
#include <string>

#pragma once

class ElgatoLightStripController
{
    public:
        ElgatoLightStripController(std::string ip);
        ~ElgatoLightStripController();

        std::string GetLocation();
        std::string GetName();
        std::string GetVersion();
        std::string GetManufacturer();
        std::string GetUniqueID();

        void SetColor(hsv_t hsv_color);
        int GetBrightness();
        void SetBrightness(int brightness);

    private:
        std::string GetRequest(int hue, int saturation, int brightness);
        std::string location;
        std::string firmware_version;
        std::string serialnumber;
        std::string displayname;
        net_port port;
        int device_brightness;
};
