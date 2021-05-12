/*-----------------------------------------*\
|  AsusAuraUSBController.h                  |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Martin Hartl (inlart) 4/25/2020          |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_MODE_OFF                       = 0,        /* OFF mode                             */
    AURA_MODE_STATIC                    = 1,        /* Static color mode                    */
    AURA_MODE_BREATHING                 = 2,        /* Breathing effect mode                */
    AURA_MODE_FLASHING                  = 3,        /* Flashing effect mode                 */
    AURA_MODE_SPECTRUM_CYCLE            = 4,        /* Spectrum Cycle mode                  */
    AURA_MODE_RAINBOW                   = 5,        /* Rainbow effect mode                  */
    AURA_MODE_SPECTRUM_CYCLE_BREATHING  = 6,        /* Rainbow Breathing effect mode        */
    AURA_MODE_CHASE_FADE                = 7,        /* Chase with Fade effect mode          */
    AURA_MODE_SPECTRUM_CYCLE_CHASE_FADE = 8,        /* Chase with Fade, Rainbow effect mode */
    AURA_MODE_CHASE                     = 9,        /* Chase effect mode                    */
    AURA_MODE_SPECTRUM_CYCLE_CHASE      = 10,       /* Chase with Rainbow effect mode       */
    AURA_MODE_SPECTRUM_CYCLE_WAVE       = 11,       /* Wave effect mode                     */
    AURA_MODE_CHASE_RAINBOW_PULSE       = 12,       /* Chase with  Rainbow Pulse effect mode*/
    AURA_MODE_RANDOM_FLICKER            = 13,       /* Random flicker effect mode           */
    AURA_MODE_MUSIC                     = 14,       /* Music effect mode                    */
    AURA_MODE_DIRECT                    = 0xFF,     /* Direct control mode                  */
};

enum
{
    AURA_REQUEST_FIRMWARE_VERSION       = 0x82,     /* Request firmware string              */
    AURA_REQUEST_CONFIG_TABLE           = 0xB0,     /* Request configuration table          */
    AURA_CONTROL_MODE_DIRECT            = 0x40,     /* Direct control mode                  */
};

enum class AuraDeviceType
{
    FIXED,
    ADDRESSABLE,
};

struct AuraDeviceInfo
{
    unsigned char effect_channel;
    unsigned char direct_channel;
    unsigned char num_leds;
    unsigned char num_headers;
    AuraDeviceType device_type;
};

class AuraUSBController
{
public:
    AuraUSBController(hid_device* dev_handle, const char* path);
    virtual ~AuraUSBController();

    unsigned int GetChannelCount();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();

    const std::vector<AuraDeviceInfo>& GetAuraDevices() const;

    virtual void SetChannelLEDs
        (
        unsigned char   channel,
        RGBColor *      colors,
        unsigned int    num_colors
        ) = 0;

    virtual void SetMode
        (
        unsigned char   channel,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu
        ) = 0;

protected:
    hid_device*                 dev;
    unsigned char               config_table[60];
    std::vector<AuraDeviceInfo> device_info;
    std::string                 location;

    void SendDirect
        (
        unsigned char   device,
        unsigned char   start_led,
        unsigned char   led_count,
        unsigned char*  led_data,
        bool apply = false
        );
private:
    char                        device_name[16];
    unsigned int                led_count;

    void GetConfigTable();

    void GetFirmwareVersion();
};
