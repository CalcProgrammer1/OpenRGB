/*-----------------------------------------*\
|  ENESMBusInterface_ROGArion.h             |
|                                           |
|  Definitions and types for ENE ASUS ROG   |
|  Arion interface                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/17/2023   |
\*-----------------------------------------*/

#pragma once

#include "ENESMBusInterface.h"
#include "scsiapi.h"

class ENESMBusInterface_ROGArion : public ENESMBusInterface
{
public:
    ENESMBusInterface_ROGArion(scsi_device* dev_handle, char* dev_path);
    ~ENESMBusInterface_ROGArion();

    ene_interface_type  GetInterfaceType();
    std::string         GetLocation();
    int                 GetMaxBlock();
    unsigned char       ENERegisterRead(ene_dev_id dev, ene_register reg);
    void                ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val);
    void                ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz);

private:
    scsi_device*    scsi_dev;
    std::string     path;

    void SendPacket
        (
        ene_register    reg,
        unsigned char * packet,
        unsigned char   packet_sz
        );
};
