/*---------------------------------------------------------*\
| find_usb_serial_port_win.cpp                              |
|                                                           |
|   Finds the serial port path(s) of USB serial port devices|
|   given the USB VID and PID of the device                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "find_usb_serial_port.h"

#include <initguid.h>
#include <windows.h>
#include <Setupapi.h>

//Buffer length
#define BUFF_LEN 20

#pragma comment (lib, "Setupapi.lib")
#pragma comment(lib, "advapi32")

/*---------------------------------------------------------------------*\
|                                                                       |
|   find_usb_serial_port                                                |
|                                                                       |
|   This function returns the name of the first USB serial port matching|
|   the given USB product and vendor ID.                                |
|                                                                       |
|   vid: Vendor ID code                                                 |
|   pid: Product ID code                                                |
|                                                                       |
|   returns: std::string containing port name "COMx" or "/dev/ttyX"     |
|                                                                       |
\*---------------------------------------------------------------------*/

std::vector<std::string *> find_usb_serial_port(unsigned short vid, unsigned short pid)
{
    std::vector<std::string *>  ret_vector;
    std::string *               tmp_string;
    HDEVINFO                    DeviceInfoSet;
    DWORD                       DeviceIndex             = 0;
    SP_DEVINFO_DATA             DeviceInfoData;
    const char *                DevEnum                 = "USB";
    char                        ExpectedDeviceId[80]    = {0};  //Store hardware id
    char                        vid_pid[10]             = {0};  //Store VID/PID
    char                        szBuffer[1024]          = {0};
    DEVPROPTYPE                 ulPropertyType;
    DWORD                       dwSize                  = 0;

    /*-----------------------------------------------------------------*\
    | Create device hardware id                                         |
    | "vid_ABCD&pid_CDEF"                                               |
    \*-----------------------------------------------------------------*/
    strcpy(ExpectedDeviceId, "USB\\VID_");
    snprintf(vid_pid, 10, "%04X", vid);
    strcat(ExpectedDeviceId, vid_pid);
    strcat(ExpectedDeviceId, "&PID_");
    snprintf(vid_pid, 10, "%04X", pid);
    strcat(ExpectedDeviceId, vid_pid);

    /*-----------------------------------------------------------------*\
    | SetupDiGetClassDevs returns a handle to a device information set  |
    \*-----------------------------------------------------------------*/
    DeviceInfoSet = SetupDiGetClassDevs( NULL, DevEnum, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);

    if (DeviceInfoSet == INVALID_HANDLE_VALUE)
    {
        return ret_vector;
    }

    /*-----------------------------------------------------------------*\
    | Set up Device Info Data                                           |
    \*-----------------------------------------------------------------*/
    memset(&DeviceInfoData, 0, sizeof(SP_DEVINFO_DATA));
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    /*-----------------------------------------------------------------*\
    | Receive information about an enumerated device                    |
    \*-----------------------------------------------------------------*/
    while (SetupDiEnumDeviceInfo( DeviceInfoSet, DeviceIndex, &DeviceInfoData))
    {
        DeviceIndex++;

        /*-------------------------------------------------------------*\
        | Retrieves a specified Plug and Play device property           |
        \*-------------------------------------------------------------*/
        if (SetupDiGetDeviceRegistryProperty (DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID, &ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize))
        {
            HKEY hDeviceRegistryKey;

            /*-----------------------------------------------------*\
            | Check if the string for this device property matches  |
            | our expected device string                            |
            \*-----------------------------------------------------*/
            if(strncmp(ExpectedDeviceId, szBuffer, 21) == 0)
            {
                hDeviceRegistryKey = SetupDiOpenDevRegKey(DeviceInfoSet, &DeviceInfoData,DICS_FLAG_GLOBAL, 0,DIREG_DEV, KEY_READ);
                if (hDeviceRegistryKey == INVALID_HANDLE_VALUE)
                {
                    break;
                }
                else
                {
                    char pszPortName[BUFF_LEN];
                    DWORD dwSize = sizeof(pszPortName);
                    DWORD dwType = 0;

                    /*-----------------------------------------------------*\
                    | Read in the name of the port                          |
                    \*-----------------------------------------------------*/
                    if( (RegQueryValueEx(hDeviceRegistryKey,"PortName", NULL, &dwType, (LPBYTE) pszPortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
                    {
                        if(strncmp(pszPortName, "COM", 3) == 0)
                        {
                            tmp_string = new std::string(pszPortName);
                            ret_vector.push_back(tmp_string);
                        }
                    }

                    // Close the key now that we are finished with it
                    RegCloseKey(hDeviceRegistryKey);
                }
            }
        }
    }

    if (DeviceInfoSet)
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    }

    return ret_vector;

}   /* find_usb_serial_port() */
