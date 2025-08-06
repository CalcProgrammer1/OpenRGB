/*---------------------------------------------------------*\
| CougarKeyboardController.cpp                              |
|                                                           |
|   Driver for Cougar keyboard                              |
|                                                           |
|   Chris M (DrNo)                              05 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "CougarKeyboardController.h"
#include "StringUtils.h"

using namespace std::chrono_literals;

static uint8_t keyvalue_map[113] =
{
/*00        ESC  F1   F2   F3   F4   F5   F6   F7   F8   F9  */
             0,   9,  18,  27,  36,  45,  54,  63,  72,  81,

/*10        F10  F11  F12  PRT  SLK  PBK  PLY  VDN  VUP  MTE */
            90,  99, 108, 117, 126,  15, 135, 140, 141, 142,

/*20         `    1    2    3    4    5    6    7    8    9  */
             1,  10,  19,  28,  37,  46,  55,  64,  73,  82,

/*30         0    -    =   BSP  INS  HME  PUP  NLK  NM/  NM* */
            91, 100, 109, 127, 128,  24,  33,  42,  51,  60,

/*40        NM-  TAB   Q    W    E    R    T    Y    U    I  */
            69,   2,  11,  20,  29,  38,  47,  56,  65,  74,

/*50         O    P    [    ]    \   DEL  END  PDN  NM7  NM8 */
            83,  92, 101, 110, 119, 129,  78,  87,  96, 105,

/*60        NM9  NM+  CAP   A    S    D    F    G    H    J  */
            16,  25,   3,  12,  21,  30,  39,  48,  57,  66,

/*70         K    L    ;    '   ENT  NM4  NM5  NM6  LSH   Z  */
            75,  84,  93, 102, 120,  34,  43,  52,   4,  22,

/*80         X    C    V    B    N    M    ,    .    /   RSH */
            31,  40,  49,  58,  67,  76,  85,  94, 103, 121,

/*90         UP  NM1  NM2  NM3 NETR LCTL LWIN LALT SPC  RALT */
           130,  70,  79,  88, 106,   5,  14,  23,  50,  86,

/*100      RWIN RFNC RCTL  LFT  DWN  RGT  NM0  NM.  G1   G2  */
            95, 104, 113, 122, 131, 132, 133,  97, 147, 148,

/*110       G3   G4   G5                                     */
           149, 150, 151
};

CougarKeyboardController::CougarKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    name                = dev_name;
}

CougarKeyboardController::~CougarKeyboardController()
{
    hid_close(dev);
}

std::string CougarKeyboardController::GetDeviceName()
{
    return(name);
}

std::string CougarKeyboardController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string CougarKeyboardController::GetLocation()
{
    return("HID: " + location);
}

