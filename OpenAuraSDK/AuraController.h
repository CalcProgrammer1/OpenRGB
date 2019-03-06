/*-----------------------------------------*\
|  AuraController.h                         |
|                                           |
|  Definitions and types for ASUS Aura RGB  |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2018   |
\*-----------------------------------------*/

#include "i2c_smbus.h"

#pragma once

typedef unsigned char	aura_dev_id;
typedef unsigned short	aura_register;

#define AURA_APPLY_VAL  0x01                /* Value for Apply Changes Register     */

enum
{
    AURA_REG_DEVICE_NAME                = 0x1000,   /* Device String 16 bytes               */
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
};

class AuraController
{
public:
    AuraController(i2c_smbus_interface* bus, aura_dev_id dev);
    ~AuraController();

    char*         GetDeviceName();
    unsigned int  GetLEDCount();
    void          SetAllColorsDirect(unsigned char red, unsigned char green, unsigned char blue);
    void          SetAllColorsEffect(unsigned char red, unsigned char green, unsigned char blue);
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
    unsigned int            led_count;
    aura_register           direct_reg;
    aura_register           effect_reg;
    i2c_smbus_interface *   bus;
    aura_dev_id             dev;
	

};