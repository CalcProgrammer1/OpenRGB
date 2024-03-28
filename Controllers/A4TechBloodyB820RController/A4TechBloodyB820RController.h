/*-----------------------------------------*\
|  A4TechBloodyB820RController.h            |
|                                           |
|  Definitions and types for AOC keyboard   |
|  lighting controller                      |
|                                           |
|  Zulfikar (o-julfikar) 3/28/2024          |
\*-----------------------------------------*/

#include "RGBController.h"
#include <hidapi/hidapi.h>
#include <string>

#pragma once

/*-----------------------------------------*\
| A4Tech Bloody B820R Keyboard Modes        |
\*-----------------------------------------*/
enum
{
    A4TechBloodyB820R_MODE_STATIC                    = 0x00,     /* Static mode                  */
    A4TechBloodyB820R_MODE_BREATHING                 = 0x01,     /* Breathing mode               */
    A4TechBloodyB820R_MODE_REACT                     = 0x02,     /* React mode                   */
    A4TechBloodyB820R_MODE_RIPPLE                    = 0x04,     /* Ripple mode                  */
    A4TechBloodyB820R_MODE_RADAR                     = 0x05,     /* Radar mode                   */
    A4TechBloodyB820R_MODE_FIREWORKS                 = 0x06,     /* Fireworks mode               */
    A4TechBloodyB820R_MODE_BLINK                     = 0x07,     /* Blink mode                   */
    A4TechBloodyB820R_MODE_WAVE                      = 0x08,     /* Wave mode                    */
    A4TechBloodyB820R_MODE_CUSTOM                    = 0x09,     /* Custom mode                  */
    A4TechBloodyB820R_MODE_CONCENTRIC_CIRCLES        = 0x0A,     /* Concentric Circles mode      */
    A4TechBloodyB820R_MODE_W_WAVE                    = 0x0B,     /* W Wave mode                  */
};

enum
{
    A4TechBloodyB820R_SPEED_SLOW                     = 0x03,     /* Slowest speed                */
    A4TechBloodyB820R_SPEED_MEDIUM                   = 0x02,     /* Medium speed                 */
    A4TechBloodyB820R_SPEED_FAST                     = 0x01,     /* Fastest speed                */
};

enum
{
    A4TechBloodyB820R_BRIGHTNESS_OFF                 = 0x00,     /* Lowest brightness (off)      */
    A4TechBloodyB820R_BRIGHTNESS_LOW                 = 0x01,     /* Low brightness               */
    A4TechBloodyB820R_BRIGHTNESS_MEDIUM              = 0x02,     /* Medium brightness            */
    A4TechBloodyB820R_BRIGHTNESS_HIGH                = 0x03,     /* Highest brightness           */
};

enum
{
    A4TechBloodyB820R_SINGLE_COLOR                   = 0x00,     /* Single color mode            */
    A4TechBloodyB820R_RANDOM                         = 0x01,     /* Random color mode            */
};

enum
{
    A4TechBloodyB820R_DIRECTION_CLOCKWISE            = 0x00,     /* Clockwise direction          */
    A4TechBloodyB820R_DIRECTION_COUNTERCLOCKWISE     = 0x01,     /* Counter-clockwise direction  */
};


class A4TechBloodyB820RController
{
public:
    A4TechBloodyB820RController(hid_device* dev_handle, const char* path);
    ~A4TechBloodyB820RController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SetLightingConfig
            (
            unsigned char   mode,
            unsigned char   random,
            unsigned char   brightness,
            unsigned char   speed,
            unsigned char   direction,
            RGBColor*       color_data
            );

    void SetCustom
            (
            RGBColor*       color_data
            );

private:
    hid_device*             dev;
    std::string             location;

    void SendStartPacket();
    void SendEndPacket();

    void SendCustomPacket
            (
            RGBColor*       color_data
            );

    void SendLightingConfigPacket
            (
            unsigned char   mode,
            unsigned char   random,
            unsigned char   brightness,
            unsigned char   speed,
            unsigned char   direction,
            RGBColor*       color_data
            );
};
