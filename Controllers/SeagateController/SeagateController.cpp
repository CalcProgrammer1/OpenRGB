/*-----------------------------------------*\
|  SeagateController.cpp                    |
|                                           |
|  Code for Seagate Firecuda External HDD   |
|  RGB controller                           |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2023   |
\*-----------------------------------------*/

#include "SeagateController.h"

#include "WinIoCtl.h"
#include "ntddscsi.h"

SeagateController::SeagateController(HANDLE fd, wchar_t* path)
{
    this->nvme_fd = fd;
	this->path    = path;

    /*-----------------------------------------------------------------------------*\
    | Create buffer to hold STORAGE_PROTOCOL_COMMAND                                |
    | Size must be enough for the STORAGE_PROTOCOL_COMMAND struct plus the command  |
    | data.  Subtract sizeof(DWORD) as the Command field in the structure overlaps  |
    | the actual command data.                                                      |
    \*-----------------------------------------------------------------------------*/
    unsigned char data[14]      = {0};
    unsigned char buffer[80]    = {0};
    
    data[0]                                 = 0x0E;
    data[1]                                 = 0x00;
    data[2]                                 = 0x01;
    data[3]                                 = 0x09;
    data[4]                                 = 0x01;
    data[5]                                 = 0x06;
    data[6]                                 = 0x00;
    data[7]                                 = 0x01;
    data[8]                                 = 0x03;
    data[9]                                 = 0x00;
    data[10]                                = 0x15;
    data[11]                                = 0xFF;
    data[12]                                = 0x00;
    data[13]                                = 0xFF;

    /*-----------------------------------------------------------------------------*\
    | Create STORAGE_PROTOCOL_COMMAND pointer and point it to the buffer            |
    \*-----------------------------------------------------------------------------*/
    PSCSI_PASS_THROUGH_DIRECT command       = (PSCSI_PASS_THROUGH_DIRECT)buffer;

    command->Length                         = sizeof(SCSI_PASS_THROUGH_DIRECT);
    command->ScsiStatus                     = 0x00;
    command->PathId                         = 0x00;
    command->TargetId                       = 0x00;
    command->Lun                            = 0x00;
    command->CdbLength                      = 0x0C;
    command->SenseInfoLength                = 0x20;
    command->DataIn                         = SCSI_IOCTL_DATA_OUT;
    command->DataTransferLength             = 0x0000000E;
    command->TimeOutValue                   = 0x00000014;
    command->DataBuffer                     = &data;
    command->SenseInfoOffset                = 0x0000002E;

    command->Cdb[0]                         = 0xD2;
    command->Cdb[1]                         = 0x53;
    command->Cdb[2]                         = 0x65;
    command->Cdb[3]                         = 0x74;
    command->Cdb[4]                         = 0x4C;
    command->Cdb[5]                         = 0x65;
    command->Cdb[6]                         = 0x64;
    command->Cdb[7]                         = 0x00;
    command->Cdb[8]                         = 0x00;
    command->Cdb[9]                         = 0x30;
    command->Cdb[10]                        = 0x0E;
    command->Cdb[11]                        = 0x00;

    DeviceIoControl(nvme_fd, IOCTL_SCSI_PASS_THROUGH_DIRECT, command, 80, command, 80, NULL, NULL);
}

SeagateController::~SeagateController()
{

}

std::string SeagateController::GetLocation()
{
	std::string str(path.begin(), path.end());
    return("SCSI: " + str);
}
