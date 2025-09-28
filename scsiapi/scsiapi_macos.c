/*---------------------------------------------------------*\
| scsiapi_macos.c                                           |
|                                                           |
|   Cross-platform SCSI access library                      |
|   MacOS implementation (NON-FUNCTIONAL)                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      28 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

/*---------------------------------------------------------*\
| Includes                                                  |
\*---------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scsiapi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UNUSED(x) (void)(x)

/*---------------------------------------------------------*\
| Functions                                                 |
\*---------------------------------------------------------*/

void scsi_close(struct scsi_device * dev)
{
    UNUSED(dev);
}

struct scsi_device_info * scsi_enumerate(const char * vendor, const char * product)
{
    UNUSED(vendor);
    UNUSED(product);

    return(NULL);
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

struct scsi_device * scsi_open_path(const char * path)
{
    UNUSED(path);

    return(NULL);
}

int scsi_write(struct scsi_device * dev, const unsigned char * data, size_t data_length, const unsigned char * cdb, size_t cdb_length, unsigned char * sense, size_t sense_length)
{
    UNUSED(dev);
    UNUSED(data);
    UNUSED(data_length);
    UNUSED(cdb);
    UNUSED(cdb_length);
    UNUSED(sense);
    UNUSED(sense_length);

    return 0;
}

#ifdef __cplusplus
}
#endif
