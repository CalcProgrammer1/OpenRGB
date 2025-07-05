/*---------------------------------------------------------*\
| NetworkServer.cpp                                         |
|                                                           |
|   OpenRGB SDK network server                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "NetworkServer.h"
#include "LogManager.h"

#ifndef WIN32
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

const char yes = 1;

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std::chrono_literals;

NetworkClientInfo::NetworkClientInfo()
{
    client_string           = "Client";
    client_ip               = OPENRGB_SDK_HOST;
    client_sock             = INVALID_SOCKET;
    client_listen_thread    = nullptr;
    client_protocol_version = 0;
}

NetworkClientInfo::~NetworkClientInfo()
{
    if(client_sock != INVALID_SOCKET)
    {
        LOG_INFO("[NetworkServer] Closing server connection: %s", client_ip.c_str());
        delete client_listen_thread;
        shutdown(client_sock, SD_RECEIVE);
        closesocket(client_sock);
    }
}

NetworkServer::NetworkServer(std::vector<RGBController *>& control) : controllers(control)
{
    host                        = OPENRGB_SDK_HOST;
    port_num                    = OPENRGB_SDK_PORT;
    server_online               = false;
    server_listening            = false;
    legacy_workaround_enabled   = false;

    for(int i = 0; i < MAXSOCK; i++)
    {
        ConnectionThread[i] = nullptr;
    }

    profile_manager  = nullptr;
}

NetworkServer::~NetworkServer()
{
    StopServer();
}

void NetworkServer::ClientInfoChanged()
{
    ClientInfoChangeMutex.lock();

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < ClientInfoChangeCallbacks.size(); callback_idx++)
    {
        ClientInfoChangeCallbacks[callback_idx](ClientInfoChangeCallbackArgs[callback_idx]);
    }

    ClientInfoChangeMutex.unlock();
}

void NetworkServer::DeviceListChanged()
{
    /*---------------------------------------------------------*\
    | Indicate to the clients that the controller list has      |
    | changed                                                   |
    \*---------------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        SendRequest_DeviceListChanged(ServerClients[client_idx]->client_sock);
    }
}

void NetworkServer::ServerListeningChanged()
{
    ServerListeningChangeMutex.lock();

    /*---------------------------------------------------------*\
    | Server state has changed, call the callbacks              |
    \*---------------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < ServerListeningChangeCallbacks.size(); callback_idx++)
    {
        ServerListeningChangeCallbacks[callback_idx](ServerListeningChangeCallbackArgs[callback_idx]);
    }

    ServerListeningChangeMutex.unlock();
}

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

void NetworkServer::SetPort(unsigned short new_port)
{
    if(server_online == false)
    {
        port_num = new_port;
    }
}

void NetworkServer::StartServer()
{
    int err;
    struct addrinfo hints, *res, *result;

    /*---------------------------------------------------------*\
    | Start a TCP server and launch threads                     |
    \*---------------------------------------------------------*/
    char port_str[6];
    snprintf(port_str, 6, "%d", port_num);

    socket_count = 0;

    /*---------------------------------------------------------*\
    | Windows requires WSAStartup before using sockets          |
    \*---------------------------------------------------------*/
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
        LOG_ERROR("[NetworkServer] Unable to get address.");
        WSACleanup();
        return;
    }

    /*---------------------------------------------------------*\
    | Create a server socket for each address returned.         |
    \*---------------------------------------------------------*/
    for(res = result; res && socket_count < MAXSOCK; res = res->ai_next)
    {
        server_sock[socket_count] = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if(server_sock[socket_count] == INVALID_SOCKET)
        {
            LOG_ERROR("[NetworkServer] Network socket could not be created.");
            WSACleanup();
            return;
        }

        /*---------------------------------------------------------*\
        | Bind the server socket                                    |
        \*---------------------------------------------------------*/
        if(bind(server_sock[socket_count], res->ai_addr, res->ai_addrlen) == SOCKET_ERROR)
        {
            if(errno == EADDRINUSE)
            {
                LOG_ERROR("[NetworkServer] Could not bind network socket. Is port %hu already being used?", GetPort());
            }
            else if(errno == EACCES)
            {
                LOG_ERROR("[NetworkServer] Could not bind network socket. Access to socket was denied.");
            }
            else if(errno == EBADF)
            {
                LOG_ERROR("[NetworkServer] Could not bind network socket. sockfd is not a valid file descriptor.");
            }
            else if(errno == EINVAL)
            {
                LOG_ERROR("[NetworkServer] Could not bind network socket. The socket is already bound to an address, or addrlen is wrong, or addr is not a valid address for this socket's domain.");
            }
            else if(errno == ENOTSOCK)
            {
                LOG_ERROR("[NetworkServer] Could not bind network socket. The file descriptor sockfd does not refer to a socket.");
            }
            else
            {
                /*---------------------------------------------------------*\
                | errno could be a Linux specific error, see:               |
                | https://man7.org/linux/man-pages/man2/bind.2.html         |
                \*---------------------------------------------------------*/
                LOG_ERROR("[NetworkServer] Could not bind network socket. Error code: %d.", errno);
            }

            WSACleanup();
            return;
        }

        /*---------------------------------------------------------*\
        | Set socket options - no delay                             |
        \*---------------------------------------------------------*/
        setsockopt(server_sock[socket_count], IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

        socket_count += 1;
    }

    freeaddrinfo(result);
    server_online = true;

    /*---------------------------------------------------------*\
    | Start the connection thread                               |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    ClientInfoChanged();
}

void NetworkServer::ConnectionThreadFunction(int socket_idx)
{
    /*---------------------------------------------------------*\
    | This thread handles client connections                    |
    \*---------------------------------------------------------*/
    LOG_INFO("[NetworkServer] Network connection thread started on port %hu", GetPort());

    while(server_online == true)
    {
        /*---------------------------------------------------------*\
        | Create new socket for client connection                   |
        \*---------------------------------------------------------*/
        NetworkClientInfo * client_info = new NetworkClientInfo();

        /*---------------------------------------------------------*\
        | Listen for incoming client connection on the server       |
        | socket.  This call blocks until a connection is           |
        | established                                               |
        \*---------------------------------------------------------*/
        if(listen(server_sock[socket_idx], 10) < 0)
        {
            LOG_INFO("[NetworkServer] Connection thread closed");
            server_online = false;

            return;
        }

        server_listening = true;
        ServerListeningChanged();

        /*---------------------------------------------------------*\
        | Accept the client connection                              |
        \*---------------------------------------------------------*/
        client_info->client_sock = accept_select((int)server_sock[socket_idx]);

        if(client_info->client_sock < 0)
        {
            LOG_INFO("[NetworkServer] Connection thread closed");
            server_online = false;

            server_listening = false;
            ServerListeningChanged();

            return;
        }

        /*---------------------------------------------------------*\
        | Get the new client socket and store it in the clients     |
        | vector                                                    |
        \*---------------------------------------------------------*/
        u_long arg = 0;
        ioctlsocket(client_info->client_sock, FIONBIO, &arg);
        setsockopt(client_info->client_sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

        /*---------------------------------------------------------*\
        | Discover the remote hosts IP                              |
        \*---------------------------------------------------------*/
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
        }
        else
        {
            struct sockaddr_in6 *s_6 = (struct sockaddr_in6 *)&tmp_addr;
            inet_ntop(AF_INET6, &s_6->sin6_addr, ipstr, sizeof(ipstr));
            client_info->client_ip = ipstr;
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

        /*---------------------------------------------------------*\
        | Client info has changed, call the callbacks               |
        \*---------------------------------------------------------*/
        ClientInfoChanged();
    }

    LOG_INFO("[NetworkServer] Connection thread closed");
    server_online = false;
    server_listening = false;
    ServerListeningChanged();
}

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
            return(recv(s, buf, len, flags));
        }
    }
}

