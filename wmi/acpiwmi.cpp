#ifdef _WIN32

#include <windows.h>
#include <Objbase.h>
#include <setupapi.h>
#include <comdef.h>
#include <Wbemidl.h>

#include <cstdint>
#include <cstring>
#include <cstdio>

static bool IsWOW64 = false;
static HANDLE hDevice = 0;
static bool WMI_Notebook = 0;
static bool coInitialized = 0;

static GUID CLSID_GUID_DEVCLASS_SYSTEM = { 0x4D36E97D, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } };

static int CheckWMIType()
{
    int n;
    signed int v3;
    int v4;
    signed int v5;
    int v6;
    int result;
    struct _SP_DEVINFO_DATA DeviceInfoData;
    wchar_t PropertyBuffer[260];

    HDEVINFO devinfo = SetupDiGetClassDevsW(&CLSID_GUID_DEVCLASS_SYSTEM, 0, 0, 2u);
    if ( devinfo == HDEVINFO(-1) )
    {
        return 0;
    }
    n = 0;
    DeviceInfoData.cbSize = sizeof(DeviceInfoData);
    v3 = 1;
    result = 0;
    while ( SetupDiEnumDeviceInfo(devinfo, n, &DeviceInfoData) ) // Invalid buffer
    {
        if ( !SetupDiGetDeviceRegistryPropertyW(devinfo, &DeviceInfoData, 0x16u, 0, PBYTE(PropertyBuffer), 0x208u, 0) )
        {
            v5 = 0;
            v4 = wcscmp(PropertyBuffer, L"ACPI");
            if ( v4 )
            {
                v4 = -(v4 < 0) | 1;
                v5 = 0;
            }
            else
            {
                v5 = 1;
            }
        }
        if ( v5 )
        {
            memset(PropertyBuffer, 0, 0x208u);
            if ( SetupDiGetDeviceInstanceIdW(devinfo, &DeviceInfoData, PropertyBuffer, 0x104u, 0) )
            {
                _wcsupr_s(PropertyBuffer, 0x104u);
                if ( wcsstr(PropertyBuffer, L"ACPI\\ATK0100") )
                {
                    result = 1;
                    break;
                }
                v6 = wcscmp(PropertyBuffer, L"ACPI\\PNP0C14\\ATK");
                if ( v6 )
                    v6 = -(v6 < 0) | 1;
                if ( !v6 )
                {
                    result = 2;
                    v3 = 0;
                }
            }
        }
        ++n;
        if ( !v3 )
            break;
    }
    SetupDiDestroyDeviceInfoList(devinfo);
    return result;
}

static bool OpenNotebook()
{
    HMODULE kernelHandler; // eax
    void (__stdcall *IsWow64Process)(HANDLE, int *); // esi
    int wow64; // [esp+4h] [ebp-4h]

    wow64 = 0;
    kernelHandler = GetModuleHandleW(L"kernel32");
    IsWow64Process = (void (__stdcall *)(HANDLE, int *))GetProcAddress(kernelHandler, "IsWow64Process");
    if ( IsWow64Process )
    {
        int iswow;
        IsWow64Process(GetCurrentProcess(), &iswow);
        IsWOW64 = iswow;
    }
    hDevice = CreateFileW(L"\\\\.\\ATKACPI", 0xC0000000, 3u, 0, 3u, 0, 0);
    return (hDevice != HANDLE(-1));
}

bool AsWMI_Open()
{
    HRESULT init = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    if ( init < 0 && init != -2147417850 )
        return 0;
    coInitialized = 1;
    int type = CheckWMIType();
    if ( type == 2 )
    {
        OpenNotebook();
        WMI_Notebook = 1;
        return 1;
    }
    return 0;
}

void AsWMI_Close()
{
    if ( WMI_Notebook )
    {
        if ( hDevice && hDevice != (HANDLE)-1 )
        {
            CloseHandle(hDevice);
            hDevice = 0;
        }
    }
    if ( coInitialized )
    {
        CoUninitialize();
        coInitialized = 0;
    }

}

static bool DeviceIoControlWrapper(const void *dataIn, int commandIndex, int dataSizeIn, void *dataOut, int *dataSizeOut)
{
    size_t BytesReturned;
    char OutBuffer[1024];

    LPDWORD inBuffer = (LPDWORD)(malloc(dataSizeIn + 8));
    inBuffer[0] = commandIndex;
    inBuffer[1] = dataSizeIn;
    memmove(inBuffer + 2, dataIn, dataSizeIn);
    memset(OutBuffer, 0, 0x400u);
    BytesReturned = 0;
    bool result = DeviceIoControl(
                hDevice,
                0x22240Cu,
                inBuffer,
                dataSizeIn + 8,
                OutBuffer,
                0x400u,
                LPDWORD(&BytesReturned),
                0);
    if ( result )
    {
        if ( *dataSizeOut < BytesReturned )
            BytesReturned = *dataSizeOut;
        memmove(dataOut, OutBuffer, BytesReturned);
    }
    free(inBuffer);
    return result;
}

bool AsWMI_NB_DeviceControl(int a1, int a2)
{
    unsigned int data[2];

    if ( WMI_Notebook )
    {
        if ( hDevice )
        {
            if ( hDevice != (HANDLE)-1 )
            {
                data[0] = a1;
                data[1] = a2;
                int result;
                int outBufSize = 4;
                if ( DeviceIoControlWrapper(&data, 1398162756, 8, &result, &outBufSize) )
                {
                    if(outBufSize < 4)
                    {
                        result = 0;
                    }
                    if ( result == 1 )
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

bool AsWMI_NB_DeviceControl_2arg(int a1, int a2, int a3)
{
    unsigned int data[3];
    data[0] = a1;
    data[1] = a2;
    data[2] = a3;
    int outBuf;
    int outBufSize = 4;

    if ( WMI_Notebook )
    {
        if ( hDevice )
        {
            if ( hDevice != (HANDLE)-1 )
            {
                if ( DeviceIoControlWrapper(data, 0x53564544, 12, &outBuf, &outBufSize) )
                {
                    if(outBufSize < 4)
                    {
                        outBuf = 0;
                    }
                    if ( outBuf == 1 )
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

char AsWMI_NB_GetDeviceStatus(int a1, int *out)
{
    int status;
    int statusSize = 4;

    if ( !WMI_Notebook )
    {
        return 0;
    }
    if ( !hDevice || hDevice == HANDLE(-1) || (!DeviceIoControlWrapper(&a1, 1398035268, 4, &status, &statusSize)) )
    {
        return 0;
    }
    if(statusSize < 4)
    {
        status = 0;
    }
    *out = status;
    return 1;
}

bool AsWMI_NB_GetDeviceStatus_MoreBYTE(int a1, int a2, int *status1, int *status2, int* status3)
{
    if ( !WMI_Notebook )
    {
        return 0;
    }
    int commandData[2];
    commandData[0] = a1;
    commandData[1] = a2;
    int statusBuffer[3];
    int statusSize = 12;
    if ( hDevice
         && hDevice != HANDLE(-1)
         && DeviceIoControlWrapper(commandData, 1398035268, 8, statusBuffer, &statusSize) )
    {
        *status1 = statusBuffer[0];
        *status2 = statusBuffer[1];
        *status3 = statusBuffer[2];
        return 1;
    }
    else
    {
        return 0;
    }
}

#endif
