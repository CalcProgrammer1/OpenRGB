/*---------------------------------------------------------*\
| EVisionV2KeyboardController.h                             |
|                                                           |
|   Driver for EVision V2 keyboard                          |
|                                                           |
|   Le Philousophe                              25 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define EVISION_V2_PACKET_SIZE                       64
#define HID_MAX_STR                                 255

#define EVISION_V2_REPORT_ID                          4

#define EVISION_V2_MATRIX_HEIGHT                      6
#define EVISION_V2_MATRIX_WIDTH                      21

enum
{
    EVISION_V2_MODE_COLOR_WAVE_SHORT                = 0x01, /* "Go with the stream"     */
    EVISION_V2_MODE_COLOR_WAVE_LONG                 = 0x02, /* "Clouds fly"             */
    EVISION_V2_MODE_COLOR_WHEEL                     = 0x03, /* "Winding paths"          */
    EVISION_V2_MODE_SPECTRUM_CYCLE                  = 0x04, /* "Spectrum"               */
    EVISION_V2_MODE_BREATHING                       = 0x05, /* "Breath"                 */
    EVISION_V2_MODE_STATIC                          = 0x06, /* "Normal"                 */
    EVISION_V2_MODE_REACTIVE                        = 0x07, /* "Pass without trace"     */
    EVISION_V2_MODE_REACTIVE_RIPPLE                 = 0x08, /* "Ripples"                */
    EVISION_V2_MODE_REACTIVE_LINE                   = 0x09, /* "Stream"                 */
    EVISION_V2_MODE_STARLIGHT_FAST                  = 0x0A, /* "Stars"                  */
    EVISION_V2_MODE_BLOOMING                        = 0x0B, /* "Flowers"                */
    EVISION_V2_MODE_RAINBOW_WAVE_VERTICAL           = 0x0C, /* "Swift action"           */
    EVISION_V2_MODE_HURRICANE                       = 0x0D, /* "Hurricane"              */
    EVISION_V2_MODE_ACCUMULATE                      = 0x0E, /* "Cartoon"                */
    EVISION_V2_MODE_STARLIGHT_SLOW                  = 0x0F, /* "Digital times"          */
    EVISION_V2_MODE_VISOR                           = 0x10, /* "Both ways"              */
    EVISION_V2_MODE_SURMOUNT                        = 0x11, /* "Surmount"               */
    EVISION_V2_MODE_RAINBOW_WAVE_CIRCLE             = 0x12, /* "Speed"                  */
    EVISION_V2_MODE_CUSTOM                          = 0x14, /* "Custom"                 */

    EVISION_V2_MODE_DIRECT                          = 0xFF, /* Software controlled mode */
};

enum
{
    EVISION_V2_MODE2_COLOR_WAVE                      = 0x00, /* "Pulsation"              */
    EVISION_V2_MODE2_BREATHING                       = 0x01, /* "Breath"                 */
    EVISION_V2_MODE2_YOYO                            = 0x02, /* "Yoyo"                   */
    EVISION_V2_MODE2_BLINK                           = 0x03, /* "Blink"                  */
    EVISION_V2_MODE2_STATIC                          = 0x04, /* "Normal"                 */
    EVISION_V2_MODE2_OFF                             = 0x05, /* "Off"                    */
};

enum
{
    /* Official software doesn't support changing edges */
    ENDORFY_MODE2_FREEZE                          = 0x00,
    ENDORFY_MODE2_COLOR_WAVE                      = 0x01,
    ENDORFY_MODE2_SPECTRUM_CYCLE                  = 0x02,
    ENDORFY_MODE2_BREATHING                       = 0x03,
    ENDORFY_MODE2_STATIC                          = 0x04,
    ENDORFY_MODE2_OFF                             = 0x05,
};

enum
{
    EVISION_V2_CMD_BEGIN_CONFIGURE                  = 0x01,
    EVISION_V2_CMD_END_CONFIGURE                    = 0x02,
    EVISION_V2_CMD_READ_CAPABILITIES                = 0x03,
    //EVISION_V2_CMD_WRITE_CAPABILITIES               = 0x04,
    EVISION_V2_CMD_READ_CONFIG                      = 0x05,
    EVISION_V2_CMD_WRITE_CONFIG                     = 0x06,
    EVISION_V2_CMD_READ_CUSTOM_COLORS               = 0x0A,
    EVISION_V2_CMD_WRITE_CUSTOM_COLORS              = 0x0B,
    EVISION_V2_CMD_SEND_DYNAMIC_COLORS              = 0x12,
    EVISION_V2_CMD_END_DYNAMIC_COLORS               = 0x13,
};

enum
{
    EVISION_V2_OFFSET_CURRENT_PROFILE               = 0x00,
    EVISION_V2_OFFSET_FIRST_PROFILE                 = 0x01,
    EVISION_V2_OFFSET_SECOND_PROFILE                = 0x41,
    EVISION_V2_OFFSET_THIRD_PROFILE                 = 0x81,
};

