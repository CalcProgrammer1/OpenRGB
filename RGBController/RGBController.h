/*---------------------------------------------------------*\
| RGBController.h                                           |
|                                                           |
|   OpenRGB's RGB controller hardware abstration layer,     |
|   provides a generic representation of an RGB device      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include "nlohmann/json.hpp"

/*---------------------------------------------------------*\
| RGB Color Type and Conversion Macros                      |
\*---------------------------------------------------------*/
typedef unsigned int RGBColor;

#define RGBGetRValue(rgb)   (rgb & 0x000000FF)
#define RGBGetGValue(rgb)   ((rgb >> 8) & 0x000000FF)
#define RGBGetBValue(rgb)   ((rgb >> 16) & 0x000000FF)

#define ToRGBColor(r, g, b) ((RGBColor)((b << 16) | (g << 8) | (r)))

#define RGBToBGRColor(rgb) ((rgb & 0xFF) << 16 | (rgb & 0xFF00) | (rgb & 0xFF0000) >> 16)

/*---------------------------------------------------------*\
| Mode Flags                                                |
\*---------------------------------------------------------*/
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
    MODE_FLAG_MANUAL_SAVE               = (1 << 8), /* Mode can manually be saved       */
    MODE_FLAG_AUTOMATIC_SAVE            = (1 << 9), /* Mode automatically saves         */
    MODE_FLAG_REQUIRES_ENTIRE_DEVICE    = (1 << 10),/* Mode always applies to entire    */
                                                    /* device, overrides per-zone modes */
};

/*---------------------------------------------------------*\
| Mode Directions                                           |
\*---------------------------------------------------------*/
enum
{
    MODE_DIRECTION_LEFT         = 0,        /* Mode direction left              */
    MODE_DIRECTION_RIGHT        = 1,        /* Mode direction right             */
    MODE_DIRECTION_UP           = 2,        /* Mode direction up                */
    MODE_DIRECTION_DOWN         = 3,        /* Mode direction down              */
    MODE_DIRECTION_HORIZONTAL   = 4,        /* Mode direction horizontal        */
    MODE_DIRECTION_VERTICAL     = 5,        /* Mode direction vertical          */
};

/*---------------------------------------------------------*\
| Mode Color Types                                          |
\*---------------------------------------------------------*/
enum
{
    MODE_COLORS_NONE            = 0,        /* Mode has no colors               */
    MODE_COLORS_PER_LED         = 1,        /* Mode has per LED colors selected */
    MODE_COLORS_MODE_SPECIFIC   = 2,        /* Mode specific colors selected    */
    MODE_COLORS_RANDOM          = 3,        /* Mode has random colors selected  */
};

/*---------------------------------------------------------*\
| Mode Class                                                |
\*---------------------------------------------------------*/
class mode
{
public:
    /*-----------------------------------------------------*\
    | Mode Information                                      |
    \*-----------------------------------------------------*/
    std::string         name;   /* Mode name                        */
    int                 value;  /* Device-specific mode value       */
    unsigned int        flags;  /* Mode flags bitfield              */
    unsigned int        speed_min;  /* speed minimum value          */
    unsigned int        speed_max;  /* speed maximum value          */
    unsigned int        brightness_min; /*brightness min value      */
    unsigned int        brightness_max; /*brightness max value      */
    unsigned int        colors_min; /* minimum number of mode colors*/
    unsigned int        colors_max; /* maximum numver of mode colors*/

    /*-----------------------------------------------------*\
    | Mode Settings                                         |
    \*-----------------------------------------------------*/
    unsigned int        speed;  /* Mode speed parameter value       */
    unsigned int        brightness; /* Mode brightness value        */
    unsigned int        direction;  /* Mode direction value         */
    unsigned int        color_mode; /* Mode color selection         */
    std::vector<RGBColor>
                        colors; /* mode-specific colors             */

    /*-----------------------------------------------------*\
    | Mode Constructor / Destructor                         |
    \*-----------------------------------------------------*/
    mode();
    ~mode();
};

/*---------------------------------------------------------*\
| LED Struct                                                |
\*---------------------------------------------------------*/
typedef struct
{
    /*-----------------------------------------------------*\
    | LED Information                                       |
    \*-----------------------------------------------------*/
    std::string         name;   /* LED name                     */
    unsigned int        value;  /* Device-specific LED value    */
} led;

/*---------------------------------------------------------*\
| Zone Flags                                                |
\*---------------------------------------------------------*/
typedef unsigned int zone_flags;

#define ZONE_FLAGS_MANUALLY_CONFIGURABLE               (ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY   |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE                |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME                |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE                |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP          |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS            |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURABLE_COLOR_ORDER)

#define ZONE_FLAGS_MANUALLY_CONFIGURED                 (ZONE_FLAG_MANUALLY_CONFIGURED_SIZE                  |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURED_NAME                  |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURED_TYPE                  |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP            |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS              |       \
                                                        ZONE_FLAG_MANUALLY_CONFIGURED_COLOR_ORDER)
