/*---------------------------------------------------------*\
| i2c_smbus_amdadl.h                                        |
|                                                           |
|   Definitions and types for AMD ADL I2C functions         |
|                                                           |
|   Niels Westphal (crashniels)                 30 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <tchar.h>
#include <stdio.h>
#include <vector>
#include "i2c_smbus.h"
#include "adl_sdk.h"
#include "adl_defines.h"
#include "adl_structures.h"
#include "windows.h"

class i2c_smbus_amdadl : public i2c_smbus_interface
{
public:
    i2c_smbus_amdadl(ADL_CONTEXT_HANDLE context, int adapter_index);

    int LoadLibraries();
    //void* __stdcall ADL_Main_Memory_Alloc ( int iSize );
    //void __stdcall ADL_Main_Memory_Free ( void* lpBuffer );
    int ADL_Initialize();

private:
    s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);
    s32 i2c_xfer(u8 addr, char read_write, int* size, u8* data);
    ADL_CONTEXT_HANDLE context = NULL;
};
