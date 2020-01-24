/*-----------------------------------------*\
|  AMDWraithPrismController.h               |
|                                           |
|  Definitions and types for AMD Wraith     |
|  Prism lighting controller                |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/6/2019   |
\*-----------------------------------------*/

#include <string>
#include <libusb-1.0/libusb.h>

#pragma once

enum
{
    AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC       = 0x01, /* Fan/Logo Static Mode         */
    AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE  = 0x02, /* Fan/Logo Color Cycle Mode    */
    AMD_WRAITH_PRISM_FAN_LOGO_MODE_BREATHING    = 0x03, /* Fan/Logo Breathing Mode      */
};

enum
{
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_STATIC      = 0x00, /* Static effect channel        */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_BREATHING   = 0x01, /* Breathing effect channel     */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_COLOR_CYCLE = 0x02, /* Color cycle effect channel   */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_LOGO_LED    = 0x05, /* Logo LED effect channel      */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_FAN_LED     = 0x06, /* Fan LED effect channel       */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_RAINBOW     = 0x07, /* Rainbow effect channel       */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_BOUNCE      = 0x08, /* Bounce effect channel        */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_CHASE       = 0x09, /* Chase effect channel         */
    AMD_WRAITH_PRISM_EFFECT_CHANNEL_SWIRL       = 0x0A, /* Swirl effect channel         */
};

class AMDWraithPrismController
{
public:
    AMDWraithPrismController(libusb_device_handle* dev_handle);
    ~AMDWraithPrismController();

    char* GetDeviceName();

    std::string GetEffectChannelString(unsigned char channel);
    std::string GetFirmwareVersionString();

    void SetRingEffectChannel(unsigned char channel);

    void SetFanMode(unsigned char mode, unsigned char speed);
    void SetFanColor(unsigned char red, unsigned char green, unsigned char blue);

    void SetLogoMode(unsigned char mode, unsigned char speed);
    void SetLogoColor(unsigned char red, unsigned char green, unsigned char blue);

    void SetRingMode(unsigned char mode, unsigned char speed, bool direction);
    void SetRingColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    char                    device_name[32];
    libusb_device_handle*   dev;

    unsigned char           current_fan_mode;
    unsigned char           current_fan_speed;

    unsigned char           current_logo_mode;
    unsigned char           current_logo_speed;

    unsigned char           current_ring_mode;
    unsigned char           current_ring_speed;
    bool                    current_ring_direction;

    void SendEnableCommand();

    void SendApplyCommand();

    void SendEffectChannelUpdate
        (
        unsigned char effect_channel,
        unsigned char speed,
        unsigned char direction,
        unsigned char mode,
        unsigned char brightness,
        unsigned char red,
        unsigned char green,
        unsigned char blue
        );

    void SendChannelRemap(unsigned char ring_channel, unsigned char logo_channel, unsigned char fan_channel);
};
