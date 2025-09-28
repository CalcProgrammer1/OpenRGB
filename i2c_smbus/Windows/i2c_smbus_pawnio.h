/*---------------------------------------------------------*\
| i2c_smbus_piix4_pawnio.h                                  |
|                                                           |
|   PawnIO PIIX4 SMBUS driver for Windows                   |
|                                                           |
|   Stephen Horvath (Steve-Tech)                21 Apr 2025 |
|   Based on original OpenRGB PIIX4 source code             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <unordered_map>

#include "windows.h"

#include "i2c_smbus.h"

#define GLOBAL_SMBUS_MUTEX_NAME "Global\\Access_SMBUS.HTP.Method"

class i2c_smbus_pawnio : public i2c_smbus_interface
{
public:
    static std::unordered_map<std::string, int> using_handle;
    i2c_smbus_pawnio(HANDLE handle, std::string name);
    ~i2c_smbus_pawnio();

    static HRESULT start_pawnio(std::string filename, PHANDLE phandle);

private:
    s32 pawnio_read(u8 addr, char read_write, u8 command, int size, i2c_smbus_data *data);
    s32 pawnio_write(u8 addr, char read_write, u8 command, int size, i2c_smbus_data *data);
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
    s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data);

    HANDLE      global_smbus_access_handle;
    std::string name;
    HANDLE      handle;
};
