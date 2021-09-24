/*-------------------------------------------------------------------*\
|  dmiinfo.cpp                                                        |
|                                                                     |
|  Read DMI information for motherboard vendor and name               |
|                                                                     |
|  Chris M (Dr_No)          30th Jun 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "dmiinfo.h"

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
    mainboard       = "";
    manufacturer    = "";
    HRESULT hres;
    Wmi wmi;

    // Query WMI for Win32_PnPSignedDriver entries with names matching "SMBUS" or "SM BUS"
    // These devices may be browsed under Device Manager -> System Devices
    std::vector<QueryObj> q_res_BaseBoard;
    hres = wmi.query("SELECT * FROM Win32_BaseBoard", q_res_BaseBoard);

    if (hres)
    {
        return;
    }

    for (QueryObj &i : q_res_BaseBoard)
    {
        manufacturer    = i["Manufacturer"].c_str();
        mainboard       = i["Product"].c_str();
    }
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
    mainboard       = "";
    manufacturer    = "";

    if ((access(SYSFSDMI "/board_vendor", R_OK)!=0) && (access(SYSFSDMI "/board_name", R_OK)!=0))
        return;

    std::ifstream mftr(SYSFSDMI "/board_vendor", std::ifstream::in);
    getline(mftr, manufacturer);
    mftr.close();

    std::ifstream prdt(SYSFSDMI "/board_name", std::ifstream::in);
    getline(prdt, mainboard);
    prdt.close();
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
