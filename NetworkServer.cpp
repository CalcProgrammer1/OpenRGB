/*---------------------------------------------------------*\
| NetworkServer.cpp                                         |
|                                                           |
|   OpenRGB SDK network server                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <queue>
#include "LogManager.h"
#include "NetworkServer.h"
#include "StringUtils.h"

#ifndef _WIN32
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <arpa/inet.h>
#else
#include <ws2tcpip.h>
#endif
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#define MSG_NOSIGNAL 0
#else
#include <unistd.h>
#endif

#ifdef __linux__
const int yes = 1;
#else
const char yes = 1;
#endif

using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| NetworkServer name for log entries                        |
\*---------------------------------------------------------*/
const char* NETWORKSERVER = "NetworkServer";

NetworkClientInfo::NetworkClientInfo()
{
    client_string           = "Client";
    client_ip               = OPENRGB_SDK_HOST;
    client_sock             = INVALID_SOCKET;
    client_listen_thread    = nullptr;
    client_protocol_version = 0;
    client_is_local         = false;
    client_is_local_client  = false;
}

NetworkClientInfo::~NetworkClientInfo()
{
    if(client_sock != INVALID_SOCKET)
    {
        LOG_INFO("[%s] Closing server connection: %s", NETWORKSERVER, client_ip.c_str());
        delete client_listen_thread;
        shutdown(client_sock, SD_RECEIVE);
        closesocket(client_sock);
    }
}

static void RGBController_UpdateCallback(void * this_ptr, unsigned int update_reason, void * controller_ptr)
{
    NetworkServer * this_obj = (NetworkServer *)this_ptr;

    this_obj->SendRequest_RGBController_SignalUpdate((RGBController *)controller_ptr, update_reason);
}

NetworkServer::NetworkServer()
{
    host                        = OPENRGB_SDK_HOST;
    port_num                    = OPENRGB_SDK_PORT;
    server_online               = false;
    server_listening            = false;
    legacy_workaround_enabled   = false;
    controller_next_idx         = 0;
    controller_updating         = false;
    server_flags                = NET_SERVER_FLAG_SUPPORTS_RGBCONTROLLER
                                | NET_SERVER_FLAG_SUPPORTS_PROFILEMANAGER
                                | NET_SERVER_FLAG_SUPPORTS_PLUGINMANAGER
                                | NET_SERVER_FLAG_SUPPORTS_SETTINGSMANAGER
                                | NET_SERVER_FLAG_SUPPORTS_DETECTION;

    for(int i = 0; i < MAXSOCK; i++)
    {
        ConnectionThread[i]     = nullptr;
    }

    plugin_manager              = nullptr;
    profile_manager             = nullptr;
    settings_manager            = nullptr;
}

NetworkServer::~NetworkServer()
{
    StopServer();

    /*-----------------------------------------------------*\
    | Unregister the server's RGBController update handler  |
    | for each RGBController in the controllers list        |
    \*-----------------------------------------------------*/
    for(std::size_t controller_idx = 0; controller_idx < controllers.size(); controller_idx++)
    {
        controllers[controller_idx]->UnregisterUpdateCallback(this);
    }
}

/*---------------------------------------------------------*\
| Server Information functions                              |
\*---------------------------------------------------------*/
std::string NetworkServer::GetHost()
{
    return host;
}

unsigned short NetworkServer::GetPort()
{
    return port_num;
}

bool NetworkServer::GetOnline()
{
    return server_online;
}

bool NetworkServer::GetListening()
{
    return server_listening;
}

unsigned int NetworkServer::GetNumClients()
{
    return (unsigned int)ServerClients.size();
}

const char * NetworkServer::GetClientString(unsigned int client_num)
{
    const char * result;

    ServerClientsMutex.lock();

    if(client_num < ServerClients.size())
    {
        result = ServerClients[client_num]->client_string.c_str();
    }
    else
    {
        result = "";
    }

    ServerClientsMutex.unlock();

    return result;
}

const char * NetworkServer::GetClientIP(unsigned int client_num)
{
    const char * result;

    ServerClientsMutex.lock();

    if(client_num < ServerClients.size())
    {
        result = ServerClients[client_num]->client_ip.c_str();
    }
    else
    {
        result = "";
    }

    ServerClientsMutex.unlock();

    return result;
}

unsigned int NetworkServer::GetClientProtocolVersion(unsigned int client_num)
{
    unsigned int result;

    ServerClientsMutex.lock();

    if(client_num < ServerClients.size())
    {
        result = ServerClients[client_num]->client_protocol_version;
    }
    else
    {
        result = 0;
    }

    ServerClientsMutex.unlock();

    return result;
}

/*---------------------------------------------------------*\
| Callback functions                                        |
\*---------------------------------------------------------*/
void NetworkServer::SignalResourceManagerUpdate(unsigned int update_reason)
{
    switch(update_reason)
    {
        case RESOURCEMANAGER_UPDATE_REASON_DEVICE_LIST_UPDATED:
            SignalDeviceListUpdated();
            break;

        case RESOURCEMANAGER_UPDATE_REASON_DETECTION_STARTED:
            SignalDetectionStarted();
            break;

        case RESOURCEMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED:
            SignalDetectionProgress();
            break;

        case RESOURCEMANAGER_UPDATE_REASON_DETECTION_COMPLETE:
            SignalDetectionCompleted();
            break;
    }
}

void NetworkServer::RegisterClientInfoChangeCallback(NetServerCallback new_callback, void * new_callback_arg)
{
    ClientInfoChangeCallbacks.push_back(new_callback);
    ClientInfoChangeCallbackArgs.push_back(new_callback_arg);
}

void NetworkServer::RegisterServerListeningChangeCallback(NetServerCallback new_callback, void * new_callback_arg)
{
    ServerListeningChangeCallbacks.push_back(new_callback);
    ServerListeningChangeCallbackArgs.push_back(new_callback_arg);
}

/*---------------------------------------------------------*\
| Server Configuration functions                            |
\*---------------------------------------------------------*/
void NetworkServer::SetHost(std::string new_host)
{
    if(server_online == false)
    {
        host = new_host;
    }
}

void NetworkServer::SetLegacyWorkaroundEnable(bool enable)
{
    legacy_workaround_enabled = enable;
}

void NetworkServer::SetName(std::string new_name)
{
    /*-----------------------------------------------------*\
    | Store the server name                                 |
    \*-----------------------------------------------------*/
    server_name = new_name;

    /*-----------------------------------------------------*\
    | Send server name to all clients                       |
    \*-----------------------------------------------------*/
    for(std::size_t client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        SendReply_ServerString(ServerClients[client_idx]->client_sock);
    }
}

void NetworkServer::SetPort(unsigned short new_port)
{
    if(server_online == false)
    {
        port_num = new_port;
    }
}

/*---------------------------------------------------------*\
| Server Control functions                                  |
\*---------------------------------------------------------*/
void NetworkServer::StartServer()
{
    int err;
    struct addrinfo hints, *res, *result;

    /*-----------------------------------------------------*\
    | Start a TCP server and launch threads                 |
    \*-----------------------------------------------------*/
    char port_str[6];
    snprintf(port_str, 6, "%d", port_num);

    socket_count = 0;

    /*-----------------------------------------------------*\
    | Windows requires WSAStartup before using sockets      |
    \*-----------------------------------------------------*/
#ifdef WIN32
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return;
    }
