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

#include "dmiinfo.h"
#include "LogManager.h"

#ifdef WIN32
#include "wmi.h"

/******************************************************************************************\
*                                                                                          *
*   DMIInfo (Windows)                                                                      *
*                                                                                          *
*       Implementing way to read motherboard vendor and name based on                      *
*       the win32_baseboard WMI query                                                      *
*       https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-baseboard        *
*                                                                                          *
\******************************************************************************************/
DMIInfo::DMIInfo()
{
    mainboard       = readWMIQuery("SELECT * FROM Win32_BaseBoard", "Product");
    manufacturer    = readWMIQuery("SELECT * FROM Win32_BaseBoard", "Manufacturer");
    product_name    = readWMIQuery("SELECT * FROM Win32_ComputerSystem", "Model");
}

std::string DMIInfo::readWMIQuery(std::string query, std::string key)
{
    HRESULT hres;
    Wmi     wmi;

    /*-----------------------------------------------------*\
    | Query WMI                                             |
    \*-----------------------------------------------------*/
    std::vector<QueryObj> q_result;
    hres = wmi.query(query, q_result);

    if(hres)
    {
        LOG_DEBUG("[DMI Info] Unable to read from %s", WMI);
        return "";
    }

    for(QueryObj &obj : q_result)
    {
        return obj[key];
    }
    return "";
}
#else /* WIN32 */


/******************************************************************************************\
*                                                                                          *
*   DMIInfo (Linux)                                                                        *
*                                                                                          *
*       Implementing way to read motherboard vendor and name based on                      *
*       the Linux DMIDecode which is read only for users in the sysfs @                    *
*       /sys/devices/virtual/dmi/id/                                                       *
*                                                                                          *
\******************************************************************************************/
DMIInfo::DMIInfo()
{
    mainboard       = readFilePath(SYSFS_MB_DMI "/board_name");
    manufacturer    = readFilePath(SYSFS_MB_DMI "/board_vendor");
    product_name    = readFilePath(SYSFS_PC_DMI "/product_name");
}

std::string DMIInfo::readFilePath(std::string path)
{
    if(access(path.c_str(), R_OK)!=0)
    {
        LOG_DEBUG("[DMI Info] Unable to read from %s", path.c_str());
        return "";
    }
    std::string read_path;

    std::ifstream path_stream(path, std::ifstream::in);
    getline(path_stream, read_path);
    path_stream.close();
    return read_path;
}
#endif  /* WIN32 */

DMIInfo::~DMIInfo()
{

}

std::string DMIInfo::getMainboard()
{
    return mainboard;
}

std::string DMIInfo::getManufacturer()
{
    return manufacturer;
}

std::string DMIInfo::getProductName()
{
    return product_name;
}
