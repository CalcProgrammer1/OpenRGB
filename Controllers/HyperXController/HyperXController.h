/*-----------------------------------------*\
|  HyperXController.h                       |
|                                           |
|  Definitions and types for HyperX Predator|
|  RGB RAM lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/29/2019   |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char	hyperx_dev_id;
typedef unsigned short	hyperx_register;

enum
{
    HYPERX_REG_BRIGHTNESS               = 0xDD,     /* Brightness control register (0-100)  */
    HYPERX_REG_APPLY                    = 0xE1,     /* Apply changes register               */
    HYPERX_REG_MODE1                    = 0xE3,     /* Mode control register 1              */
    HYPERX_REG_MODE2                    = 0xE4,     /* Mode control register 2              */
    HYPERX_REG_RED                      = 0xEC,     /* Red color register                   */
    HYPERX_REG_GREEN                    = 0xED,     /* Green color register                 */
    HYPERX_REG_BLUE                     = 0xEE,     /* Blue color register                  */
};

enum
{
    HYPERX_MODE1_RAINBOW                = 0x05,     /* Mode 1 rainbow effect                */
    HYPERX_MODE1_CYCLE                  = 0x04,     /* Mode 1 cycle effect                  */
};

enum
{
    HYPERX_MODE2_BOUNCE                 = 0x02,     /* Mode 2 bounce effect                 */
    HYPERX_MODE2_BREATHING              = 0x03,     /* Mode 2 breathing effect              */
    HYPERX_MODE2_BLINK                  = 0x06,     /* Mode 2 blink effect                  */
    HYPERX_MODE2_HEARTBEAT              = 0x07,     /* Mode 2 heartbeat effect              */
    HYPERX_MODE2_COMET                  = 0x08,     /* Mode 2 comet effect                  */
    HYPERX_MODE2_STATIC                 = 0x09,     /* Mode 2 static effect                 */
};

enum
{
    HYPERX_MODE_STATIC                  = 0,        /* Static color mode                    */
    HYPERX_MODE_RAINBOW                 = 1,        /* Rainbow wave mode                    */
    HYPERX_MODE_COMET                   = 2,        /* Comet (chase) mode                   */
    HYPERX_MODE_HEARTBEAT               = 3,        /* Heartbeat (pulsing) mode             */
    HYPERX_MODE_CYCLE                   = 4,        /* Spectrum cycle mode                  */
    HYPERX_MODE_BREATHING               = 5,        /* Breathing mode                       */
    HYPERX_MODE_BOUNCE                  = 6,        /* Bounce mode                          */
    HYPERX_MODE_BLINK                   = 7,        /* Blinking mode                        */
    HYPERX_NUMBER_MODES                             /* Number of HyperX modes               */
};


class HyperXController
{
public:
    HyperXController(i2c_smbus_interface* bus, hyperx_dev_id dev);
    ~HyperXController();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    void            SetMode(unsigned char mode);

    void            SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);

private:
    char                    device_name[32];
    unsigned int            led_count;
    i2c_smbus_interface*    bus;
    hyperx_dev_id           dev;
};
