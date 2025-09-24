/*---------------------------------------------------------*\
| RGBController_ElgatoStreamDeck.h                          |
|                                                           |
|   RGBController for Elgato Stream Deck MK.2               |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "ElgatoStreamDeckController.h"
#include "RGBController.h"

class RGBController_ElgatoStreamDeck : public RGBController
{
public:
    explicit RGBController_ElgatoStreamDeck(ElgatoStreamDeckController* controller_ptr);
    ~RGBController_ElgatoStreamDeck();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ElgatoStreamDeckController* controller;

    std::vector<unsigned char> CreateButtonImage(const RGBColor& color);
};
