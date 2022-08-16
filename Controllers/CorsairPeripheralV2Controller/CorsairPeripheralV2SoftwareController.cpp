/*---------------------------------------------------------------------*\
|  CorsairPeripheralV2SoftwareController.cpp                            |
|                                                                       |
|  Common driver for the newer Corsair peripherals that use             |
|    the `08` based USB protocol                                        |
|                                                                       |
|  Chris M (Dr_No)          11 Aug 2022                                 |
\*---------------------------------------------------------------------*/

#include "LogManager.h"
#include "CorsairPeripheralV2SoftwareController.h"

CorsairPeripheralV2SWController::CorsairPeripheralV2SWController(hid_device* dev_handle, const char* path, std::string name, uint16_t pid) : CorsairPeripheralV2Controller(dev_handle, path, name, pid)
{
    SetRenderMode(CORSAIR_V2_MODE_SW);
    LightingControl(0x5F, 0x00);
}

CorsairPeripheralV2SWController::~CorsairPeripheralV2SWController()
{

}

void CorsairPeripheralV2SWController::SetLedsDirect(std::vector<RGBColor *>colors)
{
    uint16_t count          = colors.size();
    uint16_t green          = count;
    uint16_t blue           = count * 2;
    uint16_t length         = count * 3;
    uint8_t* buffer         = new uint8_t[length];

    memset(buffer, 0, length);

    for(std::size_t i = 0; i < count; i++)
    {
        RGBColor color      = *colors[i];

        buffer[i]           = RGBGetRValue(color);
        buffer[green + i]   = RGBGetGValue(color);
        buffer[blue + i]    = RGBGetBValue(color);
    }

    SetLEDs(buffer, length);
    delete[] buffer;
}
