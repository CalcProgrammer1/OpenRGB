#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#include <thread>

#pragma once

class NetworkServer
{
public:
    NetworkServer(std::vector<RGBController *>& control);

    unsigned short  GetPort();
    bool            GetOnline();

    void            SetPort(unsigned short new_port);

    void            StartServer();
    void            StopServer();

    void            ConnectionThreadFunction();
    void            ListenThreadFunction(SOCKET * client_sock);

    void        SendReply_ControllerCount(SOCKET * client_sock);
    void        SendReply_ControllerData(SOCKET * client_sock, unsigned int dev_idx);

protected:
    unsigned short                  port_num;
    bool                            server_online;

    std::vector<RGBController *>&   controllers;

    std::vector<SOCKET *>           ServerClients;
    std::vector<std::thread *>      ListenThreads;
    std::thread *                   ConnectionThread;

private:
    net_port    port;

    int             recv_select(SOCKET s, char *buf, int len, int flags);
};