enum
{
    ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE_EFFECTS_ONLY   = (1 << 0), /* Zone size is manually configurable, but only */
                                                                    /* for hardware effects, treated as single LED  */
                                                                    /* for per-LED modes                            */
    ZONE_FLAG_MANUALLY_CONFIGURABLE_SIZE                = (1 << 1), /* Zone size is manually configurable           */
    ZONE_FLAG_MANUALLY_CONFIGURABLE_NAME                = (1 << 2), /* Zone name is manually configurable           */
    ZONE_FLAG_MANUALLY_CONFIGURABLE_TYPE                = (1 << 3), /* Zone type is manually configurable           */
    ZONE_FLAG_MANUALLY_CONFIGURABLE_MATRIX_MAP          = (1 << 4), /* Zone matrix map is manually configurable     */
    ZONE_FLAG_MANUALLY_CONFIGURABLE_SEGMENTS            = (1 << 5), /* Zone segments are manually configurable      */
    ZONE_FLAG_MANUALLY_CONFIGURABLE_COLOR_ORDER         = (1 << 6), /* Zone color order is manually configurable    */
    ZONE_FLAG_MANUALLY_CONFIGURED_SIZE                  = (1 << 12),/* Zone size has been manually configured       */
    ZONE_FLAG_MANUALLY_CONFIGURED_NAME                  = (1 << 13),/* Zone name has been manually configured       */
    ZONE_FLAG_MANUALLY_CONFIGURED_TYPE                  = (1 << 14),/* Zone type has been manually configured       */
    ZONE_FLAG_MANUALLY_CONFIGURED_MATRIX_MAP            = (1 << 15),/* Zone matrix map has been manually configured */
    ZONE_FLAG_MANUALLY_CONFIGURED_SEGMENTS              = (1 << 16),/* Zone segments have been manually configured  */
    ZONE_FLAG_MANUALLY_CONFIGURED_COLOR_ORDER           = (1 << 17),/* Zone color order has been manually configured*/
    ZONE_FLAG_SUPPORTS_COLOR_ORDER_DEFAULT              = (1 << 24),/* Zone supports default color order            */
    ZONE_FLAG_SUPPORTS_COLOR_ORDER_RGB                  = (1 << 25),/* Zone supports RGB color order                */
    ZONE_FLAG_SUPPORTS_COLOR_ORDER_RBG                  = (1 << 26),/* Zone supports RBG color order                */
    ZONE_FLAG_SUPPORTS_COLOR_ORDER_GRB                  = (1 << 27),/* Zone supports GRB color order                */
    ZONE_FLAG_SUPPORTS_COLOR_ORDER_GBR                  = (1 << 28),/* Zone supports GBR color order                */
    ZONE_FLAG_SUPPORTS_COLOR_ORDER_BRG                  = (1 << 29),/* Zone supports BRG color order                */
    ZONE_FLAG_SUPPORTS_COLOR_ORDER_BGR                  = (1 << 30),/* Zone supports BGR color order                */
};

/*---------------------------------------------------------*\
| Zone Types                                                |
\*---------------------------------------------------------*/
typedef unsigned int zone_type;

enum
{
    ZONE_TYPE_SINGLE,
    ZONE_TYPE_LINEAR,
    ZONE_TYPE_MATRIX,
    ZONE_TYPE_LINEAR_LOOP,
    ZONE_TYPE_MATRIX_LOOP_X,
    ZONE_TYPE_MATRIX_LOOP_Y,
    ZONE_TYPE_SEGMENTED
};

/*---------------------------------------------------------*\
| Zone Color Order                                          |
\*---------------------------------------------------------*/
typedef unsigned int zone_color_order;

enum
{
    ZONE_COLOR_ORDER_DEFAULT,                       /* Device default color order       */
    ZONE_COLOR_ORDER_RGB,                           /* RGB color order                  */
    ZONE_COLOR_ORDER_RBG,                           /* RBG color order                  */
    ZONE_COLOR_ORDER_GRB,                           /* GRB color order                  */
    ZONE_COLOR_ORDER_GBR,                           /* GBR color order                  */
    ZONE_COLOR_ORDER_BRG,                           /* BRG color order                  */
    ZONE_COLOR_ORDER_BGR                            /* BGR color order                  */
};

/*---------------------------------------------------------*\
| Matrix Map Struct                                         |
\*---------------------------------------------------------*/
class matrix_map_type
{
public:
    unsigned int                height;
    unsigned int                width;
    std::vector<unsigned int>   map;

    matrix_map_type();
    matrix_map_type(unsigned int height, unsigned int width, unsigned int * map);
    ~matrix_map_type();

    void Reset();
    void Set(unsigned int height, unsigned int width, unsigned int * map);
};

/*---------------------------------------------------------*\
| Segment Flags                                             |
\*---------------------------------------------------------*/
enum
{
    SEGMENT_FLAG_GROUP_START    = (1 << 0), /* Start of segment group   */
    SEGMENT_FLAG_GROUP_MEMBER   = (1 << 1), /* Segment is in group      */
};

