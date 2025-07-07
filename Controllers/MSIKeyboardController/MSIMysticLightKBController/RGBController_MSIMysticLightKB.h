/*---------------------------------------------------------*\
| RGBController_MSIMysticLight1565.h                        |
|                                                           |
|   Driver for MSI Mystic Light MS-1565 keyboard leds       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifndef RGBCONTROLLER_MSIMYSTICLIGHTKB_H
#define RGBCONTROLLER_MSIMYSTICLIGHTKB_H

#include "RGBController.h"
#include "MSIMysticLightKBController.h"

class RGBController_MSIKeyboard : public RGBController
{
public:
    RGBController_MSIKeyboard(MSIKeyboardController* controller_ptr);
    ~RGBController_MSIKeyboard();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSIKeyboardController*       controller;

    void        SetupModes();
    void        SetupMode
                (
                const std::string name,
                MS_1565_MODE     mode,
                unsigned int      flags,
                MS_1565_ZONE      zone
                );
    void        SetupZonesMode
                (
                const std::string name,
                MS_1565_MODE mod,
                unsigned int flags
                );
};

#endif // RGBCONTROLLER_MSIMYSTICLIGHTKB_H