#endif

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    err = getaddrinfo(host.c_str(), port_str, &hints, &result);

    if(err)
    {
        LOG_ERROR("[%s] Unable to get address.", NETWORKSERVER);
        WSACleanup();
        return;
    }

    /*-----------------------------------------------------*\
    | Create a server socket for each address returned.     |
    \*-----------------------------------------------------*/
    for(res = result; res && socket_count < MAXSOCK; res = res->ai_next)
    {
        server_sock[socket_count] = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if(server_sock[socket_count] == INVALID_SOCKET)
        {
            LOG_ERROR("[%s] Network socket could not be created.", NETWORKSERVER);
            WSACleanup();
            return;
        }

        /*-------------------------------------------------*\
        | Set socket options - reuse addr                   |
        \*-------------------------------------------------*/
        setsockopt(server_sock[socket_count], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

        /*-------------------------------------------------*\
        | Bind the server socket                            |
        \*-------------------------------------------------*/
        if(bind(server_sock[socket_count], res->ai_addr, res->ai_addrlen) == SOCKET_ERROR)
        {
            if(errno == EADDRINUSE)
            {
                LOG_ERROR("[%s] Could not bind network socket. Is port %hu already being used?", NETWORKSERVER, GetPort());
            }
            else if(errno == EACCES)
            {
                LOG_ERROR("[%s] Could not bind network socket. Access to socket was denied.", NETWORKSERVER);
            }
            else if(errno == EBADF)
            {
                LOG_ERROR("[%s] Could not bind network socket. sockfd is not a valid file descriptor.", NETWORKSERVER);
            }
            else if(errno == EINVAL)
            {
                LOG_ERROR("[%s] Could not bind network socket. The socket is already bound to an address, or addrlen is wrong, or addr is not a valid address for this socket's domain.", NETWORKSERVER);
            }
            else if(errno == ENOTSOCK)
            {
                LOG_ERROR("[%s] Could not bind network socket. The file descriptor sockfd does not refer to a socket.", NETWORKSERVER);
            }
            else
            {
                /*---------------------------------------------------------*\
                | errno could be a Linux specific error, see:               |
                | https://man7.org/linux/man-pages/man2/bind.2.html         |
                \*---------------------------------------------------------*/
                LOG_ERROR("[%s] Could not bind network socket. Error code: %d.", NETWORKSERVER, errno);
            }

            WSACleanup();
            return;
        }

        /*-------------------------------------------------*\
        | Set socket options - no delay                     |
        \*-------------------------------------------------*/
        setsockopt(server_sock[socket_count], IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

        socket_count += 1;
    }

    freeaddrinfo(result);
    server_online = true;

    /*-----------------------------------------------------*\
    | Start the connection thread                           |
    \*-----------------------------------------------------*/
    for(int curr_socket = 0; curr_socket < socket_count; curr_socket++)
    {
        ConnectionThread[curr_socket] = new std::thread(&NetworkServer::ConnectionThreadFunction, this, curr_socket);
        ConnectionThread[curr_socket]->detach();
    }
}

void NetworkServer::StopServer()
{
    int curr_socket;
    server_online = false;

    ServerClientsMutex.lock();

    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        delete ServerClients[client_idx];
    }

    ServerClients.clear();

    for(curr_socket = 0; curr_socket < socket_count; curr_socket++)
    {
        shutdown(server_sock[curr_socket], SD_RECEIVE);
        closesocket(server_sock[curr_socket]);
    }

    ServerClientsMutex.unlock();

    for(curr_socket = 0; curr_socket < socket_count; curr_socket++)
    {
        if(ConnectionThread[curr_socket])
        {
            delete ConnectionThread[curr_socket];
            ConnectionThread[curr_socket] = nullptr;
        }
    }

    socket_count = 0;

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    SignalClientInfoChanged();
}

/*---------------------------------------------------------*\
| Server Interface functions                                |
\*---------------------------------------------------------*/
void NetworkServer::SetControllers(std::vector<RGBController *> new_controllers)
{
    /*-----------------------------------------------------*\
    | Set the controller list updating flag to pause the    |
    | controller packet processing                          |
    \*-----------------------------------------------------*/
    controller_updating = true;

    /*-----------------------------------------------------*\
    | Lock the controller list mutex                        |
    \*-----------------------------------------------------*/
    controller_ids_mutex.lock();

    /*-----------------------------------------------------*\
    | Update the controllers list                           |
    \*-----------------------------------------------------*/
    controllers = new_controllers;

    /*-----------------------------------------------------*\
    | Create a copy of the current controller IDs list      |
    \*-----------------------------------------------------*/
    std::vector<NetworkControllerID> controller_ids_old  = controller_ids;

    /*-----------------------------------------------------*\
    | Clear the current controller IDs list                 |
    \*-----------------------------------------------------*/
    controller_ids.clear();

    /*-----------------------------------------------------*\
    | Resize controller IDs list to be the same size as the |
    | controllers list                                      |
    \*-----------------------------------------------------*/
    controller_ids.resize(controllers.size());

    /*-----------------------------------------------------*\
    | Loop through the controllers list and find the ID for |
    | each controller, adding it to the IDs list.           |
    \*-----------------------------------------------------*/
    for(std::size_t controller_idx = 0; controller_idx < controller_ids.size(); controller_idx++)
    {
        bool match                                          = false;
        for(std::size_t controller_id_idx = 0; controller_id_idx < controller_ids_old.size(); controller_id_idx++)
        {
            if(controllers[controller_idx] == controller_ids_old[controller_id_idx].controller)
            {
                controller_ids[controller_idx]              = controller_ids_old[controller_id_idx];
                match                                       = true;

                break;
            }
        }

        /*-------------------------------------------------*\
        | If an ID does not already exist, create a new ID  |
        | for this controller                               |
        \*-------------------------------------------------*/
        if(!match)
        {
            NetworkControllerID new_controller_id;

            new_controller_id.controller                    = controllers[controller_idx];
            new_controller_id.id                            = controller_next_idx;

            controller_next_idx++;

            controller_ids[controller_idx]                  = new_controller_id;
        }
    }

    /*-----------------------------------------------------*\
    | Lock the controller threads mutex                     |
    \*-----------------------------------------------------*/
    controller_threads_mutex.lock();

    /*-----------------------------------------------------*\
    | Create a copy of the current controller threads list  |
    \*-----------------------------------------------------*/
    std::vector<NetworkServerControllerThread *> controller_threads_old = controller_threads;

    /*-----------------------------------------------------*\
    | Clear the current controller threads list             |
    \*-----------------------------------------------------*/
    controller_threads.clear();

    /*-----------------------------------------------------*\
    | Resize the controller threads so that there is one    |
    | thread per controller ID                              |
    \*-----------------------------------------------------*/
    controller_threads.resize(controller_ids.size());

    /*-----------------------------------------------------*\
    | Loop through the controller IDs list and find the     |
    | thread for each ID, adding it to the threads list.    |
    \*-----------------------------------------------------*/
    for(std::size_t controller_id_idx = 0; controller_id_idx < controller_ids.size(); controller_id_idx++)
    {
        std::size_t controller_thread_old_idx   = 0;
        bool        match                       = false;

        for(; controller_thread_old_idx < controller_threads_old.size(); controller_thread_old_idx++)
        {
            if(controller_ids[controller_id_idx].id == controller_threads_old[controller_thread_old_idx]->id)
            {
                match = true;

                break;
            }
        }

        /*-----------------------------------------------------*\
        | If an existing thread was found with this ID, copy it |
        | into the new list at the new index.                   |
        \*-----------------------------------------------------*/
        if(match)
        {
            controller_threads[controller_id_idx]           = controller_threads_old[controller_thread_old_idx];
            controller_threads[controller_id_idx]->index    = controller_id_idx;

            controller_threads_old.erase(controller_threads_old.begin() + controller_thread_old_idx);
        }
        /*-----------------------------------------------------*\
        | Otherwise, if an existing thread was not found with   |
        |this ID, create a new one.                             |
        \*-----------------------------------------------------*/
        else
        {
            NetworkServerControllerThread * new_controller_thread = new NetworkServerControllerThread;

            new_controller_thread->id                       = controller_ids[controller_id_idx].id;
            new_controller_thread->index                    = controller_id_idx;
            new_controller_thread->online                   = true;
            new_controller_thread->thread                   = new std::thread(&NetworkServer::ControllerListenThread, this, new_controller_thread);

            controller_threads[controller_id_idx] = new_controller_thread;
        }
    }

    /*-----------------------------------------------------*\
    | Unlock the controller IDs mutex                       |
    \*-----------------------------------------------------*/
    controller_ids_mutex.unlock();

    /*-----------------------------------------------------*\
    | Loop through the remaining threads in the old list    |
    | and shut them down, their controller IDs are no       |
    | longer valid.                                         |
    \*-----------------------------------------------------*/
    for(std::size_t controller_thread_old_idx = 0; controller_thread_old_idx < controller_threads_old.size(); controller_thread_old_idx++)
    {
        controller_threads_old[controller_thread_old_idx]->online   = false;
        controller_threads_old[controller_thread_old_idx]->start_cv.notify_all();
        controller_threads_old[controller_thread_old_idx]->thread->join();
        delete controller_threads_old[controller_thread_old_idx]->thread;
    }

    /*-----------------------------------------------------*\
    | Unlock the controller threads mutex                   |
    \*-----------------------------------------------------*/
    controller_threads_mutex.unlock();

    /*-----------------------------------------------------*\
    | Clear the controller list updating flag to resume the |
    | controller packet processing                          |
    \*-----------------------------------------------------*/
    controller_updating = false;
}

void NetworkServer::SetPluginManager(PluginManagerInterface* plugin_manager_pointer)
{
    plugin_manager = plugin_manager_pointer;
}

void NetworkServer::SetProfileManager(ProfileManagerInterface* profile_manager_pointer)
{
    profile_manager = profile_manager_pointer;
}

void NetworkServer::SetSettingsManager(SettingsManagerInterface* settings_manager_pointer)
{
    settings_manager = settings_manager_pointer;
}

/*---------------------------------------------------------*\
| Server callback signal functions                          |
\*---------------------------------------------------------*/
void NetworkServer::SignalClientInfoChanged()
{
    ClientInfoChangeMutex.lock();

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < ClientInfoChangeCallbacks.size(); callback_idx++)
    {
        ClientInfoChangeCallbacks[callback_idx](ClientInfoChangeCallbackArgs[callback_idx]);
    }

    ClientInfoChangeMutex.unlock();
}

