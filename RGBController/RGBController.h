/*---------------------------------------------------------*\
| RGBController.h                                           |
|                                                           |
|   OpenRGB's RGB controller hardware abstration layer,     |
|   provides a generic representation of an RGB device      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                02 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

/*------------------------------------------------------------------*\
| RGB Color Type and Conversion Macros                               |
\*------------------------------------------------------------------*/
typedef unsigned int RGBColor;

#define RGBGetRValue(rgb)   (rgb & 0x000000FF)
#define RGBGetGValue(rgb)   ((rgb >> 8) & 0x000000FF)
#define RGBGetBValue(rgb)   ((rgb >> 16) & 0x000000FF)

#define ToRGBColor(r, g, b) ((RGBColor)((b << 16) | (g << 8) | (r)))

#define RGBToBGRColor(rgb) ((rgb & 0xFF) << 16 | (rgb & 0xFF00) | (rgb & 0xFF0000) >> 16)

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
    MODE_FLAG_MANUAL_SAVE               = (1 << 8), /* Mode can manually be saved       */
    MODE_FLAG_AUTOMATIC_SAVE            = (1 << 9), /* Mode automatically saves         */
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

/*------------------------------------------------------------------*\
| Mode Color Types                                                   |
\*------------------------------------------------------------------*/
enum
{
    MODE_COLORS_NONE            = 0,        /* Mode has no colors               */
    MODE_COLORS_PER_LED         = 1,        /* Mode has per LED colors selected */
    MODE_COLORS_MODE_SPECIFIC   = 2,        /* Mode specific colors selected    */
    MODE_COLORS_RANDOM          = 3,        /* Mode has random colors selected  */
};

/*------------------------------------------------------------------*\
| Mode Class                                                         |
\*------------------------------------------------------------------*/
class mode
{
public:
    /*--------------------------------------------------------------*\
    | Mode Information                                               |
    \*--------------------------------------------------------------*/
    std::string         name;   /* Mode name                        */
    int                 value;  /* Device-specific mode value       */
    unsigned int        flags;  /* Mode flags bitfield              */
    unsigned int        speed_min;  /* speed minimum value          */
    unsigned int        speed_max;  /* speed maximum value          */
    unsigned int        brightness_min; /*brightness min value      */
    unsigned int        brightness_max; /*brightness max value      */
    unsigned int        colors_min; /* minimum number of mode colors*/
    unsigned int        colors_max; /* maximum numver of mode colors*/

    /*--------------------------------------------------------------*\
    | Mode Settings                                                  |
    \*--------------------------------------------------------------*/
    unsigned int        speed;  /* Mode speed parameter value       */
    unsigned int        brightness; /* Mode brightness value        */
    unsigned int        direction;  /* Mode direction value         */
    unsigned int        color_mode; /* Mode color selection         */
    std::vector<RGBColor>
                        colors; /* mode-specific colors             */

    /*--------------------------------------------------------------*\
    | Mode Constructor / Destructor                                  |
    \*--------------------------------------------------------------*/
    mode();
    ~mode();
};

/*------------------------------------------------------------------*\
| LED Struct                                                         |
\*------------------------------------------------------------------*/
typedef struct
{
    std::string         name;   /* LED name                     */
    unsigned int        value;  /* Device-specific LED value    */
} led;

/*------------------------------------------------------------------*\
| Zone Flags                                                         |
\*------------------------------------------------------------------*/
enum
{
    ZONE_FLAG_RESIZE_EFFECTS_ONLY       = (1 << 0), /* Zone is resizable, but only for  */
                                                    /* effects - treat as single LED    */
};

/*------------------------------------------------------------------*\
| Zone Types                                                         |
\*------------------------------------------------------------------*/
typedef int zone_type;

enum
{
    ZONE_TYPE_SINGLE,
    ZONE_TYPE_LINEAR,
    ZONE_TYPE_MATRIX
};

/*------------------------------------------------------------------*\
| Matrix Map Struct                                                  |
\*------------------------------------------------------------------*/
typedef struct
{
    unsigned int            height;
    unsigned int            width;
    unsigned int *          map;
} matrix_map_type;

/*------------------------------------------------------------------*\
| Segment Struct                                                     |
\*------------------------------------------------------------------*/
typedef struct
{
    std::string             name;           /* Segment name             */
    zone_type               type;           /* Segment type             */
    unsigned int            start_idx;      /* Start index within zone  */
    unsigned int            leds_count;     /* Number of LEDs in segment*/
} segment;

