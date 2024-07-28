/*---------------------------------------------------------*\
| AsusTUFLaptopController_Windows.cpp                       |
|                                                           |
|   Driver for ASUS TUF laptop                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <Objbase.h>
#include <setupapi.h>
#include <comdef.h>
#include <Wbemidl.h>
#include "AsusTUFLaptopController_Windows.h"

static bool coInitialized = 0;

static GUID CLSID_GUID_DEVCLASS_SYSTEM = { 0x4D36E97D, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } };

int AsusTUFLaptopController::checkWMIType()
{
    int n;
    int result = 0;
    struct _SP_DEVINFO_DATA DeviceInfoData;
    const int bufsize = 260;
    wchar_t PropertyBuffer[bufsize];

    HDEVINFO devinfo = SetupDiGetClassDevsW(&CLSID_GUID_DEVCLASS_SYSTEM, 0, 0, 2u);

    if(devinfo == HDEVINFO(-1))
    {
        return 0;
    }

    n = 0;
    DeviceInfoData.cbSize = sizeof(DeviceInfoData);

    while(SetupDiEnumDeviceInfo(devinfo, n, &DeviceInfoData)) // Invalid buffer
    {
        if(SetupDiGetDeviceRegistryPropertyW(devinfo,
                                             &DeviceInfoData,
                                             SPDRP_ENUMERATOR_NAME,
                                             NULL,
                                             PBYTE(PropertyBuffer),
                                             sizeof(PropertyBuffer),
                                             0))
        {
            // If we found property "ACPI"
            if(!wcscmp(PropertyBuffer, L"ACPI"))
            {
                memset(PropertyBuffer, 0, sizeof(PropertyBuffer));
                if(SetupDiGetDeviceInstanceIdW(devinfo, &DeviceInfoData, PropertyBuffer, bufsize, 0))
                {
                    _wcsupr_s(PropertyBuffer, bufsize);
                    if(wcsstr(PropertyBuffer, L"ACPI\\ATK0100"))
                    {
                        result = 1;
                        break;
                    }
                    if(!wcscmp(PropertyBuffer, L"ACPI\\PNP0C14\\ATK"))
                    {
                        result = 2;
                        break;
                    }
                }
            }
        }

        n++;
    }
    SetupDiDestroyDeviceInfoList(devinfo);

    return(result);
}

AsusTUFLaptopController::AsusTUFLaptopController()
{
    hDevice = CreateFileW(L"\\\\.\\ATKACPI", 0xC0000000, 3u, 0, 3u, 0, 0);
}

AsusTUFLaptopController* AsusTUFLaptopController::checkAndCreate()
{
    // This might cause issues when coInitialize() is used in multiple places
    HRESULT init = CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    if(init < 0 && init != 0x80010106)
    {
        return(0);
    }

    coInitialized = 1;

    int type = checkWMIType();
    if(type == 2)
    {
        AsusTUFLaptopController* controller = new AsusTUFLaptopController();
        if(controller->hDevice != HANDLE(-1))
        {
            return(controller);
        }
        delete controller;
    }

    return(nullptr);
}

AsusTUFLaptopController::~AsusTUFLaptopController()
{
    if(hDevice && hDevice != HANDLE(-1))
    {
        CloseHandle(hDevice);
        hDevice = 0;
    }

    /*-----------------------------------------------------*\
    | This might cause issues when coInitialize() is used   |
    | in multiple places                                    |
    \*-----------------------------------------------------*/
    if(coInitialized)
    {
        CoUninitialize();
        coInitialized = 0;
    }
}

bool AsusTUFLaptopController::deviceIoControlWrapper(const void *dataIn, int commandIndex, int dataSizeIn, void *dataOut, int *dataSizeOut)
{
    size_t BytesReturned;
    const int bufsize = 1024;
    char outBuffer[bufsize];

    LPDWORD inBuffer = LPDWORD(malloc(dataSizeIn + 8));
    inBuffer[0] = commandIndex;
    inBuffer[1] = dataSizeIn;
    memmove(inBuffer + 2, dataIn, dataSizeIn);
    memset(outBuffer, 0, bufsize);
    BytesReturned = 0;
    bool result = DeviceIoControl(
                hDevice,
                0x22240Cu,
                inBuffer,
                dataSizeIn + 8,
                outBuffer,
                bufsize,
                LPDWORD(&BytesReturned),
                0);
    if(result)
    {
        if((size_t)*dataSizeOut < BytesReturned)
        {
            BytesReturned = (size_t)*dataSizeOut;
        }
        memmove(dataOut, outBuffer, BytesReturned);
    }
    free(inBuffer);

    return(result);
}

bool AsusTUFLaptopController::deviceControl(int a1, int a2)
{
    if(hDevice && hDevice != HANDLE(-1))
    {
        int data[2];
        data[0] = a1;
        data[1] = a2;
        int result;
        int outBufSize = 4;

        if(deviceIoControlWrapper(&data, 1398162756, 8, &result, &outBufSize))
        {
            if(outBufSize < 4)
            {
                result = 0;
            }
            if(result == 1)
            {
                return(1);
            }
        }
    }
    return(0);
}

