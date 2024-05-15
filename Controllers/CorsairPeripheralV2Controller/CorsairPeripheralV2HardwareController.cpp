/*---------------------------------------------------------*\
| CorsairPeripheralV2HardwareController.cpp                 |
|                                                           |
|   Driver for Corsair V2 peripherals - hardware modes      |
|                                                           |
|   Chris M (Dr_No)                             07 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"
#include "CorsairPeripheralV2HardwareController.h"

CorsairPeripheralV2HWController::CorsairPeripheralV2HWController(hid_device* dev_handle, const char* path, std::string name) : CorsairPeripheralV2Controller(dev_handle, path, name)
{
    SetRenderMode(CORSAIR_V2_MODE_SW);
    LightingControl(0x5F);
}

CorsairPeripheralV2HWController::~CorsairPeripheralV2HWController()
{

}

void CorsairPeripheralV2HWController::SetLedsDirect(std::vector<RGBColor *>colors)
{
    switch(light_ctrl)
    {
        case CORSAIR_V2_LIGHT_CTRL1:
            SetLedsDirectColourBlocks(colors);
            break;
        case CORSAIR_V2_LIGHT_CTRL2:
            SetLedsDirectTriplets(colors);
            break;
        default:
            LOG_ERROR("[%s] Error setting Direct mode: Device supportes returned %i",
                      device_name.c_str(), light_ctrl);
            break;
    }
}

void CorsairPeripheralV2HWController::SetLedsDirectColourBlocks(std::vector<RGBColor *>colors)
{
    uint16_t count          = (uint16_t)colors.size();
    uint16_t green          = count;
    uint16_t blue           = (count * 2);
    uint16_t length         = (count * 3);
    uint8_t* buffer         = new uint8_t[length];

    memset(buffer, 0, length);

    for(std::size_t i = 0; i < count; i++)
    {
        RGBColor color      = *colors[i];

        buffer[i]           = RGBGetRValue(color);
        buffer[i + green]   = RGBGetGValue(color);
        buffer[i + blue]    = RGBGetBValue(color);
    }

    SetLEDs(buffer, length);
    delete[] buffer;
}

void CorsairPeripheralV2HWController::SetLedsDirectTriplets(std::vector<RGBColor *>colors)
{
    uint16_t count          = (uint16_t)colors.size();
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
