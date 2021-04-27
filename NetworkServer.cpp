/*-----------------------------------------*\
|  NetworkServer.cpp                        |
|                                           |
|  Server code for OpenRGB SDK              |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/9/2020    |
\*-----------------------------------------*/

#include "NetworkServer.h"
#include "LogManager.h"
#include <cstring>

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
    client_sock          = INVALID_SOCKET;
    client_listen_thread = nullptr;
}

NetworkClientInfo::~NetworkClientInfo()
{
    if(client_sock != INVALID_SOCKET)
    {
        LOG_NOTICE("Closing server connection: %s", client_ip);
        delete client_listen_thread;
        shutdown(client_sock, SD_RECEIVE);
        closesocket(client_sock);
    }
}

NetworkServer::NetworkServer(std::vector<RGBController *>& control) : controllers(control)
{
    port_num         = OPENRGB_SDK_PORT;
    server_online    = false;
    server_listening = false;
    ConnectionThread = nullptr;
    profile_manager  = nullptr;
}

NetworkServer::~NetworkServer()
{
    StopServer();
}

void NetworkServer::ClientInfoChanged()
{
    ClientInfoChangeMutex.lock();

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < ClientInfoChangeCallbacks.size(); callback_idx++)
    {
        ClientInfoChangeCallbacks[callback_idx](ClientInfoChangeCallbackArgs[callback_idx]);
    }

    ClientInfoChangeMutex.unlock();
}

void NetworkServer::DeviceListChanged()
{
    /*-------------------------------------------------*\
    | Indicate to the clients that the controller list  |
    | has changed                                       |
    \*-------------------------------------------------*/
    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        SendRequest_DeviceListChanged(ServerClients[client_idx]->client_sock);
    }
}

void NetworkServer::ServerListeningChanged()
{
    ServerListeningChangeMutex.lock();

    /*-------------------------------------------------*\
    | Server state has changed, call the callbacks      |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < ServerListeningChangeCallbacks.size(); callback_idx++)
    {
        ServerListeningChangeCallbacks[callback_idx](ServerListeningChangeCallbackArgs[callback_idx]);
    }

    ServerListeningChangeMutex.unlock();
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
    return ServerClients.size();
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
        result = ServerClients[client_num]->client_ip;
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

void NetworkServer::SetPort(unsigned short new_port)
{
    if(server_online == false)
    {
        port_num = new_port;
    }
}

void NetworkServer::StartServer()
{
    //Start a TCP server and launch threads
    char port_str[6];
    snprintf(port_str, 6, "%d", port_num);

    sockaddr_in myAddress;

    /*-------------------------------------------------*\
    | Windows requires WSAStartup before using sockets  |
    \*-------------------------------------------------*/
#ifdef WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return;
    }
