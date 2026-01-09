/*---------------------------------------------------------*\
| i2c_smbus_pawnio.cpp                                      |
|                                                           |
|   PawnIO SMBUS driver for Windows                         |
|                                                           |
|   Stephen Horvath (Steve-Tech)                04 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include "DetectionManager.h"
#include "i2c_smbus_pawnio.h"
#include "LogManager.h"
#include "PawnIOLib.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "wmi.h"

std::unordered_map<std::string, int> i2c_smbus_pawnio::using_handle;

i2c_smbus_pawnio::i2c_smbus_pawnio(HANDLE handle, std::string name)
{
    /*-----------------------------------------------------*\
    | Store bus information                                 |
    | TODO: Remove name field once all drivers use the same |
    | ioctl names                                           |
    \*-----------------------------------------------------*/
    this->handle    = handle;
    this->name      = name;

    /*-----------------------------------------------------*\
    | Get driver settings                                   |
    \*-----------------------------------------------------*/
    json drivers_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Drivers");

    /*-----------------------------------------------------*\
    | Get bus information                                   |
    \*-----------------------------------------------------*/
    const SIZE_T    in_size         = 1;
    ULONG64         in[in_size]     = {0};
    const SIZE_T    out_size        = 3;
    ULONG64         out[out_size];
    SIZE_T          return_size;
    HRESULT         status;

    status = pawnio_execute(handle, "ioctl_identity", in, in_size, out, out_size, &return_size);

    if(!status)
    {
        this->pci_vendor            = (out[2] & 0x000000000000FFFF);
        this->pci_device            = (out[2] & 0x00000000FFFF0000) >> 16;
        this->pci_subsystem_vendor  = (out[2] & 0x0000FFFF0000FFFF) >> 32;
        this->pci_subsystem_device  = (out[2] & 0xFFFF000000000000) >> 48;

        char name_str[9];
        name_str[0]                 = (out[0] & 0x00000000000000FF);
        name_str[1]                 = (out[0] & 0x000000000000FF00) >> 8;
        name_str[2]                 = (out[0] & 0x0000000000FF0000) >> 16;
        name_str[3]                 = (out[0] & 0x00000000FF000000) >> 24;
        name_str[4]                 = (out[0] & 0x000000FF00000000) >> 32;
        name_str[5]                 = (out[0] & 0x0000FF0000000000) >> 40;
        name_str[6]                 = (out[0] & 0x00FF000000000000) >> 48;
        name_str[7]                 = (out[0] & 0xFF00000000000000) >> 56;
        name_str[8]                 = 0;

        strncpy( this->device_name, name_str, 512 );
    }

    /*-----------------------------------------------------*\
    | Get shared SMBus access setting                       |
    \*-----------------------------------------------------*/
    bool shared_smbus_access = true;

    if(drivers_settings.contains("shared_smbus_access"))
    {
        shared_smbus_access = drivers_settings["shared_smbus_access"].get<bool>();
    }

    /*-----------------------------------------------------*\
    | Create global SMBus mutex if enabled                  |
    \*-----------------------------------------------------*/
    if(shared_smbus_access)
    {
        global_smbus_access_handle = CreateMutexA(NULL, FALSE, GLOBAL_SMBUS_MUTEX_NAME);
    }

    using_handle[name]++;
}

i2c_smbus_pawnio::~i2c_smbus_pawnio()
{
    /*-----------------------------------------------------*\
    | Close global SMBus mutex                              |
    \*-----------------------------------------------------*/
    if(global_smbus_access_handle != NULL)
    {
        CloseHandle(global_smbus_access_handle);
    }

    /*-----------------------------------------------------*\
    | TODO: find a way to do this without name field        |
    \*-----------------------------------------------------*/
    if(--using_handle[name] == 0 && pawnio_close(handle))
    {
        LOG_ERROR("PawnIO failed to close");
    }
}

