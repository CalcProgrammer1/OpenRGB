/*---------------------------------------------------------*\
| GoveeController.h                                         |
|                                                           |
|   Driver for Govee wireless lighting devices              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      01 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <mutex>
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
    bool SupportsRazerStatus() const;
    bool RazerInitiallyArmed() const;
    std::string GetDeviceId() const;
    bool GetInitialState(bool& on, unsigned int& brightness, RGBColor& color) const;

    void ReceiveBroadcast(char* recv_buf, int size, const std::string& source_ip);

    void SendRazerData(RGBColor* colors, unsigned int size, bool gradient);
    void SendRazerDisable();
    void SendRazerEnable();

    void SendScan();
    void QueryStatus(const char* command);

    void SetBrightness(unsigned int brightness);
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void SetPower(bool enabled);

private:
    void SendCommand(const std::string& command);

    std::string         firmware_version;
    std::string         ip_address;
    std::string         module_name;
    std::string         module_mac;

    std::string         sku;
    std::string         bleVersionHard;
    std::string         bleVersionSoft;
    std::string         wifiVersionHard;
    std::string         wifiVersionSoft;

    std::atomic<bool>   broadcast_received;
    std::atomic<bool>   razer_status_received;
    std::atomic<bool>   razer_status_supported;
    std::atomic<bool>   razer_initially_armed;
    std::atomic<bool>   device_status_received;
    bool                initial_power;
    unsigned int        initial_brightness;
    RGBColor            initial_color;

    net_port            port;
    bool                port_initialized;
    std::mutex          command_mutex;

public:
    /*-----------------------------------------------------*\
    | One receive thread is shared among all instances of   |
    | GoveeController, so the receive thread function is    |
    | static and the thread is initialized in the detector  |
    | if any GoveeControllers are created.                  |
    \*-----------------------------------------------------*/
    static net_port                             broadcast_port;
    static bool                                 broadcast_initialized;
    static std::mutex                           callbacks_mutex;
    static std::vector<GoveeController*>        callbacks;
    static std::thread*                         ReceiveThread;
    static std::atomic<bool>                    ReceiveThreadRun;

    static void ReceiveBroadcastThreadFunction();
    static void RegisterReceiveBroadcastCallback(GoveeController* controller_ptr);
    static void UnregisterReceiveBroadcastCallback(GoveeController* controller_ptr);
};
