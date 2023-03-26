/*---------------------------------------------------------------------*\
|  CorsairPeripheralV2HardwareController.cpp                            |
|                                                                       |
|  Driver for the newer Corsair peripherals that use the '08'           |
|    based USB protocol and support hardware lighting modes.            |
|                                                                       |
|  Chris M (Dr_No)          07 Dec 2022                                 |
\*---------------------------------------------------------------------*/

#include "LogManager.h"
#include "CorsairPeripheralV2HardwareController.h"

CorsairPeripheralV2HWController::CorsairPeripheralV2HWController(hid_device* dev_handle, const char* path, std::string name, uint16_t pid) : CorsairPeripheralV2Controller(dev_handle, path, name, pid)
{
    SetRenderMode(CORSAIR_V2_MODE_SW);
    LightingControl(0x5F, 0x00);
}

CorsairPeripheralV2HWController::~CorsairPeripheralV2HWController()
{

}

void CorsairPeripheralV2HWController::SetLedsDirect(std::vector<RGBColor *>colors)
{
    switch(corsair_v2_device_list[device_index]->protocol)
    {
        case CORSAIR_V2_TYPE_HW_TRIPLETS:
            SetLedsDirectTriplets(colors);
            break;
        case CORSAIR_V2_TYPE_HW_COLOUR_BLOCK:
            SetLedsDirectColourBlocks(colors);
            break;
        default:
            LOG_ERROR("[%s] Error setting Direct mode: Device supportes returned %i", device_name.c_str(),
                       corsair_v2_device_list[device_index]->protocol);
            break;
    }
}

void CorsairPeripheralV2HWController::SetLedsDirectColourBlocks(std::vector<RGBColor *>colors)
{
    uint16_t count          = colors.size();
    uint16_t green          = count         + CORSAIR_V2HW_DATA_OFFSET;
    uint16_t blue           = (count * 2)   + CORSAIR_V2HW_DATA_OFFSET;
    uint16_t length         = (count * 3)   + CORSAIR_V2HW_DATA_OFFSET;
    uint8_t* buffer         = new uint8_t[length];

    memset(buffer, 0, length);

    buffer[0]               = CORSAIR_V2_MODE_DIRECT & 0xFF;
    buffer[1]               = CORSAIR_V2_MODE_DIRECT >> 8;
    for(std::size_t i = 0; i < count; i++)
    {
        RGBColor color      = *colors[i];
        std::size_t idx     = i + CORSAIR_V2HW_DATA_OFFSET;

        buffer[idx]         = RGBGetRValue(color);
        buffer[idx + green] = RGBGetGValue(color);
        buffer[idx + blue]  = RGBGetBValue(color);
    }

    SetLEDs(buffer, length);
    delete[] buffer;
}

void CorsairPeripheralV2HWController::SetLedsDirectTriplets(std::vector<RGBColor *>colors)
{
    uint16_t count          = colors.size();
    uint16_t length         = (count * 3)   + CORSAIR_V2HW_DATA_OFFSET;
    uint8_t* buffer         = new uint8_t[length];

    memset(buffer, 0, length);

    buffer[0]               = CORSAIR_V2_MODE_DIRECT & 0xFF;
    buffer[1]               = CORSAIR_V2_MODE_DIRECT >> 8;
    for(std::size_t i = 0; i < count; i++)
    {
        RGBColor color      = *colors[i];
        std::size_t idx     = (i * 3) + CORSAIR_V2HW_DATA_OFFSET;

        buffer[idx]         = RGBGetRValue(color);
        buffer[idx + 1]     = RGBGetGValue(color);
        buffer[idx + 2]     = RGBGetBValue(color);
    }

    SetLEDs(buffer, length);
    delete[] buffer;
}