void NetworkServer::ListenThreadFunction(NetworkClientInfo * client_info)
{
    SOCKET client_sock = client_info->client_sock;

    LOG_INFO("[NetworkServer] Network server started");

    /*---------------------------------------------------------*\
    | This thread handles messages received from clients        |
    \*---------------------------------------------------------*/
    while(server_online == true)
    {
        NetPacketHeader header;
        int             bytes_read  = 0;
        char *          data        = NULL;

        for(unsigned int i = 0; i < 4; i++)
        {
            /*---------------------------------------------------------*\
            | Read byte of magic                                        |
            \*---------------------------------------------------------*/
            bytes_read = recv_select(client_sock, &header.pkt_magic[i], 1, 0);

            if(bytes_read <= 0)
            {
                LOG_ERROR("[NetworkServer] recv_select failed receiving magic, closing listener");
                goto listen_done;
            }

            /*---------------------------------------------------------*\
            | Test characters of magic "ORGB"                           |
            \*---------------------------------------------------------*/
            if(header.pkt_magic[i] != openrgb_sdk_magic[i])
            {
                LOG_ERROR("[NetworkServer] Invalid magic received");
                continue;
            }
        }

        /*---------------------------------------------------------*\
        | If we get to this point, the magic is correct.  Read the  |
        | rest of the header                                        |
        \*---------------------------------------------------------*/
        bytes_read = 0;
        do
        {
            int tmp_bytes_read = 0;

            tmp_bytes_read = recv_select(client_sock, (char *)&header.pkt_dev_idx + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read, 0);

            bytes_read += tmp_bytes_read;

            if(tmp_bytes_read <= 0)
            {
                LOG_ERROR("[NetworkServer] recv_select failed receiving header, closing listener");
                goto listen_done;
            }

        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        /*---------------------------------------------------------*\
        | Header received, now receive the data                     |
        \*---------------------------------------------------------*/
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
                    LOG_ERROR("[NetworkServer] recv_select failed receiving data, closing listener");
                    goto listen_done;
                }
                bytes_read += tmp_bytes_read;

            } while ((unsigned int)bytes_read < header.pkt_size);
        }

        /*---------------------------------------------------------*\
        | Entire request received, select functionality based on    |
        | request ID                                                |
        \*---------------------------------------------------------*/
        switch(header.pkt_id)
        {
            case NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                SendReply_ControllerCount(client_sock);
                break;

            case NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                {
                    unsigned int protocol_version = 0;

                    if(header.pkt_size == sizeof(unsigned int))
                    {
                        memcpy(&protocol_version, data, sizeof(unsigned int));
                    }

                    SendReply_ControllerData(client_sock, header.pkt_dev_idx, protocol_version);
                }
                break;

            case NET_PACKET_ID_REQUEST_PROTOCOL_VERSION:
                SendReply_ProtocolVersion(client_sock);
                ProcessRequest_ClientProtocolVersion(client_sock, header.pkt_size, data);
                break;

            case NET_PACKET_ID_SET_CLIENT_NAME:
                if(data == NULL)
                {
                    break;
                }

                ProcessRequest_ClientString(client_sock, header.pkt_size, data);
                break;

            case NET_PACKET_ID_REQUEST_RESCAN_DEVICES:
                ProcessRequest_RescanDevices();
                break;

            case NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE:
                if(data == NULL)
                {
                    break;
                }

                if((header.pkt_dev_idx < controllers.size()) && (header.pkt_size == (2 * sizeof(int))))
                {
                    int zone;
                    int new_size;

                    memcpy(&zone, data, sizeof(int));
                    memcpy(&new_size, data + sizeof(int), sizeof(int));

                    controllers[header.pkt_dev_idx]->ResizeZone(zone, new_size);
                    profile_manager->SaveProfile("sizes", true);
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS:
                if(data == NULL)
                {
                    break;
                }

                /*---------------------------------------------------------*\
                | Verify the color description size (first 4 bytes of data) |
                | matches the packet size in the header                     |
                |                                                           |
                | If protocol version is 4 or below and the legacy SDK      |
                | compatibility workaround is enabled, ignore this check.   |
                | This allows backwards compatibility with old versions of  |
                | SDK applications that didn't properly implement the size  |
                | field.                                                    |
                \*---------------------------------------------------------*/
                if((header.pkt_size == *((unsigned int*)data))
                || ((client_info->client_protocol_version <= 4)
                 && (legacy_workaround_enabled)))
                {
                    if(header.pkt_dev_idx < controllers.size())
                    {
                        controllers[header.pkt_dev_idx]->SetColorDescription((unsigned char *)data);
                        controllers[header.pkt_dev_idx]->UpdateLEDs();
                    }
                }
                else
                {
                    LOG_ERROR("[NetworkServer] UpdateLEDs packet has invalid size. Packet size: %d, Data size: %d", header.pkt_size, *((unsigned int*)data));
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
                if(data == NULL)
                {
                    break;
                }

                /*---------------------------------------------------------*\
                | Verify the color description size (first 4 bytes of data) |
                | matches the packet size in the header                     |
                |                                                           |
                | If protocol version is 4 or below and the legacy SDK      |
                | compatibility workaround is enabled, ignore this check.   |
                | This allows backwards compatibility with old versions of  |
                | SDK applications that didn't properly implement the size  |
                | field.                                                    |
                \*---------------------------------------------------------*/
                if((header.pkt_size == *((unsigned int*)data))
                || ((client_info->client_protocol_version <= 4)
                 && (legacy_workaround_enabled)))
                {
                    if(header.pkt_dev_idx < controllers.size())
                    {
                        int zone;

                        memcpy(&zone, &data[sizeof(unsigned int)], sizeof(int));

                        controllers[header.pkt_dev_idx]->SetZoneColorDescription((unsigned char *)data);
                        controllers[header.pkt_dev_idx]->UpdateZoneLEDs(zone);
                    }
                }
                else
                {
                    LOG_ERROR("[NetworkServer] UpdateZoneLEDs packet has invalid size. Packet size: %d, Data size: %d", header.pkt_size, *((unsigned int*)data));
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED:
                if(data == NULL)
                {
                    break;
                }

                /*---------------------------------------------------------*\
                | Verify the single LED color description size (8 bytes)    |
                | matches the packet size in the header                     |
                \*---------------------------------------------------------*/
                if(header.pkt_size == (sizeof(int) + sizeof(RGBColor)))
                {
                    if(header.pkt_dev_idx < controllers.size())
                    {
                        int led;

                        memcpy(&led, data, sizeof(int));

                        controllers[header.pkt_dev_idx]->SetSingleLEDColorDescription((unsigned char *)data);
                        controllers[header.pkt_dev_idx]->UpdateSingleLED(led);
                    }
                }
                else
                {
                    LOG_ERROR("[NetworkServer] UpdateSingleLED packet has invalid size. Packet size: %d, Data size: %d", header.pkt_size, (sizeof(int) + sizeof(RGBColor)));
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE:
                if(header.pkt_dev_idx < controllers.size())
                {
                    controllers[header.pkt_dev_idx]->SetCustomMode();
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE:
                if(data == NULL)
                {
                    break;
                }

                /*---------------------------------------------------------*\
                | Verify the mode description size (first 4 bytes of data)  |
                | matches the packet size in the header                     |
                |                                                           |
                | If protocol version is 4 or below and the legacy SDK      |
                | compatibility workaround is enabled, ignore this check.   |
                | This allows backwards compatibility with old versions of  |
                | SDK applications that didn't properly implement the size  |
                | field.                                                    |
                \*---------------------------------------------------------*/
                if((header.pkt_size == *((unsigned int*)data))
                || ((client_info->client_protocol_version <= 4)
                 && (legacy_workaround_enabled)))
                {
                    if(header.pkt_dev_idx < controllers.size())
                    {
                        controllers[header.pkt_dev_idx]->SetModeDescription((unsigned char *)data, client_info->client_protocol_version);
                        controllers[header.pkt_dev_idx]->UpdateMode();
                    }
                }
                else
                {
                    LOG_ERROR("[NetworkServer] UpdateMode packet has invalid size. Packet size: %d, Data size: %d", header.pkt_size, *((unsigned int*)data));
                    goto listen_done;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_SAVEMODE:
                if(data == NULL)
                {
                    break;
                }

                /*---------------------------------------------------------*\
                | Verify the mode description size (first 4 bytes of data)  |
                | matches the packet size in the header                     |
                |                                                           |
                | If protocol version is 4 or below and the legacy SDK      |
                | compatibility workaround is enabled, ignore this check.   |
                | This allows backwards compatibility with old versions of  |
                | SDK applications that didn't properly implement the size  |
                | field.                                                    |
                \*---------------------------------------------------------*/
                if((header.pkt_size == *((unsigned int*)data))
                || ((client_info->client_protocol_version <= 4)
                 && (legacy_workaround_enabled)))
                {
                    if(header.pkt_dev_idx < controllers.size())
                    {
                        controllers[header.pkt_dev_idx]->SetModeDescription((unsigned char *)data, client_info->client_protocol_version);
                        controllers[header.pkt_dev_idx]->SaveMode();
                    }
                }
                break;

            case NET_PACKET_ID_REQUEST_PROFILE_LIST:
                SendReply_ProfileList(client_sock);
                break;

            case NET_PACKET_ID_REQUEST_SAVE_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_name;
                    profile_name.assign(data, header.pkt_size);

                    profile_manager->SaveProfile(profile_name);
                }

                break;

            case NET_PACKET_ID_REQUEST_LOAD_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_name;
                    profile_name.assign(data, header.pkt_size);

                    profile_manager->LoadProfile(profile_name);
                }

                for(RGBController* controller : controllers)
                {
                    controller->UpdateLEDs();
                }

                break;

            case NET_PACKET_ID_REQUEST_DELETE_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    std::string profile_name;
                    profile_name.assign(data, header.pkt_size);

                    profile_manager->DeleteProfile(profile_name);
                }

                break;

            case NET_PACKET_ID_REQUEST_PLUGIN_LIST:
                SendReply_PluginList(client_sock);
                break;

            case NET_PACKET_ID_PLUGIN_SPECIFIC:
                {
                    unsigned int plugin_pkt_type = *((unsigned int*)(data));
                    unsigned int plugin_pkt_size = header.pkt_size - (sizeof(unsigned int));
                    unsigned char* plugin_data = (unsigned char*)(data + sizeof(unsigned int));

                    if(header.pkt_dev_idx < plugins.size())
                    {
                        NetworkPlugin plugin = plugins[header.pkt_dev_idx];
                        unsigned char* output = plugin.callback(plugin.callback_arg, plugin_pkt_type, plugin_data, &plugin_pkt_size);
                        if(output != nullptr)
                        {
                            SendReply_PluginSpecific(client_sock, plugin_pkt_type, output, plugin_pkt_size);
                        }
                    }
                    break;
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS:
                if(data == NULL)
                {
                    break;
                }

                if((header.pkt_dev_idx < controllers.size()) && (header.pkt_size == sizeof(int)))
                {
                    int zone;

                    memcpy(&zone, data, sizeof(int));

                    controllers[header.pkt_dev_idx]->ClearSegments(zone);
                    profile_manager->SaveProfile("sizes", true);
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT:
                {
                    /*---------------------------------------------------------*\
                    | Verify the segment description size (first 4 bytes of     |
                    | data) matches the packet size in the header               |
                    \*---------------------------------------------------------*/
                    if(header.pkt_size == *((unsigned int*)data))
                    {
                        if(header.pkt_dev_idx < controllers.size())
                        {
                            controllers[header.pkt_dev_idx]->SetSegmentDescription((unsigned char *)data);
                            profile_manager->SaveProfile("sizes", true);
                        }
                    }
                }
                break;
        }

        delete[] data;
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

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    ClientInfoChanged();
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
    ClientInfoChanged();
}

void NetworkServer::ProcessRequest_ClientString(SOCKET client_sock, unsigned int data_size, char * data)
{
    ServerClientsMutex.lock();
    for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
    {
        if(ServerClients[this_idx]->client_sock == client_sock)
        {
            ServerClients[this_idx]->client_string.assign(data, data_size);
            break;
        }
    }
    ServerClientsMutex.unlock();

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    ClientInfoChanged();
}

void NetworkServer::ProcessRequest_RescanDevices()
{
    ResourceManager::get()->DetectDevices();
}

void NetworkServer::SendReply_ControllerCount(SOCKET client_sock)
{
    NetPacketHeader reply_hdr;
    unsigned int    reply_data;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_CONTROLLER_COUNT, sizeof(unsigned int));

    reply_data = (unsigned int)controllers.size();

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)&reply_data, sizeof(unsigned int), 0);
    send_in_progress.unlock();
}

void NetworkServer::SendReply_ControllerData(SOCKET client_sock, unsigned int dev_idx, unsigned int protocol_version)
{
    if(dev_idx < controllers.size())
    {
        NetPacketHeader reply_hdr;
        unsigned char *reply_data = controllers[dev_idx]->GetDeviceDescription(protocol_version);
        unsigned int   reply_size;

        memcpy(&reply_size, reply_data, sizeof(reply_size));

        InitNetPacketHeader(&reply_hdr, dev_idx, NET_PACKET_ID_REQUEST_CONTROLLER_DATA, reply_size);

        send_in_progress.lock();
        send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
        send(client_sock, (const char *)reply_data, reply_size, 0);
        send_in_progress.unlock();

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
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)&reply_data, sizeof(unsigned int), 0);
    send_in_progress.unlock();
}

