/*---------------------------------------------------------*\
|  Cross Platform Network Library for Windows and Linux     |
|  This library provides access to TCP and UDP ports with   |
|  a common API for both Windows and Linux systems.  It     |
|  features read and write                                  |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/15/2016       |
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

const char yes = 1;

net_port::net_port()
{

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

}

bool net_port::udp_client(const char * client_name, const char * port)
{
    sockaddr_in myAddress;

#ifdef WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return(false);
    }
#endif

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        WSACleanup();
        return(false);
    }

    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
    myAddress.sin_port = htons(0);

    if (bind(sock, (sockaddr*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
    {
        WSACleanup();
        return false;
    }

    result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if(getaddrinfo(client_name, port, &hints, &result_list) == 0)
    {
        memcpy(&addrDest, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
        return(true);
    }
    else
    {
        WSACleanup();
        return(false);
    }
}

int net_port::udp_listen(char * recv_data, int length)
{
    return(recvfrom(sock, recv_data, length, 0, NULL, NULL));
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
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return(false);
    }
#endif

    port = strtok((char *)port, "\r");

    hints.ai_family = AF_INET;
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
    if (!connected)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET)
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
        connect(sock, result_list->ai_addr, result_list->ai_addrlen);

        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        tv.tv_sec =  4;
        tv.tv_usec = 0;

        if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1)
        {
            char so_error;
            socklen_t len = sizeof(so_error);

            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

            if (so_error == 0)
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
    return(true);
}

bool net_port::tcp_server(const char * port)
{
    sockaddr_in myAddress;

#ifdef WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
    {
        WSACleanup();
        return false;
    }
#endif

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }

    port = strtok((char *)port, "\r");

    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
    myAddress.sin_port = htons(atoi(port));

    if (bind(sock, (sockaddr*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
    {
        WSACleanup();
        return false;
    }

    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

    return(true);
}

void net_port::tcp_server_listen()
{
    SOCKET * client = new SOCKET();
    listen(sock, 10);
    *client = accept(sock, NULL, NULL);
    u_long arg = 0;
    ioctlsocket(*client, FIONBIO, &arg);
    setsockopt(*client, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
    clients.push_back(client);
}

void net_port::tcp_close()
{
    closesocket(sock);
    connected = false;
}

int net_port::tcp_listen(char * recv_data, int length)
{
    int ret = 0;
    int len = 0;
    int tot = 0;
    timeval waitd;

    fd_set readfd;

    FD_ZERO(&readfd);
    FD_SET(sock, &readfd);

    if (connected)
    {
        while(ret != sizeof(len))
        {
            waitd.tv_sec = 10;
            waitd.tv_usec = 0;

            if (select(sock + 1, &readfd, NULL, NULL, &waitd))
            {
                ret = recv(sock, (char *)&len, sizeof(len), 0);

                if (ret == -1 || ret == 0)
                {
                    closesocket(sock);
                    connected = false;
                    return(0);
                }
            }
            else
            {
                closesocket(sock);
                connected = false;
                return(0);
            }
        }

        ret = 0;
        while(tot != len)
        {
            waitd.tv_sec = 10;
            waitd.tv_usec = 0;

            if (select(sock + 1, &readfd, NULL, NULL, &waitd))
            {
                ret = recv(sock, recv_data + ret, len - ret, 0);

                if (ret == -1 || ret == 0)
                {
                    closesocket(sock);
                    connected = false;
                    return(0);
                }

                tot += ret;
            }
            else
            {
                closesocket(sock);
                connected = false;
                return(0);
            }
        }
    }
    return(ret);
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

    for (unsigned int i = 0; i < clients.size(); i++)
    {
        val = length;

        FD_ZERO(&writefd);
        FD_SET(*(clients[i]), &writefd);

        waitd.tv_sec = 5;
        waitd.tv_usec = 0;

        if (select(*(clients[i]) + 1, NULL, &writefd, NULL, &waitd))
        {
            val = send(*(clients[i]), (const char *)&length, sizeof(length), 0);

            if (val == -1)
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

        if (select(*(clients[i]) + 1, NULL, &writefd, NULL, &waitd))
        {
            val = send(*(clients[i]), buffer, length, 0);

            if (val == -1)
            {
                clients.erase(clients.begin() + i);
                return 0;
            }

            if (val != length)
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