void NetworkServer::SignalDetectionCompleted()
{
    /*-----------------------------------------------------*\
    | Indicate to the clients that detection has completed  |
    \*-----------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        SendRequest_DetectionCompleted(ServerClients[client_idx]->client_sock, ServerClients[client_idx]->client_protocol_version);
    }
}

void NetworkServer::SignalDetectionProgress()
{
    unsigned int detection_percent = ResourceManager::get()->GetDetectionPercent();
    std::string  detection_string  = ResourceManager::get()->GetDetectionString();

    /*-----------------------------------------------------*\
    | Indicate to the clients detection progress changed    |
    \*-----------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        SendRequest_DetectionProgress(ServerClients[client_idx]->client_sock, ServerClients[client_idx]->client_protocol_version, detection_percent, detection_string);
    }
}

void NetworkServer::SignalDetectionStarted()
{
    /*-----------------------------------------------------*\
    | Indicate to the clients that detection has started    |
    \*-----------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        SendRequest_DetectionStarted(ServerClients[client_idx]->client_sock, ServerClients[client_idx]->client_protocol_version);
    }
}

void NetworkServer::SignalDeviceListUpdated()
{
    /*-----------------------------------------------------*\
    | Register the server's RGBController update handler    |
    | for each RGBController in the controllers list        |
    \*-----------------------------------------------------*/
    for(std::size_t controller_idx = 0; controller_idx < controllers.size(); controller_idx++)
    {
        controllers[controller_idx]->RegisterUpdateCallback(RGBController_UpdateCallback, this);
    }

    /*-----------------------------------------------------*\
    | Indicate to the clients that the controller list has  |
    | changed                                               |
    \*-----------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        SendRequest_DeviceListChanged(ServerClients[client_idx]->client_sock);
    }
}

void NetworkServer::SignalServerListeningChanged()
{
    ServerListeningChangeMutex.lock();

    /*-----------------------------------------------------*\
    | Server state has changed, call the callbacks          |
    \*-----------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < ServerListeningChangeCallbacks.size(); callback_idx++)
    {
        ServerListeningChangeCallbacks[callback_idx](ServerListeningChangeCallbackArgs[callback_idx]);
    }

    ServerListeningChangeMutex.unlock();
}

/*---------------------------------------------------------*\
| Server Thread functions                                   |
\*---------------------------------------------------------*/
void NetworkServer::ConnectionThreadFunction(int socket_idx)
{
    /*-----------------------------------------------------*\
    | This thread handles client connections                |
    \*-----------------------------------------------------*/
    LOG_INFO("[%s] Network connection thread started on port %hu", NETWORKSERVER, GetPort());

    while(server_online == true)
    {
        /*-------------------------------------------------*\
        | Create new socket for client connection           |
        \*-------------------------------------------------*/
        NetworkClientInfo * client_info = new NetworkClientInfo();

        /*---------------------------------------------------------*\
        | Listen for incoming client connection on the server       |
        | socket.  This call blocks until a connection is           |
        | established                                               |
        \*---------------------------------------------------------*/
        if(listen(server_sock[socket_idx], 10) < 0)
        {
            LOG_INFO("[%s] Connection thread closed", NETWORKSERVER);
            server_online = false;

            return;
        }

        server_listening = true;
        SignalServerListeningChanged();

        /*-------------------------------------------------*\
        | Accept the client connection                      |
        \*-------------------------------------------------*/
        client_info->client_sock = accept_select((int)server_sock[socket_idx]);

        if(client_info->client_sock < 0)
        {
            LOG_INFO("[%s] Connection thread closed", NETWORKSERVER);
            server_online = false;

            server_listening = false;
            SignalServerListeningChanged();

            return;
        }

        /*---------------------------------------------------------*\
        | Get the new client socket and store it in the clients     |
        | vector                                                    |
        \*---------------------------------------------------------*/
        u_long arg = 0;
        ioctlsocket(client_info->client_sock, FIONBIO, &arg);
        setsockopt(client_info->client_sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

        /*-------------------------------------------------*\
        | Discover the remote hosts IP                      |
        \*-------------------------------------------------*/
        struct sockaddr_storage tmp_addr;
        char ipstr[INET6_ADDRSTRLEN];
        socklen_t len;
        len = sizeof(tmp_addr);
        getpeername(client_info->client_sock, (struct sockaddr*)&tmp_addr, &len);

        if(tmp_addr.ss_family == AF_INET)
        {
            struct sockaddr_in *s_4 = (struct sockaddr_in *)&tmp_addr;
            inet_ntop(AF_INET, &s_4->sin_addr, ipstr, sizeof(ipstr));
            client_info->client_ip = ipstr;

            if(s_4->sin_addr.s_addr == htonl(INADDR_LOOPBACK))
            {
                client_info->client_is_local = true;
            }
        }
        else
        {
            struct sockaddr_in6 *s_6 = (struct sockaddr_in6 *)&tmp_addr;
            inet_ntop(AF_INET6, &s_6->sin6_addr, ipstr, sizeof(ipstr));
            client_info->client_ip = ipstr;

            if(IN6_IS_ADDR_LOOPBACK(&s_6->sin6_addr))
            {
                client_info->client_is_local = true;
            }
        }

        /*---------------------------------------------------------*\
        | We need to lock before the thread could possibly finish   |
        \*---------------------------------------------------------*/
        ServerClientsMutex.lock();

        /*---------------------------------------------------------*\
        | Start a listener thread for the new client socket         |
        \*---------------------------------------------------------*/
        client_info->client_listen_thread = new std::thread(&NetworkServer::ListenThreadFunction, this, client_info);
        client_info->client_listen_thread->detach();

        ServerClients.push_back(client_info);
        ServerClientsMutex.unlock();

        /*-------------------------------------------------*\
        | Client info has changed, call the callbacks       |
        \*-------------------------------------------------*/
        SignalClientInfoChanged();
    }

    LOG_INFO("[%s] Connection thread closed", NETWORKSERVER);
    server_online = false;
    server_listening = false;
    SignalServerListeningChanged();
}

void NetworkServer::ControllerListenThread(NetworkServerControllerThread * this_thread)
{
    while(this_thread->online == true)
    {
        /*-------------------------------------------------*\
        | Stop processing RGBController packet queues if    |
        | the controller list is being updated              |
        \*-------------------------------------------------*/
        if(!controller_updating)
        {
            std::unique_lock<std::mutex> start_lock(this_thread->start_mutex);
            this_thread->start_cv.wait(start_lock);

            while(this_thread->queue.size() > 0)
            {
                NetworkServerControllerThreadQueueEntry queue_entry;

                this_thread->queue_mutex.lock();
                queue_entry = this_thread->queue.front();
                this_thread->queue.pop();
                this_thread->queue_mutex.unlock();

                controller_ids_mutex.lock_shared();

                switch(queue_entry.pkt_id)
                {
                    case NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS:
                        ProcessRequest_RGBController_UpdateLEDs(this_thread->id, (unsigned char *)queue_entry.data, queue_entry.client_protocol_version);
                        break;

                    case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
                        ProcessRequest_RGBController_UpdateZoneLEDs(this_thread->id, (unsigned char *)queue_entry.data, queue_entry.client_protocol_version);
                        break;

                    case NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE:
                        ProcessRequest_RGBController_UpdateSaveMode(this_thread->id, (unsigned char *)queue_entry.data, queue_entry.client_protocol_version, false);
                        break;

                    case NET_PACKET_ID_RGBCONTROLLER_SAVEMODE:
                        ProcessRequest_RGBController_UpdateSaveMode(this_thread->id, (unsigned char *)queue_entry.data, queue_entry.client_protocol_version, true);
                        break;

                    case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE:
                        ProcessRequest_RGBController_UpdateZoneMode(this_thread->id, (unsigned char *)queue_entry.data, queue_entry.client_protocol_version);
                        break;
                }

                controller_ids_mutex.unlock_shared();

                delete[] queue_entry.data;
            }
        }
        else
        {
            std::this_thread::sleep_for(1ms);
        }
    }
}

void NetworkServer::ListenThreadFunction(NetworkClientInfo * client_info)
{
    SOCKET client_sock = client_info->client_sock;

    LOG_INFO("[%s] Network server started", NETWORKSERVER);

    /*-----------------------------------------------------*\
    | This thread handles messages received from clients    |
    \*-----------------------------------------------------*/
    while(server_online == true)
    {
        NetPacketHeader header;
        int             bytes_read  = 0;
        char *          data        = NULL;
        bool            delete_data = true;

        for(unsigned int i = 0; i < 4; i++)
        {
            /*---------------------------------------------*\
            | Read byte of magic                            |
            \*---------------------------------------------*/
            bytes_read = recv_select(client_sock, &header.pkt_magic[i], 1, 0);

            if(bytes_read <= 0)
            {
                LOG_ERROR("[%s] recv_select failed receiving magic, closing listener", NETWORKSERVER);
                goto listen_done;
            }

            /*---------------------------------------------*\
            | Test characters of magic "ORGB"               |
            \*---------------------------------------------*/
            if(header.pkt_magic[i] != openrgb_sdk_magic[i])
            {
                LOG_ERROR("[%s] Invalid magic received", NETWORKSERVER);
                continue;
            }
        }

        /*-------------------------------------------------*\
        | If we get to this point, the magic is correct.    |
        | Read the rest of the header                       |
        \*-------------------------------------------------*/
        bytes_read = 0;
        do
        {
            int tmp_bytes_read = 0;

            tmp_bytes_read = recv_select(client_sock, (char *)&header.pkt_dev_id + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read, 0);

            bytes_read += tmp_bytes_read;

            if(tmp_bytes_read <= 0)
            {
                LOG_ERROR("[%s] recv_select failed receiving header, closing listener", NETWORKSERVER);
                goto listen_done;
            }

        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        /*-------------------------------------------------*\
        | Header received, now receive the data             |
        \*-------------------------------------------------*/
        bytes_read = 0;
        if(header.pkt_size > 0)
        {
            data = new char[header.pkt_size];

            do
            {
                int tmp_bytes_read = 0;

                tmp_bytes_read = recv_select(client_sock, &data[(unsigned int)bytes_read], header.pkt_size - bytes_read, 0);

                if(tmp_bytes_read <= 0)
                {
                    LOG_ERROR("[%s] recv_select failed receiving data, closing listener", NETWORKSERVER);
                    goto listen_done;
                }
                bytes_read += tmp_bytes_read;

            } while ((unsigned int)bytes_read < header.pkt_size);
        }

        /*-------------------------------------------------*\
        | Entire request received, select functionality     |
        | based on request ID                               |
        \*-------------------------------------------------*/
        switch(header.pkt_id)
        {
        /*-------------------------------------------------*\
        | Network requests                                  |
        \*-------------------------------------------------*/
            case NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                SendReply_ControllerCount(client_sock, client_info->client_protocol_version);
                break;

            case NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                {
                    /*-------------------------------------*\
                    | Protocol versions < 6 sent the client |
                    | protocol version with this request,   |
                    | versions 6+ use the stored negotiated |
                    | protocol version instead.             |
                    \*-------------------------------------*/
                    if(client_info->client_protocol_version < 6)
                    {
                        unsigned int protocol_version = 0;

                        if(header.pkt_size == sizeof(unsigned int))
                        {
                            memcpy(&protocol_version, data, sizeof(unsigned int));
                        }

                        SendReply_ControllerData(client_sock, header.pkt_dev_id, protocol_version);
                    }
                    else
                    {
                        SendReply_ControllerData(client_sock, header.pkt_dev_id, client_info->client_protocol_version);
                    }
                }
                break;

            case NET_PACKET_ID_REQUEST_PROTOCOL_VERSION:
                ProcessRequest_ClientProtocolVersion(client_sock, header.pkt_size, data);
                SendReply_ProtocolVersion(client_sock);
                SendReply_ServerString(client_sock);
                break;

            case NET_PACKET_ID_SET_CLIENT_FLAGS:
                ProcessRequest_ClientFlags(client_sock, header.pkt_size, data);
                SendReply_ServerFlags(client_sock);
                break;

            case NET_PACKET_ID_SET_CLIENT_NAME:
                ProcessRequest_ClientString(client_sock, header.pkt_size, data);
                break;

            case NET_PACKET_ID_REQUEST_RESCAN_DEVICES:
                ProcessRequest_RescanDevices();
                break;

        /*-------------------------------------------------*\
        | ProfileManager functions                          |
        \*-------------------------------------------------*/
            case NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST:
                SendReply_ProfileList(client_sock);
                break;

            case NET_PACKET_ID_PROFILEMANAGER_SAVE_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_name;
                    profile_name.assign(data, header.pkt_size);
                    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

                    profile_manager->SaveProfile(profile_name);
                }
                break;

            case NET_PACKET_ID_PROFILEMANAGER_LOAD_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_name;
                    profile_name.assign(data, header.pkt_size);
                    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

                    profile_manager->LoadProfile(profile_name);
                }
                break;

            case NET_PACKET_ID_PROFILEMANAGER_DELETE_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_name;
                    profile_name.assign(data, header.pkt_size);
                    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

                    profile_manager->DeleteProfile(profile_name);
                }
                break;

            case NET_PACKET_ID_PROFILEMANAGER_UPLOAD_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_json_string;
                    profile_json_string.assign(data, header.pkt_size);
                    profile_json_string = StringUtils::remove_null_terminating_chars(profile_json_string);

                    nlohmann::json profile_json = nlohmann::json::parse(profile_json_string);

                    profile_manager->SaveProfileFromJSON(profile_json);
                }
                break;

            case NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_name;
                    profile_name.assign(data, header.pkt_size);
                    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

                    std::string profile_json_string = profile_manager->ReadProfileJSON(profile_name).dump();

                    NetPacketHeader reply_hdr;

                    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE, (unsigned int)strlen(profile_json_string.c_str()) + 1);

                    send_in_progress.lock();
                    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
                    send(client_sock, (char *)profile_json_string.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
                    send_in_progress.unlock();
                }
                break;

            case NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE:
                {
                    std::string active_profile_name = profile_manager->GetActiveProfile();

                    NetPacketHeader reply_hdr;

                    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE, (unsigned int)strlen(active_profile_name.c_str()) + 1);

                    send_in_progress.lock();
                    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
                    send(client_sock, (char *)active_profile_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
                    send_in_progress.unlock();
                }
                break;

        /*-------------------------------------------------*\
        | PluginManager functions                           |
        \*-------------------------------------------------*/
            case NET_PACKET_ID_PLUGINMANAGER_GET_PLUGIN_LIST:
                SendReply_PluginList(client_sock);
                break;

            case NET_PACKET_ID_PLUGINMANAGER_PLUGIN_SPECIFIC:
                if(plugin_manager)
                {
                    unsigned int    plugin_pkt_id   = *((unsigned int*)(data));
                    unsigned int    plugin_pkt_size = header.pkt_size - (sizeof(unsigned int));
                    unsigned char*  plugin_data     = (unsigned char*)(data + sizeof(unsigned int));
                    unsigned char*  output          = plugin_manager->OnSDKCommand(header.pkt_dev_id, plugin_pkt_id, plugin_data, &plugin_pkt_size);

                    if(output != nullptr)
                    {
                        SendReply_PluginSpecific(client_sock, plugin_pkt_id, output, plugin_pkt_size);
                    }
                }
                break;

        /*-------------------------------------------------*\
        | SettingsManager functions                         |
        \*-------------------------------------------------*/
            case NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS:
                if(data == NULL)
                {
                    break;
                }

                if(settings_manager != NULL)
                {
                    std::string settings_key;
                    settings_key.assign(data, header.pkt_size);
                    settings_key = StringUtils::remove_null_terminating_chars(settings_key);

                    nlohmann::json settings_json = settings_manager->GetSettings(settings_key);
                    std::string settings_json_str = settings_json.dump();

                    NetPacketHeader reply_hdr;

                    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS, (unsigned int)strlen(settings_json_str.c_str()) + 1);

                    send_in_progress.lock();
                    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
                    send(client_sock, (char *)settings_json_str.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
                    send_in_progress.unlock();
                }
                break;

            case NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS:
                if(data == NULL)
                {
                    break;
                }

                if(settings_manager != NULL)
                {
                    std::string settings_json_str;
                    settings_json_str.assign(data, header.pkt_size);
                    settings_json_str = StringUtils::remove_null_terminating_chars(settings_json_str);

                    settings_manager->SetSettingsFromJsonString(settings_json_str);
                }
                break;

            case NET_PACKET_ID_SETTINGSMANAGER_SAVE_SETTINGS:
                if(settings_manager != NULL)
                {
                    settings_manager->SaveSettings();
                }
                break;

        /*-------------------------------------------------*\
        | RGBController functions                           |
        \*-------------------------------------------------*/
            case NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE:
                if((data != NULL)
                && (header.pkt_size == (2 * sizeof(int))))
                {
                    ProcessRequest_RGBController_ResizeZone(header.pkt_dev_id, (unsigned char *)data, client_info->client_protocol_version);
                }
                else
                {
                    LOG_ERROR("[%s] ResizeZone packet has invalid size. Packet size: %d", header.pkt_size, NETWORKSERVER);
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS:
            case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
            case NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE:
            case NET_PACKET_ID_RGBCONTROLLER_SAVEMODE:
            case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE:
                /*-----------------------------------------*\
                | Verify the color description size (first  |
                | 4 bytes of data) matches the packet size  |
                | in the header                             |
                |                                           |
                | If protocol version is 4 or below and the |
                | legacy SDK compatibility workaround is    |
                | enabled, ignore this check.  This allows  |
                | backwards compatibility with old versions |
                | of SDK applications that didn't properly  |
                | implement the size field.                 |
                \*-----------------------------------------*/
                if((data != NULL)
                && ((header.pkt_size == *((unsigned int*)data))
                 || ((client_info->client_protocol_version <= 4)
                  && (legacy_workaround_enabled))))
                {
                    /*-------------------------------------*\
                    | Find the controller thread matching   |
                    | this ID                               |
                    \*-------------------------------------*/
                    std::size_t controller_thread_idx   = 0;
                    bool        match                   = false;

                    controller_threads_mutex.lock_shared();

                    for(; controller_thread_idx < controller_threads.size(); controller_thread_idx++)
                    {
                        if(client_info->client_protocol_version >= 6)
                        {
                            if(controller_threads[controller_thread_idx]->id == header.pkt_dev_id)
                            {
                                match = true;
                                break;
                            }
                        }
                        else
                        {
                            if(controller_threads[controller_thread_idx]->index == header.pkt_dev_id)
                            {
                                match = true;
                                break;
                            }
                        }
                    }

                    /*-------------------------------------*\
                    | If this ID exists in the list, queue  |
                    | the message to the matching thread    |
                    \*-------------------------------------*/
                    if(match)
                    {
                        controller_threads[controller_thread_idx]->queue_mutex.lock();

                        NetworkServerControllerThreadQueueEntry new_entry;
                        new_entry.data                      = data;
                        new_entry.pkt_id                    = header.pkt_id;
                        new_entry.size                      = header.pkt_size;
                        new_entry.client_protocol_version   = client_info->client_protocol_version;

                        controller_threads[controller_thread_idx]->queue.push(new_entry);
                        controller_threads[controller_thread_idx]->queue_mutex.unlock();
                        controller_threads[controller_thread_idx]->start_cv.notify_all();

                        delete_data = false;
                    }

                    controller_threads_mutex.unlock_shared();
                }
                else
                {
                    LOG_ERROR("[%s] RGBController packet with ID %d has invalid size. Packet size: %d,", header.pkt_id, header.pkt_size, NETWORKSERVER);
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED:
                /*-----------------------------------------*\
                | Verify the single LED color description   |
                | size (8 bytes) matches the packet size in |
                | the header                                |
                \*-----------------------------------------*/
                if((data != NULL)
                && (header.pkt_size == (sizeof(int) + sizeof(RGBColor))))
                {
                    ProcessRequest_RGBController_UpdateSingleLED(header.pkt_dev_id, (unsigned char *)data, client_info->client_protocol_version);
                }
                else
                {
                    LOG_ERROR("[%s] UpdateSingleLED packet has invalid size. Packet size: %d", header.pkt_size, NETWORKSERVER);
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE:
                ProcessRequest_RGBController_SetCustomMode(header.pkt_dev_id, client_info->client_protocol_version);
                break;

            case NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS:
                /*-----------------------------------------*\
                | Verify the data size                      |
                \*-----------------------------------------*/
                if((data != NULL)
                && (header.pkt_size == sizeof(int)))
                {
                    ProcessRequest_RGBController_ClearSegments(header.pkt_dev_id, (unsigned char *)data, client_info->client_protocol_version);
                }
                else
                {
                    LOG_ERROR("[%s] ClearSegments packet has invalid size. Packet size: %d", header.pkt_size, NETWORKSERVER);
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT:
                /*-----------------------------------------*\
                | Verify the segment description size       |
                | (first 4 bytes of data) matches the       |
                | packet size in the header                 |
                \*-----------------------------------------*/
                if((data != NULL)
                && (header.pkt_size >= sizeof(unsigned int))
                && (header.pkt_size == *((unsigned int*)data)))
                {
                    ProcessRequest_RGBController_AddSegment(header.pkt_dev_id, (unsigned char *)data, client_info->client_protocol_version);
                }
                else
                {
                    LOG_ERROR("[%s] AddSegment packet has invalid size. Packet size: %d", header.pkt_size, NETWORKSERVER);
                    goto listen_done;
                }
                break;
        }

        if(delete_data)
        {
            delete[] data;
        }
    }

listen_done:

    ServerClientsMutex.lock();

    for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
    {
        if(ServerClients[this_idx] == client_info)
        {
            delete client_info;
            ServerClients.erase(ServerClients.begin() + this_idx);
            break;
        }
    }

    client_info = nullptr;

    ServerClientsMutex.unlock();

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    SignalClientInfoChanged();
}

/*---------------------------------------------------------*\
| Server Protocol functions                                 |
\*---------------------------------------------------------*/
void NetworkServer::ProcessRequest_ClientFlags(SOCKET client_sock, unsigned int data_size, char * data)
{
    if((data_size == sizeof(unsigned int)) && (data != NULL))
    {
        ServerClientsMutex.lock();
        for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
        {
            if(ServerClients[this_idx]->client_sock == client_sock)
            {
                ServerClients[this_idx]->client_flags = *(unsigned int *)data;
                break;
            }
        }

        ServerClientsMutex.unlock();

        /*-----------------------------------------------------*\
        | Client info has changed, call the callbacks           |
        \*-----------------------------------------------------*/
        SignalClientInfoChanged();
    }
}

void NetworkServer::ProcessRequest_ClientProtocolVersion(SOCKET client_sock, unsigned int data_size, char * data)
{
    unsigned int protocol_version = 0;

    if(data_size == sizeof(unsigned int) && (data != NULL))
    {
        memcpy(&protocol_version, data, sizeof(unsigned int));
    }

    if(protocol_version > OPENRGB_SDK_PROTOCOL_VERSION)
    {
        protocol_version = OPENRGB_SDK_PROTOCOL_VERSION;
    }

    ServerClientsMutex.lock();
    for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
    {
        if(ServerClients[this_idx]->client_sock == client_sock)
        {
            ServerClients[this_idx]->client_protocol_version = protocol_version;
            break;
        }
    }
    ServerClientsMutex.unlock();

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    SignalClientInfoChanged();
}

void NetworkServer::ProcessRequest_ClientString(SOCKET client_sock, unsigned int data_size, char * data)
{
    if(data != NULL)
    {
        ServerClientsMutex.lock();
        for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
        {
            if(ServerClients[this_idx]->client_sock == client_sock)
            {
                ServerClients[this_idx]->client_string.assign(data, data_size);
                ServerClients[this_idx]->client_string = StringUtils::remove_null_terminating_chars(ServerClients[this_idx]->client_string);
                break;
            }
        }

        ServerClientsMutex.unlock();

        /*-----------------------------------------------------*\
        | Client info has changed, call the callbacks           |
        \*-----------------------------------------------------*/
        SignalClientInfoChanged();
    }
}

void NetworkServer::ProcessRequest_RescanDevices()
{
    ResourceManager::get()->RescanDevices();
}

void NetworkServer::ProcessRequest_RGBController_AddSegment(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);
    int             zone_idx;

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Skip data size                                        |
    \*-----------------------------------------------------*/
    data_ptr += sizeof(unsigned int);

    /*-----------------------------------------------------*\
    | Copy in zone index                                    |
    \*-----------------------------------------------------*/
    memcpy(&zone_idx, data_ptr, sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*-----------------------------------------------------*\
    | Add new segment                                       |
    \*-----------------------------------------------------*/
    segment new_segment;

    data_ptr = controllers[controller_idx]->SetSegmentDescription(data_ptr, &new_segment, protocol_version);

    controllers[controller_idx]->AddSegment(zone_idx, new_segment);

    /*-----------------------------------------------------*\
    | Save sizes                                            |
    \*-----------------------------------------------------*/
    profile_manager->SaveSizes();
}

void NetworkServer::ProcessRequest_RGBController_ClearSegments(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);
    int             zone_idx;

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Copy in zone index                                    |
    \*-----------------------------------------------------*/
    memcpy(&zone_idx, data_ptr, sizeof(zone_idx));

    /*-----------------------------------------------------*\
    | Call ClearSegments on the given controller            |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->ClearSegments(zone_idx);

    /*-----------------------------------------------------*\
    | Save sizes                                            |
    \*-----------------------------------------------------*/
    profile_manager->SaveSizes();
}

void NetworkServer::ProcessRequest_RGBController_ResizeZone(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);
    int             new_size;
    int             zone_idx;

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Copy in zone index                                    |
    \*-----------------------------------------------------*/
    memcpy(&zone_idx, data_ptr, sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*-----------------------------------------------------*\
    | Copy in new zone size                                 |
    \*-----------------------------------------------------*/
    memcpy(&new_size, data_ptr, sizeof(new_size));

    /*-----------------------------------------------------*\
    | Call ResizeZone on the given controller               |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->ResizeZone(zone_idx, new_size);

    /*-----------------------------------------------------*\
    | Save sizes                                            |
    \*-----------------------------------------------------*/
    profile_manager->SaveSizes();
}

void NetworkServer::ProcessRequest_RGBController_SetCustomMode(unsigned int controller_id, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Call SetCustomMode on the given controller            |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->SetCustomMode();
}

void NetworkServer::ProcessRequest_RGBController_UpdateLEDs(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Skip over data size                                   |
    \*-----------------------------------------------------*/
    data_ptr += sizeof(unsigned int);

    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.lock();

    /*-----------------------------------------------------*\
    | Update colors                                         |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->SetColorDescription(data_ptr, protocol_version);

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | Call UpdateLEDs on the given controller               |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->UpdateLEDs();
}

void NetworkServer::ProcessRequest_RGBController_UpdateSaveMode(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version, bool save_mode)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.lock();

    /*-----------------------------------------------------*\
    | Skip data size                                        |
    \*-----------------------------------------------------*/
    data_ptr += sizeof(unsigned int);

    /*-----------------------------------------------------*\
    | Copy in mode index                                    |
    \*-----------------------------------------------------*/
    int mode_idx;
    memcpy(&mode_idx, data_ptr, sizeof(mode_idx));
    data_ptr += sizeof(mode_idx);

    /*-----------------------------------------------------*\
    | Check if we aren't reading beyond the list of modes.  |
    \*-----------------------------------------------------*/
    if(((size_t) mode_idx) >  controllers[controller_idx]->modes.size())
    {
        /*-------------------------------------------------*\
        | Unlock access mutex                               |
        \*-------------------------------------------------*/
        controllers[controller_idx]->AccessMutex.unlock_shared();
        return;
    }

    /*-----------------------------------------------------*\
    | Set active mode                                       |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->active_mode = mode_idx;

    /*-----------------------------------------------------*\
    | Set mode description                                  |
    \*-----------------------------------------------------*/
    data_ptr = controllers[controller_idx]->SetModeDescription(data_ptr, &controllers[controller_idx]->modes[mode_idx], protocol_version);

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | Call either SaveMode or UpdateMode on the given       |
    | controller                                            |
    \*-----------------------------------------------------*/
    if(save_mode)
    {
        controllers[controller_idx]->SaveMode();
    }
    else
    {
        controllers[controller_idx]->UpdateMode();
    }
}

void NetworkServer::ProcessRequest_RGBController_UpdateSingleLED(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.lock();

    /*-----------------------------------------------------*\
    | Fixed size descrption:                                |
    |       int:      LED index                             |
    |       RGBColor: LED color                             |
    \*-----------------------------------------------------*/
    int led_idx;

    /*-----------------------------------------------------*\
    | Copy in LED index                                     |
    \*-----------------------------------------------------*/
    memcpy(&led_idx, data_ptr, sizeof(led_idx));
    data_ptr += sizeof(led_idx);

    /*-----------------------------------------------------*\
    | Check if we aren't reading beyond the list of leds.   |
    \*-----------------------------------------------------*/
    if(((size_t)led_idx) > controllers[controller_idx]->leds.size())
    {
        /*-------------------------------------------------*\
        | Unlock access mutex                               |
        \*-------------------------------------------------*/
        controllers[controller_idx]->AccessMutex.unlock();
        return;
    }

    /*-----------------------------------------------------*\
    | Copy in LED color                                     |
    \*-----------------------------------------------------*/
    memcpy(&controllers[controller_idx]->colors[led_idx], data_ptr, sizeof(controllers[controller_idx]->colors[led_idx]));
    data_ptr += sizeof(controllers[controller_idx]->colors[led_idx]);

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | Call UpdateSingleLED                                  |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->UpdateSingleLED(led_idx);
}

void NetworkServer::ProcessRequest_RGBController_UpdateZoneLEDs(unsigned int controller_id, unsigned char* data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Skip over data size                                   |
    \*-----------------------------------------------------*/
    data_ptr += sizeof(unsigned int);

    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.lock();

    /*-----------------------------------------------------*\
    | Copy in zone index                                    |
    \*-----------------------------------------------------*/
    unsigned int zone_idx;
    memcpy(&zone_idx, data_ptr, sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*-----------------------------------------------------*\
    | Check if we aren't reading beyond the list of zones.  |
    \*-----------------------------------------------------*/
    if(((size_t)zone_idx) > controllers[controller_idx]->zones.size())
    {
        /*-------------------------------------------------*\
        | Unlock access mutex                               |
        \*-------------------------------------------------*/
        controllers[controller_idx]->AccessMutex.unlock();
        return;
    }

    /*-----------------------------------------------------*\
    | Copy in number of colors                              |
    \*-----------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, data_ptr, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*-----------------------------------------------------*\
    | Copy in colors                                        |
    \*-----------------------------------------------------*/
    if(((size_t)num_colors) > controllers[controller_idx]->zones[zone_idx].leds_count)
    {
        /*-------------------------------------------------*\
        | Unlock access mutex                               |
        \*-------------------------------------------------*/
        controllers[controller_idx]->AccessMutex.unlock();
        return;
    }

    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        memcpy(&controllers[controller_idx]->zones[zone_idx].colors[color_index], data_ptr, sizeof(controllers[controller_idx]->zones[zone_idx].colors[color_index]));
        data_ptr += sizeof(controllers[controller_idx]->zones[zone_idx].colors[color_index]);
    }

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | Call UpdateZoneLEDs                                   |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->UpdateZoneLEDs(zone_idx);
}

void NetworkServer::ProcessRequest_RGBController_UpdateZoneMode(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version)
{
    /*-----------------------------------------------------*\
    | Convert ID to index                                   |
    \*-----------------------------------------------------*/
    bool            idx_valid;
    unsigned int    controller_idx = index_from_id(controller_id, protocol_version, &idx_valid);

    /*-----------------------------------------------------*\
    | If controller ID is invalid, return                   |
    \*-----------------------------------------------------*/
    if(!idx_valid)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Lock access mutex                                     |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.lock();

    /*-----------------------------------------------------*\
    | Skip data size                                        |
    \*-----------------------------------------------------*/
    data_ptr += sizeof(unsigned int);

    /*-----------------------------------------------------*\
    | Copy in mode index                                    |
    \*-----------------------------------------------------*/
    int zone_idx;
    memcpy(&zone_idx, data_ptr, sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*-----------------------------------------------------*\
    | Copy in mode index                                    |
    \*-----------------------------------------------------*/
    int mode_idx;
    memcpy(&mode_idx, data_ptr, sizeof(mode_idx));
    data_ptr += sizeof(mode_idx);

    /*-----------------------------------------------------*\
    | Check if we aren't reading beyond the list of modes.  |
    \*-----------------------------------------------------*/
    if((((size_t)zone_idx) > controllers[controller_idx]->zones.size()) || (mode_idx > (int)controllers[controller_idx]->zones[zone_idx].modes.size()))
    {
        /*-------------------------------------------------*\
        | Unlock access mutex                               |
        \*-------------------------------------------------*/
        controllers[controller_idx]->AccessMutex.unlock_shared();
        return;
    }

    /*-----------------------------------------------------*\
    | Set active mode                                       |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->zones[zone_idx].active_mode = mode_idx;

    /*-----------------------------------------------------*\
    | Set mode description                                  |
    \*-----------------------------------------------------*/
    if(mode_idx >= 0)
    {
        data_ptr = controllers[controller_idx]->SetModeDescription(data_ptr, &controllers[controller_idx]->zones[zone_idx].modes[mode_idx], protocol_version);
    }

    /*-----------------------------------------------------*\
    | Unlock access mutex                                   |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->AccessMutex.unlock();

    /*-----------------------------------------------------*\
    | Update zone mode                                      |
    \*-----------------------------------------------------*/
    controllers[controller_idx]->UpdateZoneMode(zone_idx);
}

void NetworkServer::SendReply_ControllerCount(SOCKET client_sock, unsigned int protocol_version)
{
    controller_ids_mutex.lock_shared();

    NetPacketHeader reply_hdr;
    unsigned int    controller_count    = (unsigned int)controller_ids.size();
    unsigned int    data_size           = 0;

    /*-----------------------------------------------------*\
    | Determine data size                                   |
    \*-----------------------------------------------------*/
    data_size                          += sizeof(controller_count);

    /*-----------------------------------------------------*\
    | Starting with protocol > 6, a list of controller IDs  |
    | is sent in addition to the size                       |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        data_size                      += (controller_count * sizeof(unsigned int));
    }

    unsigned char * data_buf            = new unsigned char[data_size];
    unsigned char * data_ptr            = data_buf;

    memcpy(data_ptr, &controller_count, sizeof(controller_count));
    data_ptr += sizeof(controller_count);

    if(protocol_version >= 6)
    {
        for(unsigned int controller_id_idx = 0; controller_id_idx < controller_count; controller_id_idx++)
        {
            memcpy(data_ptr, &controller_ids[controller_id_idx].id, sizeof(controller_ids[controller_id_idx].id));
            data_ptr += sizeof(controller_ids[controller_id_idx].id);
        }
    }

    controller_ids_mutex.unlock_shared();

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_CONTROLLER_COUNT, data_size);

    controller_count = (unsigned int)controllers.size();

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (const char *)data_buf, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();

    delete[] data_buf;
}

void NetworkServer::SendReply_ControllerData(SOCKET client_sock, unsigned int dev_id, unsigned int protocol_version)
{
    /*---------------------------------------------------------*\
    | Convert ID to index                                       |
    \*---------------------------------------------------------*/
    bool            idx_valid;
    unsigned int    dev_idx = index_from_id(dev_id, protocol_version, &idx_valid);

    if(idx_valid && (dev_idx < controller_ids.size()))
    {
        NetPacketHeader reply_hdr;
        unsigned char * reply_data;
        unsigned int    reply_size;

        /*-----------------------------------------------------*\
        | Lock controller's access mutex                        |
        \*-----------------------------------------------------*/
        controllers[dev_idx]->AccessMutex.lock_shared();

        /*-----------------------------------------------------*\
        | Get data size                                         |
        \*-----------------------------------------------------*/
        reply_size                  = sizeof(reply_size);
        reply_size                 += controllers[dev_idx]->GetDeviceDescriptionSize(protocol_version);

        /*-----------------------------------------------------*\
        | Create data buffer                                    |
        \*-----------------------------------------------------*/
        reply_data                  = new unsigned char[reply_size];
        unsigned char * data_ptr    = reply_data;

        /*-----------------------------------------------------*\
        | Copy in data size                                     |
        \*-----------------------------------------------------*/
        memcpy(data_ptr, &reply_size, sizeof(reply_size));
        data_ptr += sizeof(reply_size);

        /*-----------------------------------------------------*\
        | Copy in data                                          |
        \*-----------------------------------------------------*/
        data_ptr                    = controllers[dev_idx]->GetDeviceDescriptionData(data_ptr, protocol_version);

        /*-----------------------------------------------------*\
        | Unlock controller's access mutex                      |
        \*-----------------------------------------------------*/
        controllers[dev_idx]->AccessMutex.unlock_shared();

        /*-----------------------------------------------------*\
        | Initialize packet header                              |
        \*-----------------------------------------------------*/
        InitNetPacketHeader(&reply_hdr, dev_id, NET_PACKET_ID_REQUEST_CONTROLLER_DATA, reply_size);

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        send_in_progress.lock();
        send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send(client_sock, (const char *)reply_data, reply_size, MSG_NOSIGNAL);
        send_in_progress.unlock();

        /*-----------------------------------------------------*\
        | Delete data                                           |
        \*-----------------------------------------------------*/
        delete[] reply_data;
    }
}

void NetworkServer::SendReply_ProtocolVersion(SOCKET client_sock)
{
    NetPacketHeader reply_hdr;
    unsigned int    reply_data;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_PROTOCOL_VERSION, sizeof(unsigned int));

    reply_data = OPENRGB_SDK_PROTOCOL_VERSION;

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (const char *)&reply_data, sizeof(unsigned int), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkServer::SendReply_ServerFlags(SOCKET client_sock)
{
    /*-----------------------------------------------------*\
    | Send server flags to client only if protocol is 6 or  |
    | greater                                               |
    \*-----------------------------------------------------*/
    ServerClientsMutex.lock();
    for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
    {
        if(ServerClients[this_idx]->client_sock == client_sock)
        {
            if(ServerClients[this_idx]->client_protocol_version >= 6)
            {
                unsigned int    flags_value = server_flags;
                NetPacketHeader reply_hdr;

                /*-----------------------------------------*\
                | If client requested local client, grant   |
                | it if the client is a local connection    |
                \*-----------------------------------------*/
                if((ServerClients[this_idx]->client_flags & NET_CLIENT_FLAG_REQUEST_LOCAL_CLIENT)
                && (ServerClients[this_idx]->client_is_local))
                {
                    ServerClients[this_idx]->client_is_local_client = true;
                    flags_value |= NET_SERVER_FLAG_LOCAL_CLIENT;
                }

                InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_SET_SERVER_FLAGS, sizeof(flags_value));

                send_in_progress.lock();
                send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
                send(client_sock, (char *)&flags_value, reply_hdr.pkt_size, MSG_NOSIGNAL);
                send_in_progress.unlock();
            }
        }
    }
    ServerClientsMutex.unlock();
}

void NetworkServer::SendReply_ServerString(SOCKET client_sock)
{
    /*---------------------------------------------------------*\
    | Send server string to client only if protocol is 6 or     |
    | greater                                                   |
    \*---------------------------------------------------------*/
    ServerClientsMutex.lock();
    for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
    {
        if(ServerClients[this_idx]->client_sock == client_sock)
        {
            if(ServerClients[this_idx]->client_protocol_version >= 6)
            {
                NetPacketHeader reply_hdr;

                InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_SET_SERVER_NAME, (unsigned int)strlen(server_name.c_str()) + 1);

                send_in_progress.lock();
                send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
                send(client_sock, (char *)server_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
                send_in_progress.unlock();
            }
        }
    }
    ServerClientsMutex.unlock();
}

void NetworkServer::SendReply_ProfileList(SOCKET client_sock)
{
    if(!profile_manager)
    {
        return;
    }

    NetPacketHeader reply_hdr;
    unsigned char *reply_data = profile_manager->GetProfileListDescription();
    unsigned int reply_size;

    memcpy(&reply_size, reply_data, sizeof(reply_size));

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST, reply_size);

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (const char *)reply_data, reply_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkServer::SendReply_PluginList(SOCKET client_sock)
{
    unsigned int data_size = 0;
    unsigned int data_ptr = 0;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    unsigned short num_plugins = (unsigned short)plugin_manager->GetPluginCount();

    data_size += sizeof(data_size);
    data_size += sizeof(num_plugins);

    for(unsigned int i = 0; i < num_plugins; i++)
    {
        data_size += sizeof(unsigned short) * 3;
        data_size += (unsigned int)strlen(plugin_manager->GetPluginName(i).c_str()) + 1;
        data_size += (unsigned int)strlen(plugin_manager->GetPluginDescription(i).c_str()) + 1;
        data_size += (unsigned int)strlen(plugin_manager->GetPluginVersion(i).c_str()) + 1;
        data_size += sizeof(unsigned int) * 2;
    }

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in num_plugins                                       |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_plugins, sizeof(num_plugins));
    data_ptr += sizeof(num_plugins);

    for(unsigned int i = 0; i < num_plugins; i++)
    {
        /*---------------------------------------------------------*\
        | Copy in plugin name (size+data)                           |
        \*---------------------------------------------------------*/
        unsigned short str_len = (unsigned short)strlen(plugin_manager->GetPluginName(i).c_str()) + 1;

        memcpy(&data_buf[data_ptr], &str_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], plugin_manager->GetPluginName(i).c_str());
        data_ptr += str_len;

        /*---------------------------------------------------------*\
        | Copy in plugin description (size+data)                    |
        \*---------------------------------------------------------*/
        str_len = (unsigned short)strlen(plugin_manager->GetPluginDescription(i).c_str()) + 1;

        memcpy(&data_buf[data_ptr], &str_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], plugin_manager->GetPluginDescription(i).c_str());
        data_ptr += str_len;

        /*---------------------------------------------------------*\
        | Copy in plugin version (size+data)                        |
        \*---------------------------------------------------------*/
        str_len = (unsigned short)strlen(plugin_manager->GetPluginVersion(i).c_str()) + 1;

        memcpy(&data_buf[data_ptr], &str_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], plugin_manager->GetPluginVersion(i).c_str());
        data_ptr += str_len;

        /*---------------------------------------------------------*\
        | Copy in plugin index (data)                               |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &i, sizeof(unsigned int));
        data_ptr += sizeof(unsigned int);

        /*---------------------------------------------------------*\
        | Copy in plugin sdk version (data)                         |
        \*---------------------------------------------------------*/
        unsigned int protocol_version = plugin_manager->GetPluginProtocolVersion(i);
        memcpy(&data_buf[data_ptr], &protocol_version, sizeof(unsigned int));
        data_ptr += sizeof(unsigned int);
    }

    NetPacketHeader reply_hdr;
    unsigned int reply_size;

    memcpy(&reply_size, data_buf, sizeof(reply_size));

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PLUGINMANAGER_GET_PLUGIN_LIST, reply_size);

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (const char *)data_buf, reply_size, MSG_NOSIGNAL);
    send_in_progress.unlock();

    delete [] data_buf;
}

