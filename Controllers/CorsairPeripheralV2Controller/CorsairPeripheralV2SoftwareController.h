/*---------------------------------------------------------------------*\
|  CorsairPeripheralV2SoftwareController.h                              |
|                                                                       |
|  Common driver for the newer Corsair peripherals that use             |
|    the `08` based USB protocol                                        |
|                                                                       |
|  Chris M (Dr_No)          11 Aug 2022                                 |
\*---------------------------------------------------------------------*/
#pragma once

#include "RGBController.h"
#include "CorsairPeripheralV2Controller.h"

#include <string>
#include <hidapi/hidapi.h>

class CorsairPeripheralV2SWController : public CorsairPeripheralV2Controller
{
public:
    CorsairPeripheralV2SWController(hid_device* dev_handle, const char* path, std::string name, uint16_t pid);
    ~CorsairPeripheralV2SWController();

    void    SetLedsDirect(std::vector<RGBColor *> colors);

private:

};
