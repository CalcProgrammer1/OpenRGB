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
    void        ProcessReply_ControllerData(unsigned int data_size, char * data);
    
    void        SendRequest_ControllerCount();
    void        SendRequest_ControllerData(unsigned int idx);

protected:
    std::vector<RGBController *>& controllers;
    std::vector<RGBController *>  server_controllers;

private:
    net_port        port;
    bool            server_connected;
    unsigned int    server_controller_count;
};