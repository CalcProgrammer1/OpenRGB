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
#include "i2c_smbus_pawnio.h"
#include "LogManager.h"
#ifdef _WIN32
#include "PawnIOLib.h"
#endif
#include "ResourceManager.h"
#include "SettingsManager.h"

std::unordered_map<std::string, int> i2c_smbus_pawnio::using_handle;
std::unordered_map<std::string, int> i2c_smbus_pawnio::current_port;

i2c_smbus_pawnio::i2c_smbus_pawnio(void* handle, std::string name, int port)
{
#ifdef _WIN32
    json drivers_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Drivers");

    bool shared_smbus_access = true;
    if(drivers_settings.contains("shared_smbus_access"))
    {
        shared_smbus_access = drivers_settings["shared_smbus_access"].get<bool>();
    }
    if(shared_smbus_access)
    {
        global_smbus_access_handle = CreateMutexA(NULL, FALSE, GLOBAL_SMBUS_MUTEX_NAME);
    }

    pawnio_handle = (HANDLE)handle;
    this->name = name;
    this->port = port;

    // Update current_port
    pawnio_port_get();

    using_handle[name]++;
#endif
}

i2c_smbus_pawnio::~i2c_smbus_pawnio()
{
#ifdef _WIN32
    if(global_smbus_access_handle != NULL)
    {
        CloseHandle(global_smbus_access_handle);
    }

    if(--using_handle[name] == 0 && pawnio_close(pawnio_handle)) {
        LOG_ERROR("PawnIO failed to close");
    }
#endif
}

s32 i2c_smbus_pawnio::pawnio_port_get()
{
#ifdef _WIN32
    if(name != "piix4")
    {
        return 0;
    }

    const SIZE_T in_size = 1;
    ULONG64 in[in_size] = {(ULONG64)-1};
    const SIZE_T out_size = 1;
    ULONG64 out[out_size];
    SIZE_T return_size;
    HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_port_sel").c_str(), in, in_size, out, 1, &return_size);
    current_port[name] = (int)out[0];
    return status ? -EIO : 0;
#endif
}

s32 i2c_smbus_pawnio::pawnio_port_sel()
{
#ifdef _WIN32
    if(name != "piix4")
    {
        return 0;
    }

    if(port == current_port[name])
    {
        return 0;
    }

    const SIZE_T in_size = 1;
    ULONG64 in[in_size] = {(ULONG64)port};
    const SIZE_T out_size = 1;
    ULONG64 out[out_size];
    SIZE_T return_size;
    HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_port_sel").c_str(), in, in_size, out, 1, &return_size);
    current_port[name] = port;
    return status ? -EIO : 0;
#endif
}

s32 i2c_smbus_pawnio::pawnio_read(u8 addr, char /*read_write*/, u8 command, int size, i2c_smbus_data* data)
{
#ifdef _WIN32
    if(pawnio_port_sel())
    {
        return -EIO;
    }

    switch(size)
    {
    case I2C_SMBUS_BYTE:
    {
        const SIZE_T in_size = 1;
        ULONG64 in[in_size] = {addr};
        const SIZE_T out_size = 1;
        ULONG64 out[out_size];
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_byte").c_str(), in, in_size, out, out_size, &return_size);
        data->byte = (u8)out[0];
        return status ? -EIO : 0;
    }
    case I2C_SMBUS_BYTE_DATA:
    {
        const SIZE_T in_size = 2;
        ULONG64 in[in_size] = {addr, command};
        const SIZE_T out_size = 1;
        ULONG64 out[out_size];
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_byte_data").c_str(), in, in_size, out, out_size, &return_size);
        data->byte = (u8)out[0];
        return status ? -EIO : 0;
    }
    case I2C_SMBUS_WORD_DATA:
    {
        const SIZE_T in_size = 2;
        ULONG64 in[in_size] = {addr, command};
        const SIZE_T out_size = 1;
        ULONG64 out[out_size];
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_word_data").c_str(), in, in_size, out, out_size, &return_size);
        data->word = (u16)out[0];
        return status ? -EIO : 0;
    }
    case I2C_SMBUS_BLOCK_DATA:
    {
        const SIZE_T in_size = 2;
        ULONG64 in[in_size] = {addr, command};
        // Pawn only deals with 64bit cells, divide by 8 to convert bytes to qwords
        // The first cell is also the length
        const SIZE_T out_size = 1 + (I2C_SMBUS_BLOCK_MAX/8);
        ULONG64 out[out_size];
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_block_data").c_str(), in, in_size, out, out_size, &return_size);
        if(status)
        {
            return -EIO;
        }
        if(out[0] == 0 || out[0] > I2C_SMBUS_BLOCK_MAX)
        {
            return -EPROTO;
        }
        // Unpack bytes from 64bit Pawn cells
        u8 *out_bytes = (u8*)(&out[1]);
        memcpy(&data->block[1], out_bytes, out[0]);
        return 0;
    }
    default:
        return -EOPNOTSUPP;
    }
#endif
}

