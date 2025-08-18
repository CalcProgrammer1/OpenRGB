/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USBLayouts.h              |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   megadjc                                     31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <map>
#include <string>
#include <vector>

/* LED port definition */
struct LedPort
{
    std::string name;
    int         header;
    int         count;
};

/* Type aliases */
using FwdLedHeaders = std::map<std::string, int>;
using RvrseLedHeaders = std::map<int, std::string>;
using ZoneLeds = std::map<std::string, std::vector<LedPort>>;
using KnownLayout = std::map<std::string, ZoneLeds>;

extern const KnownLayout HardcodedCustom_Gen1;
extern const KnownLayout HardcodedCustom_Gen2;
extern const KnownLayout knownLayoutsLookup;
extern const FwdLedHeaders LedLookup;