/*---------------------------------------------------------*\
| i2c_smbus_pawnio.cpp                                      |
|                                                           |
|   PawnIO SMBUS driver for Windows                         |
|                                                           |
|   Stephen Horvath (Steve-Tech)                04 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "Detector.h"
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
    | Get driver settings                                   |
    \*-----------------------------------------------------*/
    json drivers_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Drivers");

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

    /*-----------------------------------------------------*\
    | Store bus information                                 |
    | TODO: Remove name field once all drivers use the same |
    | ioctl names                                           |
    \*-----------------------------------------------------*/
    this->handle    = handle;
    this->name      = name;

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

s32 i2c_smbus_pawnio::pawnio_read(u8 addr, char /*read_write*/, u8 command, int size, i2c_smbus_data* data)
{
    SIZE_T      return_size;

    switch(size)
    {
    case I2C_SMBUS_BYTE:
        {
            const SIZE_T in_size = 1;
            ULONG64 in[in_size] = {addr};
            const SIZE_T out_size = 1;
            ULONG64 out[out_size];

            /*---------------------------------------------*\
            | Execute PawnIO read_byte ioctl                |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_read_byte").c_str(), in, in_size, out, out_size, &return_size);
            data->byte = (u8)out[0];

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_BYTE_DATA:
        {
            const SIZE_T in_size = 2;
            ULONG64 in[in_size] = {addr, command};
            const SIZE_T out_size = 1;
            ULONG64 out[out_size];

            /*---------------------------------------------*\
            | Execute PawnIO read_byte_data ioctl           |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_read_byte_data").c_str(), in, in_size, out, out_size, &return_size);
            data->byte = (u8)out[0];

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_WORD_DATA:
        {
            const SIZE_T in_size = 2;
            ULONG64 in[in_size] = {addr, command};
            const SIZE_T out_size = 1;
            ULONG64 out[out_size];

            /*---------------------------------------------*\
            | Execute PawnIO read_word_data ioctl           |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_read_word_data").c_str(), in, in_size, out, out_size, &return_size);
            data->word = (u16)out[0];

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_BLOCK_DATA:
        {
            const SIZE_T in_size = 2;
            ULONG64 in[in_size] = {addr, command};

            /*---------------------------------------------*\
            | Calculate output data buffer size             |
            | Pawn only deals with 64-bit cells, divide by  |
            | 8 to convert bytes to qwords.                 |
            | The first cell is also the length.            |
            \*---------------------------------------------*/
            const SIZE_T out_size = 1 + (I2C_SMBUS_BLOCK_MAX / 8);
            ULONG64 out[out_size];

            /*---------------------------------------------*\
            | Execute PawnIO read_block_data ioctl          |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_read_block_data").c_str(), in, in_size, out, out_size, &return_size);

            if(status)
            {
                return(-EIO);
            }

            if(out[0] == 0 || out[0] > I2C_SMBUS_BLOCK_MAX)
            {
                return(-EPROTO);
            }

            /*---------------------------------------------*\
            | Unpack bytes from 64bit Pawn cells            |
            \*---------------------------------------------*/
            u8 *out_bytes = (u8*)(&out[1]);
            memcpy(&data->block[1], out_bytes, out[0]);

            return(0);
        }

    default:
        return(-EOPNOTSUPP);
    }
}

