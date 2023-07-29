/*---------------------------------------------------------*\
| scsiapi.h                                                 |
|                                                           |
|   Cross-platform SCSI access library                      |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com>      7/28/2023   |
\*---------------------------------------------------------*/

#pragma once

/*---------------------------------------------------------*\
| Includes                                                  |
\*---------------------------------------------------------*/
#include <string.h>

/*---------------------------------------------------------*\
| Platform-specific Includes                                |
\*---------------------------------------------------------*/
#ifdef WIN32
#include <fileapi.h>
#include <ntddscsi.h>
#include <windows.h>
#else
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <scsi/sg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------*\
| SCSI Device type                                          |
\*---------------------------------------------------------*/
struct scsi_device_info
{
    char *                      path;       /* path string of SCSI device       */
    char *                      vendor;     /* vendor string of SCSI device     */
    char *                      product;    /* product string of SCSI device    */
    struct scsi_device_info *   next;       /* pointer to next scsi_device_info */
};

struct scsi_device
{
    /*-----------------------------------------------------*\
    | SCSI device handle/file descriptor                    |
    \*-----------------------------------------------------*/
#ifdef WIN32
    HANDLE      fd;
#else
    int         fd;
#endif
    char *      vendor;
    char *      product;

};

/*---------------------------------------------------------*\
| Functions                                                 |
\*---------------------------------------------------------*/
void scsi_close(struct scsi_device * dev);

struct scsi_device_info * scsi_enumerate(const char * vendor, const char * product);

void scsi_free_enumeration(struct scsi_device_info * devs);

struct scsi_device * scsi_open_path(const char *path);

int scsi_write(struct scsi_device * dev, const unsigned char * data, size_t length);

#ifdef __cplusplus
}
#endif
