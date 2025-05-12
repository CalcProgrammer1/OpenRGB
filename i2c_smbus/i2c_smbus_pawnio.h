/*---------------------------------------------------------*\
| i2c_smbus_piix4_pawnio.h                                  |
|                                                           |
|   PawnIO PIIX4 SMBUS driver for Windows                   |
|                                                           |
|   Stephen Horvath (Steve-Tech)                21 Apr 2025 |
|   Based on original OpenRGB PIIX4 source code             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once
#include <string>
#include <unordered_map>
#include "i2c_smbus.h"
#ifdef _WIN32
#include "windows.h"
#endif

#ifdef _WIN32
#define GLOBAL_SMBUS_MUTEX_NAME "Global\\Access_SMBUS.HTP.Method"
#endif

class i2c_smbus_pawnio : public i2c_smbus_interface
{
public:
    static std::unordered_map<std::string, int> using_handle;
    static std::unordered_map<std::string, int> current_port;
    i2c_smbus_pawnio(void* handle, std::string name, int port = -1);
    ~i2c_smbus_pawnio();
#ifdef _WIN32
    static HRESULT start_pawnio(std::string filename, PHANDLE phandle);
#endif

private:
    s32 pawnio_port_get();
    s32 pawnio_port_sel();
    s32 pawnio_read(u8 addr, char read_write, u8 command, int size, i2c_smbus_data *data);
    s32 pawnio_write(u8 addr, char read_write, u8 command, int size, i2c_smbus_data *data);
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
    s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data);
    int port;
    std::string name;
#ifdef _WIN32
    HANDLE pawnio_handle = NULL;
    HANDLE global_smbus_access_handle = NULL;
#endif
};
