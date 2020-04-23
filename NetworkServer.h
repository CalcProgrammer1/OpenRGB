#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#pragma once

class NetworkServer
{
public:
    NetworkServer(std::vector<RGBController *>& control);

    void        ConnectionThread();
    void        ListenThread(SOCKET * client_sock);

    void        SendReply_ControllerCount(SOCKET * client_sock);
    void        SendReply_ControllerData(SOCKET * client_sock, unsigned int dev_idx);

protected:
    std::vector<RGBController *>& controllers;

private:
    net_port    port;
};