s32 i2c_smbus_pawnio::pawnio_write(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    SIZE_T  return_size;

    switch(size)
    {
    case I2C_SMBUS_QUICK:
        {
            const SIZE_T in_size = 2;
            ULONG64 in[in_size] = {addr, (u8)read_write};

            /*---------------------------------------------*\
            | Execute PawnIO write_quick ioctl              |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_write_quick").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_BYTE:
        {
            const SIZE_T in_size = 2;
            ULONG64 in[in_size] = {addr, data->byte};

            /*---------------------------------------------*\
            | Execute PawnIO write_byte ioctl               |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_write_byte").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_BYTE_DATA:
        {
            const SIZE_T in_size = 3;
            ULONG64 in[in_size] = {addr, command, data->byte};

            /*---------------------------------------------*\
            | Execute PawnIO write_byte_data ioctl          |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_write_byte_data").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_WORD_DATA:
        {
            const SIZE_T in_size = 3;
            ULONG64 in[in_size] = {addr, command, data->word};

            /*---------------------------------------------*\
            | Execute PawnIO write_word_data ioctl          |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_write_word_data").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_BLOCK_DATA:
        {
            SIZE_T len = data->block[0];
            if(len == 0 || len > I2C_SMBUS_BLOCK_MAX)
            {
                return -EINVAL;
            }
            const SIZE_T in_size = 3 + (I2C_SMBUS_BLOCK_MAX/8);
            ULONG64 in[3 + (I2C_SMBUS_BLOCK_MAX/8)] = {addr, command, len};

            /*---------------------------------------------*\
            | Pack bytes into 64bit Pawn cells              |
            \*---------------------------------------------*/
            u8 *in_bytes = (u8*)&in[3];
            memcpy(in_bytes, &data->block[1], len);

            /*---------------------------------------------*\
            | Execute PawnIO write_block_data ioctl         |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(handle, ("ioctl_" + name + "_write_block_data").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    default:
        return(-EOPNOTSUPP);
    }
}

s32 i2c_smbus_pawnio::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    if(global_smbus_access_handle != NULL)
    {
        WaitForSingleObject(global_smbus_access_handle, INFINITE);
    }

    int status = -ENXIO;

    if(read_write && size != I2C_SMBUS_QUICK) {
        status = pawnio_read(addr, read_write, command, size, data);
    }
    else
    {
        status = pawnio_write(addr, read_write, command, size, data);
    }

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

// TODO: Find a better place for this function
HRESULT i2c_smbus_pawnio::start_pawnio(std::string filename, PHANDLE phandle)
{
    char    exePath[MAX_PATH];
    HANDLE  handle;
    HRESULT status;

    /*-----------------------------------------------------*\
    | Open PawnIO driver                                    |
    \*-----------------------------------------------------*/
    status = pawnio_open(phandle);

    /*-----------------------------------------------------*\
    | Check result                                          |
    \*-----------------------------------------------------*/
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
    | Load the file into PawnIO                             |
    \*-----------------------------------------------------*/
    status = pawnio_load(handle, reinterpret_cast<const UCHAR*>(blob.data()), blob.size());

    /*-----------------------------------------------------*\
    | Check if the load was successful                      |
    \*-----------------------------------------------------*/
    if(status)
    {
        LOG_ERROR("Failed to load %s, PawnIO initialization aborted", filename.c_str());
        return(status);
    }

    /*-----------------------------------------------------*\
    | Log a message and return OK if PawnIO successfully    |
    | opened                                                |
    \*-----------------------------------------------------*/
    LOG_INFO("PawnIO initialized");
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
    HRESULT                 hres;
    Wmi                     wmi;
    HANDLE                  pawnio_handle;

    /*-----------------------------------------------------*\
    | Query WMI for Win32_PnPSignedDriver entries with      |
    | names matching "SMBUS" or "SM BUS".  These devices    |
    | may be browsed under Device Manager -> System Devices |
    \*-----------------------------------------------------*/
    std::vector<QueryObj> q_res_PnPSignedDriver;
    hres = wmi.query("SELECT * FROM Win32_PnPSignedDriver WHERE Description LIKE '%SMBUS%' OR Description LIKE '%SM BUS%'", q_res_PnPSignedDriver);

    if(hres)
    {
        LOG_INFO("WMI query failed, I2C bus detection aborted");
        return(false);
    }

    /*-----------------------------------------------------*\
    | For each detected SMBus adapter, try enumerating it   |
    | as either AMD (piix4) or Intel (i801)                 |
    \*-----------------------------------------------------*/
    for(QueryObj &i : q_res_PnPSignedDriver)
    {
        /*-------------------------------------------------*\
        | Intel SMBus controllers do show I/O resources in  |
        | Device Manager.  Analysis of many Intel boards    |
        | has shown that Intel SMBus adapter I/O space      |
        | varies between boards.  We can query              |
        | Win32_PnPAllocatedResource entries and look up    |
        | the PCI device ID to find the allocated I/O space.|
        |                                                   |
        | Intel SMBus adapters use the i801 driver          |
        \*-------------------------------------------------*/
        if((i["Manufacturer"].find("Intel") != std::string::npos)
        || (i["Manufacturer"].find("INTEL") != std::string::npos))
        {
            std::string pnp_str = i["DeviceID"];

            std::size_t ven_loc = pnp_str.find("VEN_");
            std::size_t dev_loc = pnp_str.find("DEV_");
            std::size_t sub_loc = pnp_str.find("SUBSYS_");

            std::string ven_str = pnp_str.substr(ven_loc + 4, 4);
            std::string dev_str = pnp_str.substr(dev_loc + 4, 4);
            std::string sbv_str = pnp_str.substr(sub_loc + 11, 4);
            std::string sbd_str = pnp_str.substr(sub_loc + 7, 4);

            int ven_id = (int)std::stoul(ven_str, nullptr, 16);
            int dev_id = (int)std::stoul(dev_str, nullptr, 16);
            int sbv_id = (int)std::stoul(sbv_str, nullptr, 16);
            int sbd_id = (int)std::stoul(sbd_str, nullptr, 16);

            /*---------------------------------------------*\
            | Create bus                                    |
            \*---------------------------------------------*/
            if(i2c_smbus_pawnio::start_pawnio("SmbusI801.bin", &pawnio_handle) != S_OK)
            {
                return(false);
            }

            bus                         = new i2c_smbus_pawnio(pawnio_handle, "i801");
            bus->pci_vendor             = ven_id;
            bus->pci_device             = dev_id;
            bus->pci_subsystem_vendor   = sbv_id;
            bus->pci_subsystem_device   = sbd_id;
            strncpy(bus->device_name, i["Description"].c_str(), sizeof bus->device_name);
            ResourceManager::get()->RegisterI2CBus(bus);
        }

        /*-------------------------------------------------*\
        | AMD SMBus adapters use the PIIX4 driver           |
        \*-------------------------------------------------*/
        else if(i["Manufacturer"].find("Advanced Micro Devices, Inc") != std::string::npos)
        {
            std::string pnp_str = i["DeviceID"];

            std::size_t ven_loc = pnp_str.find("VEN_");
            std::size_t dev_loc = pnp_str.find("DEV_");
            std::size_t sub_loc = pnp_str.find("SUBSYS_");

            std::string ven_str = pnp_str.substr(ven_loc + 4, 4);
            std::string dev_str = pnp_str.substr(dev_loc + 4, 4);
            std::string sbv_str = pnp_str.substr(sub_loc + 11, 4);
            std::string sbd_str = pnp_str.substr(sub_loc + 7, 4);

            int ven_id = (int)std::stoul(ven_str, nullptr, 16);
            int dev_id = (int)std::stoul(dev_str, nullptr, 16);
            int sbv_id = (int)std::stoul(sbv_str, nullptr, 16);
            int sbd_id = (int)std::stoul(sbd_str, nullptr, 16);

            /*---------------------------------------------*\
            | Create primary bus                            |
            \*---------------------------------------------*/
            if(i2c_smbus_pawnio::start_pawnio("SmbusPIIX4.bin", &pawnio_handle) != S_OK)
            {
                return(false);
            }

            /*---------------------------------------------*\
            | Select port 0                                 |
            \*---------------------------------------------*/
            piix4_port_sel(pawnio_handle, 0);

            bus                         = new i2c_smbus_pawnio(pawnio_handle, "piix4");
            bus->pci_vendor             = ven_id;
            bus->pci_device             = dev_id;
            bus->pci_subsystem_vendor   = sbv_id;
            bus->pci_subsystem_device   = sbd_id;
            strncpy(bus->device_name, i["Description"].c_str(), sizeof bus->device_name);
            strncat(bus->device_name, " port 0", sizeof bus->device_name);
            ResourceManager::get()->RegisterI2CBus(bus);

            /*---------------------------------------------*\
            | Create secondary bus                          |
            \*---------------------------------------------*/
            if(i2c_smbus_pawnio::start_pawnio("SmbusPIIX4.bin", &pawnio_handle) != S_OK)
            {
                return(false);
            }

            /*---------------------------------------------*\
            | Select port 1                                 |
            \*---------------------------------------------*/
            piix4_port_sel(pawnio_handle, 1);

            bus                         = new i2c_smbus_pawnio(pawnio_handle, "piix4");
            bus->pci_vendor             = ven_id;
            bus->pci_device             = dev_id;
            bus->pci_subsystem_vendor   = sbv_id;
            bus->pci_subsystem_device   = sbd_id;
            strncpy(bus->device_name, i["Description"].c_str(), sizeof bus->device_name);
            strncat(bus->device_name, " port 1", sizeof bus->device_name);
            ResourceManager::get()->RegisterI2CBus(bus);
        }
    }

    return(true);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_pawnio_detect);