void NetworkServer::SendReply_PluginSpecific(SOCKET client_sock, unsigned int pkt_id, unsigned char* data, unsigned int data_size)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PLUGINMANAGER_PLUGIN_SPECIFIC, data_size + sizeof(pkt_id));

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (const char *)&pkt_id, sizeof(pkt_id), MSG_NOSIGNAL);
    send(client_sock, (const char *)data, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();

    delete [] data;
}

void NetworkServer::SendRequest_DetectionCompleted(SOCKET client_sock, unsigned int protocol_version)
{
    if(protocol_version >= 6)
    {
        NetPacketHeader pkt_hdr;

        InitNetPacketHeader(&pkt_hdr, 0, NET_PACKET_ID_DETECTION_COMPLETE, 0);

        send_in_progress.lock();
        send(client_sock, (char *)&pkt_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();
    }
}

void NetworkServer::SendRequest_DetectionProgress(SOCKET client_sock, unsigned int protocol_version, unsigned int detection_percent, std::string detection_string)
{
    if(protocol_version >= 6)
    {
        unsigned int    data_size;
        unsigned short  string_length;
        NetPacketHeader reply_hdr;

        string_length   = strlen(detection_string.c_str()) + 1;
        data_size       = sizeof(data_size);
        data_size      += sizeof(detection_percent);
        data_size      += sizeof(string_length);
        data_size      += string_length;

        unsigned char * data_buf = new unsigned char[data_size];
        unsigned char * data_ptr = data_buf;

        memcpy(data_ptr, &data_size, sizeof(data_size));
        data_ptr += sizeof(data_size);

        memcpy(data_ptr, &detection_percent, sizeof(detection_percent));
        data_ptr += sizeof(detection_percent);

        memcpy(data_ptr, &string_length, sizeof(string_length));
        data_ptr += sizeof(string_length);

        memcpy(data_ptr, detection_string.c_str(), string_length);
        data_ptr += string_length;

        InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_DETECTION_PROGRESS_CHANGED, data_size);

        send_in_progress.lock();
        send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send(client_sock, (const char *)data_buf, data_size, MSG_NOSIGNAL);
        send_in_progress.unlock();

        delete [] data_buf;
    }
}

