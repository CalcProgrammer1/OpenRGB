#ifndef RGBCONTROLLER_LENOVO_GEN7USB_H
#define RGBCONTROLLER_LENOVO_GEN7USB_H

#pragma once

#include "RGBController.h"
#include "LenovoGen7USBController.h"

enum
{
    LENOVO_LEGION_GEN7_MODE_SCREW_RAINBOW                  = 0x01,
    LENOVO_LEGION_GEN7_MODE_RAINBOW_WAVE                   = 0x02,
    LENOVO_LEGION_GEN7_MODE_COLOR_CHANGE                   = 0x03,
    LENOVO_LEGION_GEN7_MODE_COLOR_PULSE                    = 0x04,
    LENOVO_LEGION_GEN7_MODE_COLOR_WAVE                     = 0x05,
    LENOVO_LEGION_GEN7_MODE_SMOOTH                         = 0x06,
    LENOVO_LEGION_GEN7_MODE_RAIN                           = 0x07,
    LENOVO_LEGION_GEN7_MODE_RIPPLE                         = 0x08,
    LENOVO_LEGION_GEN7_MODE_AUDIO_BOUNCE                   = 0x09,
    LENOVO_LEGION_GEN7_MODE_AUDIO_RIPPLE                   = 0x0A,
    LENOVO_LEGION_GEN7_MODE_STATIC                         = 0x0B,
    LENOVO_LEGION_GEN7_MODE_TYPE                           = 0x0C,
    LENOVO_LEGION_GEN7_MODE_DIRECT                         = 0x0D,
};

class RGBController_Lenovo_Gen7USB : public RGBController
{
public:
    RGBController_Lenovo_Gen7USB(LenovoGen7USBController* controller_ptr);
    ~RGBController_Lenovo_Gen7USB();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LenovoGen7USBController* controller;
    std::vector<led_group> GetLedGroups();
    void ReadDeviceSettings();
    std::unordered_map<unsigned int, size_t> led_id_to_index;
    int last_mode = 0;
    uint8_t brightness = 0x00;
    uint8_t profile_id = 0x01;
};

#endif // RGBCONTROLLER_LENOVO_GEN7USB_H
