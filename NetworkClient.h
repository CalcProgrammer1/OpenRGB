#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#pragma once

class NetworkClient
{
public:
    NetworkClient(std::vector<RGBController *>& control);

    void        ConnectionThread();
    void        ListenThread();

    void        ProcessReply_ControllerCount(unsigned int data_size, char * data);
    void        ProcessReply_ControllerData(unsigned int data_size, char * data, unsigned int dev_idx);
    
    void        SendRequest_ControllerCount();
    void        SendRequest_ControllerData(unsigned int dev_idx);

    void        SendRequest_RGBController_ResizeZone(unsigned int dev_idx, int zone, int new_size);

    void        SendRequest_RGBController_UpdateLEDs(unsigned int dev_idx);
    void        SendRequest_RGBController_UpdateZoneLEDs(unsigned int dev_idx, int zone);
    void        SendRequest_RGBController_UpdateSingleLED(unsigned int dev_idx, int led);

    void        SendRequest_RGBController_SetCustomMode(unsigned int dev_idx);

    void        SendRequest_RGBController_UpdateMode(unsigned int dev_idx);

protected:
    std::vector<RGBController *>& controllers;
    std::vector<RGBController *>  server_controllers;

private:
    net_port        port;
    bool            server_connected;
    unsigned int    server_controller_count;
};