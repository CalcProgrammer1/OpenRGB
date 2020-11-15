/*---------------------------------------------------------*\
|  Definitions for Philips Wiz                              |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 11/3/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "net_port.h"

#include <string>
#include <thread>
#include <vector>

#pragma once

class PhilipsWizController
{
public:
    PhilipsWizController(std::string ip);
    ~PhilipsWizController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

    void ReceiveThreadFunction();
    void RequestSystemConfig();

private:
    std::string         firmware_version;
    std::string         module_name;
    std::string         module_mac;
    std::string         location;
    net_port            port;
    std::thread*        ReceiveThread;
    std::atomic<bool>   ReceiveThreadRun;
};
