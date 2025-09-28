/*---------------------------------------------------------*\
| ENESMBusInterface_ROGArion.cpp                            |
|                                                           |
|   ENE SMBus interface for ASUS ROG Arion                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                17 Sep 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ENESMBusInterface_ROGArion.h"

ENESMBusInterface_ROGArion::ENESMBusInterface_ROGArion(scsi_device* dev_handle, char* dev_path)
{
    scsi_dev = dev_handle;
	path     = dev_path;
}

ENESMBusInterface_ROGArion::~ENESMBusInterface_ROGArion()
{

}

ene_interface_type ENESMBusInterface_ROGArion::GetInterfaceType()
{
    return(ENE_INTERFACE_TYPE_ROG_ARION);
}

std::string ENESMBusInterface_ROGArion::GetLocation()
{
	std::string str(path.begin(), path.end());
    return("SCSI: " + str);
}

int ENESMBusInterface_ROGArion::GetMaxBlock()
{
    return(24);
}

unsigned char ENESMBusInterface_ROGArion::ENERegisterRead(ene_dev_id /*dev*/, ene_register /*reg*/)
{
    /*-----------------------------------------------------------------------------*\
    | This interface does not support reading                                       |
    \*-----------------------------------------------------------------------------*/
    return( 0 );
}

void ENESMBusInterface_ROGArion::ENERegisterWrite(ene_dev_id /*dev*/, ene_register reg, unsigned char val)
{
    SendPacket(reg, &val, sizeof(unsigned char));
}

void ENESMBusInterface_ROGArion::ENERegisterWriteBlock(ene_dev_id /*dev*/, ene_register reg, unsigned char * data, unsigned char sz)
{
    SendPacket(reg, data, sz);
}

void ENESMBusInterface_ROGArion::SendPacket
    (
    ene_register    reg,
    unsigned char * packet,
    unsigned char   packet_sz
    )
{
    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold CDB                                                     |
    \*-----------------------------------------------------------------------------*/
    unsigned char cdb[16]                   = {0};
    cdb[0]                                  = 0xEC;
    cdb[1]                                  = 0x41;
    cdb[2]                                  = 0x53;
    cdb[3]                                  = ((reg >> 8) & 0x00FF);
    cdb[4]                                  = ( reg & 0x00FF );
    cdb[5]                                  = 0x00;
    cdb[6]                                  = 0x00;
    cdb[7]                                  = 0x00;
    cdb[8]                                  = 0x00;
    cdb[9]                                  = 0x00;
    cdb[10]                                 = 0x00;
    cdb[11]                                 = 0x00;
    cdb[12]                                 = 0x00;
    cdb[13]                                 = packet_sz;
    cdb[14]                                 = 0x00;
    cdb[15]                                 = 0x00;

    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold sense data                                              |
    \*-----------------------------------------------------------------------------*/
    unsigned char sense[32]                 = {0};

    /*-----------------------------------------------------------------------------*\
    | Write SCSI packet                                                             |
    \*-----------------------------------------------------------------------------*/
    scsi_write(scsi_dev, packet, packet_sz, cdb, 16, sense, 32);
}
