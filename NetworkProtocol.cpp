#include "NetworkProtocol.h"

NetPacketHeader * InitNetPacketHeader
    (
    unsigned int        pkt_dev_idx,
    unsigned int        pkt_id,
    unsigned int        pkt_size
    )
{
    NetPacketHeader * new_header = new NetPacketHeader;

    new_header->pkt_magic[0] = 'O';
    new_header->pkt_magic[1] = 'R';
    new_header->pkt_magic[2] = 'G';
    new_header->pkt_magic[3] = 'B';

    new_header->pkt_dev_idx  = pkt_dev_idx;
    new_header->pkt_id       = pkt_id;
    new_header->pkt_size     = pkt_size;
}