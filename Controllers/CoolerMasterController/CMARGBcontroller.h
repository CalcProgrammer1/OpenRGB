/*-------------------------------------------------------------------*\
|  CMARGBController.h                                                 |
|                                                                     |
|  Driver for Coolermaster ARGB USB Controller                        |
|                                                                     |
|  Chris M (Dr_No)          10th Oct 2020                             |
|                                                                     |
|  Simple RGB device with 5 modes                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <string>
#include <array>
#include <cmath>            //Needed by round()
#include <hidapi/hidapi.h>
#include "RGBController.h"  //Needed to set the direct mode

#pragma once

#define CM_ARGB_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define CM_ARGB_HEADER_DATA_SIZE (sizeof(argb_header_data) / sizeof(argb_headers) )
#define CM_ARGB_INTERRUPT_TIMEOUT 250
#define CM_ARGB_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define CM_RGB_OFFSET -2
#define HID_MAX_STR 255

enum
{
    CM_ARGB_REPORT_BYTE         = 1,
    CM_ARGB_COMMAND_BYTE        = 2,
    CM_ARGB_FUNCTION_BYTE       = 3,
    CM_ARGB_ZONE_BYTE           = 4,
    CM_ARGB_MODE_BYTE           = 5,
    CM_ARGB_COLOUR_INDEX_BYTE	= 6,
    CM_ARGB_SPEED_BYTE          = 7
};

struct argb_headers
{
    const char*     name;
    unsigned char   header;
    bool            digital;
    unsigned int    count;
};

static argb_headers argb_header_data[6] =
{
    { "RGB Header",         0xFF, false,  1 },
    { "Digital ARGB1",      0x01, true,  12 },
    { "Digital ARGB2",      0x02, true,  12 },
    { "Digital ARGB3",      0x04, true,  12 },
    { "Digital ARGB4",      0x08, true,  12 },
    { "All Digital ARGB",   0xFF, true,  12 }
};

enum
{
    CM_RGB_MODE_OFF             = 0,    //Turn off
    CM_RGB_MODE_COLOUR_CYCLE    = 1,    //Colour Cycle
    CM_RGB_MODE_FLASH           = 2,    //Flash
    CM_RGB_MODE_BREATHING       = 3,    //Breathing
    CM_RGB_MODE_PASSTHRU        = 4     //Motherboard Pass Thru Mode
};

enum
{
    CM_ARGB_MODE_OFF            = 0,    //Turn off
    CM_ARGB_MODE_SPECTRUM       = 1,    //Spectrum Mode
    CM_ARGB_MODE_RELOAD         = 2,    //Reload Mode
    CM_ARGB_MODE_RECOIL         = 3,    //Recoil Mode
    CM_ARGB_MODE_BREATHING      = 4,    //Breathing Mode
    CM_ARGB_MODE_REFILL         = 5,    //Refill Mode
    CM_ARGB_MODE_DEMO           = 6,    //Demo Mode
    CM_ARGB_MODE_FILLFLOW       = 7,    //Fill Flow Mode
    CM_ARGB_MODE_RAINBOW        = 8,    //Rainbow Mode
    CM_ARGB_MODE_DIRECT         = 0xFE, //Direct Led Control
    CM_ARGB_MODE_PASSTHRU       = 0xFF  //Motherboard Pass Thru Mode
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
    CMARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx);
    ~CMARGBController();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    unsigned char GetZoneIndex();
    unsigned char GetMode();
    unsigned char GetLedRed();
    unsigned char GetLedGreen();
    unsigned char GetLedBlue();
    unsigned char GetLedSpeed();
    void SetMode(unsigned char mode, unsigned char speed);
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void SetLedsDirect(RGBColor * led_colours, unsigned int led_count);
private:
    std::string             device_name;
    std::string             serial;
    std::string             location;
    hid_device*             dev;

    unsigned char           zone_index;
    unsigned char           current_mode;
    unsigned char           current_speed;

    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    unsigned int GetLargestColour(unsigned int red, unsigned int green, unsigned int blue);
    unsigned char GetColourIndex(unsigned char red, unsigned char green, unsigned char blue);
    void GetStatus();
    void SendUpdate();

};
