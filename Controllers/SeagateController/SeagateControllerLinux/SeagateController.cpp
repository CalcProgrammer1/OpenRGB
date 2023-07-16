/*-----------------------------------------*\
|  SeagateController.cpp                    |
|                                           |
|  Code for Seagate Firecuda External HDD   |
|  RGB controller                           |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2023   |
\*-----------------------------------------*/

#include "SeagateController.h"

SeagateController::SeagateController(int fd, char* path)
{
    this->fd    = fd;
	this->path  = path;
}

SeagateController::~SeagateController()
{

}

std::string SeagateController::GetLocation()
{
	std::string str(path.begin(), path.end());
    return("SCSI: " + str);
}

void SeagateController::SetLED
    (
    unsigned char   led_id,
    unsigned char   r,
    unsigned char   g,
    unsigned char   b,
    bool            save
    )
{
    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold RGB control data                                        |
    \*-----------------------------------------------------------------------------*/
    unsigned char data[14]                  = {0};
    data[0]                                 = 0x0E; // size of data packet
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = led_id;
    data[7]                                 = 0x01;
    if(save)
    {
        data[8]                             = 0x03; // 0x00 for no save, 0x03 for save
    }
    else
    {
        data[8]                             = 0x00;
    }
    data[9]                                 = r;
    data[10]                                = g;
    data[11]                                = b;
    data[12]                                = 0xFF;
    data[13]                                = 0xFF;

    SendPacket(data, 14);
}

void SeagateController::SendPacket
    (
    void *          packet,
    unsigned char   packet_sz
    )
{
    /*-----------------------------------------------------------------------------*\
    | Create buffers to hold header, cdb, and sense                                 |
    \*-----------------------------------------------------------------------------*/
    unsigned char cdb[12];
    unsigned char sense[32];
    sg_io_hdr header;

    /*-----------------------------------------------------------------------------*\
    | Set up pass through command                                                   |
    \*-----------------------------------------------------------------------------*/
    header.interface_id                     = 'S';
    header.dxfer_direction                  = SG_DXFER_TO_DEV;
    header.cmd_len                          = sizeof(cdb);
    header.mx_sb_len                        = sizeof(sense);
    header.iovec_count                      = 0;
    header.dxfer_len                        = packet_sz;
    header.dxferp                           = packet;
    header.cmdp                             = cdb;
    header.sbp                              = sense;
    header.timeout                          = 20000;
    header.flags                            = 0;

    cdb[0]                                  = 0xD2;
    cdb[1]                                  = 0x53;
    cdb[2]                                  = 0x65;
    cdb[3]                                  = 0x74;
    cdb[4]                                  = 0x4C;
    cdb[5]                                  = 0x65;
    cdb[6]                                  = 0x64;
    cdb[7]                                  = 0x00;
    cdb[8]                                  = 0x00;
    cdb[9]                                  = 0x30;
    cdb[10]                                 = packet_sz;
    cdb[11]                                 = 0x00;

    /*-----------------------------------------------------------------------------*\
    | Send pass through command                                                     |
    \*-----------------------------------------------------------------------------*/
    ioctl(fd, SG_IO, &header);
}
