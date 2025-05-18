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
#include "PawnIOLib.h"
#include "ResourceManager.h"
#include "SettingsManager.h"


std::unordered_map<std::string, int> i2c_smbus_pawnio::using_handle;

i2c_smbus_pawnio::i2c_smbus_pawnio(void* handle, std::string name, int port)
{
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

    pawnio_port_sel();

    using_handle[name]++;
}

i2c_smbus_pawnio::~i2c_smbus_pawnio()
{
    if(global_smbus_access_handle != NULL)
    {
        CloseHandle(global_smbus_access_handle);
    }

    if(--using_handle[name] == 0 && pawnio_close(pawnio_handle)) {
        LOG_ERROR("PawnIO failed to close");
    }
}

s32 i2c_smbus_pawnio::pawnio_port_sel()
{
    const SIZE_T    in_size         = 1;
    ULONG64         in[in_size]     = {(ULONG64)port};
    const SIZE_T    out_size        = 1;
    ULONG64         out[out_size];
    SIZE_T          return_size;
    HRESULT         status;

    /*-----------------------------------------------------*\
    | Port selection is only provided for piix4             |
    \*-----------------------------------------------------*/
    if(name != "piix4")
    {
        return 0;
    }

    /*-----------------------------------------------------*\
    | Execute PawnIO port_sel ioctl                         |
    \*-----------------------------------------------------*/
    status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_port_sel").c_str(), in, in_size, out, 1, &return_size);

    return(status ? -EIO : 0);
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
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_byte").c_str(), in, in_size, out, out_size, &return_size);
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
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_byte_data").c_str(), in, in_size, out, out_size, &return_size);
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
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_word_data").c_str(), in, in_size, out, out_size, &return_size);
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
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_read_block_data").c_str(), in, in_size, out, out_size, &return_size);

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
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_quick").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_BYTE:
        {
            const SIZE_T in_size = 2;
            ULONG64 in[in_size] = {addr, data->byte};

            /*---------------------------------------------*\
            | Execute PawnIO write_byte ioctl               |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_byte").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_BYTE_DATA:
        {
            const SIZE_T in_size = 3;
            ULONG64 in[in_size] = {addr, command, data->byte};

            /*---------------------------------------------*\
            | Execute PawnIO write_byte_data ioctl          |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_byte_data").c_str(), in, in_size, NULL, 0, &return_size);

            return(status ? -EIO : 0);
        }

    case I2C_SMBUS_WORD_DATA:
        {
            const SIZE_T in_size = 3;
            ULONG64 in[in_size] = {addr, command, data->word};

            /*---------------------------------------------*\
            | Execute PawnIO write_word_data ioctl          |
            \*---------------------------------------------*/
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_word_data").c_str(), in, in_size, NULL, 0, &return_size);

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
            HRESULT status = pawnio_execute(pawnio_handle, ("ioctl_" + name + "_write_block_data").c_str(), in, in_size, NULL, 0, &return_size);

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
