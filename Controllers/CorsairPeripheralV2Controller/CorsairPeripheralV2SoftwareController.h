/*---------------------------------------------------------*\
| CorsairPeripheralV2SoftwareController.h                   |
|                                                           |
|   Driver for Corsair V2 peripherals - software modes      |
|                                                           |
|   Chris M (Dr_No)                             11 Aug 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairPeripheralV2Controller.h"

#include <string>
#include <hidapi.h>

class CorsairPeripheralV2SWController : public CorsairPeripheralV2Controller
{
public:
    CorsairPeripheralV2SWController(hid_device* dev_handle, const char* path, std::string name);
    ~CorsairPeripheralV2SWController();

    void    SetLedsDirect(std::vector<RGBColor *> colors);

private:

};
