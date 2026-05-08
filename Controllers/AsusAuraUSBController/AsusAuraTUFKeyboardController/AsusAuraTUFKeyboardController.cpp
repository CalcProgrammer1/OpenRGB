/*---------------------------------------------------------*\
| AsusAuraTUFKeyboardController.cpp                         |
|                                                           |
|   Driver for ASUS Aura TUF keyboard                       |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cmath>
#include <cstring>
#include <math.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "AsusAuraTUFKeyboardController.h"
#include "LogManager.h"
#include "StringUtils.h"

#define HID_MAX_STR 128

AuraTUFKeyboardController::AuraTUFKeyboardController(hid_device* dev_handle, const char* path, uint16_t pid, unsigned short version, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    device_pid  = pid;
    rev_version = version;

    is_per_led_keyboard = (pid != AURA_TUF_K1_GAMING_PID && pid != AURA_TUF_K5_GAMING_PID);
}

AuraTUFKeyboardController::~AuraTUFKeyboardController()
{
    hid_close(dev);
}

std::string AuraTUFKeyboardController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraTUFKeyboardController::GetName()
{
    return(name);
}

std::string AuraTUFKeyboardController::GetSerialString()
{
    wchar_t serial_string[HID_MAX_STR];
    memset(serial_string, 0, sizeof(serial_string));

    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);
    if(ret != 0)
    {
        return("");
    }

    /*-------------------------------------------------------------------------*\
    | Skip non-ASCII, trailing garbage in serial numbers. Required by the       |
    | Scope II 96, whose original firmware outputs garbage, which even differs  |
    | after computer reboots and therefore breaks OpenRGB profile matching.     |
    \*-------------------------------------------------------------------------*/
    switch(device_pid)
    {
        case AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID:
            serial_string[12] = L'\0';
            break;
        default:
            break;
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AuraTUFKeyboardController::GetVersion()
{
    if(device_pid != AURA_ROG_CLAYMORE_PID)
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x12;
        usb_buf[0x02]   = 0x00;

        ClearResponses();

        hid_write(dev, usb_buf, 65);

        unsigned char usb_buf_out[65];
        hid_read(dev, usb_buf_out, 65);

        char version[9];

        switch(device_pid)
        {
            case AURA_ROG_AZOTH_USB_PID:
            case AURA_ROG_AZOTH_2_4_PID:
            case AURA_ROG_AZOTH_X_USB_PID:
            case AURA_TUF_K3_GAMING_PID:
            case AURA_TUF_K3_GAMING_GEN_II_PID:
            case AURA_ROG_STRIX_FLARE_II_ANIMATE_PID:
            case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID:
            case AURA_ROG_STRIX_SCOPE_II_PID:
            case AURA_ROG_STRIX_SCOPE_II_RX_PID:
            case AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID:
            case AURA_ROG_STRIX_SCOPE_II_96_RX_WIRELESS_USB_PID:
                snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[6], usb_buf_out[5], usb_buf_out[4]);
                break;
            case AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID:
                snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[13], usb_buf_out[12], usb_buf_out[11]);
                break;
            default:
                snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[5], usb_buf_out[6], usb_buf_out[7]);
        }

        return std::string(version);
    }
    else
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));
        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x10;
        usb_buf[0x02]   = 0x01;
        usb_buf[0x03]   = 0x00;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = 0x00; // set to 1 to get firmware version of numpad

        ClearResponses();
        hid_write(dev, usb_buf, 65);
        AwaitResponse(20);

        unsigned char usb_buf1[65];
        memset(usb_buf1, 0x00, sizeof(usb_buf1));
        usb_buf1[0x00]   = 0x00;
        usb_buf1[0x01]   = 0x12;
        usb_buf1[0x02]   = 0x22;

        ClearResponses();
        hid_write(dev, usb_buf1, 65);

        unsigned char usb_buf_out[65];
        hid_read(dev, usb_buf_out, 65);

        char version[9];
        snprintf(version, 9, "%02X.%02X.%02X", usb_buf_out[8], usb_buf_out[9], usb_buf_out[10]);

        return std::string(version);
    }
}