void NetworkServer::SendRequest_DeviceListChanged(SOCKET client_sock)
{
    NetPacketHeader pkt_hdr;

    InitNetPacketHeader(&pkt_hdr, 0, NET_PACKET_ID_DEVICE_LIST_UPDATED, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&pkt_hdr, sizeof(NetPacketHeader), 0);
    send_in_progress.unlock();
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

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_PROFILE_LIST, reply_size);

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)reply_data, reply_size, 0);
    send_in_progress.unlock();
}

void NetworkServer::SendReply_PluginList(SOCKET client_sock)
{
    unsigned int data_size = 0;
    unsigned int data_ptr = 0;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    unsigned short num_plugins = (unsigned short)plugins.size();

    data_size += sizeof(data_size);
    data_size += sizeof(num_plugins);

    for(unsigned int i = 0; i < num_plugins; i++)
    {
        data_size += sizeof(unsigned short) * 3;
        data_size += (unsigned int)strlen(plugins[i].name.c_str()) + 1;
        data_size += (unsigned int)strlen(plugins[i].description.c_str()) + 1;
        data_size += (unsigned int)strlen(plugins[i].version.c_str()) + 1;
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
        unsigned short str_len = (unsigned short)strlen(plugins[i].name.c_str()) + 1;

        memcpy(&data_buf[data_ptr], &str_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], plugins[i].name.c_str());
        data_ptr += str_len;

        /*---------------------------------------------------------*\
        | Copy in plugin description (size+data)                    |
        \*---------------------------------------------------------*/
        str_len = (unsigned short)strlen(plugins[i].description.c_str()) + 1;

        memcpy(&data_buf[data_ptr], &str_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], plugins[i].description.c_str());
        data_ptr += str_len;

        /*---------------------------------------------------------*\
        | Copy in plugin version (size+data)                        |
        \*---------------------------------------------------------*/
        str_len = (unsigned short)strlen(plugins[i].version.c_str()) + 1;

        memcpy(&data_buf[data_ptr], &str_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], plugins[i].version.c_str());
        data_ptr += str_len;

        /*---------------------------------------------------------*\
        | Copy in plugin index (data)                               |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &i, sizeof(unsigned int));
        data_ptr += sizeof(unsigned int);

        /*---------------------------------------------------------*\
        | Copy in plugin sdk version (data)                         |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &plugins[i].protocol_version, sizeof(unsigned int));
        data_ptr += sizeof(unsigned int);
    }

    NetPacketHeader reply_hdr;
    unsigned int reply_size;

    memcpy(&reply_size, data_buf, sizeof(reply_size));

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_PLUGIN_LIST, reply_size);

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)data_buf, reply_size, 0);
    send_in_progress.unlock();

    delete [] data_buf;
}

void NetworkServer::SendReply_PluginSpecific(SOCKET client_sock, unsigned int pkt_type, unsigned char* data, unsigned int data_size)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PLUGIN_SPECIFIC, data_size + sizeof(pkt_type));

    send_in_progress.lock();
    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)&pkt_type, sizeof(pkt_type), 0);
    send(client_sock, (const char *)data, data_size, 0);
    send_in_progress.unlock();

    delete [] data;
}

void NetworkServer::SetProfileManager(ProfileManagerInterface* profile_manager_pointer)
{
    profile_manager = profile_manager_pointer;
}

void NetworkServer::RegisterPlugin(NetworkPlugin plugin)
{
    plugins.push_back(plugin);
}

void NetworkServer::UnregisterPlugin(std::string plugin_name)
{
    for(std::vector<NetworkPlugin>::iterator it = plugins.begin(); it != plugins.end(); it++)
    {
        if(it->name == plugin_name)
        {
            plugins.erase(it);
            break;
        }
    }
}
