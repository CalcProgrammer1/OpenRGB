/*-----------------------------------------*\
|  AsusAuraSMBusController.h                |
|                                           |
|  Definitions and types for ASUS Aura RGB  |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	aura_dev_id;
typedef unsigned short	aura_register;

#define AURA_APPLY_VAL  0x01                /* Value for Apply Changes Register     */

enum
{
    AURA_REG_DEVICE_NAME                = 0x1000,   /* Device String 16 bytes               */
    AURA_REG_CONFIG_TABLE               = 0x1C00,   /* Start of LED configuration bytes     */
    AURA_REG_COLORS_DIRECT              = 0x8000,   /* Colors for Direct Mode 15 bytes      */
    AURA_REG_COLORS_EFFECT              = 0x8010,   /* Colors for Internal Effects 15 bytes */
    AURA_REG_DIRECT                     = 0x8020,   /* "Direct Access" Selection Register   */
    AURA_REG_MODE                       = 0x8021,   /* AURA Mode Selection Register         */
    AURA_REG_APPLY                      = 0x80A0,   /* AURA Apply Changes Register          */
    AURA_REG_SLOT_INDEX                 = 0x80F8,   /* AURA Slot Index Register (RAM only)  */
    AURA_REG_I2C_ADDRESS                = 0x80F9,   /* AURA I2C Address Register (RAM only) */
    AURA_REG_COLORS_DIRECT_V2           = 0x8100,   /* Direct Colors (v2) 30 bytes          */
    AURA_REG_COLORS_EFFECT_V2           = 0x8160,   /* Internal Colors (v2) 30 bytes        */
};

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
    AURA_NUMBER_MODES                               /* Number of Aura modes                 */
};

enum
{
    AURA_LED_CHANNEL_DRAM_2             = 0x05,     /* DRAM LED channel                     */
    AURA_LED_CHANNEL_CENTER_START       = 0x82,     /* Center zone first LED channel        */
    AURA_LED_CHANNEL_CENTER             = 0x83,     /* Center zone LED channel              */
    AURA_LED_CHANNEL_AUDIO              = 0x84,     /* Audio zone LED channel               */
    AURA_LED_CHANNEL_BACK_IO            = 0x85,     /* Back I/O zone LED channel            */
    AURA_LED_CHANNEL_RGB_HEADER         = 0x86,     /* RGB Header LED channel               */
    AURA_LED_CHANNEL_RGB_HEADER_2       = 0x87,     /* RGB Header 2 LED channel             */
    AURA_LED_CHANNEL_BACKPLATE          = 0x88,     /* Backplate zone LED channel           */
    AURA_LED_CHANNEL_DRAM               = 0x8A,     /* DRAM LED channel                     */
    AURA_LED_CHANNEL_PCIE               = 0x8B,     /* PCIe zone LED channel                */
    AURA_LED_CHANNEL_RGB_HEADER_3       = 0x91,     /* RGB Header 3 LED channel             */
};

enum
{
    AURA_CONFIG_LED_COUNT               = 0x02,     /* LED Count configuration offset       */
    AURA_CONFIG_CHANNEL_V1              = 0x13,     /* LED Channel configuration offset     */
    AURA_CONFIG_CHANNEL_V2              = 0x1B,     /* LED Channel V2 configuration offset  */
};

class AuraSMBusController
{
public:
    AuraSMBusController(i2c_smbus_interface* bus, aura_dev_id dev);
    ~AuraSMBusController();

    std::string   GetDeviceName();
    std::string   GetDeviceLocation();
    unsigned char GetChannel(unsigned int led);
    const char*   GetChannelName(unsigned int led);
    unsigned int  GetLEDCount();
    unsigned char GetLEDRed(unsigned int led);
    unsigned char GetLEDGreen(unsigned int led);
    unsigned char GetLEDBlue(unsigned int led);
    void          SetAllColorsDirect(RGBColor* colors);
    void          SetAllColorsEffect(RGBColor* colors);
    void          SetDirect(unsigned char direct);
    void          SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void          SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void          SetMode(unsigned char mode);

    void          AuraUpdateDeviceName();

    unsigned char AuraRegisterRead(aura_register reg);
    void          AuraRegisterWrite(aura_register reg, unsigned char val);
    void          AuraRegisterWriteBlock(aura_register reg, unsigned char * data, unsigned char sz);

private:
    char                    device_name[16];
    unsigned char           config_table[64];
    unsigned int            led_count;
    aura_register           direct_reg;
    aura_register           effect_reg;
    unsigned char           channel_cfg;
    i2c_smbus_interface *   bus;
    aura_dev_id             dev;

};