s32 i2c_smbus_pawnio::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    /*-----------------------------------------------------*\
    | Lock SMBus mutex                                      |
    \*-----------------------------------------------------*/
    if(global_smbus_access_handle != NULL)
    {
        WaitForSingleObject(global_smbus_access_handle, INFINITE);
    }

    /*-----------------------------------------------------*\
    | Pack input data                                       |
    \*-----------------------------------------------------*/
    const SIZE_T    in_size         = 9;
    ULONG64         in[in_size];
    const SIZE_T    out_size        = 5;
    ULONG64         out[out_size];
    SIZE_T          return_size;
    HRESULT         status;

    in[0]   = addr;
    in[1]   = read_write;
    in[2]   = command;
    in[3]   = size;

    if(data != NULL)
    {
        memcpy( &in[4], data, sizeof(i2c_smbus_data));
    }

    /*-----------------------------------------------------*\
    | Perform SMBus transfer                                |
    \*-----------------------------------------------------*/
    status  = pawnio_execute(handle, "ioctl_smbus_xfer", in, in_size, out, out_size, &return_size);

    /*-----------------------------------------------------*\
    | Unpack output data                                    |
    \*-----------------------------------------------------*/
    if(data != NULL)
    {
        memcpy( data, &out[0], sizeof(i2c_smbus_data));
    }

    /*-----------------------------------------------------*\
    | Unlock SMBus mutex                                    |
    \*-----------------------------------------------------*/
    if(global_smbus_access_handle != NULL)
    {
        ReleaseMutex(global_smbus_access_handle);
    }

    return(status);
}

s32 i2c_smbus_pawnio::i2c_xfer(u8 /*addr*/, char /*read_write*/, int* /*size*/, u8* /*data*/)
{
    return(-1);
}

