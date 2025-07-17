/*---------------------------------------------------------*\
| ENESMBusInterface_SpectrixS40G_Windows.cpp                |
|                                                           |
|   ENE SMBus interface for XPG Spectrix S40G (Windows)     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                21 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <windows.h>
#include <fileapi.h>
#include <nvme.h>
#include <winioctl.h>

#include "ENESMBusInterface_SpectrixS40G_Windows.h"
#include "StringUtils.h"

ENESMBusInterface_SpectrixS40G::ENESMBusInterface_SpectrixS40G(HANDLE fd, wchar_t* path)
{
    this->nvme_fd = fd;
	this->path    = path;
}

ENESMBusInterface_SpectrixS40G::~ENESMBusInterface_SpectrixS40G()
{

}

ene_interface_type ENESMBusInterface_SpectrixS40G::GetInterfaceType()
{
    return(ENE_INTERFACE_TYPE_SPECTRIX_S40G);
}

std::string ENESMBusInterface_SpectrixS40G::GetLocation()
{
    return("NVMe: " + StringUtils::wstring_to_string(path));
}

int ENESMBusInterface_SpectrixS40G::GetMaxBlock()
{
    return(24);
}

unsigned char ENESMBusInterface_SpectrixS40G::ENERegisterRead(ene_dev_id dev, ene_register reg)
{
    if(nvme_fd != INVALID_HANDLE_VALUE)
    {
        /*-----------------------------------------------------------------------------*\
        | Create buffer to hold STORAGE_PROTOCOL_COMMAND                                |
        | Size must be enough for the STORAGE_PROTOCOL_COMMAND struct plus the command  |
        | data.  Subtract sizeof(DWORD) as the Command field in the structure overlaps  |
        | the actual command data.                                                      |
        \*-----------------------------------------------------------------------------*/
        unsigned char buffer[sizeof(STORAGE_PROTOCOL_COMMAND) + (sizeof(DWORD) * 34) - sizeof(DWORD)] = {0};

        /*-----------------------------------------------------------------------------*\
        | Create STORAGE_PROTOCOL_COMMAND pointer and point it to the buffer            |
        \*-----------------------------------------------------------------------------*/
        PSTORAGE_PROTOCOL_COMMAND command       = (PSTORAGE_PROTOCOL_COMMAND)buffer;

        /*-----------------------------------------------------------------------------*\
        | Fill in STORAGE_PROTOCOL_COMMAND structure                                    |
        \*-----------------------------------------------------------------------------*/
        command->Version                        = STORAGE_PROTOCOL_STRUCTURE_VERSION;
        command->Length                         = sizeof(STORAGE_PROTOCOL_COMMAND);
        command->ProtocolType                   = ProtocolTypeNvme;
        command->Flags                          = STORAGE_PROTOCOL_COMMAND_FLAG_ADAPTER_REQUEST;
        command->ReturnStatus                   = 0x00000000;
        command->ErrorCode                      = 0x00000000;
        command->CommandLength                  = STORAGE_PROTOCOL_COMMAND_LENGTH_NVME;
        command->ErrorInfoLength                = 0x00000040;
        command->DataToDeviceTransferLength     = 0x00000000;
        command->DataFromDeviceTransferLength   = 0x00000001;
        command->TimeOutValue                   = 0x00000001;
        command->ErrorInfoOffset                = 0x00000090;
        command->DataToDeviceBufferOffset       = 0x00000000;
        command->DataFromDeviceBufferOffset     = 0x000000D0;
        command->CommandSpecific                = STORAGE_PROTOCOL_SPECIFIC_NVME_ADMIN_COMMAND;
        command->Reserved0                      = 0x00000000;
        command->FixedProtocolReturnData        = 0x00000000;
        command->Reserved1[0]                   = 0x00000000;
        command->Reserved1[1]                   = 0x00000000;
        command->Reserved1[2]                   = 0x00000000;

        /*-----------------------------------------------------------------------------*\
        | Create ENE Register Write command, filling in the appropriate register and    |
        | value                                                                         |
        \*-----------------------------------------------------------------------------*/
        PNVME_COMMAND CommandValue              = (PNVME_COMMAND)command->Command;

        unsigned short corrected_reg            = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

        CommandValue->CDW0.OPC                  = 0xFA;
        CommandValue->u.GENERAL.CDW12           = (corrected_reg << 16) | (dev << 1);
        CommandValue->u.GENERAL.CDW13           = 0x81100001;

        DWORD ExtraValue[18] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 };

        /*-----------------------------------------------------------------------------*\
        | Send the STORAGE_PROTOCOL_COMMAND to the device                               |
        \*-----------------------------------------------------------------------------*/
        DWORD bytesreturned = 0;
        DeviceIoControl(nvme_fd, IOCTL_STORAGE_PROTOCOL_COMMAND, buffer, sizeof(buffer), buffer, sizeof(buffer), &bytesreturned, (LPOVERLAPPED)0x0);

        /*-----------------------------------------------------------------------------*\
        | Copy the ENE Register Write extra data into the STORAGE_PROTOCOL_COMMAND      |
        | buffer                                                                        |
        \*-----------------------------------------------------------------------------*/
        memcpy(ExtraValue, &command->Command + sizeof(NVME_COMMAND), sizeof(ExtraValue));

        return((unsigned char)ExtraValue[16]);
    }

    return(0);
}

