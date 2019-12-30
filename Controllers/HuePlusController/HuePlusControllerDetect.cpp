#include "HuePlusController.h"
#include "RGBController.h"
#include "RGBController_HuePlus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <fcntl.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>

#ifndef WIN32
#include <unistd.h>
#include <dirent.h>
#endif

#ifdef WIN32
#include <initguid.h>
#include <windows.h>
#include <Setupapi.h>

//Buffer length
#define BUFF_LEN 20

#pragma comment (lib, "Setupapi.lib")
#pragma comment(lib, "advapi32")

bool GetUsbComPort(char* portname, char* vid, char* pid)
{
    HDEVINFO        DeviceInfoSet;
    DWORD           DeviceIndex             = 0;
    SP_DEVINFO_DATA DeviceInfoData;
    const char *    DevEnum                 = "USB";
    char            ExpectedDeviceId[80]    = {0}; //Store hardware id
    BYTE            szBuffer[1024]          = {0};
    DEVPROPTYPE     ulPropertyType;
    DWORD           dwSize                  = 0;

    /*-----------------------------------------------------------------*\
    | Create device hardware id                                         |
    \*-----------------------------------------------------------------*/
    strcpy(ExpectedDeviceId,"vid_");
    strcat(ExpectedDeviceId,vid);
    strcat(ExpectedDeviceId,"&pid_");
    strcat(ExpectedDeviceId,pid);

    /*-----------------------------------------------------------------*\
    | SetupDiGetClassDevs returns a handle to a device information set  |
    \*-----------------------------------------------------------------*/
    DeviceInfoSet = SetupDiGetClassDevs( NULL, DevEnum, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);

    if (DeviceInfoSet == INVALID_HANDLE_VALUE)
    {
        return false;
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
                        strcpy(portname, pszPortName);
                        return true;
                    }
                }

                // Close the key now that we are finished with it
                RegCloseKey(hDeviceRegistryKey);
            }
        } 

    }

    if (DeviceInfoSet) 
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    }

    return false;
}
#else
bool GetUsbComPort(char* portname, char* vid, char* pid)
{
    DIR *dir;
    char symlink_path[1024];
    struct dirent *ent;

    dir = opendir("/sys/class/tty");

    if(dir == NULL)
    {
        closedir(dir);
        return false;
    }

    ent = readdir(dir);

    while(ent != NULL)
    {
        if(ent->d_type == DT_LNK)
        {
            char tty_path[1024];
            strcpy(tty_path, "/sys/class/tty/");
            strcat(tty_path, ent->d_name);

            readlink(tty_path, symlink_path, 1024);

            char * usb_string = strstr(symlink_path, "usb");

            if(usb_string != NULL)
            {
                char * usb_dev = strstr(usb_string, "/");
                usb_dev++;
                usb_dev = strtok(usb_dev, "/");

                char usb_path[1024];

                strcpy(usb_path, "/sys/bus/usb/devices/");
                strcat(usb_path, usb_dev);

                char vendor_path[1024];
                char product_path[1024];

                strcpy(vendor_path, usb_path);
                strcat(vendor_path, "/idVendor");

                strcpy(product_path, usb_path);
                strcat(product_path, "/idProduct");

                std::ifstream vendor_file;
                std::ifstream product_file;
                std::string vendor_string;
                std::string product_string;

                vendor_file.open(vendor_path);
                product_file.open(product_path);

                std::getline(vendor_file, vendor_string);
                std::getline(product_file, product_string);

                transform(vendor_string.begin(), vendor_string.end(), vendor_string.begin(), ::toupper);
                transform(product_string.begin(), product_string.end(), product_string.begin(), ::toupper);

                if((vendor_string == "04D8")&&(product_string == "00DF"))
                {
                    char* port_string = NULL;
                    for(int i = strlen(tty_path); i > 0; i--)
                    {
                        if(tty_path[i] == '/')
                        {
                            port_string = &tty_path[i + 1];
                            break;
                        }
                    }
                    strcpy(portname, "/dev/");
                    strcat(portname, port_string);
                    return true;
                }
            }
        }

        ent = readdir(dir);
    }
}

#endif
/******************************************************************************************\
*                                                                                          *
*   DetectHuePlusControllers                                                               *
*                                                                                          *
*       Detect devices supported by the HuePlus driver                                     *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectHuePlusControllers(std::vector<RGBController*> &rgb_controllers)
{
    HuePlusController* new_hueplus;
    RGBController_HuePlus* new_controller;

    char portname[12];
    if( GetUsbComPort(portname, (char *)"04D8", (char *)"00DF") )
    {
        new_hueplus = new HuePlusController();
        new_hueplus->Initialize(portname);

        new_controller = new RGBController_HuePlus(new_hueplus);
        rgb_controllers.push_back(new_controller);
    }
}   /* DetectHuePlusControllers() */
