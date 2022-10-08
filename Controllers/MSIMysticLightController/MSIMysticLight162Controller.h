/*-----------------------------------------*\
|  MSIMysticLight162Controller.h            |
|                                           |
|  Definitions and types for MSI Mystic     |
|  Light (162-byte) USB lighting controllers|
|                                           |
|  T-bond 3/4/2020                          |
|  Adam Honse 3/6/2021                      |
\*-----------------------------------------*/

#include "MSIMysticLightCommon.h"
#include "RGBController.h"
#include <cstring>
#include <hidapi/hidapi.h>
#include <limits>

#pragma once

class MSIMysticLight162Controller
{
public:
    MSIMysticLight162Controller
        (
        hid_device*     handle,
        const char      *path,
        unsigned short  pid
        );
    ~MSIMysticLight162Controller();

    void            SetMode
                        (
                        MSI_ZONE        zone,
                        MSI_MODE        mode,
                        MSI_SPEED       speed,
                        MSI_BRIGHTNESS  brightness,
                        bool            rainbow_color
                        );

    MSI_MODE        GetMode();

    void            GetMode
                        (
                        MSI_ZONE        zone,
                        MSI_MODE        &mode,
                        MSI_SPEED       &speed,
                        MSI_BRIGHTNESS  &brightness,
                        bool            &rainbow_color,
                        unsigned int    &color
                        );

    void            SetZoneColor
                        (
                        MSI_ZONE        zone,
                        unsigned char   red1,
                        unsigned char   grn1,
                        unsigned char   blu1,
                        unsigned char   red2,
                        unsigned char   grn2,
                        unsigned char   blu2
                        );

    void            SetLedColor
                        (
                        MSI_ZONE        zone,
                        unsigned char   red,
                        unsigned char   grn,
                        unsigned char   blu
                        );

    bool            Update
                        (
                        bool save
                        );

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();


    void            SetDirectMode
                        (
                        bool mode
                        );
    bool            IsDirectModeActive() { return direct_mode; }
    size_t          GetMaxOnboardLeds();
    const std::vector<MSI_ZONE>*
                    GetSupportedZones() { return supported_zones; }

private:
    bool            ReadSettings();
    bool            ReadFwVersion();
    void            ReadSerial();
    void            ReadName();
    ZoneData*       GetZoneData
                        (
                        FeaturePacket_162&  dataPacket,
                        MSI_ZONE            zone
                        );

    hid_device*                     dev;
    std::string                     name;
    std::string                     location;
    std::string                     version_APROM;
    std::string                     version_LDROM;
    std::string                     chip_id;

    FeaturePacket_162               data;
    FeaturePacket_162               zone_based_per_led_data;
    bool                            direct_mode;
    size_t                          numof_onboard_leds;
    const std::vector<MSI_ZONE>*    supported_zones;
};
