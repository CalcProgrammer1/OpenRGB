/*-------------------------------------------------------------------*\
|  CMARGBController.h                                                 |
|                                                                     |
|  Driver for Coolermaster ARGB USB Controller                        |
|                                                                     |
|  Chris M (Dr_No)          10th Oct 2020                             |
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
#include <cmath>            //Needed by round()
#include <hidapi/hidapi.h>

#pragma once

#define CM_ARGB_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define CM_ARGB_INTERRUPT_TIMEOUT 250
#define CM_ARGB_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define CM_ARGB_SERIAL_SIZE (sizeof(serial) / sizeof(serial[ 0 ]))
#define HID_MAX_STR 255

enum
{
    CM_ARGB_REPORT_BYTE         = 0,
    CM_ARGB_COMMAND_BYTE        = 1,
    CM_ARGB_MODE_BYTE           = 2,
    CM_ARGB_ZONE_BYTE           = 3,
    CM_ARGB_COLOUR_INDEX_BYTE	= 5,
    CM_ARGB_SPEED_BYTE          = 6
};

// ARGB Modes
/*
 * 01 Spectrum  (random)
 * 02 Reload
 * 03 Recoil
 * 04 Breathing
 * 05 Refill
 * 06 Demo Mode (random)
 * 08 Fill Flow (random)
 * 09 Rainbow   (random)
 * 0a Turn Off
 */

// RGB Modes
/*
 * 01 Colour Cycle
 * 02 Flash
 * 03 Breathing
 * 04 Motherboard (Pass Thru)
 * 05 Turn Off
 */

enum
{
    CM_ARGB_MODE_OFF            = 1,    //Turn off
    CM_ARGB_MODE_SPECTRUM       = 2,    //Spectrum Mode
    CM_ARGB_MODE_RELOAD         = 3,    //Reload Mode
    CM_ARGB_MODE_RECOIL         = 4,    //Recoil Mode
    CM_ARGB_MODE_BREATHING      = 5,    //Breathing Mode
    CM_ARGB_MODE_REFILL         = 6,    //Refill Mode
    CM_ARGB_MODE_DEMO           = 7,    //Demo Mode
    CM_ARGB_MODE_FILLFLOW       = 8,    //Fill Flow Mode
    CM_ARGB_MODE_RAINBOW        = 9     //Rainbow Mode
};

enum
{
    CM_ARGB_SPEED_SLOWEST       = 0x00, // Slowest speed
    CM_ARGB_SPEED_SLOW          = 0x01, // Slower speed
    CM_ARGB_SPEED_NORMAL        = 0x02, // Normal speed
    CM_ARGB_SPEED_FAST          = 0x03, // Fast speed
    CM_ARGB_SPEED_FASTEST       = 0x04, // Fastest speed
};

class CMARGBController
{
public:
    CMARGBController(hid_device* dev_handle, wchar_t *_vendor, wchar_t *_device_name, char *_path);
    ~CMARGBController();

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

    unsigned char GetLargestColour(unsigned char red, unsigned char green, unsigned char blue);
    unsigned char GetColourIndex(unsigned char red, unsigned char green, unsigned char blue);
    void GetStatus();
    void SendUpdate(unsigned char zone);
};
