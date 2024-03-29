#include "NetworkProtocol.h"
#include <cstring>

/*-----------------------------------------------------*\
| OpenRGB SDK Magic Value "ORGB"                        |
\*-----------------------------------------------------*/
const char * openrgb_sdk_magic = "ORGB";

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
