/*---------------------------------------------------------*\
| scsiapi_linux.c                                           |
|                                                           |
|   Cross-platform SCSI access library                      |
|   Linux implementation                                    |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      28 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/*---------------------------------------------------------*\
| Includes                                                  |
\*---------------------------------------------------------*/
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scsiapi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------*\
| Functions                                                 |
\*---------------------------------------------------------*/

void scsi_close(struct scsi_device * dev)
{
    close(dev->fd);
}

struct scsi_device_info * scsi_enumerate(const char * vendor, const char * product)
{
    struct scsi_device_info * ret_ptr  = NULL;
    struct scsi_device_info * last_ptr = NULL;

    /*-----------------------------------------------------*\
    | Search for /dev/sgX nodes matching vendor and product |
    \*-----------------------------------------------------*/
    unsigned int sg_idx = 0;

    while(1)
    {
        /*-------------------------------------------------*\
        | Create the scsi_generic class model path          |
        \*-------------------------------------------------*/
        char sg_dev_buf[1024];
        char sg_model_buf[512];
        char sg_vendor_buf[512];
        char sg_path_buf[512];

        /*-------------------------------------------------*\
        | Open the input event path to get the model and    |
        | vendor strings                                    |
        \*-------------------------------------------------*/
        snprintf(sg_dev_buf, 1024, "/sys/class/scsi_generic/sg%d/device/model", sg_idx);
        int sg_model_fd     = open(sg_dev_buf, O_RDONLY | O_NONBLOCK);

        snprintf(sg_dev_buf, 1024, "/sys/class/scsi_generic/sg%d/device/vendor", sg_idx);
        int sg_vendor_fd    = open(sg_dev_buf, O_RDONLY | O_NONBLOCK);

        snprintf(sg_path_buf, 512, "/dev/sg%d", sg_idx);

        /*-------------------------------------------------*\
        | Fail if either path failed to open                |
        \*-------------------------------------------------*/
        if(sg_model_fd < 0 || sg_vendor_fd < 0)
        {
            close(sg_model_fd);
            close(sg_vendor_fd);
            break;
        }

        memset(sg_model_buf, 0, 512);
        memset(sg_vendor_buf, 0, 512);

        /*-------------------------------------------------*\
        | Read the model string and close the model file    |
        \*-------------------------------------------------*/
        if(read(sg_model_fd, sg_model_buf, 512) < 0)
        {
            close(sg_model_fd);
            close(sg_vendor_fd);
            break;
        }
        close(sg_model_fd);

        for(unsigned int i = 0; i < strlen(sg_model_buf); i++)
        {
            if(sg_model_buf[i] == '\r' || sg_model_buf[i] == '\n')
            {
                sg_model_buf[i] = '\0';
                break;
            }
        }

        /*-------------------------------------------------*\
        | Read the vendor string and close the vendor file  |
        \*-------------------------------------------------*/
        if(read(sg_vendor_fd, sg_vendor_buf, 512) < 0)
        {
            close(sg_vendor_fd);
            break;
        }
        close(sg_vendor_fd);

        for(unsigned int i = 0; i < strlen(sg_vendor_buf); i++)
        {
            if(sg_vendor_buf[i] == '\r' || sg_vendor_buf[i] == '\n')
            {
                sg_vendor_buf[i] = '\0';
                break;
            }
        }

        /*-------------------------------------------------*\
        | Check if this SCSI device should be added to the  |
        | list                                              |
        \*-------------------------------------------------*/
        int add_to_list = 0;

        if(vendor == NULL || product == NULL)
        {
            add_to_list = 1;
        }
        else if(strncmp(sg_model_buf, product, strlen(product)) == 0)
        {
            if(strncmp(sg_vendor_buf, vendor, strlen(vendor)) == 0)
            {
                add_to_list = 1;
            }
        }

        /*-------------------------------------------------*\
        | Create new scsi_device_info if adding to list     |
        \*-------------------------------------------------*/
        if(add_to_list == 1)
        {
            struct scsi_device_info *  info = malloc(sizeof(struct scsi_device_info));

            info->path                      = malloc(strlen(sg_path_buf) + 1);
            strcpy(info->path, sg_path_buf);

            info->vendor                    = malloc(strlen(sg_vendor_buf) + 1);
            strcpy(info->vendor, sg_vendor_buf);

            info->product                   = malloc(strlen(sg_model_buf) + 1);
            strcpy(info->product, sg_model_buf);

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

        sg_idx++;
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
    int device_fd = open(path, O_RDWR);

    struct scsi_device * device = NULL;

    if(device_fd > 0)
    {
        device      = malloc(sizeof(struct scsi_device));
        device->fd  = device_fd;
    }

    return(device);
}

int scsi_write(struct scsi_device * dev, const unsigned char * data, size_t data_length, const unsigned char * cdb, size_t cdb_length, unsigned char * sense, size_t sense_length)
{
    /*-----------------------------------------------------*\
    | Create buffer to hold header                          |
    \*-----------------------------------------------------*/
    struct sg_io_hdr    header;

    /*-----------------------------------------------------*\
    | Set up pass through command                           |
    \*-----------------------------------------------------*/
    header.interface_id                     = 'S';
    header.dxfer_direction                  = SG_DXFER_TO_DEV;
    header.cmd_len                          = cdb_length;
    header.mx_sb_len                        = sense_length;
    header.iovec_count                      = 0;
    header.dxfer_len                        = data_length;
    header.dxferp                           = (void *)data;
    header.cmdp                             = (unsigned char *)cdb;
    header.sbp                              = sense;
    header.timeout                          = 20000;
    header.flags                            = 0;

    /*-----------------------------------------------------*\
    | Send pass through command                             |
    \*-----------------------------------------------------*/
    ioctl(dev->fd, SG_IO, &header);
    return 0;
}

#ifdef __cplusplus
}
#endif
