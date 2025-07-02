/*---------------------------------------------------------*\
| GoveeController.h                                         |
|                                                           |
|   Driver for Govee wireless lighting devices              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      01 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <thread>
#include <vector>
#include "RGBController.h"
#include "net_port.h"

class GoveeController
{
public:
    GoveeController(std::string ip);
    ~GoveeController();

    std::string GetLocation();
    std::string GetSku();
    std::string GetVersion();

    void ReceiveBroadcast(char* recv_buf, int size);

    void SendRazerData(RGBColor* colors, unsigned int size);
    void SendRazerDisable();
    void SendRazerEnable();

    void SendScan();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

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

    bool                broadcast_received;

    net_port            port;

public:
    /*-----------------------------------------------------*\
    | One receive thread is shared among all instances of   |
    | GoveeController, so the receive thread function is    |
    | static and the thread is initialized in the detector  |
    | if any GoveeControllers are created.                  |
    \*-----------------------------------------------------*/
    static net_port                             broadcast_port;
    static std::vector<GoveeController*>        callbacks;
    static std::thread*                         ReceiveThread;
    static std::atomic<bool>                    ReceiveThreadRun;

    static void ReceiveBroadcastThreadFunction();
    static void RegisterReceiveBroadcastCallback(GoveeController* controller_ptr);
    static void UnregisterReceiveBroadcastCallback(GoveeController* controller_ptr);
};
