/*---------------------------------------------------------*\
| scsiapi.c                                                 |
|                                                           |
|   Cross-platform SCSI access library                      |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      7/28/2023   |
\*---------------------------------------------------------*/

#pragma once

/*---------------------------------------------------------*\
| Includes                                                  |
\*---------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scsiapi.h"

#define DEVBUFSIZE (128 * 1024)

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------*\
| Functions                                                 |
\*---------------------------------------------------------*/

void scsi_close(struct scsi_device * dev)
{
    
}

struct scsi_device_info * scsi_enumerate(const char * vendor, const char * product)
{
    struct scsi_device_info * ret_ptr  = NULL;
    struct scsi_device_info * last_ptr = NULL;
    
    char buff[DEVBUFSIZE]  = "";
    int char_count;

    /*-----------------------------------------------------*\
    | Query all devices and look for SCSI devices           |
    \*-----------------------------------------------------*/
    char_count = QueryDosDevice(NULL, buff, DEVBUFSIZE);

    if(char_count == 0)
    {
        return 0;
    }

    for(int i = 0; i < char_count; i++)
    {
        char * buf_ptr = buff + i;

        if(strstr(buf_ptr, "SCSI"))
        {
            /*---------------------------------------------*\
            | Extract vendor and product from SCSI path     |
            | Format: SCSI#Disk&Ven_VENDOR&Prod_PRODUCT#... |
            \*---------------------------------------------*/
            char c_vendor[512];
            char c_product[512];
            char c_path[MAX_PATH];

            sscanf(buf_ptr, "SCSI#Disk&Ven_%[^&]&Prod_%[^#]#", c_vendor, c_product);

            strcpy(c_path, "\\\\?\\");
            strncat(c_path, buf_ptr, MAX_PATH - 4);

            /*---------------------------------------------*\
            | Windows converts spaces to underscores so     |
            | undo that                                     |
            | There may be a better way to do this...       |
            \*---------------------------------------------*/
            for(int pos = 0; pos < strlen(c_vendor); pos++)
            {
                if(c_vendor[pos] == '_')
                {
                    c_vendor[pos] = ' ';
                }
            }

            for(int pos = 0; pos < strlen(c_product); pos++)
            {
                if(c_product[pos] == '_')
                {
                    c_product[pos] = ' ';
                }
            }

            /*---------------------------------------------*\
            | Check if this SCSI device should be added to  |
            | the list                                      |
            \*---------------------------------------------*/
            int add_to_list = 0;

            if(vendor == NULL || product == NULL)
            {
                add_to_list = 1;
            }
            else if(strncmp(c_product, product, strlen(product)) == 0)
            {
                if(strncmp(c_vendor, vendor, strlen(vendor)) == 0)
                {
                    add_to_list = 1;
                }
            }

            /*---------------------------------------------*\
            | Create new scsi_device_info if adding to list |
            \*---------------------------------------------*/
            if(add_to_list == 1)
            {
                struct scsi_device_info *  info = malloc(sizeof(struct scsi_device_info));

                info->path                      = malloc(strlen(c_path) + 1);
                strcpy(info->path, c_path);

                info->vendor                    = malloc(strlen(c_vendor) + 1);
                strcpy(info->vendor, c_vendor);

                info->product                   = malloc(strlen(c_product) + 1);
                strcpy(info->product, c_product);

                info->next                      = NULL;

                if(ret_ptr == NULL)
                {
                    ret_ptr = info;
                }
                else
                {
                    last_ptr->next = info;
                }

                last_ptr = info;
            }
        }

        i += strlen(buff + i);
    }

    return(ret_ptr);
}

void scsi_free_enumeration(struct scsi_device_info * devs)
{
	struct scsi_device_info * dev       = devs;

	while(dev)
    {
		struct scsi_device_info * next  = dev->next;

		free(dev->path);
		free(dev->vendor);
		free(dev->product);
		free(dev);

		dev = next;
	}
}

struct scsi_device * scsi_open_path(const char *path)
{
    struct scsi_device * device = malloc(sizeof(struct scsi_device));

    device->fd                  = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)0x0, OPEN_EXISTING, 0x0, (HANDLE)0x0);

    return(device);
}

int scsi_write(struct scsi_device * dev, const unsigned char * data, size_t length)
{
    /*-----------------------------------------------------*\
    | Create buffer to hold SCSI_PASS_THROUGH_DIRECT        |
    | Size must be enough for the SCSI_PASS_THROUGH_DIRECT  |
    | struct plus the sense data.  Size of 80 bytes taken   |
    | from captured data                                    |
    \*-----------------------------------------------------*/
    unsigned char buffer[sizeof(SCSI_PASS_THROUGH_DIRECT) + 32] = {0};

    /*-----------------------------------------------------*\
    | Create PSCSI_PASS_THROUGH_DIRECT pointer and point it |
    | to the buffer                                         |
    \*-----------------------------------------------------*/
    PSCSI_PASS_THROUGH_DIRECT command       = (PSCSI_PASS_THROUGH_DIRECT)buffer;

    /*-----------------------------------------------------*\
    | Set up pass through command                           |
    \*-----------------------------------------------------*/
    command->Length                         = sizeof(SCSI_PASS_THROUGH_DIRECT);
    command->ScsiStatus                     = 0x00;
    command->PathId                         = 0x00;
    command->TargetId                       = 0x00;
    command->Lun                            = 0x00;
    command->CdbLength                      = 0x0C;
    command->SenseInfoLength                = 0x20;
    command->DataIn                         = SCSI_IOCTL_DATA_OUT;
    command->DataTransferLength             = length;
    command->TimeOutValue                   = 0x00000014;
    command->DataBuffer                     = data;
    command->SenseInfoOffset                = sizeof(SCSI_PASS_THROUGH_DIRECT);

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
    command->Cdb[10]                        = length;
    command->Cdb[11]                        = 0x00;

    /*-----------------------------------------------------*\
    | Send pass through command                             |
    \*-----------------------------------------------------*/
    DeviceIoControl(dev->fd, IOCTL_SCSI_PASS_THROUGH_DIRECT, command, sizeof(buffer), command, sizeof(buffer), NULL, NULL);
}
#ifdef __cplusplus
}
#endif
