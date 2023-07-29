/*-----------------------------------------*\
|  SeagateController.cpp                    |
|                                           |
|  Code for Seagate Firecuda External HDD   |
|  RGB controller                           |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2023   |
\*-----------------------------------------*/

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
    data[0]                                 = 0x0E; /* size of data packet         */
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = led_id;
    data[7]                                 = 0x01;
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
    cdb[10]                                 = 14;   /* length of data packet       */
    cdb[11]                                 = 0x00;

    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold sense data                                              |
    \*-----------------------------------------------------------------------------*/
    unsigned char sense[32]                 = {0};

    scsi_write(dev, data, 14, cdb, 12, sense, 32);
}
