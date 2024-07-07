/*---------------------------------------------------------*\
| NetworkProtocol.cpp                                       |
|                                                           |
|   OpenRGB SDK network protocol                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "NetworkProtocol.h"

/*-----------------------------------------------------*\
| OpenRGB SDK Magic Value "ORGB"                        |
\*-----------------------------------------------------*/
const char openrgb_sdk_magic[OPENRGB_SDK_MAGIC_SIZE] = { 'O', 'R', 'G', 'B' };

void InitNetPacketHeader
    (
    NetPacketHeader *   pkt_hdr,
    unsigned int        pkt_dev_idx,
    unsigned int        pkt_id,
    unsigned int        pkt_size
    )
{
    memcpy(pkt_hdr->pkt_magic, openrgb_sdk_magic, sizeof(openrgb_sdk_magic));

    pkt_hdr->pkt_dev_idx  = pkt_dev_idx;
    pkt_hdr->pkt_id       = pkt_id;
    pkt_hdr->pkt_size     = pkt_size;
}
