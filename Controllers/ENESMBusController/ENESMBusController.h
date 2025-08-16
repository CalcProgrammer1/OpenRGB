/*---------------------------------------------------------*\
| ENESMBusController.h                                      |
|                                                           |
|   Driver for ENE SMBus devices                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Aug 2018 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "ENESMBusInterface.h"
#include "RGBController.h"

#define ENE_APPLY_VAL                   0x01        /* Value for Apply Changes Register     */
#define ENE_SAVE_VAL                    0xAA        /* Value for Save Changes               */
#define ENE_NUM_ZONES                   8           /* Number of ENE config table zones     */

enum
{
    ENE_REG_DEVICE_NAME                 = 0x1000,   /* Device String 16 bytes               */
    ENE_REG_MICRON_CHECK                = 0x1030,   /* If "Micron" appears here, skip       */
    ENE_REG_CONFIG_TABLE                = 0x1C00,   /* Start of LED configuration bytes     */
    ENE_REG_COLORS_DIRECT               = 0x8000,   /* Colors for Direct Mode 15 bytes      */
    ENE_REG_COLORS_EFFECT               = 0x8010,   /* Colors for Internal Effects 15 bytes */
    ENE_REG_DIRECT                      = 0x8020,   /* "Direct Access" Selection Register   */
    ENE_REG_MODE                        = 0x8021,   /* Mode Selection Register              */
    ENE_REG_SPEED                       = 0x8022,   /* Speed Control Register               */
    ENE_REG_DIRECTION                   = 0x8023,   /* Direction Control Register           */
    ENE_REG_APPLY                       = 0x80A0,   /* Apply Changes Register               */
    ENE_REG_SLOT_INDEX                  = 0x80F8,   /* Slot Index Register (RAM only)       */
    ENE_REG_I2C_ADDRESS                 = 0x80F9,   /* I2C Address Register (RAM only)      */
    ENE_REG_COLORS_DIRECT_V2            = 0x8100,   /* Direct Colors (v2) 30 bytes          */
    ENE_REG_COLORS_EFFECT_V2            = 0x8160,   /* Internal Colors (v2) 30 bytes        */
};

enum
{
    ENE_MODE_OFF                        = 0,        /* OFF mode                             */
    ENE_MODE_STATIC                     = 1,        /* Static color mode                    */
    ENE_MODE_BREATHING                  = 2,        /* Breathing effect mode                */
    ENE_MODE_FLASHING                   = 3,        /* Flashing effect mode                 */
    ENE_MODE_SPECTRUM_CYCLE             = 4,        /* Spectrum Cycle mode                  */
    ENE_MODE_RAINBOW                    = 5,        /* Rainbow effect mode                  */
    ENE_MODE_SPECTRUM_CYCLE_BREATHING   = 6,        /* Rainbow Breathing effect mode        */
    ENE_MODE_CHASE_FADE                 = 7,        /* Chase with Fade effect mode          */
    ENE_MODE_SPECTRUM_CYCLE_CHASE_FADE  = 8,        /* Chase with Fade, Rainbow effect mode */
    ENE_MODE_CHASE                      = 9,        /* Chase effect mode                    */
    ENE_MODE_SPECTRUM_CYCLE_CHASE       = 10,       /* Chase with Rainbow effect mode       */
    ENE_MODE_SPECTRUM_CYCLE_WAVE        = 11,       /* Wave effect mode                     */
    ENE_MODE_CHASE_RAINBOW_PULSE        = 12,       /* Chase with  Rainbow Pulse effect mode*/
    ENE_MODE_RANDOM_FLICKER             = 13,       /* Random flicker effect mode           */
    ENE_MODE_DOUBLE_FADE                = 14,       /* Rainbow fade to dual color           */
    ENE_NUMBER_MODES                                /* Number of Aura modes                 */
};

enum
{
    ENE_SPEED_SLOWEST                   = 0x04,     /* Slowest effect speed                 */
    ENE_SPEED_SLOW                      = 0x03,     /* Slow effect speed                    */
    ENE_SPEED_NORMAL                    = 0x02,     /* Normal effect speed                  */
    ENE_SPEED_FAST                      = 0x01,     /* Fast effect speed                    */
    ENE_SPEED_FASTEST                   = 0x00,     /* Fastest effect speed                 */
};