void CougarKeyboardController::SetMode(uint8_t mode, uint8_t speed, uint8_t brightness, uint8_t direction, std::vector<RGBColor> colours, bool random_colours)
{
    uint8_t buffer[COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE]  = { 0x00, 0x14, 0x2C };

    buffer[COUGARKEYBOARDCONTROLLER_MODE_BYTE]                  = mode;
    buffer[COUGARKEYBOARDCONTROLLER_SPEED_BYTE]                 = speed;
    buffer[COUGARKEYBOARDCONTROLLER_BRIGHTNESS_BYTE]            = brightness;
    buffer[COUGARKEYBOARDCONTROLLER_RANDOM_BYTE]                = (random_colours) ? 1 : 0;
    buffer[COUGARKEYBOARDCONTROLLER_DIRECTION_BYTE]             = direction;

    switch(mode)
    {
        /*-----------------------------------------------------*\
        | Off mode does not need any further settings & should  |
        |    skip the default case to avoid an indexing error   |
        \*-----------------------------------------------------*/
        case COUGARKEYBOARDCONTROLLER_MODE_OFF:
            break;

        /*-----------------------------------------------------*\
        | Spectrum Cycle mode (Circle) always sets random       |
        \*-----------------------------------------------------*/
        case COUGARKEYBOARDCONTROLLER_MODE_CIRCLE:
            buffer[COUGARKEYBOARDCONTROLLER_RANDOM_BYTE]        = 1;
            break;

        /*-----------------------------------------------------*\
        | Wave mode does not have a true "random" colour and    |
        |   needs to set the "rainbow" mode (val = 2) instead   |
        \*-----------------------------------------------------*/
        case COUGARKEYBOARDCONTROLLER_MODE_WAVE:
            buffer[COUGARKEYBOARDCONTROLLER_RANDOM_BYTE]        = (random_colours) ? 2 : 0;
            buffer[COUGARKEYBOARDCONTROLLER_DATA_BYTE]          = 2;

        case COUGARKEYBOARDCONTROLLER_MODE_RIPPLE:
        case COUGARKEYBOARDCONTROLLER_MODE_SCAN:
            {
                uint8_t count                                   = (uint8_t)colours.size();
                uint8_t timer                                   = 100 / count;
                buffer[COUGARKEYBOARDCONTROLLER_DATA_BYTE + 1]  = count;

                for(uint8_t i = 0; i < count; i++)
                {
                    uint8_t offset                              = 11 + (i * 4);

                    buffer[offset]                              = (i + 1) * timer;
                    buffer[offset + 1]                          = RGBGetRValue(colours[i]);
                    buffer[offset + 2]                          = RGBGetGValue(colours[i]);
                    buffer[offset + 3]                          = RGBGetBValue(colours[i]);
                }
            }
            break;

        case COUGARKEYBOARDCONTROLLER_MODE_RHYTHM:
            buffer[COUGARKEYBOARDCONTROLLER_RANDOM_BYTE]        = (random_colours) ? 2 : 0;

        default:
            buffer[COUGARKEYBOARDCONTROLLER_DATA_BYTE + 1]      = RGBGetRValue(colours[0]);
            buffer[COUGARKEYBOARDCONTROLLER_DATA_BYTE + 2]      = RGBGetGValue(colours[0]);
            buffer[COUGARKEYBOARDCONTROLLER_DATA_BYTE + 3]      = RGBGetBValue(colours[0]);
            break;
    }

    hid_write(dev, buffer, COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE);
}

void  CougarKeyboardController::SetLedsDirect(std::vector<RGBColor> colours)
{
    uint8_t max_leds                                            = 14;
    uint8_t leds_remaining                                      = (uint8_t)colours.size();
    uint8_t packet_flag                                         = COUGARKEYBOARDCONTROLLER_DIRECTION_BYTE;
    uint8_t buffer[COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE]  = { 0x00, 0x14, 0x2C, 0x0B, 0x00, 0xFF, 0x64, 0x00, 0x01 };

    /*-----------------------------------------------------------------*\
    | Set up Direct packet                                              |
    |   keyvalue_map is the index of the Key from full_matrix_map       |
    \*-----------------------------------------------------------------*/
    for(uint8_t leds2send = 0; leds2send < leds_remaining; leds2send += max_leds)
    {
        /*-----------------------------------------------------------------*\
        | Check if there is enough leds for another pass                    |
        \*-----------------------------------------------------------------*/
        if(leds2send + max_leds > leds_remaining)
        {
            max_leds            = leds_remaining - leds2send;

            /*-----------------------------------------------------------------*\
            | The last packet flag should be 0x03                               |
            \*-----------------------------------------------------------------*/
            buffer[packet_flag] = 3;
        }

        for(uint8_t i = 0; i < max_leds; i++)
        {
            uint8_t offset      = COUGARKEYBOARDCONTROLLER_DATA_BYTE + (i * 4);
            uint8_t led_num     = leds2send + i;

            buffer[offset]      = keyvalue_map[led_num];
            buffer[offset + 1]  = RGBGetRValue(colours[led_num]);
            buffer[offset + 2]  = RGBGetGValue(colours[led_num]);
            buffer[offset + 3]  = RGBGetBValue(colours[led_num]);
        }
        hid_write(dev, buffer, COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE);
        std::this_thread::sleep_for(1ms);

        /*-----------------------------------------------------------------*\
        | After the first packet the packet flag should be 0x02             |
        \*-----------------------------------------------------------------*/
        buffer[packet_flag]     = 2;
    }
}

void CougarKeyboardController::Save(uint8_t flag)
{
    uint8_t buffer[COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE]   = { 0x00, 0x12, flag, 0x00, 0x00 };

    hid_write(dev, buffer, COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE);
}

void CougarKeyboardController::SendProfile(uint8_t profile, uint8_t light)
{
    uint8_t buffer[COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE]   = { 0x00, 0x14, 0x00, 0x00, 0x00, profile, light, 0x00, 0x00};

    hid_write(dev, buffer, COUGARKEYBOARDCONTROLLER_WRITE_PACKET_SIZE);
}
