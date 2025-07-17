/*---------------------------------------------------------*\
| dmiinfo.h                                                 |
|                                                           |
|   Read DMI information for motherboard vendor and name    |
|                                                           |
|   Chris M (Dr_No)                             30 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

#ifdef WIN32
#include "wmi.h"

#define WMI "WMI"
#else
#include <unistd.h>
#include <fstream>

/*---------------------------------------------------------*\
| Linux file paths for Motherboard and Product Info         |
\*---------------------------------------------------------*/
#define SYSFS_MB_DMI "/sys/devices/virtual/dmi/id/"
#define SYSFS_PC_DMI "/sys/class/dmi/id/"
#endif

class DMIInfo
{
public:
    DMIInfo();
    ~DMIInfo();

#ifdef WIN32
    std::string             readWMIQuery(std::string query, std::string key);
#else
    std::string             readFilePath(std::string path);
#endif

    std::string             getMainboard();
    std::string             getManufacturer();
    std::string             getProductName();
private:
    std::string             mainboard;
    std::string             manufacturer;
    std::string             product_name;
};
