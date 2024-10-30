/*---------------------------------------------------------*\
| HPOmen30LController.h                                     |
|                                                           |
|   Driver for HP Omen 30L                                  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

typedef struct
{
    unsigned char  value;
    unsigned char  mode;
    unsigned char  speed;
    unsigned char  brightness;
} hp_zone;

enum
{
    HP_OMEN_30L_STATIC      = 0x01, /* Static effect channel        */
    HP_OMEN_30L_DIRECT      = 0x04, /* Direct for effects plugin    */
    HP_OMEN_30L_OFF         = 0x05, /* Turns off the led            */
    HP_OMEN_30L_BREATHING   = 0x06, /* Breathing effect channel     */
    HP_OMEN_30L_COLOR_CYCLE = 0x07, /* Color cycle effect channel   */
    HP_OMEN_30L_BLINKING    = 0x08, /* Blinking effect channel      */
    HP_OMEN_30L_WAVE        = 0x09, /* Wave effect channel          */
    HP_OMEN_30L_RADIAL      = 0x0A, /* Radial effect channel        */
};

enum
{
    HP_OMEN_30L_SPEED_SLOW  = 0x01, /* Slow speed                   */
    HP_OMEN_30L_SPEED_MED   = 0x02, /* Normal speed                 */
    HP_OMEN_30L_SPEED_FAST  = 0x03, /* Fast speed                   */
};

enum
{
    HP_OMEN_30L_LOGO_ZONE       = 0x01,
    HP_OMEN_30L_BAR_ZONE        = 0x02,
    HP_OMEN_30L_FAN_ZONE        = 0x03,
    HP_OMEN_30L_CPU_ZONE        = 0x04,
    HP_OMEN_30L_BOT_FAN_ZONE    = 0x05,
    HP_OMEN_30L_MID_FAN_ZONE    = 0x06,
    HP_OMEN_30L_TOP_FAN_ZONE    = 0x07,
};

enum
{
    HP_OMEN_30L_POWER_ON        = 0x01, /* Settings for powered on   */
    HP_OMEN_30L_POWER_SUSPEND   = 0x02, /* Settings for suspended    */
};

enum
{
    HP_OMEN_30L_THEME_CUSTOM    = 0x00,
    HP_OMEN_30L_THEME_GALAXY    = 0x01,
    HP_OMEN_30L_THEME_VOLCANO   = 0x02,
    HP_OMEN_30L_THEME_JUNGLE    = 0x03,
    HP_OMEN_30L_THEME_OCEAN     = 0x04,
    HP_OMEN_30L_THEME_UNICORN   = 0x05,
};

class HPOmen30LController
{
public:
    HPOmen30LController(hid_device* dev_handle, const char* path);
    ~HPOmen30LController();

    char* GetDeviceName();

    std::string GetEffectChannelString(unsigned char channel);
    std::string GetFirmwareVersionString();
    std::string GetLocationString();
    std::string GetSerialString();

    void SetZoneMode(int zone,unsigned char mode, unsigned char speed, unsigned char brightness);
    void SetZoneColor(int zone, std::vector<RGBColor> colors);

private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;

    std::vector<hp_zone>    hp_zones;

    void SendZoneUpdate(int zone, std::vector<RGBColor> colors);
};
