/*---------------------------------------------------------*\
| NetworkProtocol.cpp                                       |
|                                                           |
|   OpenRGB SDK network protocol                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <climits>
#endif

#include "NetworkProtocol.h"

/*-----------------------------------------------------*\
| OpenRGB SDK Magic Value "ORGB"                        |
\*-----------------------------------------------------*/
const char openrgb_sdk_magic[OPENRGB_SDK_MAGIC_SIZE] = { 'O', 'R', 'G', 'B' };

std::string GetHostname()
{
    /*-----------------------------------------------------*\
    | Determine maximum hostname limit or fallback to       |
    | defensive size                                        |
    \*-----------------------------------------------------*/
    #if defined(HOST_NAME_MAX)
        const size_t buffer_size = HOST_NAME_MAX + 1;
    #else
        const size_t buffer_size = 256; 
    #endif

    char buffer[buffer_size];

    #if defined(_WIN32) || defined(_WIN64)
        /*-------------------------------------------------*\
        | Initialize Winsock data required on Windows       |
        | before socket calls                               |
        \*-------------------------------------------------*/
        WSADATA wsa_data;
        if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        {
            return "unknown";
        }
    #endif

    std::string hostname = "unknown";

    if(gethostname(buffer, sizeof(buffer)) == 0)
    {
        hostname = buffer;
    }

    #if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
    #endif

    return(hostname);
}

void InitNetPacketHeader
    (
    NetPacketHeader *   pkt_hdr,
    unsigned int        pkt_dev_id,
    unsigned int        pkt_id,
    unsigned int        pkt_size
    )
{
    memcpy(pkt_hdr->pkt_magic, openrgb_sdk_magic, sizeof(openrgb_sdk_magic));

    pkt_hdr->pkt_dev_id   = pkt_dev_id;
    pkt_hdr->pkt_id       = pkt_id;
    pkt_hdr->pkt_size     = pkt_size;
}
