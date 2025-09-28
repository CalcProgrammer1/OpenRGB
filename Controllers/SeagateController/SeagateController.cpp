/*---------------------------------------------------------*\
| SeagateController.cpp                                     |
|                                                           |
|   Driver for Seagate                                      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "SeagateController.h"

SeagateController::SeagateController(scsi_device* dev_handle, char* path)
{
    this->dev   = dev_handle;
	this->path  = path;
}

SeagateController::~SeagateController()
{
    scsi_close(dev);
}

std::string SeagateController::GetLocation()
{
	std::string str(path.begin(), path.end());
    return("SCSI: " + str);
}

void SeagateController::SetLEDBlink
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
    unsigned char data[0x10]                = {0};
    data[0]                                 = 0x10; /* size of data packet         */
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = led_id;
    data[7]                                 = SEAGATE_MODE_BLINK;
    if(save)
    {
        data[8]                             = 0x03; /* 0x00 for no save, 0x03 for */
                                                    /* save                       */
    }
    else
    {
        data[8]                             = 0x00;
    }
    data[9]                                 = 0x10;
    data[10]                                = 0x10;
    data[11]                                = r;
    data[12]                                = g;
    data[13]                                = b;
    data[14]                                = 0xFF;
    data[15]                                = 0xFF;

    /*-----------------------------------------------------------------------------*\
    | Send packet                                                                   |
    \*-----------------------------------------------------------------------------*/
    SendPacket(data, 0x10);
}

void SeagateController::SetLEDBreathing
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
    unsigned char data[0x14]                = {0};
    data[0]                                 = 0x14; /* size of data packet         */
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = led_id;
    data[7]                                 = SEAGATE_MODE_BREATHING;
    if(save)
    {
        data[8]                             = 0x03; /* 0x00 for no save, 0x03 for */
                                                    /* save                       */
    }
    else
    {
        data[8]                             = 0x00;
    }
    data[9]                                 = 0x0F;
    data[10]                                = 0x0F;
    data[11]                                = 0x0F;
    data[12]                                = 0x0F;
    data[13]                                = r;
    data[14]                                = g;
    data[15]                                = b;
    data[16]                                = 0xFF;
    data[17]                                = 0xFF;
    data[18]                                = 0xFF;
    data[19]                                = 0x00;

    /*-----------------------------------------------------------------------------*\
    | Send packet                                                                   |
    \*-----------------------------------------------------------------------------*/
    SendPacket(data, 0x14);
}

void SeagateController::SetLEDsSpectrum
    (
    unsigned char   led_id,
    bool            /*save*/
    )
{
    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold RGB control data                                        |
    \*-----------------------------------------------------------------------------*/
    unsigned char data[0x0A]                = {0};
    data[0]                                 = 0x0A; /* size of data packet         */
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = led_id;
    data[7]                                 = SEAGATE_MODE_SPECTRUM;
    data[8]                                 = 0x02;
    data[9]                                 = 0xB4;

    /*-----------------------------------------------------------------------------*\
    | Send packet                                                                   |
    \*-----------------------------------------------------------------------------*/
    SendPacket(data, 0x0A);
}

void SeagateController::SetLEDStatic
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
    unsigned char data[0x0E]                = {0};
    data[0]                                 = 0x0E; /* size of data packet         */
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = led_id;
    data[7]                                 = SEAGATE_MODE_STATIC;
    if(save)
    {
        data[8]                             = 0x03; /* 0x00 for no save, 0x03 for */
                                                    /* save                       */
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

    /*-----------------------------------------------------------------------------*\
    | Send packet                                                                   |
    \*-----------------------------------------------------------------------------*/
    SendPacket(data, 0x0E);
}

void SeagateController::SendPacket
    (
    unsigned char * packet,
    unsigned char   packet_sz
    )
{
    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold CDB                                                     |
    \*-----------------------------------------------------------------------------*/
    unsigned char cdb[12]                   = {0};
    cdb[0]                                  = 0xD2;
    cdb[1]                                  = 0x53; /* S                           */
    cdb[2]                                  = 0x65; /* e                           */
    cdb[3]                                  = 0x74; /* t                           */
    cdb[4]                                  = 0x4C; /* L                           */
    cdb[5]                                  = 0x65; /* e                           */
    cdb[6]                                  = 0x64; /* d                           */
    cdb[7]                                  = 0x00;
    cdb[8]                                  = 0x00;
    cdb[9]                                  = 0x30;
    cdb[10]                                 = packet_sz;
    cdb[11]                                 = 0x00;

    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold sense data                                              |
    \*-----------------------------------------------------------------------------*/
    unsigned char sense[32]                 = {0};

    /*-----------------------------------------------------------------------------*\
    | Write SCSI packet                                                             |
    \*-----------------------------------------------------------------------------*/
    scsi_write(dev, packet, packet_sz, cdb, 12, sense, 32);
}
