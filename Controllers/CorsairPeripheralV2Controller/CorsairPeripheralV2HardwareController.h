/*---------------------------------------------------------*\
| CorsairPeripheralV2HardwareController.h                   |
|                                                           |
|   Driver for Corsair V2 peripherals - hardware modes      |
|                                                           |
|   Chris M (Dr_No)                             07 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairPeripheralV2Controller.h"

#include <string>
#include <hidapi.h>

#undef  CORSAIR_V2_WRITE_SIZE
#define CORSAIR_V2_WRITE_SIZE       1025
#define CORSAIR_V2HW_DATA_OFFSET    2

class CorsairPeripheralV2HWController : public CorsairPeripheralV2Controller
{
public:
    CorsairPeripheralV2HWController(hid_device* dev_handle, const char* path, std::string name);
    ~CorsairPeripheralV2HWController();

    void    SetLedsDirect(std::vector<RGBColor *> colors);

private:
    void    SetLedsDirectColourBlocks(std::vector<RGBColor *> colors);
    void    SetLedsDirectTriplets(std::vector<RGBColor *> colors);
};