#endif

    /*-------------------------------------------------*\
    | Create the server socket                          |
    \*-------------------------------------------------*/
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET)
    {
        printf("Error: network socket could not be created\n");
        WSACleanup();
        return;
    }

    /*-------------------------------------------------*\
    | Fill in server address info with port value       |
    \*-------------------------------------------------*/
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
    myAddress.sin_port = htons(atoi(port_str));

    /*-------------------------------------------------*\
    | Bind the server socket                            |
    \*-------------------------------------------------*/
    if (bind(server_sock, (sockaddr*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
    {
        if(errno == EADDRINUSE)
        {
            printf("Error: Could not bind network socket \nIs port %hu already being used?\n", GetPort());
        }
        else if(errno == EACCES)
        {
            printf("Error: Access to socket was denied.\n");
        }
        else if(errno == EBADF)
        {
            printf("Error: sockfd is not a valid file descriptor.\n");
        }
        else if(errno == EINVAL)
        {
            printf("Error: The socket is already bound to an address, or addrlen is wrong, or addr is not a valid address for this socket's domain..\n");
        }
        else if(errno == ENOTSOCK)
        {
            printf("Error: The file descriptor sockfd does not refer to a socket.\n");
        }
        else
        {
            // could be a linux specific error
            // https://man7.org/linux/man-pages/man2/bind.2.html
            printf("Error: Could not bind network socket, error code:%d\n", errno);
        }

        WSACleanup();
        return;
    }

    /*-------------------------------------------------*\
    | Set socket options - no delay                     |
    \*-------------------------------------------------*/
    setsockopt(server_sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

    server_online = true;
    /*-------------------------------------------------*\
    | Start the connection thread                       |
    \*-------------------------------------------------*/
    ConnectionThread = new std::thread(&NetworkServer::ConnectionThreadFunction, this);
    ConnectionThread->detach();
}

void NetworkServer::StopServer()
{
    server_online = false;

    ServerClientsMutex.lock();

    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        delete ServerClients[client_idx];
    }

    ServerClients.clear();

    shutdown(server_sock, SD_RECEIVE);
    closesocket(server_sock);

    ServerClientsMutex.unlock();

    if(ConnectionThread)
    {
        delete ConnectionThread;
        ConnectionThread = nullptr;
    }

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    ClientInfoChanged();
}

void NetworkServer::ConnectionThreadFunction()
{
    //This thread handles client connections

    printf("Network connection thread started on port %hu\n", GetPort());
    while(server_online == true)
    {
        /*-------------------------------------------------*\
        | Create new socket for client connection           |
        \*-------------------------------------------------*/
        NetworkClientInfo * client_info = new NetworkClientInfo();

        /*-------------------------------------------------*\
        | Listen for incoming client connection on the      |
        | server socket.  This call blocks until a          |
        | connection is established                         |
        \*-------------------------------------------------*/
        if(listen(server_sock, 10) < 0)
        {
            printf("Connection thread closed\r\n");
            server_online = false;

            return;
        }

        server_listening = true;
        ServerListeningChanged();

        /*-------------------------------------------------*\
        | Accept the client connection                      |
        \*-------------------------------------------------*/
        struct sockaddr_in client_addr;
        socklen_t          client_addr_len = sizeof(client_addr);
        client_info->client_sock = accept_select(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);

        if(client_info->client_sock < 0)
        {
            printf("Connection thread closed\r\n");
            server_online = false;

            server_listening = false;
            ServerListeningChanged();

            return;
        }

        /*-------------------------------------------------*\
        | Get the new client socket and store it in the     |
        | clients vector                                    |
        \*-------------------------------------------------*/
        u_long arg = 0;
        ioctlsocket(client_info->client_sock, FIONBIO, &arg);
        setsockopt(client_info->client_sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

        inet_ntop(AF_INET, &client_addr.sin_addr, client_info->client_ip, INET_ADDRSTRLEN);

        client_info->client_string = "Client";

        /* We need to lock before the thread could possibly finish */
        ServerClientsMutex.lock();

        //Start a listener thread for the new client socket
        client_info->client_listen_thread = new std::thread(&NetworkServer::ListenThreadFunction, this, client_info);
        client_info->client_listen_thread->detach();

        ServerClients.push_back(client_info);
        ServerClientsMutex.unlock();

        /*-------------------------------------------------*\
        | Client info has changed, call the callbacks       |
        \*-------------------------------------------------*/
        ClientInfoChanged();
    }

    printf("Connection thread closed\r\n");
    server_online = false;
    server_listening = false;
    ServerListeningChanged();
}

int NetworkServer::accept_select(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    fd_set              set;
    struct timeval      timeout;

    while(1)
    {
        timeout.tv_sec          = TCP_TIMEOUT_SECONDS;
        timeout.tv_usec         = 0;

        FD_ZERO(&set);          /* clear the set */
        FD_SET(sockfd, &set);   /* add our file descriptor to the set */

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
            // socket has something to read
            return(accept(sockfd, addr, addrlen));
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

        FD_ZERO(&set);      /* clear the set */
        FD_SET(s, &set);    /* add our file descriptor to the set */

        int rv = select(s + 1, &set, NULL, NULL, &timeout);

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
            // socket has something to read
            return(recv(s, buf, len, flags));
        }
    }
}

void NetworkServer::ListenThreadFunction(NetworkClientInfo * client_info)
{
    SOCKET client_sock = client_info->client_sock;

    printf("Network server started\n");
    //This thread handles messages received from clients
    while(server_online == true)
    {
        NetPacketHeader header;
        int             bytes_read  = 0;
        char *          data        = NULL;

        //Read first byte of magic
        bytes_read = recv_select(client_sock, &header.pkt_magic[0], 1, 0);

        if(bytes_read <= 0)
        {
            goto listen_done;
        }

        //Test first character of magic - 'O'
        if(header.pkt_magic[0] != 'O')
        {
            continue;
        }

        //Read second byte of magic
        bytes_read = recv_select(client_sock, &header.pkt_magic[1], 1, 0);

        if(bytes_read <= 0)
        {
            goto listen_done;
        }

        //Test second character of magic - 'R'
        if(header.pkt_magic[1] != 'R')
        {
            continue;
        }

        //Read third byte of magic
        bytes_read = recv_select(client_sock, &header.pkt_magic[2], 1, 0);

        if(bytes_read <= 0)
        {
            goto listen_done;
        }

        //Test third character of magic - 'G'
        if(header.pkt_magic[2] != 'G')
        {
            continue;
        }

        //Read fourth byte of magic
        bytes_read = recv_select(client_sock, &header.pkt_magic[3], 1, 0);

        if(bytes_read <= 0)
        {
            goto listen_done;
        }

        //Test fourth character of magic - 'B'
        if(header.pkt_magic[3] != 'B')
        {
            continue;
        }

        //If we get to this point, the magic is correct.  Read the rest of the header
        bytes_read = 0;
        do
        {
            int tmp_bytes_read = 0;

            tmp_bytes_read = recv_select(client_sock, (char *)&header.pkt_dev_idx + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read, 0);

            bytes_read += tmp_bytes_read;

            if(tmp_bytes_read <= 0)
            {
                goto listen_done;
            }

        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        //Header received, now receive the data
        if(header.pkt_size > 0)
        {
            bytes_read = 0;

            data = new char[header.pkt_size];

            do
            {
                int tmp_bytes_read = 0;

                tmp_bytes_read = recv_select(client_sock, &data[(unsigned int)bytes_read], header.pkt_size - bytes_read, 0);

                if(tmp_bytes_read <= 0)
                {
                    goto listen_done;
                }
                bytes_read += tmp_bytes_read;

            } while ((unsigned int)bytes_read < header.pkt_size);
        }

        //Entire request received, select functionality based on request ID
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
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS:
                if(data == NULL)
                {
                    break;
                }

                if(header.pkt_dev_idx < controllers.size())
                {
                    controllers[header.pkt_dev_idx]->SetColorDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateLEDs();
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
                if(data == NULL)
                {
                    break;
                }

                if(header.pkt_dev_idx < controllers.size())
                {
                    int zone;

                    memcpy(&zone, &data[sizeof(unsigned int)], sizeof(int));

                    controllers[header.pkt_dev_idx]->SetZoneColorDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateZoneLEDs(zone);
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED:
                if(data == NULL)
                {
                    break;
                }

                if(header.pkt_dev_idx < controllers.size())
                {
                    int led;

                    memcpy(&led, data, sizeof(int));

                    controllers[header.pkt_dev_idx]->SetSingleLEDColorDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateSingleLED(led);
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

                if(header.pkt_dev_idx < controllers.size())
                {
                    controllers[header.pkt_dev_idx]->SetModeDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateMode();
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
                    profile_manager->SaveProfile(data);
                }

                break;

            case NET_PACKET_ID_REQUEST_LOAD_PROFILE:
                if(data == NULL)
                {
                    break;
                }

                if(profile_manager)
                {
                    profile_manager->LoadProfile(data);
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
                    profile_manager->DeleteProfile(data);
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

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
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

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    ClientInfoChanged();
}

void NetworkServer::ProcessRequest_ClientString(SOCKET client_sock, unsigned int /*data_size*/, char * data)
{
    ServerClientsMutex.lock();
    for(unsigned int this_idx = 0; this_idx < ServerClients.size(); this_idx++)
    {
        if(ServerClients[this_idx]->client_sock == client_sock)
        {
            ServerClients[this_idx]->client_string = data;
            break;
        }
    }
    ServerClientsMutex.unlock();

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    ClientInfoChanged();
}

void NetworkServer::SendReply_ControllerCount(SOCKET client_sock)
{
    NetPacketHeader reply_hdr;
    unsigned int    reply_data;

    reply_hdr.pkt_magic[0] = 'O';
    reply_hdr.pkt_magic[1] = 'R';
    reply_hdr.pkt_magic[2] = 'G';
    reply_hdr.pkt_magic[3] = 'B';

    reply_hdr.pkt_dev_idx  = 0;
    reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_COUNT;
    reply_hdr.pkt_size     = sizeof(unsigned int);

    reply_data             = controllers.size();

    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)&reply_data, sizeof(unsigned int), 0);
}

void NetworkServer::SendReply_ControllerData(SOCKET client_sock, unsigned int dev_idx, unsigned int protocol_version)
{
    if(dev_idx < controllers.size())
    {
        NetPacketHeader reply_hdr;
        unsigned char *reply_data = controllers[dev_idx]->GetDeviceDescription(protocol_version);
        unsigned int   reply_size;

        memcpy(&reply_size, reply_data, sizeof(reply_size));
        
        reply_hdr.pkt_magic[0] = 'O';
        reply_hdr.pkt_magic[1] = 'R';
        reply_hdr.pkt_magic[2] = 'G';
        reply_hdr.pkt_magic[3] = 'B';

        reply_hdr.pkt_dev_idx  = dev_idx;
        reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_DATA;
        reply_hdr.pkt_size     = reply_size;

        send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
        send(client_sock, (const char *)reply_data, reply_size, 0);

        delete[] reply_data;
    }
}

void NetworkServer::SendReply_ProtocolVersion(SOCKET client_sock)
{
    NetPacketHeader reply_hdr;
    unsigned int    reply_data;

    reply_hdr.pkt_magic[0] = 'O';
    reply_hdr.pkt_magic[1] = 'R';
    reply_hdr.pkt_magic[2] = 'G';
    reply_hdr.pkt_magic[3] = 'B';

    reply_hdr.pkt_dev_idx  = 0;
    reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_PROTOCOL_VERSION;
    reply_hdr.pkt_size     = sizeof(unsigned int);

    reply_data             = OPENRGB_SDK_PROTOCOL_VERSION;

    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)&reply_data, sizeof(unsigned int), 0);
}

void NetworkServer::SendRequest_DeviceListChanged(SOCKET client_sock)
{
    NetPacketHeader pkt_hdr;

    pkt_hdr.pkt_magic[0] = 'O';
    pkt_hdr.pkt_magic[1] = 'R';
    pkt_hdr.pkt_magic[2] = 'G';
    pkt_hdr.pkt_magic[3] = 'B';

    pkt_hdr.pkt_dev_idx  = 0;
    pkt_hdr.pkt_id       = NET_PACKET_ID_DEVICE_LIST_UPDATED;
    pkt_hdr.pkt_size     = 0;

    send(client_sock, (char *)&pkt_hdr, sizeof(NetPacketHeader), 0);
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

    reply_hdr.pkt_magic[0] = 'O';
    reply_hdr.pkt_magic[1] = 'R';
    reply_hdr.pkt_magic[2] = 'G';
    reply_hdr.pkt_magic[3] = 'B';

    reply_hdr.pkt_dev_idx  = 0;
    reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_PROFILE_LIST;
    reply_hdr.pkt_size     = reply_size;

    send(client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(client_sock, (const char *)reply_data, reply_size, 0);
}

void NetworkServer::SetProfileManager(ProfileManagerInterface* profile_manager_pointer)
{
    profile_manager = profile_manager_pointer;
}

