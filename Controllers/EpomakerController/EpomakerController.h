/*-------------------------------------------------------------------*\
|  EpomakerController.h                                               |
|                                                                     |
|  Driver for Epomaker Keyboard                                       |
|                                                                     |
|  Alvaro Munoz (alvaromunoz)          2023-06-05                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#ifndef EpomakerCONTROLLER_H
#define EpomakerCONTROLLER_H

#include <string>
#include <hidapi/hidapi.h>

#define Epomaker_PACKET_LENGTH 0x40
#define Epomaker_COMMAND_RGB   0x07
#define Epomaker_COMMAND_SET   0xf60a
#define Epomaker_COMMAND_PING  0xf7

enum
{
    Epomaker_BYTE_COMMAND                           = 0,
    Epomaker_BYTE_MODE                              = 1,
    Epomaker_BYTE_SPEED                             = 2,
    Epomaker_BYTE_BRIGHTNESS                        = 3,
    Epomaker_BYTE_FLAGS                             = 4,
    Epomaker_BYTE_RED                               = 5,
    Epomaker_BYTE_GREEN                             = 6,
    Epomaker_BYTE_BLUE                              = 7,
    Epomaker_BYTE_FILLER                            = 8
};

enum
{
    Epomaker_MODE_ALWAYS_ON                         = 0x01,
    Epomaker_MODE_DYNAMIC_BREATHING                 = 0x02,
    Epomaker_MODE_SPECTRUM_CYCLE                    = 0x03,
    Epomaker_MODE_DRIFT                             = 0x04,
    Epomaker_MODE_WAVES_RIPPLE                      = 0x05,
    Epomaker_MODE_STARS_TWINKLE                     = 0x06,
    Epomaker_MODE_STEADY_STREAM                     = 0x07,
    Epomaker_MODE_SHADOWING                         = 0x08,
    Epomaker_MODE_PEAKS_RISING_ONE_AFTER_ANOTHER    = 0x09,
    Epomaker_MODE_SINE_WAVE                         = 0x0a,
    Epomaker_MODE_CAISPRING_SURGING                 = 0x0b,
    Epomaker_MODE_FLOWERS_BLOOMING                  = 0x0c,
    Epomaker_MODE_LASER                             = 0x0e,
    Epomaker_MODE_PEAK_TURN                         = 0x0f,
    Epomaker_MODE_INCLINED_RAIN                     = 0x10,
    Epomaker_MODE_SNOW                              = 0x11,
    Epomaker_MODE_METEOR                            = 0x12,
    Epomaker_MODE_THROUGH_THE_SNOW_NON_TRACE        = 0x13,
    Epomaker_MODE_LIGHT_SHADOW                      = 0x15
};

enum
{
    Epomaker_SPEED_MIN                              = 0x00,
    Epomaker_SPEED_MAX                              = 0x05,
    Epomaker_SPEED_MAX_SPECIAL                      = 0x04,
    Epomaker_SPEED_DEFAULT                          = 0x04
};

enum
{
    Epomaker_BRIGHTNESS_MIN                         = 0x00,
    Epomaker_BRIGHTNESS_MAX                         = 0x04,
    Epomaker_BRIGHTNESS_DEFAULT                     = 0x04
};

enum
{
    Epomaker_OPTION_DAZZLE_OFF                      = 0x07,
    Epomaker_OPTION_DAZZLE_ON                       = 0x08,
    Epomaker_OPTION_DEFAULT                         = 0x00,
    Epomaker_OPTION_DRIFT_RIGHT                     = 0X00,
    Epomaker_OPTION_DRIFT_LEFT                      = 0X10,
    Epomaker_OPTION_DRIFT_DOWN                      = 0X20,
    Epomaker_OPTION_DRIFT_UP                        = 0X30,
    Epomaker_OPTION_STEADY_STREAM_ZIG_ZAG           = 0x00,
    Epomaker_OPTION_STEADY_STREAM_RETURN            = 0x10,
    Epomaker_OPTION_CAISPRING_SURGING_OUT           = 0x00,
    Epomaker_OPTION_CAISPRING_SURGING_IN            = 0x10,
    Epomaker_OPTION_FLOWERS_BLOOMING_RIGHT          = 0x00,
    Epomaker_OPTION_FLOWERS_BLOOMING_LEFT           = 0x10,
    Epomaker_OPTION_PEAK_TURN_ANTI_CLOCKWISE        = 0x00,
    Epomaker_OPTION_PEAK_TURN_CLOCKWISE             = 0x10,
};

class EpomakerController
{
public:
    EpomakerController(hid_device* dev_handle, char *_path);
    ~EpomakerController();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    void SetDazzle(bool is_dazzle);
    void SetOption(unsigned char option);

    void SetMode(unsigned char mode, unsigned char speed, unsigned char brightness);
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    std::string             device_name;
    std::string             serial;
    std::string             location;
    hid_device*             dev;

    unsigned char           current_mode;
    unsigned char           current_speed;
    unsigned char           current_brightness;
    unsigned char           current_dazzle;
    unsigned char           current_option;

    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    void SendUpdate();
};

#endif // EpomakerCONTROLLER_H