/*---------------------------------------------------------*\
| Segment Class                                             |
\*---------------------------------------------------------*/
class segment
{
public:
    std::string             name;           /* Segment name             */
    zone_type               type;           /* Segment type             */
    unsigned int            start_idx;      /* Start index within zone  */
    unsigned int            leds_count;     /* Number of LEDs in segment*/
    matrix_map_type         matrix_map;     /* Matrix map               */
    unsigned int            flags;          /* Segment flags            */

    /*-----------------------------------------------------*\
    | Zone Constructor / Destructor                         |
    \*-----------------------------------------------------*/
    segment();
    ~segment();
};

/*---------------------------------------------------------*\
| Zone Class                                                |
\*---------------------------------------------------------*/
class zone
{
public:
    std::string             name;           /* Zone name                */
    zone_type               type;           /* Zone type                */
    led *                   leds;           /* List of LEDs in zone     */
    RGBColor *              colors;         /* Colors of LEDs in zone   */
    unsigned int            start_idx;      /* Start index of led/color */
    unsigned int            leds_count;     /* Number of LEDs in zone   */
    unsigned int            leds_min;       /* Minimum number of LEDs   */
    unsigned int            leds_max;       /* Maximum number of LEDs   */
    matrix_map_type         matrix_map;     /* Matrix map               */
    std::vector<segment>    segments;       /* Segments in zone         */
    zone_flags              flags;          /* Zone flags bitfield      */
    std::vector<mode>       modes;          /* Zone-specific modes      */
    int                     active_mode;    /* Active zone-specific mode*/
    zone_color_order        color_order;    /* Zone color order         */

    /*-----------------------------------------------------*\
    | Zone Constructor / Destructor                         |
    \*-----------------------------------------------------*/
    zone();
    ~zone();
};

/*---------------------------------------------------------*\
| Device Types                                              |
|   The enum order should be maintained as is for the API   |
|   however DEVICE_TYPE_UNKNOWN needs to remain last. Any   |
|   new device types need to be inserted at the end of the  |
|   list but before unknown.                                |
\*---------------------------------------------------------*/
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
    DEVICE_TYPE_HEADSET_STAND,
    DEVICE_TYPE_GAMEPAD,
    DEVICE_TYPE_LIGHT,
    DEVICE_TYPE_SPEAKER,
    DEVICE_TYPE_VIRTUAL,
    DEVICE_TYPE_STORAGE,
    DEVICE_TYPE_CASE,
    DEVICE_TYPE_MICROPHONE,
    DEVICE_TYPE_ACCESSORY,
    DEVICE_TYPE_KEYPAD,
    DEVICE_TYPE_LAPTOP,
    DEVICE_TYPE_MONITOR,
    DEVICE_TYPE_UNKNOWN,
};

/*---------------------------------------------------------*\
| Controller Flags                                          |
\*---------------------------------------------------------*/
enum
{
    CONTROLLER_FLAG_LOCAL               = (1 << 0), /* Device is local to this instance */
    CONTROLLER_FLAG_REMOTE              = (1 << 1), /* Device is on a remote instance   */
    CONTROLLER_FLAG_VIRTUAL             = (1 << 2), /* Device is a virtual device       */
    CONTROLLER_FLAG_HIDDEN              = (1 << 3), /* Device is hidden                 */

    CONTROLLER_FLAG_RESET_BEFORE_UPDATE = (1 << 8), /* Device resets update flag before */
                                                    /* calling update function          */
};

/*---------------------------------------------------------*\
| RGBController Callback Types                              |
\*---------------------------------------------------------*/
typedef void (*RGBControllerCallback)(void *, unsigned int, void *);

/*---------------------------------------------------------*\
| Update Reason Codes                                       |
\*---------------------------------------------------------*/
enum
{
    RGBCONTROLLER_UPDATE_REASON_UPDATELEDS,         /* UpdateLEDs() called              */
    RGBCONTROLLER_UPDATE_REASON_UPDATEMODE,         /* UpdateMode() called              */
    RGBCONTROLLER_UPDATE_REASON_SAVEMODE,           /* SaveMode() called                */
    RGBCONTROLLER_UPDATE_REASON_RESIZEZONE,         /* ResizeZone() called              */
    RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS,      /* ClearSegments() called           */
    RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT,         /* AddSegment() called              */
    RGBCONTROLLER_UPDATE_REASON_HIDDEN,             /* Hidden flag set                  */
    RGBCONTROLLER_UPDATE_REASON_UNHIDDEN,           /* Hidden flag cleared              */
};

std::string device_type_to_str(device_type type);

class RGBControllerInterface
{
public:
    /*-----------------------------------------------------*\
    | Controller Information                                |
    \*-----------------------------------------------------*/
    virtual std::string             GetName()                                                                                                           = 0;
    virtual std::string             GetVendor()                                                                                                         = 0;
    virtual std::string             GetDescription()                                                                                                    = 0;
    virtual std::string             GetVersion()                                                                                                        = 0;
    virtual std::string             GetSerial()                                                                                                         = 0;
    virtual std::string             GetLocation()                                                                                                       = 0;