/*------------------------------------------------------------------*\
| Zone Class                                                         |
\*------------------------------------------------------------------*/
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
    matrix_map_type *       matrix_map;     /* Matrix map pointer       */
	std::vector<segment>    segments;       /* Segments in zone         */
    unsigned int            flags;          /* Zone flags bitfield      */

    /*--------------------------------------------------------------*\
    | Zone Constructor / Destructor                                  |
    \*--------------------------------------------------------------*/
    zone();
    ~zone();
};

/*------------------------------------------------------------------*\
| Device Types                                                       |
|   The enum order should be maintained as is for the API however    |
|   DEVICE_TYPE_UNKNOWN needs to remain last. Any new device types   |
|   need to be inserted at the end of the list but before unknown.   |
\*------------------------------------------------------------------*/
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

/*------------------------------------------------------------------*\
| Controller Flags                                                   |
\*------------------------------------------------------------------*/
enum
{
    CONTROLLER_FLAG_LOCAL               = (1 << 0), /* Device is local to this instance */
    CONTROLLER_FLAG_REMOTE              = (1 << 1), /* Device is on a remote instance   */
    CONTROLLER_FLAG_VIRTUAL             = (1 << 2), /* Device is a virtual device       */

    CONTROLLER_FLAG_RESET_BEFORE_UPDATE = (1 << 8), /* Device resets update flag before */
                                                    /* calling update function          */
};

/*------------------------------------------------------------------*\
| RGBController Callback Types                                       |
\*------------------------------------------------------------------*/
typedef void (*RGBControllerCallback)(void *);

std::string device_type_to_str(device_type type);

class RGBControllerInterface
{
public:
    virtual void            SetupColors()                                                                       = 0;

    virtual unsigned int    GetLEDsInZone(unsigned int zone)                                                    = 0;
    virtual std::string     GetName()                                                                           = 0;
    virtual std::string     GetVendor()                                                                         = 0;
    virtual std::string     GetDescription()                                                                    = 0;
    virtual std::string     GetVersion()                                                                        = 0;
    virtual std::string     GetSerial()                                                                         = 0;
    virtual std::string     GetLocation()                                                                       = 0;

    virtual std::string     GetModeName(unsigned int mode)                                                      = 0;
    virtual std::string     GetZoneName(unsigned int zone)                                                      = 0;
    virtual std::string     GetLEDName(unsigned int led)                                                        = 0;

    virtual RGBColor        GetLED(unsigned int led)                                                            = 0;
    virtual void            SetLED(unsigned int led, RGBColor color)                                            = 0;
    virtual void            SetAllLEDs(RGBColor color)                                                          = 0;
    virtual void            SetAllZoneLEDs(int zone, RGBColor color)                                            = 0;

    virtual int             GetMode()                                                                           = 0;
    virtual void            SetMode(int mode)                                                                   = 0;

    virtual unsigned char * GetDeviceDescription(unsigned int protocol_version)                                 = 0;
    virtual void            ReadDeviceDescription(unsigned char* data_buf, unsigned int protocol_version)       = 0;

    virtual unsigned char * GetModeDescription(int mode, unsigned int protocol_version)                         = 0;
    virtual void            SetModeDescription(unsigned char* data_buf, unsigned int protocol_version)          = 0;

    virtual unsigned char * GetColorDescription()                                                               = 0;
    virtual void            SetColorDescription(unsigned char* data_buf)                                        = 0;

    virtual unsigned char * GetZoneColorDescription(int zone)                                                   = 0;
    virtual void            SetZoneColorDescription(unsigned char* data_buf)                                    = 0;

    virtual unsigned char * GetSingleLEDColorDescription(int led)                                               = 0;
    virtual void            SetSingleLEDColorDescription(unsigned char* data_buf)                               = 0;

    virtual void            RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg) = 0;
    virtual void            UnregisterUpdateCallback(void * callback_arg)                                       = 0;
    virtual void            ClearCallbacks()                                                                    = 0;
    virtual void            SignalUpdate()                                                                      = 0;

    virtual void            UpdateLEDs()                                                                        = 0;
    //virtual void          UpdateZoneLEDs(int zone)                                                            = 0;
    //virtual void          UpdateSingleLED(int led)                                                            = 0;

    virtual void            UpdateMode()                                                                        = 0;
    virtual void            SaveMode()                                                                          = 0;

    virtual void            DeviceCallThreadFunction()                                                          = 0;

    virtual void            ClearSegments(int zone)                                                             = 0;
    virtual void            AddSegment(int zone, segment new_segment)                                           = 0;

    /*---------------------------------------------------------*\
    | Functions to be implemented in device implementation      |
    \*---------------------------------------------------------*/
    virtual void            SetupZones()                                                                        = 0;

    virtual void            ResizeZone(int zone, int new_size)                                                  = 0;

    virtual void            DeviceUpdateLEDs()                                                                  = 0;
    virtual void            UpdateZoneLEDs(int zone)                                                            = 0;
    virtual void            UpdateSingleLED(int led)                                                            = 0;

    virtual void            DeviceUpdateMode()                                                                  = 0;
    virtual void            DeviceSaveMode()                                                                    = 0;

    virtual void            SetCustomMode()                                                                     = 0;
};

