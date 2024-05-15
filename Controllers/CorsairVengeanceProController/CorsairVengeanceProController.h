/*---------------------------------------------------------*\
| CorsairVengeanceProController.h                           |
|                                                           |
|   Driver for Corsair Vengeance Pro RGB RAM                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "i2c_smbus.h"
#include "CRC.h"

typedef unsigned char	corsair_dev_id;

#define CORSAIR_PRO_LED_COUNT   ( 10 )
#define CORSAIR_VENGEANCE_RGB_PRO_NAME "Corsair Vengeance RGB PRO"

enum
{
    CORSAIR_PRO_REG_COMMAND             = 0x20,     /* Command write register               */
    CORSAIR_PRO_DIRECT_COMMAND          = 0x31,     /* Where it writes direct mode colors   */
};

enum
{
    CORSAIR_PRO_MODE_DIRECT             = 0xDD,     /* Arbitrary value to compare against later. Not the actual packet  */
    CORSAIR_PRO_MODE_COLOR_SHIFT        = 0x00,     /* Color Shift mode                                                 */
    CORSAIR_PRO_MODE_COLOR_PULSE        = 0x01,     /* Color Pulse mode                                                 */
    CORSAIR_PRO_MODE_RAINBOW_WAVE       = 0x03,     /* Rainbow Wave mode                                                */
    CORSAIR_PRO_MODE_COLOR_WAVE         = 0x04,     /* Color Wave mode                                                  */
    CORSAIR_PRO_MODE_VISOR              = 0x05,     /* Visor mode                                                       */
    CORSAIR_PRO_MODE_RAIN               = 0x06,     /* Rain mode                                                        */
    CORSAIR_PRO_MODE_MARQUEE            = 0x07,     /* Marquee mode                                                     */
    CORSAIR_PRO_MODE_RAINBOW            = 0x08,     /* Rainbow mode                                                     */
    CORSAIR_PRO_MODE_SEQUENTIAL         = 0x09,     /* Sequential mode                                                  */
    CORSAIR_PRO_MODE_STATIC             = 0x10,     /* Static mode                                                      */

    CORSAIR_PRO_NUMBER_MODES            = 10,       /* Number of Corsair Pro modes          */
};

enum
{
    CORSAIR_PRO_SPEED_SLOW              = 0x00,     /* Slow speed                           */
    CORSAIR_PRO_SPEED_MEDIUM            = 0x01,     /* Medium speed                         */
    CORSAIR_PRO_SPEED_FAST              = 0x02,     /* Fast speed                           */
};

enum
{
    CORSAIR_PRO_EFFECT_RANDOM_COLORS    = 0x00,     /* Random colors                        */
    CORSAIR_PRO_EFFECT_CUSTOM_COLORS    = 0x01,     /* Custom colors                        */
};

enum
{
    CORSAIR_PRO_DIRECTION_UP            = 0x00,     /* Up direction                         */
    CORSAIR_PRO_DIRECTION_DOWN          = 0x01,     /* Down direction                       */
    CORSAIR_PRO_DIRECTION_LEFT          = 0x02,     /* Left direction                       */
    CORSAIR_PRO_DIRECTION_RIGHT         = 0x03,     /* Right direction                      */
    CORSAIR_PRO_DIRECTION_VERTICAL      = 0x01,     /* Vertical direction                   */
    CORSAIR_PRO_DIRECTION_HORIZONTAL    = 0x03,     /* Horizontal direction                 */
};

class CorsairVengeanceProController
{
public:
    CorsairVengeanceProController(i2c_smbus_interface* bus, corsair_dev_id dev);
    ~CorsairVengeanceProController();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    unsigned int    GetLEDCount();
    unsigned char   GetEffect();
    void            SetEffect(unsigned char mode,
                              unsigned char speed,
                              unsigned char direction,
                              bool          random,
                              unsigned char red1,
                              unsigned char grn1,
                              unsigned char blu1,
                              unsigned char red2,
                              unsigned char grn2,
                              unsigned char blu2
                             );

    void            SetAllColors(unsigned char red, unsigned char green, unsigned char blue);
    void            SetLEDColor(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void            ApplyColors();
    bool            WaitReady();

    void            SetDirect(bool direct);

private:
    char                    device_name[32];
    unsigned int            led_count;
    bool                    direct_mode;
    unsigned char           effect_mode;
    unsigned char           led_red[CORSAIR_PRO_LED_COUNT];
    unsigned char           led_green[CORSAIR_PRO_LED_COUNT];
    unsigned char           led_blue[CORSAIR_PRO_LED_COUNT];

    i2c_smbus_interface*    bus;
    corsair_dev_id          dev;
};
