/*---------------------------------------------------------*\
| TForceXtreemController.h                                  |
|                                                           |
|   Driver for T-Force Xtreem DRAM                          |
|                                                           |
|   Milan Cermak (krysmanta)                    28 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "RGBController.h"
#include "i2c_smbus.h"

#define XTREEM_APPLY_VAL                0x01        /* Value for Apply Changes Register     */
#define XTREEM_LED_COUNT                15

typedef unsigned short	ene_register;
typedef unsigned char	ene_dev_id;

enum
{
    XTREEM_REG_DIRECT                   = 0xE020,   /* "Direct Access" Selection Register   */
    XTREEM_REG_MODE                     = 0xE021,   /* Mode Selection Register              */
    XTREEM_REG_SPEED                    = 0xE022,   /* Speed Control Register               */
    XTREEM_REG_DIRECTION                = 0xE023,   /* Direction Control Register           */
    XTREEM_REG_APPLY                    = 0xE02F,   /* Apply Changes Register               */
    XTREEM_REG_SLOT_INDEX               = 0xE0F8,   /* Slot Index Register (RAM only)       */
    XTREEM_REG_I2C_ADDRESS              = 0xE0F9,   /* I2C Address Register (RAM only)      */
    XTREEM_REG_COLORS_DIRECT            = 0xE100,   /* Colors for Direct Mode 45 bytes      */
    XTREEM_REG_COLORS_EFFECT            = 0xE300,   /* Colors for Internal Effects 45 bytes */
};

enum
{
    XTREEM_MODE_OFF                       = 0,      /* OFF mode                             */
    XTREEM_MODE_STATIC                    = 1,      /* Static color mode                    */
    XTREEM_MODE_BREATHING                 = 2,      /* Breathing effect mode                */
    XTREEM_MODE_FLASHING                  = 3,      /* Flashing effect mode                 */
    XTREEM_MODE_SPECTRUM_CYCLE            = 4,      /* Spectrum Cycle mode                  */
    XTREEM_MODE_RAINBOW                   = 5,      /* Rainbow effect mode                  */
    XTREEM_MODE_SPECTRUM_CYCLE_BREATHING  = 6,      /* Rainbow Breathing effect mode        */
    XTREEM_MODE_CHASE_FADE                = 7,      /* Chase with Fade effect mode          */
    XTREEM_MODE_SPECTRUM_CYCLE_CHASE_FADE = 8,      /* Chase with Fade, Rainbow effect mode */
    XTREEM_MODE_CHASE                     = 9,      /* Chase effect mode                    */
    XTREEM_MODE_SPECTRUM_CYCLE_CHASE      = 10,     /* Chase with Rainbow effect mode       */
    XTREEM_MODE_SPECTRUM_CYCLE_WAVE       = 11,     /* Wave effect mode                     */
    XTREEM_MODE_CHASE_RAINBOW_PULSE       = 12,     /* Chase with  Rainbow Pulse effect mode*/
    XTREEM_MODE_RANDOM_FLICKER            = 13,     /* Random flicker effect mode           */
    XTREEM_MODE_STACK                     = 14,     /* Stacking effect mode                 */
    XTREEM_MODE_PONG                      = 15,     /* Pong effect mode                     */
    XTREEM_MODE_FILLUP                    = 16,     /* Fill up effect mode                  */
    XTREEM_MODE_NEON                      = 17,     /* Neon effect mode                     */
    XTREEM_MODE_COLOR_WAVE                = 18,     /* Color Wave effect mode               */
    XTREEM_MODE_COLOR_DOUBLE_WAVE         = 19,     /* Color double wave effect mode        */
    XTREEM_MODE_MIXER                     = 20,     /* Mixer effect mode                    */
    XTREEM_MODE_SPECTRUM_CYCLE_2          = 21,     /* Spectrum cycle 2 effect mode         */
    XTREEM_MODE_FIRE_BREATHING            = 22,     /* Color shift breathing effect mode    */
    XTREEM_MODE_SPECTRUM_CYCLE_3          = 23,     /* Spectrum cycle 3 effect mode         */
    XTREEM_MODE_SLITHER                   = 24,     /* Slither effect mode                  */
    XTREEM_MODE_TFORCE_XTREEM             = 25,     /* Default T-Force Xtreem mode          */
    XTREEM_NUMBER_MODES                             /* Number of Aura modes                 */
};

enum
{
    XTREEM_SPEED_SLOWEST                = 0x04,     /* Slowest effect speed                 */
    XTREEM_SPEED_SLOW                   = 0x03,     /* Slow effect speed                    */
    XTREEM_SPEED_NORMAL                 = 0x02,     /* Normal effect speed                  */
    XTREEM_SPEED_FAST                   = 0x01,     /* Fast effect speed                    */
    XTREEM_SPEED_FASTEST                = 0x00,     /* Fastest effect speed                 */
};

enum
{
    XTREEM_DIRECTION_FORWARD            = 0x0,      /* Forward effect direction             */
    XTREEM_DIRECTION_REVERSE            = 0x1,      /* Reverse effect direction             */
};

class TForceXtreemController
{
public:
    TForceXtreemController(i2c_smbus_interface *bus, ene_dev_id dev);
    ~TForceXtreemController();

    std::string   GetDeviceLocation();
    unsigned int  GetLEDCount();
    unsigned char GetLEDRed(unsigned int led);
    unsigned char GetLEDGreen(unsigned int led);
    unsigned char GetLEDBlue(unsigned int led);
    unsigned char GetLEDRedEffect(unsigned int led);
    unsigned char GetLEDGreenEffect(unsigned int led);
    unsigned char GetLEDBlueEffect(unsigned int led);
    void          SetAllColorsDirect(RGBColor* colors);
    void          SetAllColorsEffect(RGBColor* colors);
    void          SetDirect(unsigned char direct);
    void          SetLEDColorDirect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void          SetLEDColorEffect(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void          SetMode(unsigned char mode, unsigned char speed, unsigned char direction);

    unsigned char ENERegisterRead(ene_register reg);
    void          ENERegisterWrite(ene_register reg, unsigned char val);
    void          ENERegisterWriteBlock(ene_register reg, unsigned char * data, unsigned char sz);

private:
    i2c_smbus_interface *   bus;
    ene_dev_id              dev;
};
