/*---------------------------------------------------------*\
| net_port.cpp                                              |
|                                                           |
|   Cross Platform Network Library for Windows and Linux    |
|   This library provides access to TCP and UDP ports with  |
|   a common API for both Windows and Linux systems         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Dec 2016 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "net_port.h"

#ifndef WIN32
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#endif
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>

#ifdef _WIN32
#define connect_socklen_t int
#else
#define connect_socklen_t socklen_t
#endif

const char yes = 1;

net_port::net_port()
{
    result_list = NULL;
}

//net_port (constructor)
//	When created with port information, the constructor
//	will automatically open client address <client_name> on port <port>
net_port::net_port(const char * client_name, const char * port)
{
    udp_client(client_name, port);
}

net_port::~net_port()
{
    if(result_list)
    {
        freeaddrinfo(result_list);
    }
}

bool net_port::udp_client(const char* client_name, const char * port)
{
    return(udp_client(client_name, port, "0"));
}

bool net_port::udp_client(const char * client_name, const char * send_port, const char * recv_port)
{
    sockaddr_in myAddress;

#ifdef WIN32
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return(false);
    }
#endif

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == INVALID_SOCKET)
    {
        WSACleanup();
        return(false);
    }

    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
    myAddress.sin_port = htons(atoi(recv_port));

    if(bind(sock, (sockaddr*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
    {
        WSACleanup();
        return false;
    }

    result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if(getaddrinfo(client_name, send_port, &hints, &result_list) == 0)
    {
        memcpy(&addrDest, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
        result_list = NULL;
        return(true);
    }
    else
    {
        WSACleanup();
        return(false);
    }
}

void net_port::udp_join_multicast_group(const char * group_name)
{
    struct ip_mreq group;

    group.imr_multiaddr.s_addr = inet_addr(group_name);
    group.imr_interface.s_addr = inet_addr("0.0.0.0");

    setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
}

int net_port::udp_listen(char * recv_data, int length)
{
    return(recvfrom(sock, recv_data, length, 0, NULL, NULL));
}

void net_port::set_receive_timeout(int sec, int usec)
{
#ifdef WIN32
    DWORD tv = ( sec * 1000 ) + ( usec / 1000 );
#else
    struct timeval tv;
    tv.tv_sec   = sec;
    tv.tv_usec  = usec;
#endif

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

int net_port::udp_write(char * buffer, int length)
{
    return(sendto(sock, buffer, length, 0, (sockaddr *)&addrDest, sizeof(addrDest)));
}

bool net_port::tcp_client(const char * client_name, const char * port)
{
    addrinfo    hints = {};

    connected = false;
    result_list = NULL;

#ifdef WIN32
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return(false);
    }
#endif

    port = strtok((char *)port, "\r");

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(client_name, port, &hints, &result_list);

    if(result_list == NULL)
    {
        WSACleanup();
        return(false);
    }

    return(true);
}

bool net_port::tcp_client_connect()
{
    struct addrinfo *res;
    connected = false;

    for(res = result_list; res; res = res->ai_next)
    {
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sock == INVALID_SOCKET)
        {
            WSACleanup();
            return(false);
        }

        u_long arg = 1;
        fd_set fdset;
        timeval tv;

        ioctlsocket(sock, FIONBIO, &arg);

        if(result_list == NULL)
        {
            connected = false;
            return(false);
        }
        connect(sock, res->ai_addr, (connect_socklen_t)res->ai_addrlen);

        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        tv.tv_sec =  4;
        tv.tv_usec = 0;

        /*-------------------------------------------------*\
        | Set socket options - no delay                     |
        \*-------------------------------------------------*/
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

        if(select(sock + 1, NULL, &fdset, NULL, &tv) == 1)
        {
            char so_error;
            socklen_t len = sizeof(so_error);

            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

            if(so_error == 0)
            {
                connected = true;
                arg = 0;
                ioctlsocket(sock, FIONBIO, &arg);
            }
            else
            {
                connected = false;
                closesocket(sock);
            }
        }
        else
        {
            connected = false;
            closesocket(sock);
        }
    }
    return(connected);
}

bool net_port::tcp_server(const char * port)
{
    sockaddr_in myAddress;

    /*-------------------------------------------------*\
    | Windows requires WSAStartup before using sockets  |
    \*-------------------------------------------------*/
#ifdef WIN32
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return false;
    }
#endif

    /*-------------------------------------------------*\
    | Create the server socket                          |
    \*-------------------------------------------------*/
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }

    /*-------------------------------------------------*\
    | Fill in server address info with port value       |
    \*-------------------------------------------------*/
    port = strtok((char *)port, "\r");

    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
    myAddress.sin_port = htons(atoi(port));

    /*-------------------------------------------------*\
    | Bind the server socket                            |
    \*-------------------------------------------------*/
    if(bind(sock, (sockaddr*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
    {
        WSACleanup();
        return false;
    }

    /*-------------------------------------------------*\
    | Set socket options - no delay                     |
    \*-------------------------------------------------*/
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

    return(true);
}

SOCKET * net_port::tcp_server_listen()
{
    /*-------------------------------------------------*\
    | Create new socket for client connection           |
    \*-------------------------------------------------*/
    SOCKET * client = new SOCKET();

    /*-------------------------------------------------*\
    | Listen for incoming client connection on the      |
    | server socket.  This call blocks until a          |
    | connection is established                         |
    \*-------------------------------------------------*/
    listen(sock, 10);

    /*-------------------------------------------------*\
    | Accept the client connection                      |
    \*-------------------------------------------------*/
    *client = accept(sock, NULL, NULL);

    /*-------------------------------------------------*\
    | Get the new client socket and store it in the     |
    | clients vector                                    |
    \*-------------------------------------------------*/
    u_long arg = 0;
    ioctlsocket(*client, FIONBIO, &arg);
    setsockopt(*client, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
    clients.push_back(client);

    return client;
}

void net_port::tcp_close()
{
    closesocket(sock);
    connected = false;
}

int net_port::tcp_listen(char * recv_data, int length)
{
    return(recv(sock, recv_data, length, 0));
}

int net_port::tcp_client_write(char * buffer, int length)
{
    return(send(sock, buffer, length, 0));
}

int net_port::tcp_write(char * buffer, int length)
{
    int ret = length;
    int val = length;

    timeval waitd;
    fd_set writefd;

    for(unsigned int i = 0; i < clients.size(); i++)
    {
        val = length;

        FD_ZERO(&writefd);
        FD_SET(*(clients[i]), &writefd);

        waitd.tv_sec = 5;
        waitd.tv_usec = 0;

        if(select((int)*(clients[i]) + 1, NULL, &writefd, NULL, &waitd))
        {
            val = send(*(clients[i]), (const char *)&length, sizeof(length), 0);

            if(val == -1)
            {
                clients.erase(clients.begin() + i);
                return 0;
            }
        }
        else
        {
            clients.erase(clients.begin() + i);
            return 0;
        }

        waitd.tv_sec = 5;
        waitd.tv_usec = 0;

        if(select(*(clients[i]) + 1, NULL, &writefd, NULL, &waitd))
        {
            val = send(*(clients[i]), buffer, length, 0);

            if(val == -1)
            {
                clients.erase(clients.begin() + i);
                return 0;
            }

            if(val != length)
            {
                ret = val;
            }
        }
        else
        {
            clients.erase(clients.begin() + i);
            return 0;
        }
    }
    return(ret);
}