void ENESMBusInterface_SpectrixS40G::ENERegisterWrite(ene_dev_id dev, ene_register reg, unsigned char val)
{
    if(nvme_fd != INVALID_HANDLE_VALUE)
    {
        /*-----------------------------------------------------------------------------*\
        | Create buffer to hold STORAGE_PROTOCOL_COMMAND                                |
        | Size must be enough for the STORAGE_PROTOCOL_COMMAND struct plus the command  |
        | data.  Subtract sizeof(DWORD) as the Command field in the structure overlaps  |
        | the actual command data.                                                      |
        \*-----------------------------------------------------------------------------*/
        unsigned char buffer[sizeof(STORAGE_PROTOCOL_COMMAND) + (sizeof(DWORD) * 34) - sizeof(DWORD)];

        /*-----------------------------------------------------------------------------*\
        | Create STORAGE_PROTOCOL_COMMAND pointer and point it to the buffer            |
        \*-----------------------------------------------------------------------------*/
        PSTORAGE_PROTOCOL_COMMAND command       = (PSTORAGE_PROTOCOL_COMMAND)buffer;

        /*-----------------------------------------------------------------------------*\
        | Fill in STORAGE_PROTOCOL_COMMAND structure                                    |
        \*-----------------------------------------------------------------------------*/
        command->Version                        = STORAGE_PROTOCOL_STRUCTURE_VERSION;
        command->Length                         = sizeof(STORAGE_PROTOCOL_COMMAND);
        command->ProtocolType                   = ProtocolTypeNvme;
        command->Flags                          = STORAGE_PROTOCOL_COMMAND_FLAG_ADAPTER_REQUEST;
        command->ReturnStatus                   = 0x00000000;
        command->ErrorCode                      = 0x00000000;
        command->CommandLength                  = STORAGE_PROTOCOL_COMMAND_LENGTH_NVME;
        command->ErrorInfoLength                = 0x00000040;
        command->DataToDeviceTransferLength     = 0x00000001;
        command->DataFromDeviceTransferLength   = 0x00000000;
        command->TimeOutValue                   = 0x00000001;
        command->ErrorInfoOffset                = 0x00000090;
        command->DataToDeviceBufferOffset       = 0x000000D0;
        command->DataFromDeviceBufferOffset     = 0x00000000;
        command->CommandSpecific                = STORAGE_PROTOCOL_SPECIFIC_NVME_ADMIN_COMMAND;
        command->Reserved0                      = 0x00000000;
        command->FixedProtocolReturnData        = 0x00000000;
        command->Reserved1[0]                   = 0x00000000;
        command->Reserved1[1]                   = 0x00000000;
        command->Reserved1[2]                   = 0x00000000;

        /*-----------------------------------------------------------------------------*\
        | Create ENE Register Write command, filling in the appropriate register and    |
        | value                                                                         |
        \*-----------------------------------------------------------------------------*/
        PNVME_COMMAND CommandValue              = (PNVME_COMMAND)command->Command;

        unsigned short corrected_reg            = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

        CommandValue->CDW0.OPC                  = 0xFB;
        CommandValue->u.GENERAL.CDW12           = (corrected_reg << 16) | (dev << 1);
        CommandValue->u.GENERAL.CDW13           = 0x01100001;

        DWORD ExtraValue[18] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 };

        ExtraValue[16]                          = val;

        /*-----------------------------------------------------------------------------*\
        | Copy the ENE Register Write extra data into the STORAGE_PROTOCOL_COMMAND      |
        | buffer                                                                        |
        \*-----------------------------------------------------------------------------*/
        memcpy(&command->Command + sizeof(NVME_COMMAND), ExtraValue, sizeof(ExtraValue));

        /*-----------------------------------------------------------------------------*\
        | Send the STORAGE_PROTOCOL_COMMAND to the device                               |
        \*-----------------------------------------------------------------------------*/
        DeviceIoControl(nvme_fd, IOCTL_STORAGE_PROTOCOL_COMMAND, buffer, sizeof(buffer), buffer, sizeof(buffer), 0x0, (LPOVERLAPPED)0x0);
    }
}

