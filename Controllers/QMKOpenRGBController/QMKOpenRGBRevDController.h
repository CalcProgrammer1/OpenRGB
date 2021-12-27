/*-------------------------------------------------------------------*\
|  QMKOpenRGBRevDController.h                                         |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol (Revision D)       |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#pragma once

#include "QMKOpenRGBController.h"

class QMKOpenRGBRevDController
{
public:
    QMKOpenRGBRevDController(hid_device *dev_handle, const char *path);
    ~QMKOpenRGBRevDController();

    std::string     GetLocation();
    std::string     GetDeviceName();
    std::string     GetDeviceVendor();

    unsigned int    GetTotalNumberOfLEDs();
    unsigned int    GetTotalNumberOfLEDsWithEmptySpace();
    unsigned int    GetMode();
    unsigned int    GetModeSpeed();
    unsigned int    GetModeColor();

    std::vector<point_t>        GetLEDPoints();
    std::vector<unsigned int>   GetLEDFlags();
    std::vector<std::string>    GetLEDNames();
    std::vector<RGBColor>       GetLEDColors();
    std::vector<unsigned int>   GetLEDValues();

    unsigned int                GetProtocolVersion();
    std::string                 GetQMKVersion();
    void                        GetDeviceInfo();
    void                        GetModeInfo();
    void                        GetLEDInfo(unsigned int leds_count);
    std::vector<unsigned int>   GetEnabledModes();

    void            SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed, bool save);
    void            DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int num_colors);

protected:
    hid_device *dev;

private:
    unsigned int    leds_per_update;

    std::string     location;

    std::string     device_name;
    std::string     device_vendor;

    std::chrono::milliseconds   delay;

    unsigned int    total_number_of_leds;
    unsigned int    total_number_of_leds_with_empty_space;
    unsigned int    mode;
    unsigned int    mode_speed;

    RGBColor        mode_color;

    std::vector<point_t>        led_points;
    std::vector<unsigned int>   led_flags;
    std::vector<std::string>    led_names;
    std::vector<RGBColor>       led_colors;
    std::vector<unsigned int>   led_values;
};
