/*---------------------------------------------------------*\
| SeagateController.h                                       |
|                                                           |
|   Driver for Seagate                                      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "scsiapi.h"

enum
{
    SEAGATE_MODE_STATIC     = 0x01,     /* Static mode      */
    SEAGATE_MODE_BLINK      = 0x02,     /* Blink mode       */
    SEAGATE_MODE_BREATHING  = 0x03,     /* Breathing mode   */
    SEAGATE_MODE_SPECTRUM   = 0x05,     /* Spectrum mode    */
};

class SeagateController
{
public:
    SeagateController(scsi_device* dev_handle, char* path);
    ~SeagateController();

    std::string GetLocation();

    void SetLEDBlink
        (
        unsigned char   led_id,
        unsigned char   r,
        unsigned char   g,
        unsigned char   b,
        bool            save
        );

    void SetLEDBreathing
        (
        unsigned char   led_id,
        unsigned char   r,
        unsigned char   g,
        unsigned char   b,
        bool            save
        );

    void SetLEDsSpectrum
        (
        unsigned char   led_id,
        bool            save
        );

    void SetLEDStatic
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
        unsigned char * packet,
        unsigned char   packet_sz
        );
};
