/*-------------------------------------------------------------------*\
|  XPGSpectrixS40GController.h                                        |
|                                                                     |
|  Driver for XPG's Spectrix S40G NVMe                                |
|                                                                     |
|  NicolasNewman          25th Mar 2021                               |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <hidapi/hidapi.h>
#include <stdint.h>

#pragma once

#define XPG_SPECTRIX_LED_COUNT   ( 8 )
#define BIG_PACKET_SIZE ( 236 )
#define SMALL_PACKET_SIZE ( 216 )

#ifdef _WIN32
    #include <windows.h>
    #include <fileapi.h>
#else

#endif

enum
{
    XPG_SPECTRIX_S40G_MODE_STATIC                  = 0x01,
    XPG_SPECTRIX_S40G_MODE_BREATHING               = 0x02,
    XPG_SPECTRIX_S40G_MODE_STROBING                = 0x03,
    XPG_SPECTRIX_S40G_MODE_CYCLE                   = 0x04,
    XPG_SPECTRIX_S40G_MODE_RAINBOW                 = 0x05,
    XPG_SPECTRIX_S40G_MODE_BREATHING_RAINBOW       = 0x06,
    XPG_SPECTRIX_S40G_MODE_COMET                   = 0x07,
    XPG_SPECTRIX_S40G_MODE_COMET_RAINBOW           = 0x08,
    XPG_SPECTRIX_S40G_MODE_FLASH                   = 0x09,
    XPG_SPECTRIX_S40G_MODE_FLASH_RAINBOW           = 0x10,
    XPG_SPECTRIX_S40G_MODE_WAVE                    = 0x11,
    XPG_SPECTRIX_S40G_MODE_YOYO                    = 0x12,
    XPG_SPECTRIX_S40G_MODE_STARRY_NIGHT            = 0x13,
};

enum
{
    XPG_SPECTRIX_S40G_SPEED_SLOWEST           = 0x00, // 0xfe
    XPG_SPECTRIX_S40G_SPEED_SLOW              = 0x01, // 0xff
    XPG_SPECTRIX_S40G_SPEED_NORMAL            = 0x02, // 0x00
    XPG_SPECTRIX_S40G_SPEED_FAST              = 0x03, // 0x01
    XPG_SPECTRIX_S40G_SPEED_FASTEST           = 0x04, // 0x02
};

enum
{
    XPG_SPECTRIX_S40G_DIRECTION_DEFAULT = 0x0,
    XPG_SPECTRIX_S40G_DIRECTION_REVERSE = 0x1,
};

class XPGSpectrixS40GController
{
    public:
        XPGSpectrixS40GController();
        ~XPGSpectrixS40GController();

        #ifdef _WIN32
            /*-----------------------------------------------------*\
            | Windows specific function that allows the devices     |
            | handle to be passed from elsewhere once detected      |
            \*-----------------------------------------------------*/
            int SetHandle(wchar_t dev_name[MAX_PATH]);
        #else

        #endif

        unsigned char GetEffect();
        void SetEffect(unsigned char mode,
                        unsigned char speed,
                        unsigned char direction,
                        uint8_t mode_colors[]
                        );
        void SetMode(unsigned char mode);
        void SetSpeed(unsigned char speed);
        void SetDirection(unsigned char direction);

        unsigned int GetLEDCount();
        void SetAllColors(uint8_t red, uint8_t green, uint8_t blue);
        void SetLEDColor(unsigned int led, uint8_t red, uint8_t green, uint8_t blue);
        void ApplyColors();

    private:
        #ifdef _WIN32
            HANDLE                  hDevice;
        #else

        #endif
        unsigned int            led_count;
        unsigned char           active_mode;
        unsigned char           active_direction;
        unsigned char           active_speed;
        
        /*-----------------------------------------------------------------------------------*\
        | packet_[1-3] contain the data needed to be sent to the device, in sequential order. |
        | These packets should initially contain the data in packet_[1-3]_template, which can |
        | then have its bytes corresponding to color, speed, and direction updated via the    |
        | setter functions. Once the packets are ready, they can be sent via ApplyColors.     |
        \*-----------------------------------------------------------------------------------*/
        // pointer containing the first packet to be sent to the device
        uint32_t*               packet_one;
        // pointer containing the second packet to be sent to the device
        uint32_t*               packet_two;
        // pointer containing the third packet to be sent to the device
        uint32_t*               packet_three;
        const uint32_t          packet_one_template[59] = {0x00000001, 0x00000054, 0x00000003, 0x80000000, 0x00000000, 0x00000000, 0x00000040, 0x00000040, 0x00000018, 0x00000000, 
                                                            0x00000001, 0x00000090, 0x000000D0, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x003000FB, 0x00000031, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x00000006, 0x00000000, 0x608100CE, 0x03100018, 0x51987856, 0x80000B00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x00000000, 0x00000000,
                                                            0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, // COLORS
                                                            0x00000000};

        const uint32_t          packet_two_template[54] = {0x00000001, 0x00000054, 0x00000003, 0x80000000, 0x00000000, 0x00000000, 0x00000040, 0x00000040, 0x00000004, 0x00000000, 
                                                            0x00000001, 0x00000090, 0x000000D0, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x003000FB, 0x00000031, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x00000001, 0x00000000, 0x208000CE, 0x03100004, 0x51987856, 0x80000B00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                            0x00000000, 0x00000000, 0x00000100, 0x00000000};

        const uint32_t          packet_three_template[54] = {0x00000001, 0x00000054, 0x00000003, 0x80000000, 0x00000000, 0x00000000, 0x00000040, 0x00000040, 0x00000004, 0x00000000, 
                                                                0x00000001, 0x00000090, 0x000000D0, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                                0x003000FB, 0x00000031, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                                0x00000001, 0x00000000, 0x2F8000CE, 0x01100001, 0x51987856, 0x80000B00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                                0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                                                0x00000000, 0x00000000, 0x00000001, 0x00000000};
        
        void InitializePackets();
};