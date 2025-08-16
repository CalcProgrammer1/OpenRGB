/*---------------------------------------------------------*\
| XPGSpectrixS40GDetect_Windows.cpp                         |
|                                                           |
|   Detector for XPG Spectrix S40G (Windows)                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <windows.h>
#include <fileapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "Detector.h"
#include "ENESMBusController.h"
#include "ENESMBusInterface_SpectrixS40G_Windows.h"
#include "RGBController.h"
#include "RGBController_ENESMBus.h"

#define DEVBUFSIZE (128 * 1024)

/*----------------------------------------------------------------------*\
| Windows defines "interface" for some reason.  Work around this         |
\*----------------------------------------------------------------------*/
#ifdef interface
#undef interface
#endif

/******************************************************************************************\
*                                                                                          *
*   Search                                                                                 *
*                                                                                          *
*           Search for an NVMe device matching "XPG SPECTRIX S40G"                         *
*                                                                                          *
\******************************************************************************************/

int Search(wchar_t *dev_name)
{
    wchar_t buff[DEVBUFSIZE]  = L"";
    int wchar_count;

    wchar_count = QueryDosDeviceW(NULL, buff, DEVBUFSIZE);

    if(wchar_count == 0)
    {
        return 0;
    }

    for(int i = 0; i < wchar_count; i++)
    {
        if(wcsstr(buff + i, L"SCSI#Disk&Ven_NVMe&Prod_XPG_SPECTRIX_S40#"))
        {
            wcsncpy(dev_name, buff + i, MAX_PATH);
            (dev_name)[MAX_PATH - 1] = '\0';
            return 1;
        }

        i += (int)wcslen(buff + i);
    }

    return 0;
}

/******************************************************************************************\
*                                                                                          *
*   OpenDevice                                                                             *
*                                                                                          *
*           Open a handle to the given device path                                         *
*                                                                                          *
\******************************************************************************************/

HANDLE OpenDevice(wchar_t buff[MAX_PATH])
{
    wchar_t path[MAX_PATH];

    wcscpy(path, L"\\\\?\\");
    wcsncat(path, buff, MAX_PATH - 4);

    for(size_t i = 0; i < MAX_PATH && path[i] != '\0'; i++)
    {
        path[i] = tolower(path[i]);
    }

    wprintf(L"%s\n", path);

    HANDLE hDevice = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)0x0, OPEN_EXISTING, 0x0, (HANDLE)0x0);

    return(hDevice);
}

/******************************************************************************************\
*                                                                                          *
*   DetectSpectrixS40GControllers                                                          *
*                                                                                          *
*           Detects ENE SMBus controllers on XPG Spectrix S40G NVMe devices                *
*                                                                                          *
*           Tests for the existance of a file descriptor matching                          *
*           SCSI#Disk&Ven_NVMe&Prod_XPG_SPECTRIX_S40# on Windows machines                  *
*                                                                                          *
\******************************************************************************************/

void DetectSpectrixS40GControllers()
{
    /*-------------------------------------------------------------------------------------------------*\
    | https://docs.microsoft.com/en-us/windows-hardware/drivers/install/identifiers-for-scsi-devices    |
    \*-------------------------------------------------------------------------------------------------*/
    wchar_t dev_name[MAX_PATH];

    if(Search(dev_name))
    {
        HANDLE nvme_fd = OpenDevice(dev_name);

        if(nvme_fd != INVALID_HANDLE_VALUE)
        {
            ENESMBusInterface_SpectrixS40G* interface      = new ENESMBusInterface_SpectrixS40G(nvme_fd, dev_name);
            ENESMBusController*             controller     = new ENESMBusController(interface, 0x67, "XPG Spectrix S40G", DEVICE_TYPE_STORAGE);
            RGBController_ENESMBus*         rgb_controller = new RGBController_ENESMBus(controller);

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectSpectrixS40GControllers() */


REGISTER_DETECTOR(    "XPG Spectrix S40G",              DetectSpectrixS40GControllers);
