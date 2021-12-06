/*-------------------------------------------------------------------*\
|  dmiinfo.h                                                          |
|                                                                     |
|  Read DMI information for motherboard vendor and name               |
|                                                                     |
|  Chris M (Dr_No)          30th Jun 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#ifndef DMIINFO_H
#define DMIINFO_H

#include <string>

#ifdef WIN32
#include "wmi.h"

#define WMI "WMI"
#else
#include <unistd.h>     //Linux specific filesystem operation
#include <fstream>

#define SYSFSDMI "/sys/devices/virtual/dmi/id/"     //Linux specific file path
#endif

class DMIInfo
{
public:   
    DMIInfo();
    ~DMIInfo();

    std::string             getMainboard();
    std::string             getManufacturer();
private:
    std::string             mainboard;
    std::string             manufacturer;
};

#endif // DMIINFO_H
