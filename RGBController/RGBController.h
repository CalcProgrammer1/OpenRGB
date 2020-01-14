/*-----------------------------------------*\
|  RGBController.h                          |
|                                           |
|  Definitions and types for generic RGB    |
|  lighting controller interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/2/2019    |
\*-----------------------------------------*/

#pragma once

#include <vector>
#include <string>

typedef unsigned int RGBColor;

#define RGBGetRValue(rgb)   (rgb & 0x000000FF)
#define RGBGetGValue(rgb)   ((rgb >> 8) & 0x000000FF)
#define RGBGetBValue(rgb)   ((rgb >> 16) & 0x000000FF)

#define ToRGBColor(r, g, b) ((b << 16) | (g << 8) | (r))

/*------------------------------------------------------------------*\
| Mode Flags                                                         |
\*------------------------------------------------------------------*/
enum
{
    MODE_FLAG_HAS_SPEED         = (1 << 0), /* Mode has speed parameter         */
    MODE_FLAG_HAS_DIRECTION_LR  = (1 << 1), /* Mode has left/right parameter    */
    MODE_FLAG_HAS_DIRECTION_UD  = (1 << 2), /* Mode has up/down parameter       */
    MODE_FLAG_HAS_DIRECTION_HV  = (1 << 3), /* Mode has horiz/vert parameter    */
    MODE_FLAG_HAS_BRIGHTNESS    = (1 << 4), /* Mode has brightness parameter    */
    MODE_FLAG_HAS_COLOR         = (1 << 5), /* Mode has custom color parameter  */
    MODE_FLAG_RANDOM_COLOR      = (1 << 6), /* Mode has random color option     */
    MODE_FLAG_PER_LED_COLOR     = (1 << 7), /* Mode uses device LED colors      */
};

/*------------------------------------------------------------------*\
| Mode Type                                                          |
\*------------------------------------------------------------------*/
typedef struct
{
    /*--------------------------------------------------------------*\
    | Mode Information                                               |
    \*--------------------------------------------------------------*/
    std::string         name;   /* Mode name                        */
    int                 value;  /* Device-specific mode value       */
    unsigned int        flags;  /* Mode flags bitfield              */

    /*--------------------------------------------------------------*\
    | Mode Settings                                                  |
    \*--------------------------------------------------------------*/
    unsigned int        speed;  /* Mode speed parameter value       */
    bool                random; /* Random color mode enabled        */
} mode;

typedef struct
{
    std::string         name;   /* LED name         */
} led;

typedef int zone_type;

enum
{
    ZONE_TYPE_SINGLE,
    ZONE_TYPE_LINEAR,
    ZONE_TYPE_MATRIX
};

typedef int device_type;

enum
{
    DEVICE_TYPE_MOTHERBOARD,
    DEVICE_TYPE_DRAM,
    DEVICE_TYPE_GPU,
    DEVICE_TYPE_COOLER,
    DEVICE_TYPE_LEDSTRIP,
    DEVICE_TYPE_KEYBOARD,
    DEVICE_TYPE_MOUSE,
    DEVICE_TYPE_MOUSEMAT,
    DEVICE_TYPE_HEADSET,
    DEVICE_TYPE_UNKNOWN
};

typedef struct
{
    std::string         name;   /* Zone name        */
	zone_type           type;   /* Zone type        */
    std::vector<std::vector<int>>
                        map;    /* LED index map    */
} zone;

class RGBController
{
public:
    std::string             name;           /* controller name          */
    std::string             description;    /* controller description   */
    std::string             version;        /* controller version       */
    std::string             serial;         /* controller serial number */
    std::string             location;       /* controller location      */
    std::vector<led>        leds;           /* LEDs                     */
    std::vector<zone>       zones;          /* Zones                    */
    std::vector<mode>       modes;          /* Modes                    */
    std::vector<RGBColor>   colors;         /* Color buffer             */
    device_type             type;           /* device type              */
    int                     active_mode;    /* active mode              */

    virtual ~RGBController() = default;

    /*---------------------------------------------------------*\
    | Generic functions implemented in RGBController.cpp        |
    \*---------------------------------------------------------*/
    RGBColor                GetLED(int led);
    void                    SetLED(int led, RGBColor color);
    void                    SetAllLEDs(RGBColor color);
    void                    SetAllZoneLEDs(int zone, RGBColor color);

    /*---------------------------------------------------------*\
    | Functions to be implemented in device implementation      |
    \*---------------------------------------------------------*/
    virtual int             GetMode()                                   = 0;
    virtual void            SetMode(int mode)                           = 0;
    virtual void            SetCustomMode()                             = 0;
    virtual void            UpdateLEDs()                                = 0;
    virtual void            UpdateZoneLEDs(int zone)                    = 0;
    virtual void            UpdateSingleLED(int led)                    = 0;
};
