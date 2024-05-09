/*---------------------------------------------------------*\
| ASRockPolychromeV1SMBusController.h                       |
|                                                           |
|   Driver for SMBus ASRock Polychrome V1 motherboards      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

typedef uint8_t	polychrome_dev_id;

#define ASROCK_V1_CONTROLLER_NAME  "ASRock Polychrome v1 SMBus Controller"

enum
{
    /*------------------------------------------------------------------------------------------*\
    | ASRock Polychrome v1 Registers                                                             |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_REG_FIRMWARE_VER          = 0x00,     /* Firmware version Major.Minor         */
    POLYCHROME_V1_REG_MODE                  = 0x30,     /* Mode selection register              */
    POLYCHROME_V1_REG_ZONE_SELECT           = 0x31,     /* Zone selection register              */
    POLYCHROME_V1_REG_SET_ALL               = 0x32,     /* Set All register 0x1 = set all       */
    POLYCHROME_V1_REG_ZONE_SIZE             = 0x33,     /* Zone size configuration register     */
    POLYCHROME_V1_REG_ARGB_GRB              = 0x35,     /* ARGB bitstream reversing register    */
};

enum
{
    POLYCHROME_V1_ZONE_1                    = 0x00,     /* RGB LED 1 Header                     */
    POLYCHROME_V1_ZONE_2                    = 0x01,     /* RGB LED 2 Header                     */
    POLYCHROME_V1_ZONE_3                    = 0x02,     /* PCH Zone                             */
    POLYCHROME_V1_ZONE_4                    = 0x03,     /* IO Cover Zone                        */
    POLYCHROME_V1_ZONE_5                    = 0x04,     /* Audio Zone LEDs                      */
    POLYCHROME_V1_ZONE_ADDRESSABLE          = 0x05,     /* Addressable LED header               */
    POLYCHROME_V1_ZONE_COUNT                = 0x06,     /* Total number of zones                */
    POLYCHROME_V1_ZONE_ADDRESSABLE_MAX      = 0x64,     /* Maximum number of ARGB LEDs          */
};

/*----------------------------------------------------------------------------------------------*\
| Definitions for Polychrome V1                                                                  |
\*----------------------------------------------------------------------------------------------*/
#define POLYCHROME_V1_NUM_MODES             16          /* Number of Polychrome V1 modes        */

enum
{
    POLYCHROME_V1_MODE_OFF                  = 0x10,     /* OFF mode                             */
    POLYCHROME_V1_MODE_STATIC               = 0x11,     /* Static color mode                    */
    POLYCHROME_V1_MODE_BREATHING            = 0x12,     /* Breathing effect mode                */
    POLYCHROME_V1_MODE_STROBE               = 0x13,     /* Strobe effect mode                   */
    POLYCHROME_V1_MODE_SPECTRUM_CYCLE       = 0x14,     /* Spectrum Cycle effect mode           */
    POLYCHROME_V1_MODE_RANDOM               = 0x15,     /* Random effect mode                   */
    POLYCHROME_V1_MODE_MUSIC                = 0x17,     /* Music effect mode                    */
    POLYCHROME_V1_MODE_WAVE                 = 0x18,     /* Wave effect mode                     */
    /*------------------------------------------------------------------------------------------*\
    | Modes only available on ARGB headers                                                       |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_MODE_SPRING               = 0x19,     /* Spring effect mode                   */
    POLYCHROME_V1_MODE_METEOR               = 0x1A,     /* Meteor effect mode                   */
    POLYCHROME_V1_MODE_STACK                = 0x1B,     /* Stack effect mode                    */
    POLYCHROME_V1_MODE_CRAM                 = 0x1C,     /* Cram effect mode                     */
    POLYCHROME_V1_MODE_SCAN                 = 0x1D,     /* Scan effect mode                     */
    POLYCHROME_V1_MODE_NEON                 = 0x1E,     /* Neon effect mode                     */
    POLYCHROME_V1_MODE_WATER                = 0x1F,     /* Water effect mode                    */
    POLYCHROME_V1_MODE_RAINBOW              = 0x20,     /* Rainbow chase effect mode            */
};

enum
{
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_BREATHING                                                               |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_BREATHING       = 0x0A,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_BREATHING   = 0x02,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_BREATHING       = 0x02,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_STROBE                                                                  |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_STROBE          = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_STROBE      = 0x14,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_STROBE          = 0x05,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_SPECTRUM_CYCLE                                                          |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_CYCLE           = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_CYCLE       = 0x14,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_CYCLE           = 0x0A,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_RANDOM                                                                  |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_RANDOM          = 0xA0,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_RANDOM      = 0x28,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_RANDOM          = 0x05,     /* Fastest speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_WAVE                                                                    |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_WAVE            = 0x06,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_WAVE        = 0x02,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_WAVE            = 0x01,     /* Fastest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_SPRING      = 0x04,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_METEOR      = 0x0A,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_STACK       = 0x04,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_CRAM        = 0x04,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_SCAN        = 0x0A,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_NEON        = 0x20,     /* Default speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_WATER       = 0x0A,     /* Default speed                        */
    /*------------------------------------------------------------------------------------------*\
    | POLYCHROME_V1_MODE_RAINBOW                                                                 |
    \*------------------------------------------------------------------------------------------*/
    POLYCHROME_V1_SPEED_MIN_RAINBOW         = 0x12,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_DEFAULT_RAINBOW     = 0x0A,     /* Default speed                        */
    POLYCHROME_V1_SPEED_MAX_RAINBOW         = 0x01,     /* Fastest speed                        */
    POLYCHROME_V1_SPEED_MIN_ARGB            = 0x20,     /* Slowest speed                        */
    POLYCHROME_V1_SPEED_MAX_ARGB            = 0x02,     /* Fastest speed                        */
};

struct zone_cfg
{
    uint8_t     mode;
    uint8_t     speed;
    RGBColor    color;
};

class ASRockPolychromeV1SMBusController
{
public:
    ASRockPolychromeV1SMBusController(i2c_smbus_interface* bus, polychrome_dev_id dev);
    ~ASRockPolychromeV1SMBusController();

    std::string             GetDeviceLocation();
    std::string             GetDeviceName();
    std::string             GetFirmwareVersion();

    uint8_t                 GetARGBColorOrder();
    RGBColor                GetZoneColor(uint8_t zone);
    uint8_t                 GetZoneMode(uint8_t zone);
    void                    LoadZoneConfig();
    void                    SetARGBColorOrder(bool value);
    bool                    SetARGBSize(uint8_t led_count);
    void                    SetColorsAndSpeed(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
    void                    SetMode(uint8_t zone, uint8_t mode, uint8_t speed);

    uint8_t                 zone_led_count[6];
    zone_cfg                zone_config[6];
    uint16_t                fw_version;

private:
    std::string             device_name;
    i2c_smbus_interface*    bus;
    polychrome_dev_id       dev;

    void                    ReadLEDConfiguration();
};