void NetworkServer::SendRequest_DetectionStarted(SOCKET client_sock, unsigned int protocol_version)
{
    if(protocol_version >= 6)
    {
        NetPacketHeader pkt_hdr;

        InitNetPacketHeader(&pkt_hdr, 0, NET_PACKET_ID_DETECTION_STARTED, 0);

        send_in_progress.lock();
        send(client_sock, (char *)&pkt_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();
    }
}

void NetworkServer::SendRequest_DeviceListChanged(SOCKET client_sock)
{
    NetPacketHeader pkt_hdr;

    InitNetPacketHeader(&pkt_hdr, 0, NET_PACKET_ID_DEVICE_LIST_UPDATED, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&pkt_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkServer::SendRequest_RGBController_SignalUpdate(RGBController * controller_ptr, unsigned int update_reason)
{
    /*-----------------------------------------------------*\
    | Find the index of this controller in the list         |
    \*-----------------------------------------------------*/
    std::size_t                 controller_idx      = 0;
    bool                        found               = false;
    unsigned int                controller_id;

    for(; controller_idx < controllers.size(); controller_idx++)
    {
        if(controller_ids[controller_idx].controller == controller_ptr)
        {
            controller_id   = controller_ids[controller_idx].id;
            found           = true;
            break;
        }
    }

    /*-----------------------------------------------------*\
    | Only send SignalUpdate requests if the controller is  |
    | in the list                                           |
    \*-----------------------------------------------------*/
    if(found)
    {
        /*-------------------------------------------------*\
        | Send to each connected client with protocol 6 or  |
        | higher                                            |
        \*-------------------------------------------------*/
        for(std::size_t client_idx = 0; client_idx < ServerClients.size(); client_idx++)
        {
            if(ServerClients[client_idx]->client_protocol_version >= 6)
            {
                SOCKET          client_sock         = ServerClients[client_idx]->client_sock;
                unsigned int    protocol_version    = ServerClients[client_idx]->client_protocol_version;
                NetPacketHeader reply_hdr;
                unsigned char * reply_data;
                unsigned int    reply_size;

                /*-----------------------------------------*\
                | Lock controller's access mutex            |
                \*-----------------------------------------*/
                controller_ptr->AccessMutex.lock_shared();

                /*-----------------------------------------*\
                | Get data size                             |
                \*-----------------------------------------*/
                reply_size                          = sizeof(reply_size);
                reply_size                         += sizeof(update_reason);

                switch(update_reason)
                {
                    /*-------------------------------------*\
                    | UpdateLEDs() sends color description  |
                    \*-------------------------------------*/
                    case RGBCONTROLLER_UPDATE_REASON_UPDATELEDS:
                        reply_size                 += controller_ptr->GetColorDescriptionSize(protocol_version);
                        break;

                    /*-------------------------------------*\
                    | Everything else send controller       |
                    | description                           |
                    \*-------------------------------------*/
                    case RGBCONTROLLER_UPDATE_REASON_UPDATEMODE:
                    case RGBCONTROLLER_UPDATE_REASON_SAVEMODE:
                    case RGBCONTROLLER_UPDATE_REASON_RESIZEZONE:
                    case RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS:
                    case RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT:
                    case RGBCONTROLLER_UPDATE_REASON_HIDDEN:
                    case RGBCONTROLLER_UPDATE_REASON_UNHIDDEN:
                    default:
                        reply_size                 += controller_ptr->GetDeviceDescriptionSize(protocol_version);
                        break;
                }

                /*-----------------------------------------*\
                | Create data buffer                        |
                \*-----------------------------------------*/
                reply_data                          = new unsigned char[reply_size];
                unsigned char * data_ptr            = reply_data;

                /*-----------------------------------------*\
                | Copy in data size                         |
                \*-----------------------------------------*/
                memcpy(data_ptr, &reply_size, sizeof(reply_size));
                data_ptr += sizeof(reply_size);

                /*-----------------------------------------*\
                | Copy in update reason                     |
                \*-----------------------------------------*/
                memcpy(data_ptr, &update_reason, sizeof(update_reason));
                data_ptr += sizeof(update_reason);

                /*-----------------------------------------*\
                | Copy in data                              |
                \*-----------------------------------------*/
                switch(update_reason)
                {
                    /*-------------------------------------*\
                    | UpdateLEDs() sends color description  |
                    \*-------------------------------------*/
                    case RGBCONTROLLER_UPDATE_REASON_UPDATELEDS:
                        data_ptr                    = controller_ptr->GetColorDescriptionData(data_ptr, protocol_version);
                        break;

                    /*-------------------------------------*\
                    | Everything else send controller       |
                    | description                           |
                    \*-------------------------------------*/
                    case RGBCONTROLLER_UPDATE_REASON_UPDATEMODE:
                    case RGBCONTROLLER_UPDATE_REASON_SAVEMODE:
                    case RGBCONTROLLER_UPDATE_REASON_RESIZEZONE:
                    case RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS:
                    case RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT:
                    case RGBCONTROLLER_UPDATE_REASON_HIDDEN:
                    case RGBCONTROLLER_UPDATE_REASON_UNHIDDEN:
                    default:
                        data_ptr                    = controller_ptr->GetDeviceDescriptionData(data_ptr, protocol_version);
                        break;
                }

                /*-----------------------------------------*\
                | Unlock controller's access mutex          |
                \*-----------------------------------------*/
                controller_ptr->AccessMutex.unlock_shared();

                /*-----------------------------------------*\
                | Initialize packet header                  |
                | Since this packet is only supported on    |
                | protocols 6+, we do not need to handle    |
                | index based controller IDs.               |
                \*-----------------------------------------*/
                InitNetPacketHeader(&reply_hdr, controller_id, NET_PACKET_ID_RGBCONTROLLER_SIGNALUPDATE, reply_size);

                /*-----------------------------------------*\
                | Send packet                               |
                \*-----------------------------------------*/
                send_in_progress.lock();
                send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
                send(client_sock, (const char *)reply_data, reply_size, MSG_NOSIGNAL);
                send_in_progress.unlock();

                /*-----------------------------------------*\
                | Delete data                               |
                \*-----------------------------------------*/
                delete[] reply_data;
            }
        }
    }
}

/*---------------------------------------------------------*\
| Private helper functions                                  |
\*---------------------------------------------------------*/
int NetworkServer::accept_select(int sockfd)
{
    fd_set              set;
    struct timeval      timeout;

    while(1)
    {
        timeout.tv_sec          = TCP_TIMEOUT_SECONDS;
        timeout.tv_usec         = 0;

        FD_ZERO(&set);
        FD_SET(sockfd, &set);

        int rv = select(sockfd + 1, &set, NULL, NULL, &timeout);

        if(rv == SOCKET_ERROR || server_online == false)
        {
            return -1;
        }
        else if(rv == 0)
        {
            continue;
        }
        else
        {
            return(accept((int)sockfd, NULL, NULL));
        }
    }
}

unsigned int NetworkServer::index_from_id(unsigned int id, unsigned int protocol_version, bool* index_valid)
{
    /*-----------------------------------------------------*\
    | For protocol < 6, the ID is the index                 |
    \*-----------------------------------------------------*/
    unsigned int index  = id;
    *index_valid        = true;

    /*-----------------------------------------------------*\
    | For protocol >= 6, look up the ID from the index      |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        controller_ids_mutex.lock_shared();

        *index_valid = false;

        for(std::size_t controller_id_idx = 0; controller_id_idx < controller_ids.size(); controller_id_idx++)
        {
            if(controller_ids[controller_id_idx].id == id)
            {
                index           = controller_id_idx;
                *index_valid    = true;
                break;
            }
        }

        controller_ids_mutex.unlock_shared();
    }
    else
    {
        if(index >= controllers.size())
        {
            *index_valid = false;
        }
    }

    return(index);
}

int NetworkServer::recv_select(SOCKET s, char *buf, int len, int flags)
{
    fd_set              set;
    struct timeval      timeout;

    while(1)
    {
        timeout.tv_sec          = TCP_TIMEOUT_SECONDS;
        timeout.tv_usec         = 0;

        FD_ZERO(&set);
        FD_SET(s, &set);

        int rv = select((int)s + 1, &set, NULL, NULL, &timeout);

        if(rv == SOCKET_ERROR || server_online == false)
        {
            return 0;
        }
        else if(rv == 0)
        {
            continue;
        }
        else
        {
        /*-------------------------------------------------*\
        | Set QUICKACK socket option on Linux to improve    |
        | performance                                       |
        \*-------------------------------------------------*/
#ifdef __linux__
            setsockopt(s, IPPROTO_TCP, TCP_QUICKACK, &yes, sizeof(yes));
#endif
            return(recv(s, buf, len, flags));
        }
    }
}