int AuraTUFKeyboardController::GetLayout()
{
    if(device_pid != AURA_ROG_CLAYMORE_PID)
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));

        usb_buf[0x00]   = 0x00;
        usb_buf[0x01]   = 0x12;
        usb_buf[0x02]   = 0x12;

        ClearResponses();
        hid_write(dev, usb_buf, 65);

        unsigned char usb_buf_out[65];
        hid_read(dev, usb_buf_out, 65);

        return(usb_buf_out[4] * 100 + usb_buf_out[5]);
    }
    else
    {
        switch(rev_version >> 0b1100)
        {
            case 1:
                return 117;
            case 2:
                return 204;
            case 3:
                return 221;
            case 4:
                return 117;
            default:
                return 117;
        }
    }
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
int AuraTUFKeyboardController::GetNumpadLocation()
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x40;
    usb_buf[0x02]   = 0x60;

    ClearResponses();
    hid_write(dev, usb_buf, 65);

    unsigned char usb_buf_out[65];
    hid_read(dev, usb_buf_out, 65);

    return(usb_buf_out[5] * 2 + usb_buf_out[4]);
}

void AuraTUFKeyboardController::SaveMode()
{
    unsigned char usb_save_buf[65];
    memset(usb_save_buf, 0x00, sizeof(usb_save_buf));

    usb_save_buf[0x00]   = 0x00;
    usb_save_buf[0x01]   = 0x50;
    usb_save_buf[0x02]   = 0x55;

    ClearResponses();
    hid_write(dev, usb_save_buf, 65);
    AwaitResponse(60);
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
void AuraTUFKeyboardController::AllowRemoteControl(unsigned char type)
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x41;
    usb_buf[0x02]   = type;

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateSingleLed
    (
    int             led,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    /*-----------------------------------------------------*\
    | Set up message packet for single LED                  |
    \*-----------------------------------------------------*/
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0] = 0x00;
    usb_buf[1] = 0xC0;
    usb_buf[2] = 0x81;
    usb_buf[3] = 0x01;
    usb_buf[4] = 0x00;

    /*-----------------------------------------------------*\
    | Convert LED index                                     |
    \*-----------------------------------------------------*/
    usb_buf[5] = led;
    usb_buf[6] = red;
    usb_buf[7] = green;
    usb_buf[8] = blue;

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateLeds
    (
    std::vector<led_color>    colors
    )
{
    int packets = (int)ceil((float)colors.size() / 15.0f);

    for(int i = 0; i < packets; i++)
    {
        unsigned char usb_buf[65];
        memset(usb_buf, 0x00, sizeof(usb_buf));

        int remaining = (int)colors.size() - i * 15;

        int leds = (remaining > 0x0F) ? 0x0F : remaining;

        usb_buf[0] = 0x00;
        usb_buf[1] = 0xC0;
        usb_buf[2] = 0x81;
        usb_buf[3] = leds;
        usb_buf[4] = 0x00;

        for(int j = 0; j < leds; j++)
        {
            usb_buf[j * 4 + 5] = (is_per_led_keyboard) ? colors[i * 15 + j].value : 0x00;
            usb_buf[j * 4 + 6] = RGBGetRValue(colors[i * 15 + j].color);
            usb_buf[j * 4 + 7] = RGBGetGValue(colors[i * 15 + j].color);
            usb_buf[j * 4 + 8] = RGBGetBValue(colors[i * 15 + j].color);
        }

        ClearResponses();
        hid_write(dev, usb_buf, 65);
        AwaitResponse(20);
    }
}

void AuraTUFKeyboardController::UpdateK1Wave
    (
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;
    usb_buf[0x03]   = 0x03;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = brightness;
    usb_buf[0x07]   = 0x00;
    usb_buf[0x08]   = direction;
    usb_buf[0x09]   = 0x00;

    usb_buf[10] = 5;
    usb_buf[11] = RGBGetRValue(colors[4]);
    usb_buf[12] = RGBGetGValue(colors[4]);
    usb_buf[13] = RGBGetBValue(colors[4]);

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    for(unsigned int i = 0; i < 4; i ++)
    {
        usb_buf[10 + i * 4] = i + 1;
        usb_buf[11 + i * 4] = RGBGetRValue(colors[i]);
        usb_buf[12 + i * 4] = RGBGetGValue(colors[i]);
        usb_buf[13 + i * 4] = RGBGetBValue(colors[i]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateScopeIIRainbowRipple
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           color_mode,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;
    usb_buf[0x03]   = mode;
    usb_buf[0x04]   = 0x02;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = brightness;
    usb_buf[0x07]   = color_mode;
    usb_buf[0x08]   = direction;
    usb_buf[0x09]   = 0x02;
    usb_buf[0x0A]   = (unsigned char)colors.size();

    for(unsigned int i = 0; i < 2; i++)
    {
        if(i >= colors.size())
        {
            continue;
        }

        usb_buf[11 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1));
        usb_buf[12 + i * 4] = RGBGetRValue(colors[i]);
        usb_buf[13 + i * 4] = RGBGetGValue(colors[i]);
        usb_buf[14 + i * 4] = RGBGetBValue(colors[i]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    memset(usb_buf + 4, 0x00, sizeof(usb_buf) - 4);

    usb_buf[0x04] = 0x01;

    for(unsigned int i = 0; i < 4; i++)
    {
        if((i + 2) >= colors.size())
        {
            continue;
        }
        usb_buf[5 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1 + 2));
        usb_buf[6 + i * 4] = RGBGetRValue(colors[i + 2]);
        usb_buf[7 + i * 4] = RGBGetGValue(colors[i + 2]);
        usb_buf[8 + i * 4] = RGBGetBValue(colors[i + 2]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    memset(usb_buf + 4, 0x00, sizeof(usb_buf) - 4);

    usb_buf[0x04] = 0x00;

    for(unsigned int i = 0; i < 1; i ++)
    {
        if((i + 6) >= colors.size())
        {
            continue;
        }

        usb_buf[5 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1 + 6));
        usb_buf[6 + i * 4] = RGBGetRValue(colors[i + 6]);
        usb_buf[7 + i * 4] = RGBGetGValue(colors[i + 6]);
        usb_buf[8 + i * 4] = RGBGetBValue(colors[i + 6]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::UpdateScopeIIQuicksand
    (
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           color_mode,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;
    usb_buf[0x03]   = 0x07;
    usb_buf[0x04]   = 0x01;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = brightness;
    usb_buf[0x07]   = color_mode;
    usb_buf[0x08]   = direction;
    usb_buf[0x09]   = 0x02;

    for(unsigned int i = 0; i < 3; i ++)
    {
        usb_buf[10 + i * 3] = RGBGetRValue(colors[i]);
        usb_buf[11 + i * 3] = RGBGetGValue(colors[i]);
        usb_buf[12 + i * 3] = RGBGetBValue(colors[i]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);

    memset(usb_buf + 4, 0x00, sizeof(usb_buf) - 4);

    usb_buf[0x04] = 0x00;

    for(unsigned int i = 0; i < 3; i ++)
    {
        usb_buf[5 + i * 3] = RGBGetRValue(colors[i+3]);
        usb_buf[6 + i * 3] = RGBGetGValue(colors[i+3]);
        usb_buf[7 + i * 3] = RGBGetBValue(colors[i+3]);
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

/*---------------------------------------------------------------------------*\
| Azoth X 51 2C protocol — named protocol constants.                         |
| Kept in an anonymous namespace so they stay file-local.                    |
\*---------------------------------------------------------------------------*/
namespace
{
    /*-----------------------------------------------------------------------*\
    | [07] color_mode byte values                                            |
    \*-----------------------------------------------------------------------*/
    constexpr uint8_t AZX_CM_SINGLE         = 0x00;
    constexpr uint8_t AZX_CM_DOUBLE         = 0x10;
    constexpr uint8_t AZX_CM_RANDOM         = 0x01;
    constexpr uint8_t AZX_CM_RANDOM_ALT     = 0x11;  // Breathing / Starry Night / Rain Drop

    /*-----------------------------------------------------------------------*\
    | Sub-packet page index byte [04] for multi-packet effects               |
    \*-----------------------------------------------------------------------*/
    constexpr uint8_t AZX_PAGE_PRIMARY      = 0x02;  // main params + first stops
    constexpr uint8_t AZX_PAGE_SECONDARY    = 0x01;  // continued stops
    constexpr uint8_t AZX_PAGE_COMMIT       = 0x00;  // last stop / commit

    /*-----------------------------------------------------------------------*\
    | Wave / Ripple thickness byte [09]                                      |
    \*-----------------------------------------------------------------------*/
    constexpr uint8_t AZX_THICKNESS_THICK   = 0x02;  // hardware default

    /*-----------------------------------------------------------------------*\
    | Gradient stop position bytes                                           |
    \*-----------------------------------------------------------------------*/
    constexpr uint8_t AZX_POS_MAX           = 0x64;  // 100%
    constexpr uint8_t AZX_RIPPLE_LEFT_POS   = 0x07;  // hardware-fixed Ripple left stop

    /*-----------------------------------------------------------------------*\
    | Sentinel speed used for Static and out-of-range mode indices           |
    \*-----------------------------------------------------------------------*/
    constexpr uint8_t AZX_SPEED_NONE        = 0xFF;

    /*-----------------------------------------------------------------------*\
    | OpenRGB color_mode byte semantics passed in by DeviceUpdateMode        |
    \*-----------------------------------------------------------------------*/
    constexpr unsigned char OPENRGB_CM_RANDOM = 1;

    constexpr size_t        AZX_PACKET_SIZE = 65;

    /*-----------------------------------------------------------------------*\
    | Initialise a fresh 65-byte Azoth X multi-packet header in `buf`.       |
    | Zeroes the buffer, then writes the 51 2C command, mode byte and page.  |
    \*-----------------------------------------------------------------------*/
    inline void azx_init_packet(uint8_t* buf, uint8_t mode_byte, uint8_t page)
    {
        memset(buf, 0, AZX_PACKET_SIZE);
        buf[1] = 0x51;
        buf[2] = 0x2C;
        buf[3] = mode_byte;
        buf[4] = page;
    }
}

/*---------------------------------------------------------------------------*\
| Azoth X lighting packet — 51 2C command, single-packet modes.              |
| Fixed fields (cmd, param1/2) are set by default initialisers; the caller   |
| only fills the variable fields (mode, speed, brightness, colour).          |
\*---------------------------------------------------------------------------*/
struct AzothXLightingPkt
{
    uint8_t report_id  = 0x00;
    uint8_t cmd1       = 0x51;
    uint8_t cmd2       = 0x2C;
    uint8_t mode       = 0x00;  // [03]
    uint8_t reserved1  = 0x00;  // [04]
    uint8_t speed      = 0x00;  // [05]
    uint8_t brightness = 0x00;  // [06]
    uint8_t color_mode = AZX_CM_SINGLE;  // [07]
    uint8_t param1     = 0xFF;  // [08] fixed 0xFF
    uint8_t param2     = 0xFF;  // [09] fixed 0xFF
    uint8_t r1         = 0x00;  // [0A] primary colour R
    uint8_t g1         = 0x00;  // [0B] primary colour G
    uint8_t b1         = 0x00;  // [0C] primary colour B
    uint8_t r2         = 0x00;  // [0D] secondary colour R (Double) or blend (Single/Current)
    uint8_t g2         = 0x00;  // [0E] secondary colour G
    uint8_t b2         = 0x00;  // [0F] secondary colour B
    uint8_t bg_r       = 0x00;  // [10] background R (Current / Raindrop; 0=OFF)
    uint8_t bg_g       = 0x00;  // [11] background G
    uint8_t bg_b       = 0x00;  // [12] background B
    uint8_t padding[46] = {};   // [13-40] zeros
};
static_assert(sizeof(AzothXLightingPkt) == AZX_PACKET_SIZE, "AzothXLightingPkt size mismatch");

void AuraTUFKeyboardController::UpdateAzothXDevice
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           color_mode,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    // Mode-specific speed range: {fast_hex, slow_hex} for mode indices 0x00-0x09.
    // Static (0x00) uses 0xFF (no speed). Linear interpolation maps the OpenRGB
    // speed value [0=fastest .. 255=slowest] to each mode's USB speed range.
    static const uint8_t speed_fast[10] = { 0xFF, 0x0F, 0x12, 0x08, 0x23, 0x50, 0x0A, 0x0C, 0x07, 0x0E };
    static const uint8_t speed_slow[10] = { 0xFF, 0x2A, 0x3E, 0x16, 0x55, 0x78, 0x19, 0x14, 0x0C, 0x1B };

    AzothXLightingPkt pkt;
    pkt.mode       = mode;
    pkt.brightness = brightness;

    /*-----------------------------------------------------------------------*\
    | Map OpenRGB speed [0=fastest .. 255=slowest] onto each mode's USB      |
    | speed range. Static (0x00) and out-of-range modes use the no-speed     |
    | sentinel.                                                              |
    \*-----------------------------------------------------------------------*/
    if(mode == AURA_KEYBOARD_MODE_STATIC || mode >= 10)
    {
        pkt.speed = AZX_SPEED_NONE;
    }
    else
    {
        const uint8_t fast = speed_fast[mode];
        const uint8_t slow = speed_slow[mode];
        pkt.speed = static_cast<uint8_t>(fast + (static_cast<uint32_t>(speed) * (slow - fast)) / 255);
    }

    /*-----------------------------------------------------------------------*\
    | Rainbow Wave: 3-packet multi-page format.                              |
    | Gradient is the hardware default rainbow (7 stops, 2+4+1 split across  |
    | the three packets); only speed, brightness, and direction are exposed. |
    \*-----------------------------------------------------------------------*/
    if(mode == AURA_KEYBOARD_MODE_WAVE)
    {
        // Stops 1-2 (chartreuse @ 7%, yellow-green @ 16%) — type 0x02 buf[0A+]
        static const uint8_t wave_stops_1_2[] = {
            0x07, 0x08, 0xFF, 0x00,
            0x10, 0x17, 0xE8, 0x00,
            0xFF, 0x00, 0x00
        };
        // Stops 3-6 (blue, cyan, green, yellow) — type 0x01 buf[05+]
        static const uint8_t wave_stops_3_6[] = {
            0x27, 0x08, 0x08, 0xFF,
            0x36, 0x00, 0xFF, 0xF9,
            0x46, 0x00, 0xFF, 0x08,
            0x56, 0xE9, 0xFF, 0x00
        };
        // Stop 7 (orange-red @ 100%) — type 0x00 buf[05+]
        static const uint8_t wave_stop_7[] = {
            AZX_POS_MAX, 0xFF, 0x14, 0x00
        };

        uint8_t buf[AZX_PACKET_SIZE];

        // Packet 1 — main parameters + gradient stops 1-2
        azx_init_packet(buf, AURA_KEYBOARD_MODE_WAVE, AZX_PAGE_PRIMARY);
        buf[5] = pkt.speed;
        buf[6] = pkt.brightness;
        buf[8] = direction;
        buf[9] = AZX_THICKNESS_THICK;
        memcpy(&buf[10], wave_stops_1_2, sizeof(wave_stops_1_2));

        LOG_DEBUG("[%s] AzothX Wave pkt02 spd=%02X bri=%02X dir=%02X",
                  name.c_str(), pkt.speed, pkt.brightness, direction);
        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);

        // Packet 2 — gradient stops 3-6
        azx_init_packet(buf, AURA_KEYBOARD_MODE_WAVE, AZX_PAGE_SECONDARY);
        memcpy(&buf[5], wave_stops_3_6, sizeof(wave_stops_3_6));
        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);

        // Packet 3 — gradient stop 7 + commit
        azx_init_packet(buf, AURA_KEYBOARD_MODE_WAVE, AZX_PAGE_COMMIT);
        memcpy(&buf[5], wave_stop_7, sizeof(wave_stop_7));
        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);
        SaveMode();
        return;
    }

    /*-----------------------------------------------------------------------*\
    | Quicksand: 2-packet format. 6 user-selectable colour stops are split   |
    | as stops 1-3 in the secondary page and stops 4-6 in the commit page.  |
    \*-----------------------------------------------------------------------*/
    if(mode == AURA_KEYBOARD_MODE_QUICKSAND)
    {
        const uint8_t qs_cm   = (color_mode == OPENRGB_CM_RANDOM) ? AZX_CM_RANDOM : AZX_CM_SINGLE;
        const bool    is_rand = (qs_cm == AZX_CM_RANDOM);

        uint8_t buf[AZX_PACKET_SIZE];

        // Packet 1 — main params + gradient stops 1-3
        azx_init_packet(buf, AURA_KEYBOARD_MODE_QUICKSAND, AZX_PAGE_SECONDARY);
        buf[5] = pkt.speed;
        buf[6] = pkt.brightness;
        buf[7] = qs_cm;
        buf[8] = direction;
        if(!is_rand)
        {
            for(size_t i = 0; i < 3 && i < colors.size(); i++)
            {
                buf[10 + i * 3] = RGBGetRValue(colors[i]);
                buf[11 + i * 3] = RGBGetGValue(colors[i]);
                buf[12 + i * 3] = RGBGetBValue(colors[i]);
            }
        }

        LOG_DEBUG("[%s] AzothX Quicksand pkt01 spd=%02X bri=%02X cm=%02X dir=%02X",
                  name.c_str(), pkt.speed, pkt.brightness, qs_cm, direction);
        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);

        // Packet 2 — commit + gradient stops 4-6
        azx_init_packet(buf, AURA_KEYBOARD_MODE_QUICKSAND, AZX_PAGE_COMMIT);
        if(!is_rand)
        {
            for(size_t i = 3; i < 6 && i < colors.size(); i++)
            {
                buf[5 + (i - 3) * 3] = RGBGetRValue(colors[i]);
                buf[6 + (i - 3) * 3] = RGBGetGValue(colors[i]);
                buf[7 + (i - 3) * 3] = RGBGetBValue(colors[i]);
            }
        }

        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);
        SaveMode();
        return;
    }

    /*-----------------------------------------------------------------------*\
    | Ripple: 3-packet format. Left stop is hardware-fixed (black at pos 7); |
    | only the right stop colour is user-selectable. Pattern uses [pos R G  |
    | B] for the right stop; Random sends zeros and the firmware picks.     |
    \*-----------------------------------------------------------------------*/
    if(mode == AURA_KEYBOARD_MODE_RIPPLE)
    {
        const uint8_t rp_cm   = (color_mode == OPENRGB_CM_RANDOM) ? AZX_CM_RANDOM : AZX_CM_SINGLE;
        const bool    is_rand = (rp_cm == AZX_CM_RANDOM);

        uint8_t buf[AZX_PACKET_SIZE];

        // Packet 1 — main params + right gradient stop
        azx_init_packet(buf, AURA_KEYBOARD_MODE_RIPPLE, AZX_PAGE_PRIMARY);
        buf[5]  = pkt.speed;
        buf[6]  = pkt.brightness;
        buf[7]  = rp_cm;
        buf[9]  = AZX_THICKNESS_THICK;
        buf[10] = AZX_RIPPLE_LEFT_POS;
        if(!is_rand && !colors.empty())
        {
            buf[11] = AZX_POS_MAX;
            buf[12] = RGBGetRValue(colors[0]);
            buf[13] = RGBGetGValue(colors[0]);
            buf[14] = RGBGetBValue(colors[0]);
        }

        LOG_DEBUG("[%s] AzothX Ripple pkt02 spd=%02X bri=%02X cm=%02X",
                  name.c_str(), pkt.speed, pkt.brightness, rp_cm);
        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);

        // Packet 2 — secondary page (all zeros)
        azx_init_packet(buf, AURA_KEYBOARD_MODE_RIPPLE, AZX_PAGE_SECONDARY);
        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);

        // Packet 3 — commit (all zeros = black left stop)
        azx_init_packet(buf, AURA_KEYBOARD_MODE_RIPPLE, AZX_PAGE_COMMIT);
        ClearResponses();
        hid_write(dev, buf, sizeof(buf));
        AwaitResponse(20);
        SaveMode();
        return;
    }

    /*-----------------------------------------------------------------------*\
    | Color mode encoding for single-packet modes.                            |
    | DeviceUpdateMode delivers: 0=Single, 0x10=Double, 1=Random.            |
    | Breathing / Starry Night / Rain Drop use 0x11 for Random; others 0x01. |
    \*-----------------------------------------------------------------------*/
    const bool is_random_alt_mode = (mode == AURA_KEYBOARD_MODE_BREATHING
                                  || mode == AURA_KEYBOARD_MODE_STARRY_NIGHT
                                  || mode == AURA_KEYBOARD_MODE_RAIN_DROP);

    if(color_mode == OPENRGB_CM_RANDOM && is_random_alt_mode)
    {
        pkt.color_mode = AZX_CM_RANDOM_ALT;
    }
    else
    {
        pkt.color_mode = color_mode;
    }

    /*-----------------------------------------------------------------------*\
    | Primary colour [0A-0C]                                                 |
    \*-----------------------------------------------------------------------*/
    if(!colors.empty())
    {
        pkt.r1 = RGBGetRValue(colors[0]);
        pkt.g1 = RGBGetGValue(colors[0]);
        pkt.b1 = RGBGetBValue(colors[0]);
    }

    /*-----------------------------------------------------------------------*\
    | Secondary colour / blend [0D-0F]                                       |
    \*-----------------------------------------------------------------------*/
    if(pkt.color_mode == AZX_CM_DOUBLE && colors.size() >= 2)
    {
        pkt.r2 = RGBGetRValue(colors[1]);
        pkt.g2 = RGBGetGValue(colors[1]);
        pkt.b2 = RGBGetBValue(colors[1]);
    }
    else if(mode == AURA_KEYBOARD_MODE_CURRENT && pkt.color_mode == AZX_CM_SINGLE)
    {
        // Current Single-mode blend pattern: min(R,G) / G / B
        pkt.r2 = std::min(pkt.r1, pkt.g1);
        pkt.g2 = pkt.g1;
        pkt.b2 = pkt.b1;
    }

    /*-----------------------------------------------------------------------*\
    | Background colour [10-12] (Current and Raindrop only).                 |
    | Passed as colors[2]; black (0,0,0) = Background OFF.                   |
    \*-----------------------------------------------------------------------*/
    if((mode == AURA_KEYBOARD_MODE_CURRENT || mode == AURA_KEYBOARD_MODE_RAIN_DROP)
       && colors.size() >= 3)
    {
        pkt.bg_r = RGBGetRValue(colors[2]);
        pkt.bg_g = RGBGetGValue(colors[2]);
        pkt.bg_b = RGBGetBValue(colors[2]);
    }

    LOG_DEBUG("[%s] AzothX mode=%02X spd=%02X bri=%02X cm=%02X "
              "c1=(%02X,%02X,%02X) c2=(%02X,%02X,%02X) bg=(%02X,%02X,%02X)",
              name.c_str(), pkt.mode, pkt.speed, pkt.brightness, pkt.color_mode,
              pkt.r1, pkt.g1, pkt.b1, pkt.r2, pkt.g2, pkt.b2,
              pkt.bg_r, pkt.bg_g, pkt.bg_b);

    ClearResponses();
    hid_write(dev, reinterpret_cast<unsigned char*>(&pkt), sizeof(pkt));
    AwaitResponse(20);
    SaveMode();
}

void AuraTUFKeyboardController::UpdateDevice
    (
    unsigned char           mode,
    std::vector<RGBColor>   colors,
    unsigned char           direction,
    unsigned char           color_mode,
    unsigned char           speed,
    unsigned char           brightness
    )
{
    if(device_pid == AURA_TUF_K1_GAMING_PID && mode == AURA_KEYBOARD_MODE_WAVE)
    {
        return UpdateK1Wave(colors, direction, speed, brightness);
    }

    if(device_pid == AURA_ROG_AZOTH_X_USB_PID)
    {
        return UpdateAzothXDevice(mode, colors, direction, color_mode, speed, brightness);
    }

    if(device_pid == AURA_ROG_AZOTH_USB_PID
    || device_pid == AURA_ROG_AZOTH_2_4_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_II_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_II_RX_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID
    || device_pid == AURA_ROG_STRIX_SCOPE_II_96_RX_WIRELESS_USB_PID)
    {
        if(mode == AURA_KEYBOARD_MODE_WAVE || mode == AURA_KEYBOARD_MODE_RIPPLE)
        {
            return UpdateScopeIIRainbowRipple(mode, colors, direction, color_mode, speed, brightness);
        }
        else if (mode == AURA_KEYBOARD_MODE_QUICKSAND)
        {
            return UpdateScopeIIQuicksand(colors, direction, color_mode, speed, brightness);
        }
    }

    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x2C;

    if(device_pid != AURA_ROG_CLAYMORE_PID)
    {
        usb_buf[0x03]   = mode;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = speed;
        usb_buf[0x06]   = brightness;
        usb_buf[0x07]   = color_mode;
        usb_buf[0x08]   = direction;

        if(is_per_led_keyboard)
        {
            usb_buf[0x09]   = 0x02;

            if(mode == AURA_KEYBOARD_MODE_WAVE || mode == AURA_KEYBOARD_MODE_RIPPLE)
            {
                usb_buf[0x0A]   = (unsigned char)colors.size();

                /*-----------------------------------------------------*\
                | Loop over every color given                           |
                \*-----------------------------------------------------*/
                for(unsigned int i = 0; i < colors.size(); i ++)
                {
                    if(colors[i])
                    {
                        usb_buf[11 + i * 4] = (unsigned char)(100.0f / (float)colors.size() * (i + 1));
                        usb_buf[12 + i * 4] = RGBGetRValue(colors[i]);
                        usb_buf[13 + i * 4] = RGBGetGValue(colors[i]);
                        usb_buf[14 + i * 4] = RGBGetBValue(colors[i]);
                    }
                }
            }
            else
            {
                /*-----------------------------------------------------*\
                | Loop over Color1, Color2 and Background if there      |
                \*-----------------------------------------------------*/
                for(unsigned int i = 0; i != colors.size(); i++)
                {
                    if(colors[i])
                    {
                        usb_buf[10 + i * 3] = RGBGetRValue(colors[i]);
                        usb_buf[11 + i * 3] = RGBGetGValue(colors[i]);
                        usb_buf[12 + i * 3] = RGBGetBValue(colors[i]);
                    }
                }

            }
        }
        else
        {
            /*-----------------------------------------------------------------*\
            | Only handles K5 Rainbow.                                          |
            | K1 is filtered out earlier and is executed in `UpdateK1Wave()`    |
            \*-----------------------------------------------------------------*/
            if(mode == AURA_KEYBOARD_MODE_WAVE)
            {
                usb_buf[0x09]   = 0x05;

                for(unsigned int i = 0; i < 5; i ++)
                {
                    usb_buf[10 + i * 4] = i + 1;
                    usb_buf[11 + i * 4] = RGBGetRValue(colors[i]);
                    usb_buf[12 + i * 4] = RGBGetGValue(colors[i]);
                    usb_buf[13 + i * 4] = RGBGetBValue(colors[i]);
                }
            }
            else
            {
                /*-----------------------------------------------------*\
                | Loop over Color1, Color2 and Background if there      |
                \*-----------------------------------------------------*/
                for(unsigned int i = 0; i != colors.size(); i++)
                {
                    if(colors[i])
                    {
                        usb_buf[ 9 + i * 3] = RGBGetRValue(colors[i]);
                        usb_buf[10 + i * 3] = RGBGetGValue(colors[i]);
                        usb_buf[11 + i * 3] = RGBGetBValue(colors[i]);
                    }
                }
            }
        }
    }
    else
    {
        usb_buf[0x03]   = 0x00;
        usb_buf[0x04]   = 0x00;
        usb_buf[0x05]   = mode;
        usb_buf[0x06]   = speed;

        bool random     = (color_mode == 1);
        bool pattern    = (color_mode == 2);

        usb_buf[0x07]   = random * 128 + pattern * 32 + direction;
        usb_buf[0x08]   = 0xFF; // "byteExt1" unknown usage
        usb_buf[0x09]   = 0xFF; // "byteExt2" unknown usage
        usb_buf[0x0A]   = (mode == 2) ? 0x80 : 0xFF; // "Lightness" (not Brightness)

        if(mode == 7)
        {
            UpdateQuicksandColors(colors);
        }
        else
        {
            if(mode == 4 && color_mode == 0)
            {
                usb_buf[11] = 0xFF;
                usb_buf[12] = 0xFF;
                usb_buf[13] = 0xFF;
            }

            for(unsigned int i = 0; i < colors.size(); i ++)
            {
                if(colors[i])
                {
                    usb_buf[11 + i * 3] = RGBGetRValue(colors[i]);
                    usb_buf[12 + i * 3] = RGBGetGValue(colors[i]);
                    usb_buf[13 + i * 3] = RGBGetBValue(colors[i]);
                }
            }
        }

        for(int i = 1; i < 5; i++)
        {
            usb_buf[5 + i * 12] = 0xFF;
        }
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
void AuraTUFKeyboardController::UpdateQuicksandColors
    (
    std::vector<RGBColor>   colors
    )
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x91;

    for(unsigned int i = 0; i < 6; i++)
    {
        if(colors[i])
        {
            usb_buf[5 + i * 3] = RGBGetRValue(colors[i]);
            usb_buf[6 + i * 3] = RGBGetGValue(colors[i]);
            usb_buf[7 + i * 3] = RGBGetBValue(colors[i]);
        }
    }

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

/*---------------------------------------------------------*\
| only needed for Claymore                                  |
\*---------------------------------------------------------*/
void AuraTUFKeyboardController::UpdateMode(unsigned char mode)
{
    unsigned char usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0x51;
    usb_buf[0x02]   = 0x28;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = mode;

    ClearResponses();
    hid_write(dev, usb_buf, 65);
    AwaitResponse(20);
}

void AuraTUFKeyboardController::AwaitResponse(int ms)
{
    unsigned char usb_buf_out[65];
    hid_read_timeout(dev, usb_buf_out, 65, ms);
}

void AuraTUFKeyboardController::ClearResponses()
{
    int result = 1;
    unsigned char usb_buf_flush[65];
    while(result > 0)
    {
        result = hid_read_timeout(dev, usb_buf_flush, 65, 0);
    }
}

