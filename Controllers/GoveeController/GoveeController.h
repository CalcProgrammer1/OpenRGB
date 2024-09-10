/*---------------------------------------------------------*\
|  GoveeController.h                                        |
|                                                           |
|  Definitions for Govee controller                         |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/1/2023        |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "net_port.h"

#include <string>
#include <thread>
#include <vector>

class GoveeController
{
public:
    GoveeController(std::string ip);
    ~GoveeController();

    std::string GetLocation();
    std::string GetSku();
    std::string GetVersion();

    void ReceiveThreadFunction();

    void SendRazerData(RGBColor* colors, unsigned int size);
    void SendRazerDisable();
    void SendRazerEnable();

    void SendScan();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness);

private:
    std::string         firmware_version;
    std::string         ip_address;
    std::string         module_name;
    std::string         module_mac;

    std::string         sku;
    std::string         bleVersionHard;
    std::string         bleVersionSoft;
    std::string         wifiVersionHard;
    std::string         wifiVersionSoft;

    net_port            port;
    net_port            broadcast_port;

    std::thread*        ReceiveThread;
    std::atomic<bool>   ReceiveThreadRun;
};
