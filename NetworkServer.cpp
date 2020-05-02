#include "NetworkServer.h"
#include <cstring>

#ifndef WIN32
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
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

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

NetworkServer::NetworkServer(std::vector<RGBController *>& control) : controllers(control)
{
    port_num      = 1337;
    server_online = false;
}

unsigned short NetworkServer::GetPort()
{
    return port_num;
}

bool NetworkServer::GetOnline()
{
    return server_online;
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
}

void NetworkServer::StopServer()
{
    server_online = false;

    for(unsigned int client_idx = 0; client_idx < ServerClients.size(); client_idx++)
    {
        shutdown(*ServerClients[client_idx], SD_RECEIVE);
        closesocket(*ServerClients[client_idx]);
        ListenThreads[client_idx]->join();
    }

    shutdown(server_sock, SD_RECEIVE);
    closesocket(server_sock);

    ConnectionThread->join();

    ServerClients.clear();
    ListenThreads.clear();
}

void NetworkServer::ConnectionThreadFunction()
{
    //This thread handles client connections

    printf("Network connection thread started\n");
    while(server_online == true)
    {
        /*-------------------------------------------------*\
        | Create new socket for client connection           |
        \*-------------------------------------------------*/
        SOCKET * client_sock = new SOCKET();

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

        /*-------------------------------------------------*\
        | Accept the client connection                      |
        \*-------------------------------------------------*/
        *client_sock = accept_select(server_sock, NULL, NULL);

        if(*client_sock < 0)
        {
            printf("Connection thread closed\r\n");
            server_online = false;

            return;
        }

        /*-------------------------------------------------*\
        | Get the new client socket and store it in the     |
        | clients vector                                    |
        \*-------------------------------------------------*/
        u_long arg = 0;
        ioctlsocket(*client_sock, FIONBIO, &arg);
        setsockopt(*client_sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

        //Start a listener thread for the new client socket
        std::thread * NewListenThread = new std::thread(&NetworkServer::ListenThreadFunction, this, client_sock);

        ListenThreads.push_back(NewListenThread);
        ServerClients.push_back(client_sock);
    }

    printf("Connection thread closed\r\n");
    server_online = false;
}

int NetworkServer::accept_select(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    fd_set              set;
    struct timeval      timeout;
    timeout.tv_sec      = 5;
    timeout.tv_usec     = 0;

    while(1)
    {
        FD_ZERO(&set);      /* clear the set */
        FD_SET(sockfd, &set);    /* add our file descriptor to the set */

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
    timeout.tv_sec      = 5;
    timeout.tv_usec     = 0;

    while(1)
    {
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

void NetworkServer::ListenThreadFunction(SOCKET * client_sock)
{
    printf("Network server started\n");
    //This thread handles messages received from clients
    while(server_online == true)
    {
        NetPacketHeader header;
        int             bytes_read  = 0;
        char *          data        = NULL;

        //Read first byte of magic
        bytes_read = recv_select(*client_sock, &header.pkt_magic[0], 1, 0);

        if(bytes_read == 0)
        {
            break;
        }

        //Test first character of magic - 'O'
        if(header.pkt_magic[0] != 'O')
        {
            continue;
        }

        //Read second byte of magic
        bytes_read = recv_select(*client_sock, &header.pkt_magic[1], 1, 0);

        if(bytes_read == 0)
        {
            break;
        }

        //Test second character of magic - 'R'
        if(header.pkt_magic[1] != 'R')
        {
            continue;
        }

        //Read third byte of magic
        bytes_read = recv_select(*client_sock, &header.pkt_magic[2], 1, 0);

        if(bytes_read == 0)
        {
            break;
        }

        //Test third character of magic - 'G'
        if(header.pkt_magic[2] != 'G')
        {
            continue;
        }

        //Read fourth byte of magic
        bytes_read = recv_select(*client_sock, &header.pkt_magic[3], 1, 0);

        if(bytes_read == 0)
        {
            break;
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

            tmp_bytes_read = recv_select(*client_sock, (char *)&header.pkt_dev_idx + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read, 0);

            bytes_read += tmp_bytes_read;

            if(tmp_bytes_read == 0)
            {
                printf("Server connection closed\r\n");
                break;
            }

        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        //printf( "Server: Received header, now receiving data of size %d\r\n", header.pkt_size);

        //Header received, now receive the data
        if(header.pkt_size > 0)
        {
            bytes_read = 0;

            data = new char[header.pkt_size];

            do
            {
                int tmp_bytes_read = 0;

                tmp_bytes_read = recv_select(*client_sock, &data[bytes_read], header.pkt_size - bytes_read, 0);

                if(tmp_bytes_read == 0)
                {
                    printf("Server connection closed\r\n");
                    return;
                }
                bytes_read += tmp_bytes_read;

            } while (bytes_read < header.pkt_size);
        }

        //printf( "Server: Received header and data\r\n" );
        //printf( "Server: Packet ID: %d \r\n", header.pkt_id);

        //Entire request received, select functionality based on request ID
        switch(header.pkt_id)
        {
            case NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                //printf( "NET_PACKET_ID_REQUEST_CONTROLLER_COUNT\r\n" );
                SendReply_ControllerCount(client_sock);
                break;

            case NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                //printf( "NET_PACKET_ID_REQUEST_CONTROLLER_DATA\r\n" );
                SendReply_ControllerData(client_sock, header.pkt_dev_idx);
                break;

            case NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE:
                //printf( "NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE\r\n" );

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
                //printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS\r\n" );

                if(header.pkt_dev_idx < controllers.size())
                {
                    controllers[header.pkt_dev_idx]->SetColorDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateLEDs();
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
                //printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS\r\n" );

                if(header.pkt_dev_idx < controllers.size())
                {
                    int zone;

                    memcpy(&zone, &data[sizeof(unsigned int)], sizeof(int));

                    controllers[header.pkt_dev_idx]->SetZoneColorDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateZoneLEDs(zone);
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED:
                //printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED\r\n" );

                if(header.pkt_dev_idx < controllers.size())
                {
                    int led;

                    memcpy(&led, data, sizeof(int));

                    controllers[header.pkt_dev_idx]->SetSingleLEDColorDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateSingleLED(led);
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE:
                //printf( "NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE\r\n" );

                if(header.pkt_dev_idx < controllers.size())
                {
                    controllers[header.pkt_dev_idx]->SetCustomMode();
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE:
                //printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE\r\n" );

                if(header.pkt_dev_idx < controllers.size())
                {
                    controllers[header.pkt_dev_idx]->SetModeDescription((unsigned char *)data);
                    controllers[header.pkt_dev_idx]->UpdateMode();
                }
                break;
        }

        delete[] data;
    }

    printf("Server connection closed\r\n");
}

void NetworkServer::SendReply_ControllerCount(SOCKET * client_sock)
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

    send(*client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
    send(*client_sock, (const char *)&reply_data, sizeof(unsigned int), 0);
}

void NetworkServer::SendReply_ControllerData(SOCKET * client_sock, unsigned int dev_idx)
{
    if(dev_idx < controllers.size())
    {
        NetPacketHeader reply_hdr;
        unsigned char *reply_data = controllers[dev_idx]->GetDeviceDescription();
        unsigned int   reply_size;

        memcpy(&reply_size, reply_data, sizeof(reply_size));
        
        reply_hdr.pkt_magic[0] = 'O';
        reply_hdr.pkt_magic[1] = 'R';
        reply_hdr.pkt_magic[2] = 'G';
        reply_hdr.pkt_magic[3] = 'B';

        reply_hdr.pkt_dev_idx  = dev_idx;
        reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_DATA;
        reply_hdr.pkt_size     = reply_size;

        send(*client_sock, (const char *)&reply_hdr, sizeof(NetPacketHeader), 0);
        send(*client_sock, (const char *)reply_data, reply_size, 0);
    }
}
