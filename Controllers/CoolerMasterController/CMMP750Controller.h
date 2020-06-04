/*-------------------------------------------------------------------*\
|  CMMP750Controller.h                                                |
|                                                                     |
|  Driver for Coolermaster MP750 mousepad                             |
|                                                                     |
|  Chris M (Dr_No)          16th Apr 2020                             |
|                                                                     |
|  Simple RGB device with 5 modes                                     |
|  BYTE0 = Mode (0x01 thru 0x05                                       |
|  BYTE1 = ?? Must be set to 0x04 for colour modes otherwise ignored  |
|  BYTE2 = Colour Modes: RED    else Cycle SPEED                      |
|  BYTE3 = Colour Modes: GREEN  else ignored                          |
|  BYTE4 = Colour Modes: BLUE   else ignored                          |
|  BYTE5 = Colour Modes: SPEED  else ignored                          |
\*-------------------------------------------------------------------*/

#include <string>
#include <array>
#include <hidapi/hidapi.h>

#pragma once

#define CM_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define CM_INTERRUPT_TIMEOUT 250
#define CM_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define CM_SERIAL_SIZE (sizeof(serial) / sizeof(serial[ 0 ]))
#define HID_MAX_STR 255

enum
{
    CM_MODE_BYTE   = 0,
    CM_RED_BYTE    = 2,
    CM_GREEN_BYTE  = 3,
    CM_BLUE_BYTE   = 4,
    CM_SPEED_BYTE  = 5
};

enum
{
    MP750_MODE_STATIC       = 0x00, //Static Mode
    MP750_MODE_BLINK        = 0x01, //Blinking Mode
    MP750_MODE_BREATHING    = 0x02, //Breathing Mode
    MP750_MODE_COLOR_CYCLE  = 0x03, //Color Cycle Mode
    MP750_MODE_BREATH_CYCLE = 0x04  //Breathing Cycle Mode
};

static unsigned char colour_mode_data[][6] =
{
    { 0x01, 0x04, 0xFF, 0x00, 0xFF, 0x00 },                  /* Static                       */
    { 0x02, 0x04, 0xFF, 0x00, 0xFF, 0x80 },                  /* Blinking                     */
    { 0x03, 0x04, 0xFF, 0x00, 0xFF, 0x80 },                  /* Breathing                    */
    { 0x04, 0x04, 0x80, 0x00, 0x00, 0x00 },                  /* Colour Cycle                 */
    { 0x05, 0x04, 0x80, 0x00, 0x00, 0x00 }                   /* Colour Breath                */
};

static unsigned char speed_mode_data[][9] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* Static                       */
    { 0xFF, 0xE0, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x00 },/* Blinking                     */
    { 0xFF, 0xE0, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x00 },/* Breathing                    */
    { 0xFF, 0xE0, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x00 },/* Colour Cycle                 */
    { 0xFF, 0xE0, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x00 } /* Colour Breath                */
};

enum
{
    MP750_SPEED_SLOWEST              = 0x00, /* Slowest speed                */
    MP750_SPEED_SLOWER               = 0x01, /* Slower speed                 */
    MP750_SPEED_SLOW                 = 0x02, /* Slow speed                   */
    MP750_SPEED_SLOWISH              = 0x03, /* Slowish speed                */
    MP750_SPEED_NORMAL               = 0x04, /* Normal speed                 */
    MP750_SPEED_FASTISH              = 0x05, /* Fastish speed                */
    MP750_SPEED_FAST                 = 0x06, /* Fast speed                   */
    MP750_SPEED_FASTER               = 0x07, /* Faster speed                 */
    MP750_SPEED_FASTEST              = 0x08, /* Fastest speed                */
};

class CMMP750Controller
{
public:
    CMMP750Controller(hid_device* dev_handle, wchar_t *_vendor, wchar_t *_device_name, char *_path);
    ~CMMP750Controller();

    char* GetDeviceName();
    char* GetSerial();
    std::string GetLocation();

    unsigned char GetMode();
    unsigned char GetLedRed();
    unsigned char GetLedGreen();
    unsigned char GetLedBlue();
    unsigned char GetLedSpeed();
    void SetMode(unsigned char mode, unsigned char speed);
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    char                    device_name[32];
    char                    serial[32];
    std::string             location;
    hid_device*             dev;

    unsigned char           current_mode;
    unsigned char           current_speed;

    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    void GetStatus();
    void SendUpdate();
};
