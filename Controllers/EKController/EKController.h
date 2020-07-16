/*-------------------------------------------------------------------*\
|  EKController.h                                                     |
|                                                                     |
|  Driver for EK Loop Connect                                         |
|                                                                     |
|  Chris M (Dr_No)          16th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#ifndef EKCONTROLLER_H
#define EKCONTROLLER_H

#include <string>
#include <hidapi/hidapi.h>

#define EK_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define EK_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define EK_PACKET_LENGTH    0x3F

enum
{
    EK_MODE_BYTE   = 12,
    EK_SPEED_BYTE  = 14,
    EK_RED_BYTE    = 16,
    EK_GREEN_BYTE  = 17,
    EK_BLUE_BYTE   = 18
};

enum
{
    EK_MODE_STATIC              = 0x00,                         //Static Mode
    EK_MODE_BREATHING           = 0x01,                         //Breathing Mode
    EK_MODE_FADING              = 0x02,                         //Fading Mode
    EK_MODE_MARQUEE             = 0x03,                         //Marquee Mode
    EK_MODE_COVERING_MARQUEE    = 0x04,                         //Covering Marquee Mode
    EK_MODE_PULSE               = 0x05,                         //Pulse Mode
    EK_MODE_SPECTRUM_WAVE       = 0x06,                         //Spectrum Wave Mode
    EK_MODE_ALTERNATING         = 0x07,                         //Alternating Mode
    EK_MODE_CANDLE              = 0x08                          //Candle Mode
};

static unsigned char colour_mode_data[][16] =
{
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x01, 0x00, 0xFF, 0x64},          // Static
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x02, 0x00, 0xFF, 0x64},          // Breathing
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x03, 0xFF, 0xFF, 0x64},          // Fading
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x04, 0x00, 0xFF, 0x64},          // Marquee
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x05, 0x00, 0xFF, 0x64},          // Covering Marquee
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x06, 0x00, 0xFF, 0x64},          // Pulse
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x07, 0x00, 0xFF, 0x64},          // Wave
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x08, 0x00, 0xFF, 0x64},          // Alternating
    { 0x10, 0x12, 0x29, 0xAA, 0x01, 0x10, 0xA2, 0x60,
      0x00, 0x10, 0x20, 0x01, 0x09, 0x00, 0xFF, 0x64},          // Candle
};

static unsigned char speed_mode_data[][9] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // Static
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 },   // Breathing
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 },   // Fading
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 },   // Marquee
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 },   // Covering Marquee
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 },   // Pulse
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 },   // Wave
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 },   // Alternating
    { 0x00, 0x0C, 0x19, 0x25, 0x32, 0x3E, 0x4B, 0x57, 0x64 }    // Candle
};

enum
{
    EK_SPEED_SLOWEST              = 0x00, // Slowest speed
    EK_SPEED_SLOWER               = 0x01, // Slower speed
    EK_SPEED_SLOW                 = 0x02, // Slow speed
    EK_SPEED_SLOWISH              = 0x03, // Slowish speed
    EK_SPEED_NORMAL               = 0x04, // Normal speed
    EK_SPEED_FASTISH              = 0x05, // Fastish speed
    EK_SPEED_FAST                 = 0x06, // Fast speed
    EK_SPEED_FASTER               = 0x07, // Faster speed
    EK_SPEED_FASTEST              = 0x08, // Fastest speed
};

class EKController
{
public:
    EKController(hid_device* dev_handle, wchar_t *_vendor, wchar_t *_device_name, char *_path);
    ~EKController();

    char* GetDeviceName();
    char* GetSerial();
    std::string GetLocation();

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

    void SendUpdate();
};

#endif // EKCONTROLLER_H
