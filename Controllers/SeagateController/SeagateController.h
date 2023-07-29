/*-----------------------------------------*\
|  SeagateController.h                      |
|                                           |
|  Definitions for Seagate Firecuda         |
|  External HDD RGB controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2023   |
\*-----------------------------------------*/

#pragma once

#include <string>
#include "scsiapi.h"

class SeagateController
{
public:
    SeagateController(scsi_device* dev_handle, char* path);
    ~SeagateController();

    std::string GetLocation();

    void SetLED
        (
        unsigned char   led_id,
        unsigned char   r,
        unsigned char   g,
        unsigned char   b,
        bool            save
        );

private:
    scsi_device*    dev;
    std::string     path;

    void SendPacket
        (
        void *          packet,
        unsigned char   packet_sz
        );
};
