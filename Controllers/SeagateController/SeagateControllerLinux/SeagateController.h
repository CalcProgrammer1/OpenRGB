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

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>

class SeagateController
{
public:
    SeagateController(int fd, char* path);
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
    int         fd;
    std::string path;

    void SendPacket
        (
        void *          packet,
        unsigned char   packet_sz
        );
};