HRESULT i2c_smbus_pawnio::start_pawnio(std::string filename, PHANDLE phandle)
{
    char    exePath[MAX_PATH];
    HANDLE  handle;
    HRESULT status;

    LOG_INFO("Start PawnIO: %s", filename.c_str());

    /*-----------------------------------------------------*\
    | Get the path of the executable                        |
    \*-----------------------------------------------------*/
    if(!GetModuleFileNameA(NULL, exePath, MAX_PATH))
    {
        LOG_ERROR("Failed to get executable path, PawnIO initialization aborted");
        return E_FAIL;
    }

    /*-----------------------------------------------------*\
    | Construct the path to `filename` in the executable's  |
    | directory                                             |
    \*-----------------------------------------------------*/
    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    std::filesystem::path filePath = exeDir / filename;

    /*-----------------------------------------------------*\
    | Check if the file exists                              |
    \*-----------------------------------------------------*/
    if(!std::filesystem::exists(filePath))
    {
        LOG_ERROR("Failed to find %s in the executable's directory, PawnIO initialization aborted", filename.c_str());
        return(E_FAIL);
    }

    /*-----------------------------------------------------*\
    | Open the file                                         |
    \*-----------------------------------------------------*/
    std::ifstream file(filePath, std::ios::binary);

    if(!file.is_open())
    {
        LOG_ERROR("Failed to open %s, PawnIO initialization aborted", filename.c_str());
        return(E_FAIL);
    }

    /*-----------------------------------------------------*\
    | Read the contents of the file into a vector of bytes  |
    \*-----------------------------------------------------*/
    std::vector<char> blob((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    /*-----------------------------------------------------*\
    | Close the file                                        |
    \*-----------------------------------------------------*/
    file.close();

    /*-----------------------------------------------------*\
    | Open PawnIO driver                                    |
    \*-----------------------------------------------------*/
    status = pawnio_open(phandle);

    if(status)
    {
        if(status == E_ACCESSDENIED)
        {
            LOG_ERROR("Permission Denied, PawnIO initialization aborted");
        }
        else
        {
            LOG_ERROR("Could not open PawnIO, PawnIO initialization aborted");
        }

        return(status);
    }

    handle = *phandle;

    /*-----------------------------------------------------*\
    | Load the file into PawnIO                             |
    \*-----------------------------------------------------*/
    status = pawnio_load(handle, reinterpret_cast<const UCHAR*>(blob.data()), blob.size());

    /*-----------------------------------------------------*\
    | Check if the load was successful                      |
    \*-----------------------------------------------------*/
    if(status)
    {
        pawnio_close(handle);

        if (status == HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED))
        {
            LOG_INFO("PawnIO module initialization aborted (unsupported)");
        }
        else
        {
            LOG_ERROR("PawnIO module initialization aborted (code=%ld)", status);
        }

        return(status);
    }

    /*-----------------------------------------------------*\
    | Log a message and return OK if PawnIO successfully    |
    | opened                                                |
    \*-----------------------------------------------------*/
    LOG_INFO("PawnIO initialized successully");
    return(S_OK);
}

s32 piix4_port_sel(HANDLE pawnio_handle, s32 port)
{
    const SIZE_T    in_size         = 1;
    ULONG64         in[in_size]     = {(ULONG64)port};
    const SIZE_T    out_size        = 1;
    ULONG64         out[out_size];
    SIZE_T          return_size;
    HRESULT         status;

    /*-----------------------------------------------------*\
    | Execute PIIX4 port_sel ioctl                          |
    \*-----------------------------------------------------*/
    status = pawnio_execute(pawnio_handle, "ioctl_piix4_port_sel", in, in_size, out, 1, &return_size);

    return(status ? -EIO : 0);
}

bool i2c_smbus_pawnio_detect()
{
    ULONG dll_version;
    if(pawnio_version(&dll_version))
    {
        LOG_INFO("PawnIO is not loaded, PawnIO I2C bus detection aborted");
        return(false);
    }

    i2c_smbus_interface *   bus;
    HANDLE                  pawnio_handle;

    /*-----------------------------------------------------*\
    | Try to load Intel (i801) SMBus driver                 |
    \*-----------------------------------------------------*/
    if(i2c_smbus_pawnio::start_pawnio("SmbusI801.bin", &pawnio_handle) == S_OK)
    {
        bus = new i2c_smbus_pawnio(pawnio_handle, "i801");
        DetectionManager::get()->RegisterI2CBus(bus);
    }

    /*-----------------------------------------------------*\
    | Try to load AMD (PIIX4) SMBus driver - primary bus    |
    \*-----------------------------------------------------*/
    if(i2c_smbus_pawnio::start_pawnio("SmbusPIIX4.bin", &pawnio_handle) == S_OK)
    {
        /*-------------------------------------------------*\
        | Select port 0                                     |
        \*-------------------------------------------------*/
        piix4_port_sel(pawnio_handle, 0);

        bus = new i2c_smbus_pawnio(pawnio_handle, "piix4");
        DetectionManager::get()->RegisterI2CBus(bus);
    }

    /*-----------------------------------------------------*\
    | Try to load AMD (PIIX4 SMBus driver - secondary bus   |
    \*-----------------------------------------------------*/
    if(i2c_smbus_pawnio::start_pawnio("SmbusPIIX4.bin", &pawnio_handle) == S_OK)
    {
        /*--------------------------------------------------*\
        | Select port 1                                     |
        \*-------------------------------------------------*/
        piix4_port_sel(pawnio_handle, 1);

        bus = new i2c_smbus_pawnio(pawnio_handle, "piix4");
        DetectionManager::get()->RegisterI2CBus(bus);
    }

    /*-----------------------------------------------------*\
    | Try to load Nuvoton (NCT6793) SMBus driver            |
    \*-----------------------------------------------------*/
    if(i2c_smbus_pawnio::start_pawnio("SmbusNCT6793.bin", &pawnio_handle) == S_OK)
    {
        bus = new i2c_smbus_pawnio(pawnio_handle, "NCT6793");
        DetectionManager::get()->RegisterI2CBus(bus);
    }

    return(true);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_pawnio_detect);
