#include "Detector.h"
#include "SeagateController.h"
#include "RGBController.h"
#include "RGBController_Seagate.h"
#include <vector>
#include <fileapi.h>
#include <ntddscsi.h>

#include "Detector.h"
#include "ENESMBusController.h"
#include "ENESMBusInterface_SpectrixS40G_Windows.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_ENESMBus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#define DEVBUFSIZE (128 * 1024)
#include <windows.h>
#include <fileapi.h>

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

static int Search(wchar_t *dev_name)
{
    wchar_t buff[DEVBUFSIZE]  = L"";
    int wchar_count;

    wchar_count = QueryDosDeviceW(NULL, buff, DEVBUFSIZE);

    for(int i = 0; i < wchar_count; i++)
    {
        buff[i] = towupper(buff[i]);
    }

    if(wchar_count == 0)
    {
        return 0;
    }

    for(int i = 0; i < wchar_count; i++)
    {
        wchar_t * buf_ptr = buff + i;
        if(wcsstr(buf_ptr, L"SCSI#DISK&VEN_SEAGATE&PROD_FIRECUDA_HDD"))
        {
            wcsncpy(dev_name, buff + i, MAX_PATH);
            (dev_name)[MAX_PATH - 1] = '\0';
            return 1;
        }

        i += wcslen(buff + i);
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

static HANDLE OpenDevice(wchar_t buff[MAX_PATH])
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
*   DetectSeagateControllers                                                               *
*                                                                                          *
*           Detects ENE SMBus controllers on XPG Spectrix S40G NVMe devices                *
*                                                                                          *
*           Tests for the existance of a file descriptor matching                          *
*           SCSI#Disk&Ven_NVMe&Prod_XPG_SPECTRIX_S40# on Windows machines                  *
*                                                                                          *
\******************************************************************************************/

void DetectSeagateControllers(std::vector<RGBController*>& rgb_controllers)
{
    /*-------------------------------------------------------------------------------------------------*\
    | https://docs.microsoft.com/en-us/windows-hardware/drivers/install/identifiers-for-scsi-devices    |
    \*-------------------------------------------------------------------------------------------------*/
    wchar_t dev_name[MAX_PATH];
    
    if(Search(dev_name))
    {
        HANDLE fd = OpenDevice(dev_name);

        if(fd != INVALID_HANDLE_VALUE)
        {
            SeagateController*     controller     = new SeagateController(fd, dev_name);
            RGBController_Seagate* rgb_controller = new RGBController_Seagate(controller);

            rgb_controllers.push_back(rgb_controller);
        }
    }
}   /* DetectSpectrixS40GControllers() */

REGISTER_DETECTOR(    "Seagate Firecuda HDD", DetectSeagateControllers);