class RGBController : public RGBControllerInterface
{
public:
    std::string             name;           /* controller name          */
    std::string             vendor;         /* controller vendor        */
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
    std::vector<std::string>
                            led_alt_names;  /* alternate LED names      */
    unsigned int            flags;          /* controller flags         */

    /*---------------------------------------------------------*\
    | RGBController base class constructor                      |
    \*---------------------------------------------------------*/
    RGBController();
    virtual ~RGBController();

    /*---------------------------------------------------------*\
    | Generic functions implemented in RGBController.cpp        |
    \*---------------------------------------------------------*/
    void                    SetupColors();

    unsigned int            GetLEDsInZone(unsigned int zone);
    std::string             GetName();
    std::string             GetVendor();
    std::string             GetDescription();
    std::string             GetVersion();
    std::string             GetSerial();
    std::string             GetLocation();

    std::string             GetModeName(unsigned int mode);
    std::string             GetZoneName(unsigned int zone);
    std::string             GetLEDName(unsigned int led);

    RGBColor                GetLED(unsigned int led);
    void                    SetLED(unsigned int led, RGBColor color);
    void                    SetAllLEDs(RGBColor color);
    void                    SetAllZoneLEDs(int zone, RGBColor color);

    int                     GetMode();
    void                    SetMode(int mode);

    unsigned char *         GetDeviceDescription(unsigned int protocol_version);
    void                    ReadDeviceDescription(unsigned char* data_buf, unsigned int protocol_version);

    unsigned char *         GetModeDescription(int mode, unsigned int protocol_version);
    void                    SetModeDescription(unsigned char* data_buf, unsigned int protocol_version);

    unsigned char *         GetColorDescription();
    void                    SetColorDescription(unsigned char* data_buf);

    unsigned char *         GetZoneColorDescription(int zone);
    void                    SetZoneColorDescription(unsigned char* data_buf);

    unsigned char *         GetSingleLEDColorDescription(int led);
    void                    SetSingleLEDColorDescription(unsigned char* data_buf);

    unsigned char *         GetSegmentDescription(int zone, segment new_segment);
    void                    SetSegmentDescription(unsigned char* data_buf);

    void                    RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg);
    void                    UnregisterUpdateCallback(void * callback_arg);
    void                    ClearCallbacks();
    void                    SignalUpdate();

    void                    UpdateLEDs();
    //void                    UpdateZoneLEDs(int zone);
    //void                    UpdateSingleLED(int led);

    void                    UpdateMode();
    void                    SaveMode();

    void                    DeviceCallThreadFunction();

    void                    ClearSegments(int zone);
    void                    AddSegment(int zone, segment new_segment);

    /*---------------------------------------------------------*\
    | Functions to be implemented in device implementation      |
    \*---------------------------------------------------------*/
    virtual void            SetupZones()                                = 0;

    virtual void            ResizeZone(int zone, int new_size)          = 0;

    virtual void            DeviceUpdateLEDs()                          = 0;
    virtual void            UpdateZoneLEDs(int zone)                    = 0;
    virtual void            UpdateSingleLED(int led)                    = 0;

    virtual void            DeviceUpdateMode()                          = 0;
    void                    DeviceSaveMode();

    void                    SetCustomMode();

private:
    std::thread*            DeviceCallThread;
    std::atomic<bool>       CallFlag_UpdateLEDs;
    std::atomic<bool>       CallFlag_UpdateMode;
    std::atomic<bool>       DeviceThreadRunning;
    //bool                    CallFlag_UpdateZoneLEDs                     = false;
    //bool                    CallFlag_UpdateSingleLED                    = false;
    //bool                    CallFlag_UpdateMode                         = false;

    std::mutex                          UpdateMutex;
    std::vector<RGBControllerCallback>  UpdateCallbacks;
    std::vector<void *>                 UpdateCallbackArgs;
};
