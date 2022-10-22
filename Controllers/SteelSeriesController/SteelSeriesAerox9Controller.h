/*----------------------------------------------------------------------*\
|  SteelSeriesAerox9Controller.h                                         |
|                                                                        |
|  OpenRGB controller driver for the Steelseries Aerox9 Wireless (Wired) |
|                                                                        |
|  rom4ster         11th October 2022                                    |
\*---------------------------------------------------------------------*/
#include <string>
#include <cstring>
#include <hidapi/hidapi.h>

#include "RGBController.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesMouseController.h"

#pragma once

#define STEELSERIES_AEORX9_PACKET_SIZE              65
#define STEELSERIES_AEORX9_TIMEOUT                  250

static const steelseries_mouse aerox_9 =
{
    {   0x04  },
    {
        {"Front",           0 },
        {"Middle",          1 },
        {"Rear",            2 },
        {"Reactive",        3 },
    }
};

class SteelSeriesAerox9Controller: public SteelSeriesMouseController
{
public:
    SteelSeriesAerox9Controller(hid_device* dev_handle, steelseries_type proto_type, const char* path);
    ~SteelSeriesAerox9Controller();

    std::string         GetFirmwareVersion();
    steelseries_mouse   GetMouse();

    void                Save() override;
    void                SetLightEffectAll(uint8_t effect);
    void                SetColor
                            (
                            unsigned char   zone_id,
                            unsigned char   red,
                            unsigned char   green,
                            unsigned char   blue,
                            unsigned char   brightness
                            );
private:
    void                SendInit();
    void                SetBrightness(uint8_t brightness);
    uint8_t             current_brightness;
};
