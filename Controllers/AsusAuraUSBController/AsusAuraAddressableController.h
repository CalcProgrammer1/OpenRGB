/*-----------------------------------------*\
|  AsusAuraAddressableController.h          |
|                                           |
|  Definitions and types for ASUS Aura      |
|  Addressable RGB lighting controller      |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2020   |
\*-----------------------------------------*/

#include "RGBController.h"
#include "AsusAuraUSBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_ADDRESSABLE_CONTROL_MODE_EFFECT            = 0x3B,     /* Effect control mode                  */
};

class AuraAddressableController : public AuraUSBController
{
public:
    AuraAddressableController(hid_device* dev_handle, const char* path);
    ~AuraAddressableController();

    void SetChannelLEDs
        (
        unsigned char   channel,
        RGBColor *      colors,
        unsigned int    num_colors
        );

    void SetMode
        (
        unsigned char   channel,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu
        );

private:

    void SendEffect
        (
        unsigned char   channel,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu
        );

    void    SendDirectApply
        (
        unsigned char   channel
        );
};
