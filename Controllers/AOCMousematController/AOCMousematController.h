/*-----------------------------------------*\
|  AOCMousematController.h                  |
|                                           |
|  Definitions and types for AOC mousemat   |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/15/2023   |
\*-----------------------------------------*/

#include "RGBController.h"
#include <hidapi/hidapi.h>
#include <string>

#pragma once

/*-----------------------------------------*\
| AOC Mousemat Modes                        |
|   Note: The 0x80 bit is the random flag   |
\*-----------------------------------------*/
enum
{
    AOC_MOUSEMAT_MODE_STATIC_SINGLE_COLOR       = 0x00,     /* Static single color mode     */
    AOC_MOUSEMAT_MODE_STATIC_RANDOM             = 0x80,     /* Static random color mode     */
    AOC_MOUSEMAT_MODE_BREATHING_SINGLE_COLOR    = 0x01,     /* Breathing single color mode  */
    AOC_MOUSEMAT_MODE_BREATHING_RANDOM          = 0x81,     /* Breathing random color mode  */
    AOC_MOUSEMAT_MODE_BLINK_SINGLE_COLOR        = 0x02,     /* Blink single color mode      */
    AOC_MOUSEMAT_MODE_BLINK_RANDOM              = 0x82,     /* Blink random color mode      */
    AOC_MOUSEMAT_MODE_WAVE_SINGLE_COLOR         = 0x03,     /* Wave single color mode       */
    AOC_MOUSEMAT_MODE_WAVE_RANDOM               = 0x83,     /* Wave random color mode       */
};

enum
{
    AOC_MOUSEMAT_SPEED_SLOW                     = 0x03,     /* Slowest speed                */
    AOC_MOUSEMAT_SPEED_MEDIUM                   = 0x02,     /* Medium speed                 */
    AOC_MOUSEMAT_SPEED_FAST                     = 0x01,     /* Fastest speed                */
};

enum
{
    AOC_MOUSEMAT_BRIGHTNESS_OFF                 = 0x00,     /* Lowest brightness (off)      */
    AOC_MOUSEMAT_BRIGHTNESS_LOW                 = 0x01,     /* Low brightness               */
    AOC_MOUSEMAT_BRIGHTNESS_MEDIUM              = 0x02,     /* Medium brightness            */
    AOC_MOUSEMAT_BRIGHTNESS_HIGH                = 0x03,     /* Highest brightness           */
};

enum
{
    AOC_MOUSEMAT_DIRECTION_CLOCKWISE            = 0x00,     /* Clockwise direction          */
    AOC_MOUSEMAT_DIRECTION_COUNTERCLOCKWISE     = 0x01,     /* Counter-clockwise direction  */
};

class AOCMousematController
{
public:
    AOCMousematController(hid_device* dev_handle, const char* path);
    ~AOCMousematController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
            (
            RGBColor*       color_data
            );

    void SendPacket
            (
            unsigned char   mode,
            unsigned char   brightness,
            unsigned char   speed,
            unsigned char   direction,
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;
};
