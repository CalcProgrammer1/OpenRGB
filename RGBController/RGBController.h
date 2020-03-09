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
    MODE_FLAG_HAS_SPEED                 = (1 << 0), /* Mode has speed parameter         */
    MODE_FLAG_HAS_DIRECTION_LR          = (1 << 1), /* Mode has left/right parameter    */
    MODE_FLAG_HAS_DIRECTION_UD          = (1 << 2), /* Mode has up/down parameter       */
    MODE_FLAG_HAS_DIRECTION_HV          = (1 << 3), /* Mode has horiz/vert parameter    */
    MODE_FLAG_HAS_BRIGHTNESS            = (1 << 4), /* Mode has brightness parameter    */
    MODE_FLAG_HAS_PER_LED_COLOR         = (1 << 5), /* Mode has per-LED colors          */
    MODE_FLAG_HAS_MODE_SPECIFIC_COLOR   = (1 << 6), /* Mode has mode specific colors    */
    MODE_FLAG_HAS_RANDOM_COLOR          = (1 << 7), /* Mode has random color option     */
};

/*------------------------------------------------------------------*\
| Mode Directions                                                    |
\*------------------------------------------------------------------*/
enum
{
    MODE_DIRECTION_LEFT         = 0,        /* Mode direction left              */
    MODE_DIRECTION_RIGHT        = 1,        /* Mode direction right             */
    MODE_DIRECTION_UP           = 2,        /* Mode direction up                */
    MODE_DIRECTION_DOWN         = 3,        /* Mode direction down              */
    MODE_DIRECTION_HORIZONTAL   = 4,        /* Mode direction horizontal        */
    MODE_DIRECTION_VERTICAL     = 5,        /* Mode direction vertical          */
};

enum
{
    MODE_COLORS_NONE            = 0,        /* Mode has no colors               */
    MODE_COLORS_PER_LED         = 1,        /* Mode has per LED colors selected */
    MODE_COLORS_MODE_SPECIFIC   = 2,        /* Mode specific colors selected    */
    MODE_COLORS_RANDOM          = 3,        /* Mode has random colors selected  */
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
    unsigned int        speed_min;  /* speed minimum value          */
    unsigned int        speed_max;  /* speed maximum value          */
    unsigned int        colors_min; /* minimum number of mode colors*/
    unsigned int        colors_max; /* maximum numver of mode colors*/
    
    /*--------------------------------------------------------------*\
    | Mode Settings                                                  |
    \*--------------------------------------------------------------*/
    unsigned int        speed;  /* Mode speed parameter value       */
    unsigned int        direction;  /* Mode direction value         */
    unsigned int        color_mode; /* Mode color selection         */
    std::vector<RGBColor>
                        colors; /* mode-specific colors             */
} mode;

typedef struct
{
    std::string         name;   /* LED name                     */
    unsigned int        value;  /* Device-specific LED value    */
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

std::string device_type_to_str(device_type type);

typedef struct
{
    std::string             name;           /* Zone name                */
    zone_type               type;           /* Zone type                */
    led *                   leds;           /* List of LEDs in zone     */
    RGBColor *              colors;         /* Colors of LEDs in zone   */
    unsigned int            start_idx;      /* Start index of led/color */
    unsigned int            leds_count;     /* Number of LEDs in zone   */
    unsigned int            leds_min;       /* Minimum number of LEDs   */
    unsigned int            leds_max;       /* Maximum number of LEDs   */
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
    int                     active_mode = 0;/* active mode              */

    virtual ~RGBController() = default;

    /*---------------------------------------------------------*\
    | Generic functions implemented in RGBController.cpp        |
    \*---------------------------------------------------------*/
    void                    SetupColors();

    RGBColor                GetLED(int led);
    void                    SetLED(int led, RGBColor color);
    void                    SetAllLEDs(RGBColor color);
    void                    SetAllZoneLEDs(int zone, RGBColor color);

    int                     GetMode();
    void                    SetMode(int mode);

    unsigned char *         GetDeviceDescription();
    void                    ReadDeviceDescription(unsigned char* data_buf);

    /*---------------------------------------------------------*\
    | Functions to be implemented in device implementation      |
    \*---------------------------------------------------------*/
    virtual void            SetupZones()                                = 0;

    virtual void            ResizeZone(int zone, int new_size)          = 0;

    virtual void            UpdateLEDs()                                = 0;
    virtual void            UpdateZoneLEDs(int zone)                    = 0;
    virtual void            UpdateSingleLED(int led)                    = 0;

    virtual void            SetCustomMode()                             = 0;
    virtual void            UpdateMode()                                = 0;
};
