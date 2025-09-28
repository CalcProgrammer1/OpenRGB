/*---------------------------------------------------------*\
| AlienwareController.h                                     |
|                                                           |
|   Driver for Dell Alienware RGB USB controller            |
|                                                           |
|   Gabriel Marcano (gemarcano)                 19 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*----------------------------------------------------------------------------------------------*\
| Definitions for Alienware Controller                                                           |
\*----------------------------------------------------------------------------------------------*/

#define ALIENWARE_REPORT_SIZE               33
#define HIDAPI_ALIENWARE_REPORT_SIZE        (ALIENWARE_REPORT_SIZE + 1)
#define ALIENWARE_CONTROLLER_NAME           "AlienWare Controller"
enum
{
    ALIENWARE_COMMAND_REPORT                = 0x20,     /* Set report type to get               */
    ALIENWARE_COMMAND_USER_ANIM             = 0x21,     /* Set user animation settings          */
    ALIENWARE_COMMAND_POWER_ANIM            = 0x22,     /* Set power animation settings         */
    ALIENWARE_COMMAND_SELECT_ZONES          = 0x23,     /* Select zones to apply actions to     */
    ALIENWARE_COMMAND_ADD_ACTION            = 0x24,     /* Set actions to apply                 */
    ALIENWARE_COMMAND_UNKNOWN1              = 0x25,     /* Supposedly set event?                */
    ALIENWARE_COMMAND_DIM                   = 0x26,     /* Set dim percentage                   */
    ALIENWARE_COMMAND_SET_COLOR             = 0x27,     /* Unclear (causes color flash)         */
    ALIENWARE_COMMAND_RESET                 = 0x28,     /* Reset                                */
    ALIENWARE_COMMAND_ERASE_FLASH           = 0xFF,     /* Erases flash memory on controller    */
};

enum
{
    ALIENWARE_COMMAND_REPORT_FIRMWARE       = 0x00,     /* Get firmware verion                  */
    ALIENWARE_COMMAND_REPORT_STATUS         = 0x01,     /* Get status                           */
    ALIENWARE_COMMAND_REPORT_CONFIG         = 0x02,     /* Get firmware config                  */
    ALIENWARE_COMMAND_REPORT_ANIMATION      = 0x03,     /* Get animation count and last id      */
    ALIENWARE_COMMAND_REPORT_UNKNOWN1       = 0x04,     /* Get ELC animation by ID              */
    ALIENWARE_COMMAND_REPORT_UNKNOWN2       = 0x05,     /* Read series???                       */
    ALIENWARE_COMMAND_REPORT_UNKNOWN3       = 0x06,     /* Get action???                        */
    ALIENWARE_COMMAND_REPORT_UNKNOWN4       = 0x07,     /* Get Caldera status???                */
};

enum
{
    ALIENWARE_COMMAND_USER_ANIM_NEW         = 0x0001,   /* Start new animation                  */
    ALIENWARE_COMMAND_USER_ANIM_FINISH_SAVE = 0x0002,   /* Finish and save animation            */
    ALIENWARE_COMMAND_USER_ANIM_FINISH_PLAY = 0x0003,   /* Finish and play animation            */
    ALIENWARE_COMMAND_USER_ANIM_REMOVE      = 0x0004,   /* Remove/erase animation               */
    ALIENWARE_COMMAND_USER_ANIM_PLAY        = 0x0005,   /* Play animation                       */
    ALIENWARE_COMMAND_USER_ANIM_DEFAULT     = 0x0006,   /* Set default animation                */
    ALIENWARE_COMMAND_USER_ANIM_STARTUP     = 0x0007,   /* Set startup animation                */
};

enum
{
    ALIENWARE_ANIM_DEFAULT_STARTUP          = 0x0008,   /* Default slot for startup             */
    ALIENWARE_ANIM_DEFAULT                  = 0x0061,   /* Default slot                         */
    ALIENWARE_COMMAND_USER_ANIM_KEYBOARD    = 0xFFFF,   /* Non-saved animation slot             */
};