    virtual device_type             GetDeviceType()                                                                                                     = 0;
    virtual unsigned int            GetFlags()                                                                                                          = 0;

    /*-----------------------------------------------------*\
    | Hidden Flag Functions                                 |
    \*-----------------------------------------------------*/
    virtual bool                    GetHidden()                                                                                                         = 0;
    virtual void                    SetHidden(bool hidden)                                                                                              = 0;

    /*-----------------------------------------------------*\
    | Zone Functions                                        |
    \*-----------------------------------------------------*/
    virtual zone                    GetZone(unsigned int zone_idx)                                                                                      = 0;
    virtual int                     GetZoneActiveMode(unsigned int zone)                                                                                = 0;
    virtual RGBColor                GetZoneColor(unsigned int zone, unsigned int color_index)                                                           = 0;
    virtual RGBColor*               GetZoneColorsPointer(unsigned int zone)                                                                             = 0;
    virtual std::size_t             GetZoneCount()                                                                                                      = 0;
    virtual unsigned int            GetZoneFlags(unsigned int zone)                                                                                     = 0;
    virtual unsigned int            GetZoneLEDsCount(unsigned int zone)                                                                                 = 0;
    virtual unsigned int            GetZoneLEDsMax(unsigned int zone)                                                                                   = 0;
    virtual unsigned int            GetZoneLEDsMin(unsigned int zone)                                                                                   = 0;
    virtual matrix_map_type         GetZoneMatrixMap(unsigned int zone)                                                                                 = 0;
    virtual const unsigned int*     GetZoneMatrixMapData(unsigned int zone)                                                                             = 0;
    virtual unsigned int            GetZoneMatrixMapHeight(unsigned int zone)                                                                           = 0;
    virtual unsigned int            GetZoneMatrixMapWidth(unsigned int zone)                                                                            = 0;
    virtual std::size_t             GetZoneModeCount(unsigned int zone)                                                                                 = 0;
    virtual unsigned int            GetZoneModeBrightness(unsigned int zone, unsigned int mode)                                                         = 0;
    virtual unsigned int            GetZoneModeBrightnessMax(unsigned int zone, unsigned int mode)                                                      = 0;
    virtual unsigned int            GetZoneModeBrightnessMin(unsigned int zone, unsigned int mode)                                                      = 0;
    virtual RGBColor                GetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index)                                    = 0;
    virtual unsigned int            GetZoneModeColorMode(unsigned int zone, unsigned int mode)                                                          = 0;
    virtual std::size_t             GetZoneModeColorsCount(unsigned int zone, unsigned int mode)                                                        = 0;
    virtual unsigned int            GetZoneModeColorsMax(unsigned int zone, unsigned int mode)                                                          = 0;
    virtual unsigned int            GetZoneModeColorsMin(unsigned int zone, unsigned int mode)                                                          = 0;
    virtual unsigned int            GetZoneModeDirection(unsigned int zone, unsigned int mode)                                                          = 0;
    virtual unsigned int            GetZoneModeFlags(unsigned int zone, unsigned int mode)                                                              = 0;
    virtual std::string             GetZoneModeName(unsigned int zone, unsigned int mode)                                                               = 0;
    virtual unsigned int            GetZoneModeSpeed(unsigned int zone, unsigned int mode)                                                              = 0;
    virtual unsigned int            GetZoneModeSpeedMax(unsigned int zone, unsigned int mode)                                                           = 0;
    virtual unsigned int            GetZoneModeSpeedMin(unsigned int zone, unsigned int mode)                                                           = 0;
    virtual int                     GetZoneModeValue(unsigned int zone, unsigned int mode)                                                              = 0;
    virtual std::string             GetZoneName(unsigned int zone)                                                                                      = 0;
    virtual std::size_t             GetZoneSegmentCount(unsigned int zone)                                                                              = 0;
    virtual unsigned int            GetZoneSegmentLEDsCount(unsigned int zone, unsigned int segment)                                                    = 0;
    virtual matrix_map_type         GetZoneSegmentMatrixMap(unsigned int zone, unsigned int segment)                                                    = 0;
    virtual const unsigned int *    GetZoneSegmentMatrixMapData(unsigned int zone, unsigned int segment)                                                = 0;
    virtual unsigned int            GetZoneSegmentMatrixMapHeight(unsigned int zone, unsigned int segment)                                              = 0;
    virtual unsigned int            GetZoneSegmentMatrixMapWidth(unsigned int zone, unsigned int segment)                                               = 0;
    virtual std::string             GetZoneSegmentName(unsigned int zone, unsigned int segment)                                                         = 0;
    virtual unsigned int            GetZoneSegmentStartIndex(unsigned int zone, unsigned int segment)                                                   = 0;
    virtual unsigned int            GetZoneSegmentType(unsigned int zone, unsigned int segment)                                                         = 0;
    virtual unsigned int            GetZoneStartIndex(unsigned int zone)                                                                                = 0;
    virtual zone_type               GetZoneType(unsigned int zone)                                                                                      = 0;

    virtual unsigned int            GetLEDsInZone(unsigned int zone)                                                                                    = 0;

    virtual void                    SetZoneActiveMode(unsigned int zone, int mode)                                                                      = 0;
    virtual void                    SetZoneColor(unsigned int zone, unsigned int color_index, RGBColor color)                                           = 0;
    virtual void                    SetZoneModeBrightness(unsigned int zone, unsigned int mode, unsigned int brightness)                                = 0;
    virtual void                    SetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index, RGBColor color)                    = 0;
    virtual void                    SetZoneModeColorMode(unsigned int zone, unsigned int mode, unsigned int color_mode)                                 = 0;
    virtual void                    SetZoneModeColorsCount(unsigned int zone, unsigned int mode, std::size_t count)                                     = 0;
    virtual void                    SetZoneModeDirection(unsigned int zone, unsigned int mode, unsigned int direction)                                  = 0;
    virtual void                    SetZoneModeSpeed(unsigned int zone, unsigned int mode, unsigned int speed)                                          = 0;

    virtual bool                    SupportsPerZoneModes()                                                                                              = 0;

    /*-----------------------------------------------------*\
    | Mode Functions                                        |
    \*-----------------------------------------------------*/
    virtual std::size_t             GetModeCount()                                                                                                      = 0;
    virtual unsigned int            GetModeBrightness(unsigned int mode)                                                                                = 0;
    virtual unsigned int            GetModeBrightnessMax(unsigned int mode)                                                                             = 0;
    virtual unsigned int            GetModeBrightnessMin(unsigned int mode)                                                                             = 0;
    virtual RGBColor                GetModeColor(unsigned int mode, unsigned int color_index)                                                           = 0;
    virtual unsigned int            GetModeColorMode(unsigned int mode)                                                                                 = 0;
    virtual std::size_t             GetModeColorsCount(unsigned int mode)                                                                               = 0;
    virtual unsigned int            GetModeColorsMax(unsigned int mode)                                                                                 = 0;
    virtual unsigned int            GetModeColorsMin(unsigned int mode)                                                                                 = 0;
    virtual unsigned int            GetModeDirection(unsigned int mode)                                                                                 = 0;
    virtual unsigned int            GetModeFlags(unsigned int mode)                                                                                     = 0;
    virtual std::string             GetModeName(unsigned int mode)                                                                                      = 0;
    virtual unsigned int            GetModeSpeed(unsigned int mode)                                                                                     = 0;
    virtual unsigned int            GetModeSpeedMax(unsigned int mode)                                                                                  = 0;
    virtual unsigned int            GetModeSpeedMin(unsigned int mode)                                                                                  = 0;
    virtual int                     GetModeValue(unsigned int mode)                                                                                     = 0;

    virtual void                    SetModeBrightness(unsigned int mode, unsigned int brightness)                                                       = 0;
    virtual void                    SetModeColor(unsigned int mode, unsigned int color_index, RGBColor color)                                           = 0;
    virtual void                    SetModeColorMode(unsigned int mode, unsigned int color_mode)                                                        = 0;
    virtual void                    SetModeColorsCount(unsigned int mode, std::size_t count)                                                            = 0;
    virtual void                    SetModeDirection(unsigned int mode, unsigned int direction)                                                         = 0;
    virtual void                    SetModeSpeed(unsigned int mode, unsigned int speed)                                                                 = 0;

    virtual int                     GetActiveMode()                                                                                                     = 0;
    virtual void                    SetActiveMode(int mode)                                                                                             = 0;
    virtual void                    SetCustomMode()                                                                                                     = 0;

    /*-----------------------------------------------------*\
    | LED Functions                                         |
    \*-----------------------------------------------------*/
    virtual std::size_t             GetLEDCount()                                                                                                       = 0;
    virtual std::string             GetLEDName(unsigned int led)                                                                                        = 0;
    virtual unsigned int            GetLEDValue(unsigned int led)                                                                                       = 0;

    virtual std::string             GetLEDDisplayName(unsigned int led)                                                                                 = 0;

    /*-----------------------------------------------------*\
    | Color Functions                                       |
    \*-----------------------------------------------------*/
    virtual RGBColor                GetColor(unsigned int led)                                                                                          = 0;
    virtual RGBColor*               GetColorsPointer()                                                                                                  = 0;
    virtual void                    SetColor(unsigned int led, RGBColor color)                                                                          = 0;

    virtual void                    SetAllColors(RGBColor color)                                                                                        = 0;
    virtual void                    SetAllZoneColors(int zone, RGBColor color)                                                                          = 0;

    /*-----------------------------------------------------*\
    | Update Callback Functions                             |
    \*-----------------------------------------------------*/
    virtual void                    RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg)                                 = 0;
    virtual void                    UnregisterUpdateCallback(void * callback_arg)                                                                       = 0;
    virtual void                    ClearCallbacks()                                                                                                    = 0;
    virtual void                    SignalUpdate(unsigned int update_reason)                                                                            = 0;

    /*-----------------------------------------------------*\
    | Device Update Functions                               |
    \*-----------------------------------------------------*/
    virtual void                    UpdateLEDs()                                                                                                        = 0;
    virtual void                    UpdateZoneLEDs(int zone)                                                                                            = 0;
    virtual void                    UpdateSingleLED(int led)                                                                                            = 0;

    virtual void                    UpdateMode()                                                                                                        = 0;
    virtual void                    UpdateZoneMode(int zone)                                                                                            = 0;
    virtual void                    SaveMode()                                                                                                          = 0;

    virtual void                    ClearSegments(int zone)                                                                                             = 0;
    virtual void                    AddSegment(int zone, segment new_segment)                                                                           = 0;

    virtual void                    ConfigureZone(int zone_idx, zone new_zone)                                                                          = 0;
    virtual void                    ResizeZone(int zone, int new_size)                                                                                  = 0;
};

