/*---------------------------------------------------------*\
| RGBController_ElgatoStreamDeck.h                          |
|                                                           |
|   RGBController for Elgato Stream Deck MK.2               |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "ElgatoStreamDeckController.h"
#include "RGBController.h"

class RGBController_ElgatoStreamDeck : public RGBController
{
public:
    explicit RGBController_ElgatoStreamDeck(ElgatoStreamDeckController* controller_ptr);
    ~RGBController_ElgatoStreamDeck() override;

    void SetupZones() override;
    void ResizeZone(int zone, int new_size) override;
    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;
    void DeviceUpdateMode() override;

private:
    ElgatoStreamDeckController* controller;

    std::vector<unsigned char> CreateButtonImage(const RGBColor& color);
};
