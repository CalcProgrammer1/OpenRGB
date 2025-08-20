/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USBBoards.h               |
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

using MBName = std::map<std::string, std::string>;

extern const MBName MBName2LayoutLookup8297;
extern const MBName MBName2LayoutLookup8950;
extern const MBName MBName2LayoutLookup5702;
extern const MBName MBName2LayoutLookup5711;


