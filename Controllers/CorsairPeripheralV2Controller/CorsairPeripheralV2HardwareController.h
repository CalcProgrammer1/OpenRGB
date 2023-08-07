/*---------------------------------------------------------------------*\
|  CorsairPeripheralV2HardwareController.h                              |
|                                                                       |
|  Driver for the newer Corsair peripherals that use the '08'           |
|    based USB protocol and support hardware lighting modes.            |
|                                                                       |
|  Chris M (Dr_No)          07 Dec 2022                                 |
\*---------------------------------------------------------------------*/
#pragma once

#include "RGBController.h"
#include "CorsairPeripheralV2Controller.h"

#include <string>
#include <hidapi/hidapi.h>

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
