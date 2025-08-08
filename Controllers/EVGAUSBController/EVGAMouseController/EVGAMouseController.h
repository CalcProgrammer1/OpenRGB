/*---------------------------------------------------------*\
| EVGAMouseController.h                                     |
|                                                           |
|   Driver for EVGA mouse                                   |
|                                                           |
|   Cooper Knaak                                23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define EVGA_PERIPHERAL_PACKET_SIZE 65
#define EVGA_PERIPHERAL_LED_COUNT 3
#define HID_MAX_STR 255

enum
{
    EVGA_PERIPHERAL_MODE_STATIC     = 1,
    EVGA_PERIPHERAL_MODE_BREATHING  = 2,
    EVGA_PERIPHERAL_MODE_RAINBOW    = 3,
    EVGA_PERIPHERAL_MODE_PULSE      = 4,
    EVGA_PERIPHERAL_MODE_TRIGGER    = 6
};

enum
{
    EVGA_PERIPHERAL_LED_FRONT   = 0,
    EVGA_PERIPHERAL_LED_WHEEL   = 1,
    EVGA_PERIPHERAL_LED_LOGO    = 2
};

/*----------------------------------------------------------------------*\
| All values in this enum account for the required 0x0 byte at index 0   |
| when using hid* APIs. The byte controlling the red value of an LED is  |
| at index 15 in the buffer passed to hid* APIs because there is a 0x0   |
| byte at the beginning of said buffer. It would only be index 14 in the |
| raw packet received by the peripheral.                                 |
\*----------------------------------------------------------------------*/
enum
{
    EVGA_PERIPHERAL_CONNECTION_TYPE_BYTE    = 3,
    EVGA_PERIPHERAL_LED_INDEX_BYTE          = 8,
    EVGA_PERIPHERAL_MODE_BYTE               = 9,
    /*--------------*\
    | Range [0, 100] |
    \*--------------*/
    EVGA_PERIPHERAL_BRIGHTNESS_BYTE         = 10,
    /*--------------*\
    | Range [0, 100] |
    \*--------------*/
    EVGA_PERIPHERAL_SPEED_BYTE              = 11,
    /*-------------------------------------------------------*\
    | Determines when the lights initiate and terminate:      |
    | immediately, on key press, or on key release.           |
    | Currently unused because OpenRGB does not support this. |
    \*-------------------------------------------------------*/
    EVGA_PERIPHERAL_EFFECT_DURATION_BYTE    = 13,
    /*---------------------------------------------------------------------------------*\
    | The byte at this index specifies how many colors are being passed in this packet. |
    | The colors are a sequence of 3 bytes per color: red, green, then blue. Thus, when |
    | passing 1 for this byte, the device will read the 3 next bytes as the color. When |
    | passing 7, the device will read the next 21 bytes in sets of 3.                   |
    \*---------------------------------------------------------------------------------*/
    EVGA_PERIPHERAL_COLOR_COUNT_BYTE        = 14,
    EVGA_PERIPHERAL_RED_BYTE                = 15,
    EVGA_PERIPHERAL_GREEN_BYTE              = 16,
    EVGA_PERIPHERAL_BLUE_BYTE               = 17,
};

enum
{
    EVGA_PERIPHERAL_CONNECTION_TYPE_WIRED       = 0,
    EVGA_PERIPHERAL_CONNECTION_TYPE_WIRELESS    = 2,
};

struct EVGAMouseControllerDeviceState
{
    uint8_t mode;
    uint8_t brightness;
    uint8_t speed;
    std::vector<RGBColor> colors;
};

class EVGAMouseController
{
public:
    EVGAMouseController(hid_device* dev_handle, char * path, int connection_type, std::string dev_name);
    ~EVGAMouseController();

    std::string GetName();
    std::string GetSerial();
    std::string GetLocation();

    /*---------------------------------------------------------------*\
    | Gets the mode, colors, or entire state currently on the device. |
    | OpenRGB does not support per-zone modes. All zones must be set  |
    | to the same mode. It's possible to use the vendor's software    |
    | to set each LED to separate states. These methods use the logo  |
    | LED (#2) as the source of truth.                                |
    \*---------------------------------------------------------------*/
    uint8_t GetMode();
    EVGAMouseControllerDeviceState GetState();

