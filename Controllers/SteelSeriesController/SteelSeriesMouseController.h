/*-------------------------------------------------------------------*\
|  SteelSeriesMouseController.h                                       |
|                                                                     |
|  OpenRGB abstract driver for SteelSeries Mice                       |
|                                                                     |
|  Chris M (Dr_No)         9th June 2022                              |
\*-------------------------------------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>
#include <vector>
#include "SteelSeriesGeneric.h"
#include <cstdint>

#pragma once

#define STEELSERIES_MOUSE_BRIGHTNESS_MAX          0x64

/*-----------------------------------------------------------*\
| Theses are the specific values that get sent to set a mode  |
\*-----------------------------------------------------------*/
enum
{
    STEELSERIES_MOUSE_EFFECT_SPECTRUM_CYCLE         = 0x00,
    STEELSERIES_MOUSE_EFFECT_BREATHING_MAX          = 0x01,
    STEELSERIES_MOUSE_EFFECT_BREATHING_MID          = 0x02,
    STEELSERIES_MOUSE_EFFECT_BREATHING_MIN          = 0x03,
    STEELSERIES_MOUSE_EFFECT_DIRECT                 = 0x04,
    STEELSERIES_MOUSE_EFFECT_RAINBOW_BREATHING      = 0x05,
    STEELSERIES_MOUSE_EFFECT_DISCO                  = 0x06
};

typedef struct
{
    const char* name;
    const int   value;
} led_info;

typedef struct
{
    std::vector<uint8_t>    modes;
    std::vector<led_info>   leds;
} steelseries_mouse;

class SteelSeriesMouseController
{
public:
    SteelSeriesMouseController(hid_device*  dev_handle, steelseries_type proto_type, const char* path);
    virtual ~SteelSeriesMouseController();

    std::string                 GetDeviceLocation();
    char*                       GetDeviceName();
    std::string                 GetSerialString();
    steelseries_type            GetMouseType();

    /*-----------------------------------------------------------------*\
    | Save has a common function but can be overridden                  |
    \*-----------------------------------------------------------------*/
    virtual void                Save();

    virtual steelseries_mouse   GetMouse()                                                          = 0;
    virtual std::string         GetFirmwareVersion()                                                = 0;
    virtual void                SetLightEffectAll(uint8_t effect)                                   = 0;
    virtual void                SetColor
                                    (
                                    unsigned char   zone_id,
                                    unsigned char   red,
                                    unsigned char   green,
                                    unsigned char   blue,
                                    unsigned char   brightness
                                    )                                                               = 0;

protected:
    char                        device_name[32];
    hid_device*                 dev;
    std::string                 location;
    steelseries_type            proto;

private:

};