bool AsusTUFLaptopController::deviceControl(int a1, int a2, int a3)
{
    unsigned int data[3];
    data[0] = a1;
    data[1] = a2;
    data[2] = a3;
    int outBuf;
    int outBufSize = 4;

    if(hDevice && hDevice != HANDLE(-1))
    {
        if(deviceIoControlWrapper(data, 0x53564544, 12, &outBuf, &outBufSize))
        {
            if(outBufSize < 4)
            {
                outBuf = 0;
            }
            if(outBuf == 1)
            {
                return(1);
            }
        }
    }
    return(0);
}

bool AsusTUFLaptopController::getStatus(int a1, int *out)
{
    int status;
    int statusSize = 4;

    if(!hDevice || hDevice == HANDLE(-1) || (!deviceIoControlWrapper(&a1, 1398035268, 4, &status, &statusSize)))
    {
        return(0);
    }
    if(statusSize < 4)
    {
        status = 0;
    }

    *out = status;

    return(1);
}

bool AsusTUFLaptopController::getStatusExtended(int a1, int a2, int *status1, int *status2, int* status3)
{
    int commandData[2];
    commandData[0] = a1;
    commandData[1] = a2;
    int statusBuffer[3];
    int statusSize = 12;

    if(hDevice && hDevice != HANDLE(-1) && deviceIoControlWrapper(commandData, 1398035268, 8, statusBuffer, &statusSize))
    {
        *status1 = statusBuffer[0];
        *status2 = statusBuffer[1];
        *status3 = statusBuffer[2];

        return(1);
    }
    else
    {
        return(0);
    }
}

void AsusTUFLaptopController::setMode(unsigned char red,
                                      unsigned char green,
                                      unsigned char blue,
                                      unsigned char mode,
                                      unsigned char speed,
                                      bool save)
{
    /*--------------------------------------------------------*\
    | Use switch case since our speed values are magic numbers |
    | Default to Medium/Normal speed                           |
    \*--------------------------------------------------------*/
    unsigned char speed_val;

    switch(speed)
    {
        case(1):
            speed_val = ASUS_WMI_KEYBOARD_SPEED_SLOW;
            break;
        default:
        case(2):
            speed_val = ASUS_WMI_KEYBOARD_SPEED_NORMAL;
            break;
        case(3):
            speed_val = ASUS_WMI_KEYBOARD_SPEED_FAST;
            break;
    }

    /*----------------------------------------------------------*\
    | We need to use a magic value to save to firmware in order  |
    | To persist reboots. Save is normal op with different magic |
    \*----------------------------------------------------------*/
    unsigned char save_val = ASUS_WMI_KEYBOARD_MAGIC_USE;

    if(save)
    {
        save_val = ASUS_WMI_KEYBOARD_MAGIC_SAVE;
    }

    // B3 is store value
    unsigned int high = save_val | (mode<<8) | (red<<16) | (green<<24);
    unsigned int low  = blue | (speed_val<<8);

    deviceControl(ASUS_WMI_DEVID_TUF_RGB_MODE, high, low);
}

unsigned char AsusTUFLaptopController::getBrightness()
{
    int backlight_state = 0;
    getStatus(ASUS_WMI_DEVID_KBD_BACKLIGHT, &backlight_state);

    /*-----------------------------------------------------*\
    | Only lowest two bits indicate brightness level        |
    \*-----------------------------------------------------*/
    return backlight_state & 0x7F;
}

void AsusTUFLaptopController::setBrightness(unsigned char brightness)
{
    /*-----------------------------------------------------*\
    | Only calls in this format persistently set brightness |
    \*-----------------------------------------------------*/
    int ctrl_param = 0x80 | (brightness & 0x7F);
    deviceControl(ASUS_WMI_DEVID_KBD_BACKLIGHT, ctrl_param);
}

/*-----------------------------------------------------------*\
| These settings will not persist a reboot unless save is set |
\*-----------------------------------------------------------*/
void AsusTUFLaptopController::setPowerState(bool boot,
                                            bool awake,
                                            bool sleep,
                                            bool shutdown,
                                            bool save)
{
    unsigned int state = 0xBD;

    if(boot)     state = state | ASUS_WMI_KEYBOARD_POWER_BOOT;
    if(awake)    state = state | ASUS_WMI_KEYBOARD_POWER_AWAKE;
    if(sleep)    state = state | ASUS_WMI_KEYBOARD_POWER_SLEEP;
    if(shutdown) state = state | ASUS_WMI_KEYBOARD_POWER_SHUTDOWN;
    if(save)     state = state | ASUS_WMI_KEYBOARD_POWER_SAVE;

    deviceControl(ASUS_WMI_DEVID_TUF_RGB_STATE, state);
}

void AsusTUFLaptopController::setFanMode(int mode)
{
    deviceControl(ASUS_WMI_DEVID_FAN_BOOST_MODE, mode);
}