class RGBController : public RGBControllerInterface
{
public:
    /*-----------------------------------------------------*\
    | RGBController base class constructor                  |
    \*-----------------------------------------------------*/
    RGBController();
    virtual ~RGBController();

    /*-----------------------------------------------------*\
    | Controller Information                                |
    \*-----------------------------------------------------*/
    std::string             GetName();
    std::string             GetVendor();
    std::string             GetDescription();
    std::string             GetVersion();
    std::string             GetSerial();
    std::string             GetLocation();

    device_type             GetDeviceType();
    unsigned int            GetFlags();

    /*-----------------------------------------------------*\
    | Hidden Flag Functions                                 |
    \*-----------------------------------------------------*/
    bool                    GetHidden();
    void                    SetHidden(bool hidden);

    /*-----------------------------------------------------*\
    | Zone Functions                                        |
    \*-----------------------------------------------------*/
    zone                    GetZone(unsigned int zone_idx);
    int                     GetZoneActiveMode(unsigned int zone);
    RGBColor                GetZoneColor(unsigned int zone, unsigned int color_index);
    RGBColor*               GetZoneColorsPointer(unsigned int zone);
    std::size_t             GetZoneCount();
    unsigned int            GetZoneFlags(unsigned int zone);
    unsigned int            GetZoneLEDsCount(unsigned int zone);
    unsigned int            GetZoneLEDsMax(unsigned int zone);
    unsigned int            GetZoneLEDsMin(unsigned int zone);
    matrix_map_type         GetZoneMatrixMap(unsigned int zone);
    const unsigned int*     GetZoneMatrixMapData(unsigned int zone);
    unsigned int            GetZoneMatrixMapHeight(unsigned int zone);
    unsigned int            GetZoneMatrixMapWidth(unsigned int zone);
    std::size_t             GetZoneModeCount(unsigned int zone);
    unsigned int            GetZoneModeBrightness(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeBrightnessMax(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeBrightnessMin(unsigned int zone, unsigned int mode);
    RGBColor                GetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index);
    unsigned int            GetZoneModeColorMode(unsigned int zone, unsigned int mode);
    std::size_t             GetZoneModeColorsCount(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeColorsMax(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeColorsMin(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeDirection(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeFlags(unsigned int zone, unsigned int mode);
    std::string             GetZoneModeName(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeSpeed(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeSpeedMax(unsigned int zone, unsigned int mode);
    unsigned int            GetZoneModeSpeedMin(unsigned int zone, unsigned int mode);
    int                     GetZoneModeValue(unsigned int zone, unsigned int mode);
    std::string             GetZoneName(unsigned int zone);
    std::size_t             GetZoneSegmentCount(unsigned int zone);
    unsigned int            GetZoneSegmentLEDsCount(unsigned int zone, unsigned int segment);
    matrix_map_type         GetZoneSegmentMatrixMap(unsigned int zone, unsigned int segment);
    const unsigned int *    GetZoneSegmentMatrixMapData(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentMatrixMapHeight(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentMatrixMapWidth(unsigned int zone, unsigned int segment);
    std::string             GetZoneSegmentName(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentStartIndex(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneSegmentType(unsigned int zone, unsigned int segment);
    unsigned int            GetZoneStartIndex(unsigned int zone);
    zone_type               GetZoneType(unsigned int zone);

    unsigned int            GetLEDsInZone(unsigned int zone);

    void                    SetZoneActiveMode(unsigned int zone, int mode);
    void                    SetZoneColor(unsigned int zone, unsigned int color_index, RGBColor color);
    void                    SetZoneModeBrightness(unsigned int zone, unsigned int mode, unsigned int brightness);
    void                    SetZoneModeColor(unsigned int zone, unsigned int mode, unsigned int color_index, RGBColor color);
    void                    SetZoneModeColorMode(unsigned int zone, unsigned int mode, unsigned int color_mode);
    void                    SetZoneModeColorsCount(unsigned int zone, unsigned int mode, std::size_t count);
    void                    SetZoneModeDirection(unsigned int zone, unsigned int mode, unsigned int direction);
    void                    SetZoneModeSpeed(unsigned int zone, unsigned int mode, unsigned int speed);

    bool                    SupportsPerZoneModes();

    /*-----------------------------------------------------*\
    | Mode Functions                                        |
    \*-----------------------------------------------------*/
    std::size_t             GetModeCount();
    unsigned int            GetModeBrightness(unsigned int mode);
    unsigned int            GetModeBrightnessMax(unsigned int mode);
    unsigned int            GetModeBrightnessMin(unsigned int mode);
    RGBColor                GetModeColor(unsigned int mode, unsigned int color_index);
    unsigned int            GetModeColorMode(unsigned int mode);
    std::size_t             GetModeColorsCount(unsigned int mode);
    unsigned int            GetModeColorsMax(unsigned int mode);
    unsigned int            GetModeColorsMin(unsigned int mode);
    unsigned int            GetModeDirection(unsigned int mode);
    unsigned int            GetModeFlags(unsigned int mode);
    std::string             GetModeName(unsigned int mode);
    unsigned int            GetModeSpeed(unsigned int mode);
    unsigned int            GetModeSpeedMax(unsigned int mode);
    unsigned int            GetModeSpeedMin(unsigned int mode);
    int                     GetModeValue(unsigned int mode);

    void                    SetModeBrightness(unsigned int mode, unsigned int brightness);
    void                    SetModeColor(unsigned int mode, unsigned int color_index, RGBColor color);
    void                    SetModeColorMode(unsigned int mode, unsigned int color_mode);
    void                    SetModeColorsCount(unsigned int mode, std::size_t count);
    void                    SetModeDirection(unsigned int mode, unsigned int direction);
    void                    SetModeSpeed(unsigned int mode, unsigned int speed);

    int                     GetActiveMode();
    void                    SetActiveMode(int mode);
    void                    SetCustomMode();

    /*-----------------------------------------------------*\
    | LED Functions                                         |
    \*-----------------------------------------------------*/
    std::size_t             GetLEDCount();
    std::string             GetLEDName(unsigned int led);
    unsigned int            GetLEDValue(unsigned int led);

    std::string             GetLEDDisplayName(unsigned int led);

    /*-----------------------------------------------------*\
    | Color Functions                                       |
    \*-----------------------------------------------------*/
    RGBColor                GetColor(unsigned int led);
    RGBColor*               GetColorsPointer();
    void                    SetColor(unsigned int led, RGBColor color);
    void                    SetAllColors(RGBColor color);
    void                    SetAllZoneColors(int zone, RGBColor color);

    /*-----------------------------------------------------*\
    | Update Callback Functions                             |
    \*-----------------------------------------------------*/
    void                    RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg);
    void                    UnregisterUpdateCallback(void * callback_arg);
    void                    ClearCallbacks();
    void                    SignalUpdate(unsigned int update_reason);

    /*-----------------------------------------------------*\
    | Device Update Functions                               |
    \*-----------------------------------------------------*/
    void                    Shutdown();

    void                    UpdateLEDs();
    void                    UpdateZoneLEDs(int zone);
    void                    UpdateSingleLED(int led);

    void                    UpdateMode();
    void                    UpdateZoneMode(int zone);
    void                    SaveMode();

    void                    DeviceCallThreadFunction();

    void                    ClearSegments(int zone);
    void                    AddSegment(int zone, segment new_segment);

    void                    ConfigureZone(int zone_idx, zone new_zone);
    void                    ResizeZone(int zone, int new_size);

    /*-----------------------------------------------------*\
    | Functions to be implemented in device implementation  |
    \*-----------------------------------------------------*/
    virtual void            DeviceConfigureZone(int zone_idx);

    virtual void            DeviceUpdateLEDs();
    virtual void            DeviceUpdateZoneLEDs(int zone);
    virtual void            DeviceUpdateSingleLED(int led);

    virtual void            DeviceUpdateMode();
    virtual void            DeviceUpdateZoneMode(int zone);
    virtual void            DeviceSaveMode();

    /*-----------------------------------------------------*\
    | Static Serialized Description Functions               |
    \*-----------------------------------------------------*/
    static unsigned char *  GetColorDescriptionData(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version);
    static unsigned int     GetColorDescriptionSize(RGBController* controller, unsigned int protocol_version);
    static unsigned char *  GetDeviceDescriptionData(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version);
    static unsigned int     GetDeviceDescriptionSize(RGBController* controller, unsigned int protocol_version);
    static unsigned char *  GetLEDDescriptionData(unsigned char* data_ptr, led led, unsigned int protocol_version);
    static unsigned int     GetLEDDescriptionSize(led led, unsigned int protocol_version);
    static unsigned char *  GetMatrixMapDescriptionData(unsigned char* data_ptr, matrix_map_type matrix_map, unsigned int protocol_version);
    static unsigned int     GetMatrixMapDescriptionSize(matrix_map_type matrix_map, unsigned int protocol_version);
    static unsigned char *  GetModeDescriptionData(unsigned char* data_ptr, mode mode, unsigned int protocol_version);
    static unsigned int     GetModeDescriptionSize(mode mode, unsigned int protocol_version);
    static unsigned char *  GetSegmentDescriptionData(unsigned char* data_ptr, segment segment, unsigned int protocol_version);
    static unsigned int     GetSegmentDescriptionSize(segment segment, unsigned int protocol_version);
    static unsigned char *  GetZoneDescriptionData(unsigned char* data_ptr, zone zone, unsigned int protocol_version);
    static unsigned int     GetZoneDescriptionSize(zone zone, unsigned int protocol_version);

    static unsigned char*   SetColorDescription(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version, bool resize = false);
    static unsigned char*   SetDeviceDescription(unsigned char* data_ptr, RGBController* controller, unsigned int protocol_version);
    static unsigned char*   SetLEDDescription(unsigned char* data_ptr, led* led, unsigned int protocol_version);
    static unsigned char*   SetMatrixMapDescription(unsigned char* data_ptr, matrix_map_type* matrix_map, unsigned int protocol_version);
    static unsigned char*   SetModeDescription(unsigned char* data_ptr, mode* mode, unsigned int protocol_version);
    static unsigned char*   SetSegmentDescription(unsigned char* data_ptr, segment* segment, unsigned int protocol_version);
    static unsigned char*   SetZoneDescription(unsigned char* data_ptr, zone* zone, unsigned int protocol_version);

    /*-----------------------------------------------------*\
    | Static JSON Description Functions                     |
    \*-----------------------------------------------------*/
    static nlohmann::json   GetDeviceDescriptionJSON(RGBController* controller);
    static nlohmann::json   GetLEDDescriptionJSON(led led);
    static nlohmann::json   GetMatrixMapDescriptionJSON(matrix_map_type matrix_map);
    static nlohmann::json   GetModeDescriptionJSON(mode mode);
    static nlohmann::json   GetSegmentDescriptionJSON(segment segment);
    static nlohmann::json   GetZoneDescriptionJSON(zone zone);

    static void             SetDeviceDescriptionJSON(nlohmann::json controller_json, RGBController* controller);
    static led              SetLEDDescriptionJSON(nlohmann::json led_json);
    static matrix_map_type  SetMatrixMapDescriptionJSON(nlohmann::json matrix_map_json);
    static mode             SetModeDescriptionJSON(nlohmann::json mode_json);
    static segment          SetSegmentDescriptionJSON(nlohmann::json segment_json);
    static zone             SetZoneDescriptionJSON(nlohmann::json zone_json);

protected:
    /*-----------------------------------------------------*\
    | Controller information strings                        |
    \*-----------------------------------------------------*/
    std::string             description;    /* controller description   */
    std::string             location;       /* controller location      */
    std::string             name;           /* controller name          */
    std::string             serial;         /* controller serial number */
    std::string             vendor;         /* controller vendor        */
    std::string             version;        /* controller version       */

    /*-----------------------------------------------------*\
    | Controller variables                                  |
    \*-----------------------------------------------------*/
    int                     active_mode = 0;/* active mode              */
    unsigned int            flags;          /* controller flags         */
    device_type             type;           /* device type              */

    /*-----------------------------------------------------*\
    | Controller member vectors                             |
    \*-----------------------------------------------------*/
    std::vector<RGBColor>   colors;         /* Color buffer             */
    std::vector<led>        leds;           /* LEDs                     */
    std::vector<std::string>
                            led_alt_names;  /* alternate LED names      */
    std::vector<mode>       modes;          /* Modes                    */
    std::vector<zone>       zones;          /* Zones                    */

    /*-----------------------------------------------------*\
    | Access mutex variables                                |
    \*-----------------------------------------------------*/
    std::shared_mutex       AccessMutex;

    /*-----------------------------------------------------*\
    | Functions not part of interface for internal use only |
    \*-----------------------------------------------------*/
    unsigned int            LEDsInZone(unsigned int zone);
    void                    SetupColors();
    void                    UpdateLEDsInternal();

private:
    /*-----------------------------------------------------*\
    | Device thread variables                               |
    \*-----------------------------------------------------*/
    std::thread*            DeviceCallThread;
    std::atomic<bool>       CallFlag_UpdateLEDs;
    std::atomic<bool>       CallFlag_UpdateMode;
    std::atomic<bool>       DeviceThreadRunning;

    /*-----------------------------------------------------*\
    | Update callback variables                             |
    \*-----------------------------------------------------*/
    std::mutex                          UpdateMutex;
    std::vector<RGBControllerCallback>  UpdateCallbacks;
    std::vector<void *>                 UpdateCallbackArgs;

    /*-----------------------------------------------------*\
    | Private path used internally by DetectionManager      |
    | for HID hotplug path tracking                         |
    \*-----------------------------------------------------*/
    std::string             detection_path;

    /*-----------------------------------------------------*\
    | Certain internal OpenRGB framework classes can modify |
    | protected members                                     |
    \*-----------------------------------------------------*/
    friend class            DetectionManager;
    friend class            NetworkClient;
    friend class            NetworkServer;
    friend class            OpenRGBPluginAPI;
    friend class            ProfileManager;
    friend class            ResourceManager;
};
