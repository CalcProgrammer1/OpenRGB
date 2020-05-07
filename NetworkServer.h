#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#include <thread>

#pragma once

struct NetworkClientInfo
{
    SOCKET          client_sock;
    std::thread *   client_listen_thread;
    std::string     client_string;
    char            client_ip[INET_ADDRSTRLEN];
};

class NetworkServer
{
public:
    NetworkServer(std::vector<RGBController *>& control);

    unsigned short                      GetPort();
    bool                                GetOnline();
    unsigned int                        GetNumClients();
    const char *                        GetClientString(unsigned int client_num);
    const char *                        GetClientIP(unsigned int client_num);

    void                                SetPort(unsigned short new_port);

    void                                StartServer();
    void                                StopServer();

    void                                ConnectionThreadFunction();
    void                                ListenThreadFunction(NetworkClientInfo * client_sock);

    void                                SendReply_ControllerCount(SOCKET client_sock);
    void                                SendReply_ControllerData(SOCKET client_sock, unsigned int dev_idx);

protected:
    unsigned short                      port_num;
    bool                                server_online;

    std::vector<RGBController *>&       controllers;

    std::vector<NetworkClientInfo *>    ServerClients;
    std::thread *                       ConnectionThread;

private:
#ifdef WIN32
    WSADATA     wsa;
#endif

    SOCKET          server_sock;

    int             accept_select(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    int             recv_select(SOCKET s, char *buf, int len, int flags);
};