void ENESMBusInterface_SpectrixS40G::ENERegisterWriteBlock(ene_dev_id dev, ene_register reg, unsigned char * data, unsigned char sz)
{
    if(nvme_fd != INVALID_HANDLE_VALUE)
    {
        /*-----------------------------------------------------------------------------*\
        | Create buffer to hold STORAGE_PROTOCOL_COMMAND                                |
        | Size must be enough for the STORAGE_PROTOCOL_COMMAND struct plus the command  |
        | data.  Subtract sizeof(DWORD) as the Command field in the structure overlaps  |
        | the actual command data.                                                      |
        \*-----------------------------------------------------------------------------*/
        unsigned char buffer[sizeof(STORAGE_PROTOCOL_COMMAND) + (sizeof(DWORD) * 39) - sizeof(DWORD)];

        /*-----------------------------------------------------------------------------*\
        | Create STORAGE_PROTOCOL_COMMAND pointer and point it to the buffer            |
        \*-----------------------------------------------------------------------------*/
        PSTORAGE_PROTOCOL_COMMAND command       = (PSTORAGE_PROTOCOL_COMMAND)buffer;

        /*-----------------------------------------------------------------------------*\
        | Fill in STORAGE_PROTOCOL_COMMAND structure                                    |
        \*-----------------------------------------------------------------------------*/
        command->Version                        = STORAGE_PROTOCOL_STRUCTURE_VERSION;
        command->Length                         = sizeof(STORAGE_PROTOCOL_COMMAND);
        command->ProtocolType                   = ProtocolTypeNvme;
        command->Flags                          = STORAGE_PROTOCOL_COMMAND_FLAG_ADAPTER_REQUEST;
        command->ReturnStatus                   = 0x00000000;
        command->ErrorCode                      = 0x00000000;
        command->CommandLength                  = STORAGE_PROTOCOL_COMMAND_LENGTH_NVME;
        command->ErrorInfoLength                = 0x00000040;
        command->DataToDeviceTransferLength     = sz;
        command->DataFromDeviceTransferLength   = 0x00000000;
        command->TimeOutValue                   = 0x00000001;
        command->ErrorInfoOffset                = 0x00000090;
        command->DataToDeviceBufferOffset       = 0x000000D0;
        command->DataFromDeviceBufferOffset     = 0x00000000;
        command->CommandSpecific                = STORAGE_PROTOCOL_SPECIFIC_NVME_ADMIN_COMMAND;
        command->Reserved0                      = 0x00000000;
        command->FixedProtocolReturnData        = 0x00000000;
        command->Reserved1[0]                   = 0x00000000;
        command->Reserved1[1]                   = 0x00000000;
        command->Reserved1[2]                   = 0x00000000;

        /*-----------------------------------------------------------------------------*\
        | Create ENE Register Write Block command, filling in the appropriate register  |
        | and value                                                                     |
        \*-----------------------------------------------------------------------------*/
        PNVME_COMMAND CommandValue              = (PNVME_COMMAND)command->Command;

        unsigned short corrected_reg            = ((reg << 8) & 0xFF00) | ((reg >> 8) & 0x00FF);

        CommandValue->CDW0.OPC                  = 0xFB;
        CommandValue->u.GENERAL.CDW12           = (corrected_reg << 16) | (dev << 1);
        CommandValue->u.GENERAL.CDW13           = 0x03100000 | sz;

        DWORD ExtraValue[23] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                 0x00000000, 0x00000000, 0x00000000 };

        memcpy(&ExtraValue[16], data, sz);

        /*-----------------------------------------------------------------------------*\
        | Copy the ENE Register Write Block extra data into the                         |
        | STORAGE_PROTOCOL_COMMAND buffer                                               |
        \*-----------------------------------------------------------------------------*/
        memcpy(&command->Command + sizeof(NVME_COMMAND), ExtraValue, sizeof(ExtraValue));

        /*-----------------------------------------------------------------------------*\
        | Send the STORAGE_PROTOCOL_COMMAND to the device                               |
        \*-----------------------------------------------------------------------------*/
        DeviceIoControl(nvme_fd, IOCTL_STORAGE_PROTOCOL_COMMAND, buffer, sizeof(buffer), buffer, sizeof(buffer), 0x0, (LPOVERLAPPED)0x0);
    }
}
