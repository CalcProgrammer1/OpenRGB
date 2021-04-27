/*-----------------------------------------*\
|  NetworkServer.h                          |
|                                           |
|  Server header for OpenRGB SDK            |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/9/2020    |
\*-----------------------------------------*/

#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"
#include "ProfileManager.h"

#include <mutex>
#include <thread>
#include <chrono>

#pragma once

#define TCP_TIMEOUT_SECONDS 5

typedef void (*NetServerCallback)(void *);

class NetworkClientInfo
{
public:
    NetworkClientInfo();
    ~NetworkClientInfo();

    SOCKET          client_sock;
    std::thread *   client_listen_thread;
    std::string     client_string;
    unsigned int    client_protocol_version;
    char            client_ip[INET_ADDRSTRLEN];
};

class NetworkServer
{
public:
    NetworkServer(std::vector<RGBController *>& control);
    ~NetworkServer();

    unsigned short                      GetPort();
    bool                                GetOnline();
    bool                                GetListening();
    unsigned int                        GetNumClients();
    const char *                        GetClientString(unsigned int client_num);
    const char *                        GetClientIP(unsigned int client_num);
    unsigned int                        GetClientProtocolVersion(unsigned int client_num);

    void                                ClientInfoChanged();
    void                                DeviceListChanged();
    void                                RegisterClientInfoChangeCallback(NetServerCallback, void * new_callback_arg);

    void                                ServerListeningChanged();
    void                                RegisterServerListeningChangeCallback(NetServerCallback, void * new_callback_arg);

    void                                SetPort(unsigned short new_port);

    void                                StartServer();
    void                                StopServer();

    void                                ConnectionThreadFunction();
    void                                ListenThreadFunction(NetworkClientInfo * client_sock);

    void                                ProcessRequest_ClientProtocolVersion(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_ClientString(SOCKET client_sock, unsigned int data_size, char * data);

    void                                SendReply_ControllerCount(SOCKET client_sock);
    void                                SendReply_ControllerData(SOCKET client_sock, unsigned int dev_idx, unsigned int protocol_version);
    void                                SendReply_ProtocolVersion(SOCKET client_sock);

    void                                SendRequest_DeviceListChanged(SOCKET client_sock);
    void                                SendReply_ProfileList(SOCKET client_sock);

    void                                SetProfileManager(ProfileManagerInterface* profile_manager_pointer);

protected:
    unsigned short                      port_num;
    bool                                server_online;
    bool                                server_listening;

    std::vector<RGBController *>&       controllers;

    std::mutex                          ServerClientsMutex;
    std::vector<NetworkClientInfo *>    ServerClients;
    std::thread *                       ConnectionThread;

    std::mutex                          ClientInfoChangeMutex;
    std::vector<NetServerCallback>      ClientInfoChangeCallbacks;
    std::vector<void *>                 ClientInfoChangeCallbackArgs;

    std::mutex                          ServerListeningChangeMutex;
    std::vector<NetServerCallback>      ServerListeningChangeCallbacks;
    std::vector<void *>                 ServerListeningChangeCallbackArgs;

    ProfileManagerInterface*            profile_manager;

private:
#ifdef WIN32
    WSADATA     wsa;
#endif

    SOCKET          server_sock;

    int             accept_select(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    int             recv_select(SOCKET s, char *buf, int len, int flags);
};