enum
{
    EVISION_V2_PARAMETER_MODE                       = 0x00, /* Mode parameter               */
    EVISION_V2_PARAMETER_BRIGHTNESS                 = 0x01, /* Brightness parameter         */
    EVISION_V2_PARAMETER_SPEED                      = 0x02, /* Speed parameter              */
    EVISION_V2_PARAMETER_DIRECTION                  = 0x03, /* Direction parameter          */
    EVISION_V2_PARAMETER_RANDOM_COLOR_FLAG          = 0x04, /* Random color parameter       */
    EVISION_V2_PARAMETER_MODE_COLOR                 = 0x05, /* Mode color (RGB)             */
    EVISION_V2_PARAMETER_COLOR_OFFSET               = 0x08, /* Unknown color offset         */
    EVISION_V2_PARAMETER_LED_MODE_COLOR             = 0x11, /* Led mode color               */
    EVISION_V2_PARAMETER_CURRENT_CUSTOM_MODE        = 0x19, /* Custom mode current colorset */
    EVISION_V2_PARAMETER_LOGO                       = 0x1a, /* Logo parameters              */
    EVISION_V2_PARAMETER_LOGO_ON_OFF                = 0x23, /* Logo on/off                  */
    EVISION_V2_PARAMETER_EDGE                       = 0x24, /* Edge parameters              */
    EVISION_V2_PARAMETER_END                        = 0x2b, /* Address after last parameter */
};

enum
{
    EVISION_V2_BRIGHTNESS_LOWEST                    = 0x00, /* Lowest brightness (off)  */
    EVISION_V2_BRIGHTNESS_HIGHEST                   = 0x04, /* Highest brightness       */
};

enum
{
    EVISION_V2_SPEED_SLOWEST                      = 0x05, /* Slowest speed setting    */
    EVISION_V2_SPEED_NORMAL                       = 0x03, /* Normal speed setting     */
    EVISION_V2_SPEED_FASTEST                      = 0x00, /* Fastest speed setting    */
};

enum
{
    EVISION_V2_SURMOUNT_MODE_COLOR_RED            = 0x00, /* Red surmount color       */
    EVISION_V2_SURMOUNT_MODE_COLOR_YELLOW         = 0x01, /* Yellow surmount color    */
    EVISION_V2_SURMOUNT_MODE_COLOR_GREEN          = 0x02, /* Green surmount color     */
    EVISION_V2_SURMOUNT_MODE_COLOR_CYAN           = 0x03, /* Cyan surmount color      */
};

enum EVisionV2KeyboardLayout
{
    EVISION_V2_KEYBOARD_LAYOUT,
    ENDORFY_KEYBOARD_LAYOUT,
};

enum EVisionV2KeyboardPart
{
    EVISION_V2_KEYBOARD_PART_KEYBOARD,
    EVISION_V2_KEYBOARD_PART_LOGO,
    EVISION_V2_KEYBOARD_PART_EDGE,
    ENDORFY_KEYBOARD_PART_EDGE,
};

struct EvisionV2ModeConfig
{
    uint8_t mode;
    uint8_t brightness;
    uint8_t speed;
    uint8_t direction;
    bool random_colours;
    RGBColor colour;
    uint8_t ledmode;
};

class EVisionV2KeyboardController
{
public:
    EVisionV2KeyboardController(hid_device* dev_handle, const char* path, EVisionV2KeyboardLayout dev_layout, std::string dev_name);
    ~EVisionV2KeyboardController();

    std::string GetName();
    std::string GetSerial();
    std::string GetLocation();

    int  Query(uint8_t cmd, uint16_t offset = 0, const uint8_t* idata = nullptr, uint8_t size = 0, uint8_t* odata = nullptr);
    int  BeginConfigure();
    int  EndConfigure();
    int  Read(uint8_t cmd, uint16_t offset, uint16_t size, uint8_t* odata);
    int  Write(uint8_t cmd, uint16_t offset, const uint8_t* idata, uint16_t size);

    int  GetMode(EVisionV2KeyboardPart part, EvisionV2ModeConfig& config);
    void SetMode(EVisionV2KeyboardPart part, const EvisionV2ModeConfig& config);
    void SetLedsDirect(const std::vector<RGBColor>& colours);
    void SetLedDirect(int led, RGBColor colours);
    void RefreshLedDirect();
    void EndLedsDirect();
    int  GetLedsCustom(uint8_t colorset, std::vector<RGBColor>& colours);
    void SetLedsCustom(uint8_t colorset, const std::vector<RGBColor>& colours);

    EVisionV2KeyboardLayout  layout;

private:
    std::string name;
    std::string serial;
    std::string location;
    hid_device* dev;

    size_t      map_size;
    size_t      macros_size;

    uint8_t *   keyvalue_map;
    size_t      led_count;

    std::mutex  query_mutex;
};