enum
{
    ALIENWARE_MODE_COLOR                    = 0x00,     /* Action to set color mode             */
    ALIENWARE_MODE_PULSE                    = 0x01,     /* Action to set pulse mode             */
    ALIENWARE_MODE_MORPH                    = 0x02,     /* Action to set morph mode             */
    ALIENWARE_MODE_SPECTRUM,                            /* Abitrary code for spectrum mode      */
    ALIENWARE_MODE_RAINBOW,                             /* Arbitrary code for rainbow wave mode */
    ALIENWARE_MODE_BREATHING,                           /* Arbitrary code for rainbow wave mode */
};

enum
{
    ALIENWARE_TEMPO_MIN                     = 0x0064,   /* Min tempo (as used by AWCC)          */
    ALIENWARE_TEMPO_MAX                     = 0x00FA,   /* Max tempo (as used by AWCC)          */
    ALIENWARE_TEMPO_SPECTRUM                = 0x000F,   /* Used by Spectrum mode                */
};

enum
{
    ALIENWARE_DURATION_LONG                 = 0x09C4,   /* Min tempo (as used by AWCC)          */
    ALIENWARE_DURATION_MED                  = 0x05DC,   /* Max tempo (as used by AWCC)          */
    ALIENWARE_DURATION_SHORT                = 0x01F3,   /* Max tempo (as used by AWCC)          */
    ALIENWARE_DURATION_SPECTRUM             = 0x01AC,   /* Used by Spectrum mode                */
};

typedef struct
{
    unsigned char           data[ALIENWARE_REPORT_SIZE];
} AlienwareReport;

class AlienwareController
{
public:
    AlienwareController(hid_device* dev_handle, const hid_device_info& info, std::string name);
    ~AlienwareController();

    std::string                 GetSerialString();
    std::string                 GetDeviceLocation();
    std::string                 GetDeviceName();
    std::string                 GetFirmwareVersion();
    unsigned                    GetZoneCount();
    std::vector<const char*>    GetZoneNames();

    void                        SetColor(uint8_t zone, RGBColor color);
    void                        SetColor(uint8_t zone, RGBColor color1, RGBColor color2);
    void                        SetMode(uint8_t zone, uint8_t mode);
    void                        SetPeriod(uint8_t zone, uint16_t period);
    void                        SetTempo(uint8_t zone, uint16_t tempo);
    void                        SetDim(uint8_t zone, uint8_t dim);
    AlienwareReport             GetStatus(uint8_t subcommand);

    void                        UpdateDim();
    void                        UpdateMode();
    void                        UpdateController();

protected:
    hid_device* dev;

private:
    typedef struct
    {
        RGBColor                color[2];
        uint8_t                 mode;
        uint16_t                period;
        uint16_t                tempo;
        uint8_t                 dim;
    } alienware_zone;

    typedef struct
    {
        unsigned char           data[HIDAPI_ALIENWARE_REPORT_SIZE];
    } HidapiAlienwareReport;

    std::string                 device_name;
    std::string                 location;
    std::vector<alienware_zone> zones;
    std::string                 serial_number;
    std::string                 version;
    std::vector<const char*>    zone_names;
    bool                        dirty;
    bool                        dirty_dim;

    HidapiAlienwareReport       GetResponse();
    HidapiAlienwareReport       Report(uint8_t subcommand);

    bool                        Dim(std::vector<uint8_t> zones, double percent);
    bool                        UserAnimation(uint16_t subcommand, uint16_t animation, uint16_t duration);
    bool                        SelectZones(const std::vector<uint8_t>& zones);
    bool                        ModeAction(uint8_t mode, uint16_t duration, uint16_t tempo, RGBColor color);
    bool                        ModeAction(const uint8_t *mode, const uint16_t *duration, const uint16_t *tempo, const RGBColor *color, unsigned amount);
    bool                        MultiModeAction(const uint8_t *mode, const uint16_t *duration, const uint16_t *tempo, const RGBColor *color, unsigned amount);
    bool                        SetColorDirect(RGBColor color, std::vector<uint8_t> zones);
    bool                        UpdateDirect();
    bool                        Reset();
};
