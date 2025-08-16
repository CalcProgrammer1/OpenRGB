/*---------------------------------------------------------*\
| SteelSeriesApex8ZoneController.h                          |
|                                                           |
|   Driver for SteelSeries Apex 8 Zone                      |
|                                                           |
|   Chris M (Dr_No)                             23 Feb 2022 |
|   Paul K. Gerke                               27 Oct 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesApex3Controller.h"

#define STEELSERIES_8Z_LED_COUNT            8
#define STEELSERIES_8Z_WRITE_PACKET_SIZE    65
#define STEELSERIES_8Z_BRIGHTNESS_MAX       0x10

class SteelSeriesApex8ZoneController : public SteelSeriesApex3Controller
{
public:
    SteelSeriesApex8ZoneController(hid_device *dev_handle, const char *path, std::string dev_name);
    ~SteelSeriesApex8ZoneController();

    void        SetColor(std::vector<RGBColor> colors, uint8_t mode, uint8_t brightness);
    void        Save();
    uint8_t     GetLedCount();
    uint8_t     GetMaxBrightness();
    bool        SupportsRainbowWave();
    bool        SupportsSave();

private:
    uint8_t     current_brightness;

    void        SetBrightness(uint8_t brightness);
    uint8_t     GetBrightness();
};

/*-----------------------------------------------------------------------------------------------*\
# General keyboard behavior overview

- The keyboard does not appear to have persistent memory: Cycling the USB
  connection resets the keyboard to a `Rainbow Wave` mode. SteelSeries GG
  has to be running to set color patterns, indicating `Direct` mode.
- The brightness settings of the keyboard can be read back by the
  SteelSeries GG Software from the keyboard. Changing the brightness with
  Mod+F11/F12 moves the slider in the SteelSeries GG software. Also, when
  changing the brightness and opening the configuration dialog, the slider
  is updated to the value set via Mod+F11/F12.

# Message protocol

The HID-Messages contain a data-packet that is sent with HID-Requests to the
keyboard to change the keyboard's mode. The first byte seems to be a command-ID.
The full message-length is 64 bytes.

Values not explicitly mentioned below are zero-bytes.

## 0x21: Set LED color

  0x21 BM R1 G1 B1 R2 G2 B2 ... [up to rgb values for zone 8]

  - Bit Mask<br/>A bit mask of LEDs to set where 0xFF sets all LEDs. Setting 2<sup>N</sup>
  as 0 will ***not*** set LED N.

  - The next sequence of bytes are RGB triplets for each of the 8 LEDs.
  Value range from 0 up to 255 for each color value.

## 0x22: Set the keyboard to `Rainbow Wave` mode. Message

  0x22 0xFF

  - As for 0x21, 0xFF seems to be constant.

## 0x23: Set Brightness

   Sets the overall brightness of all color channels. Seems to act as a factor
   for the individual RGB values, but can be individually adjusted by the user
   through the Mod+F11/F12 key combinations.

  0x23 [brightness]

  - Brightness values are in the range from 0x00 (dark) to 0x10 (full
  brightness)

# Other observed commands/messages of unknown purpose were:

## 0x6C 0x00 0x01 0x01
   This triggers some sort of URB_INTERRUPT from USB endpoint .2
   reporting back some data however unsure what the data is. It seems
   to be 0x6C followed by only zeroes.

## 0xA3
   Query brightness?

## 0x90
   Best guess "get firmware version" or similar... or some other settings. Again
   a complex answer is echoed from endpoint .2 starting with 0x90 and a
   few bytes (~8-10) of data.

\*-----------------------------------------------------------------------------------------------*/