s32 i2c_smbus_pawnio::pawnio_write(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
#ifdef _WIN32
    if(pawnio_port_sel())
    {
        return -EIO;
    }

    switch(size)
    {
    case I2C_SMBUS_QUICK:
    {
        const SIZE_T in_size = 2;
        ULONG64 in[in_size] = {addr, (u8)read_write};
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_quick").c_str(), in, in_size, NULL, 0, &return_size);
        return status ? -EIO : 0;
    }
    case I2C_SMBUS_BYTE:
    {
        const SIZE_T in_size = 2;
        ULONG64 in[in_size] = {addr, data->byte};
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_byte").c_str(), in, in_size, NULL, 0, &return_size);
        return status ? -EIO : 0;
    }
    case I2C_SMBUS_BYTE_DATA:
    {
        const SIZE_T in_size = 3;
        ULONG64 in[in_size] = {addr, command, data->byte};
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_byte_data").c_str(), in, in_size, NULL, 0, &return_size);
        return status ? -EIO : 0;
    }
    case I2C_SMBUS_WORD_DATA:
    {
        const SIZE_T in_size = 3;
        ULONG64 in[in_size] = {addr, command, data->word};
        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_word_data").c_str(), in, in_size, NULL, 0, &return_size);
        return status ? -EIO : 0;
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

        // Pack bytes into 64bit Pawn cells
        u8 *in_bytes = (u8*)&in[3];
        memcpy(in_bytes, &data->block[1], len);

        SIZE_T return_size;
        HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_block_data").c_str(), in, in_size, NULL, 0, &return_size);
        return status ? -EIO : 0;
    }
    default:
        return -EOPNOTSUPP;
    }
#endif
}

s32 i2c_smbus_pawnio::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
#ifdef _WIN32
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

    return status;
#endif
}

s32 i2c_smbus_pawnio::i2c_xfer(u8 /*addr*/, char /*read_write*/, int* /*size*/, u8* /*data*/)
{
    return -1;
}

// TODO: Find a better place for this function
#ifdef _WIN32
HRESULT i2c_smbus_pawnio::start_pawnio(std::string filename, PHANDLE phandle) {
    HRESULT status;

    status = pawnio_open(phandle);
    if(status)
    {
        if(status == E_ACCESSDENIED)
        {
            LOG_ERROR("Permission Denied, PawnIO I2C bus detection aborted");
        }
        else
        {
            LOG_ERROR("Could not open PawnIO, PawnIO I2C bus detection aborted");
        }
        return status;
    }

    HANDLE handle = *phandle;

    // Get the path of the executable
    char exePath[MAX_PATH];
    if(!GetModuleFileNameA(NULL, exePath, MAX_PATH)) {
        LOG_ERROR("Failed to get executable path, PawnIO I2C bus detection aborted");
        return E_FAIL;
    }

    // Construct the path to `filename` in the executable's directory
    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    std::filesystem::path filePath = exeDir / filename;

    // Check if the file exists
    if(!std::filesystem::exists(filePath)) {
        LOG_ERROR("Failed to find %s in the executable's directory, PawnIO I2C bus detection aborted", filename.c_str());
        return E_FAIL;
    }

    // Open the file and read its contents
    std::ifstream file(filePath, std::ios::binary);
    if(!file.is_open()) {
        LOG_ERROR("Failed to open %s, PawnIO I2C bus detection aborted", filename.c_str());
        return E_FAIL;
    }

    std::vector<char> blob((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Load the file into PawnIO
    status = pawnio_load(handle, reinterpret_cast<const UCHAR*>(blob.data()), blob.size());
    if(status)
    {
        LOG_ERROR("Failed to load %s, PawnIO I2C bus detection aborted", filename.c_str());
        return status;
    }

    return S_OK;
}
#endif
