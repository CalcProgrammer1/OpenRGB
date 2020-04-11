#include "NetworkServer.h"

NetworkServer::NetworkServer(std::vector<RGBController *>& control) : controllers(control)
{
    //Start a TCP server and launch threads
    port.tcp_server("1337");
}

void NetworkServer::ConnectionThread()
{
    //This thread handles client connections
    while(1)
    {
        port.tcp_server_listen();
    }
}

void NetworkServer::ListenThread()
{
    //This thread handles messages received from clients
    while(1)
    {
        NetPacketHeader header;
        int             bytes_read = 0;

        //Read first byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[0], 1);
        } while(bytes_read == 0);

        //Test first character of magic - 'O'
        if(header.pkt_magic[0] != 'O')
        {
            continue;
        }

        //Read second byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[1], 1);
        } while(bytes_read == 0);

        //Test second character of magic - 'R'
        if(header.pkt_magic[1] != 'R')
        {
            continue;
        }

        //Read third byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[2], 1);
        } while(bytes_read == 0);

        //Test third character of magic - 'G'
        if(header.pkt_magic[2] != 'O')
        {
            continue;
        }

        //Read fourth byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[3], 1);
        } while(bytes_read == 0);

        //Test fourth character of magic - 'B'
        if(header.pkt_magic[3] != 'O')
        {
            continue;
        }

        //If we get to this point, the magic is correct.  Read the rest of the header
        do
        {
            bytes_read = port.tcp_listen((char *)&header.pkt_dev_idx, sizeof(header) - sizeof(header.pkt_magic));
        } while(bytes_read == sizeof(header) - sizeof(header.pkt_magic));

        //Header received, now receive the data
        if(header.pkt_size > 0)
        {

        }

        //Entire request received, select functionality based on request ID
        switch(header.pkt_id)
        {
            NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                
                break;

            NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                break;

            NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED:
                break;

            NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE:
                break;
        }
    }
}