    /*-------------------------------------------------------------------------*\
    | Gets the color of the given LED from the device. If a device is in a mode |
    | with multiple colors, returns the first color in the list.                |
    \*-------------------------------------------------------------------------*/
    RGBColor GetColorOfLed(int led);

    inline void SetMode(uint8_t mode)
    {
        SetMode(mode, 0);
        SetMode(mode, 1);
        SetMode(mode, 2);
    }
    void SetMode(uint8_t mode, uint8_t index);

    /*-----------------------------------*\
    | Set a single LED to a single color. |
    \*-----------------------------------*/
    void SetLed(uint8_t index, uint8_t brightness, uint8_t speed, RGBColor color);
    /*---------------------------------------------------*\
    | Set the LED at the given index to a list of colors. |
    \*---------------------------------------------------*/
    void SetLed(uint8_t index, uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors);
    /*---------------------------------------------------------------------------*\
    | Set the LED at the given index to a list of colors, then activate the mode. |
    \*---------------------------------------------------------------------------*/
    void SetLedAndActivate(uint8_t index, uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors);
    /*---------------------------------*\
    | Set all LEDs to a list of colors. |
    \*---------------------------------*/
    void SetAllLeds(uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors);
    /*---------------------------------------------------------*\
    | Set all LEDs to a list of colors, then activate the mode. |
    \*---------------------------------------------------------*/
    void SetAllLedsAndActivate(uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors);

private:
    hid_device* dev;
    std::string location;
    std::string name;
    int         connection_type;

    std::vector<EVGAMouseControllerDeviceState> led_states;

    /*----------------------------------------------------------------------------------------------------------------*\
    | Sets the led to the given colors with the given brightness and speed. if activate is true, activates the current |
    | mode. If false, just sets the colors.                                                                            |
    \*----------------------------------------------------------------------------------------------------------------*/
    void SetLed(uint8_t index, uint8_t brightness, uint8_t speed, const std::vector<RGBColor>& colors, bool activate);

    /*-----------------------------------------------------------------------------*\
    | Requests and stores the current mode and colors for all leds from the device. |
    \*-----------------------------------------------------------------------------*/
    void RefreshDeviceState();

    /*----------------------------------------------------------------------------------*\
    | Requests and stores the current mode and colors for the given led from the device. |
    \*----------------------------------------------------------------------------------*/
    void RefreshDeviceState(int led);

    /*-----------------------------------------------------------------------------------*\
    | Repeatedly reads a packet from the device until a valid packet is received. If no   |
    | such packet is received after max_attempts tries, returns false. Otherwise, returns |
    | true. buffer must be an array with size EVGA_PERIPHERAL_PACKET_SIZE.                |
    \*-----------------------------------------------------------------------------------*/
    bool ReadPacketOrLogErrors(unsigned char *buffer, int max_attempts);

    /*-----------------------------------------------------------------------------------*\
    | Repeatedly reads a packet from the device until a valid packet is received. If a    |
    | "response not ready" packet is returned, try again, up to a maximum number of tries |
    | max_attempts. buffer must be an array with size EVGA_PERIPHERAL_PACKET_SIZE.        |
    | Returns the number of bytes read or -1 on error.                                    |
    \*-----------------------------------------------------------------------------------*/
    int ReadPacketOrWait(unsigned char *buffer, int max_attempts);

    /*-----------------------------------------------------------------------------*\
    | Waits a predetermined amount of time to avoid sending packets to frequently.  |
    | In wireless mode, packets sent too close together might overwrite each other, |
    | causing earlier ones to silently not propagate. Does not wait in connection   |
    | types that do not have this problem.                                          |
    \*-----------------------------------------------------------------------------*/
    void Wait();

    /*------------------------------------------------------------------------------*\
    | Returns true if the packet received from the device signals that the device is |
    | asleep and will not send or receive other packets.                             |
    \*------------------------------------------------------------------------------*/
    bool IsAsleepPacket(unsigned char *buffer);

    /*------------------------------------------------------------------------------*\
    | Returns true if the packet received from the device signals that the device is |
    | still processing a request device state packet. In this case, the request to   |
    | read from the device should be retried at a later time.                        |
    \*------------------------------------------------------------------------------*/
    bool IsResponseNotReadyPacket(unsigned char *buffer);
};

