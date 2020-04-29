#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#include <thread>

#pragma once

class NetworkServer
{
public:
    NetworkServer(std::vector<RGBController *>& control);

    void        ConnectionThreadFunction();
    void        ListenThreadFunction(SOCKET * client_sock);

    void        SendReply_ControllerCount(SOCKET * client_sock);
    void        SendReply_ControllerData(SOCKET * client_sock, unsigned int dev_idx);

protected:
    std::vector<RGBController *>&   controllers;

    std::vector<std::thread *>      ListenThreads;
    std::thread *                   ConnectionThread;

private:
    net_port    port;
};