enum
{
    ENE_DIRECTION_FORWARD               = 0x0,      /* Forward effect direction             */
    ENE_DIRECTION_REVERSE               = 0x1,      /* Reverse effect direction             */
};

enum
{
    ENE_LED_CHANNEL_DRAM_2              = 0x05,     /* DRAM LED channel                     */
    ENE_LED_CHANNEL_DRAM_3              = 0x0E,     /* DRAM LED channel                     */
    ENE_LED_CHANNEL_CENTER_START        = 0x82,     /* Center zone first LED channel        */
    ENE_LED_CHANNEL_CENTER              = 0x83,     /* Center zone LED channel              */
    ENE_LED_CHANNEL_AUDIO               = 0x84,     /* Audio zone LED channel               */
    ENE_LED_CHANNEL_BACK_IO             = 0x85,     /* Back I/O zone LED channel            */
    ENE_LED_CHANNEL_RGB_HEADER          = 0x86,     /* RGB Header LED channel               */
    ENE_LED_CHANNEL_RGB_HEADER_2        = 0x87,     /* RGB Header 2 LED channel             */
    ENE_LED_CHANNEL_BACKPLATE           = 0x88,     /* Backplate zone LED channel           */
    ENE_LED_CHANNEL_DRAM                = 0x8A,     /* DRAM LED channel                     */
    ENE_LED_CHANNEL_PCIE                = 0x8B,     /* PCIe zone LED channel                */
    ENE_LED_CHANNEL_RGB_HEADER_3        = 0x91,     /* RGB Header 3 LED channel             */
};

enum
{
    ENE_CONFIG_LED_COUNT                = 0x02,     /* LED Count configuration offset       */
    ENE_CONFIG_LED_COUNT_0107           = 0x03,     /* LED Count configuration offset       */
    ENE_CONFIG_LED_COUNT_1110           = 0x03,     /* LED Count configuration offset       */
    ENE_CONFIG_CHANNEL_V1               = 0x13,     /* LED Channel configuration offset     */
    ENE_CONFIG_CHANNEL_V2               = 0x1B,     /* LED Channel V2 configuration offset  */
};

class ENESMBusController
{
public:
    ENESMBusController(ENESMBusInterface* interface, ene_dev_id dev, std::string dev_name, device_type dev_type);
    ~ENESMBusController();

    std::string   GetLocation();
    std::string   GetName();
    std::string   GetVersion();
    device_type   GetType();

    const char*   GetChannelName(unsigned int cfg_zone);
    unsigned int  GetLEDCount(unsigned int cfg_zone);
    unsigned char GetLEDRed(unsigned int led);
    unsigned char GetLEDGreen(unsigned int led);
    unsigned char GetLEDBlue(unsigned int led);
    unsigned char GetLEDRedEffect(unsigned int led);
    unsigned char GetLEDGreenEffect(unsigned int led);
    unsigned char GetLEDBlueEffect(unsigned int led);
    void          SaveMode();
    void          SetAllColorsDirect(RGBColor* colors);
    void          SetAllColorsEffect(RGBColor* colors);
    void          SetDirect(unsigned char direct);
    void          SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void          SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void          SetMode(unsigned char mode, unsigned char speed, unsigned char direction);
    bool          SupportsMode14();

    void          UpdateDeviceName();

    unsigned char ENERegisterRead(ene_register reg);
    void          ENERegisterWrite(ene_register reg, unsigned char val);
    void          ENERegisterWriteBlock(ene_register reg, unsigned char * data, unsigned char sz);

private:
    char                    device_version[16];
    unsigned char           config_table[64];
    unsigned int            led_count;
    ene_register            direct_reg;
    ene_register            effect_reg;
    unsigned char           channel_cfg;
    ENESMBusInterface*      interface;
    ene_dev_id              dev;
    bool                    supports_mode_14;
    std::string             name;
    device_type             type;
};
