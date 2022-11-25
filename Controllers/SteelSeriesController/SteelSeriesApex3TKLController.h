/*-----------------------------------------*\
|  SteelSeriesApex3TKLController.h          |
|                                           |
|  Paul K. Gerke - 27.10.2022               |
|  Dr_no (Chris M)                          |
|                                           |
|  Paul K: I attached my notes on reverse   |
|  engineering the protocol to the end of   |
|  this file                                |
\*-----------------------------------------*/

#include <string>
#include <hidapi/hidapi.h>

#include "SteelSeriesGeneric.h"
#include "SteelSeriesApexBaseController.h"

#pragma once

#define STEELSERIES_APEX3TKL_WRITE_PACKET_SIZE    65
#define STEELSERIES_APEX3TKL_BRIGHTNESS_MIN       0x00
#define STEELSERIES_APEX3TKL_BRIGHTNESS_MAX       0x10

class SteelSeriesApex3TKLController
{
public:
    SteelSeriesApex3TKLController
        (
        hid_device*         dev_handle,
        steelseries_type    proto_type,
        const char*         path,
        const char*         device_name
        );

    ~SteelSeriesApex3TKLController();

    std::string GetDeviceLocation();
    char*       GetDeviceName();
    std::string GetSerialString();
    steelseries_type GetKeyboardType();

    unsigned char GetBrightness();
    void SetBrightness(unsigned char brightness);
    void SetColor(std::vector<RGBColor> colors);
    void SetRainbowMode();
private:
    char                    device_name[32];
    hid_device*             dev;
    std::string             location;
    steelseries_type        proto;
};

/*

# Reverse engineering protocol of APEX 3 TKL

Paul K. Gerke - 2022-10-27

I used a Windows 10 VM on my Linux host and virtually attached my
SteelSeries Apex3 TKL keyboard to the Windows VM using VirtualBox. On Windows,
I used Wireshark 4.x to listen to the USB-HID traffic between the SteelSeries
keyboard and the SteelSeries GG Software version 26.0.0.

## General keyboard behavior overview

- The keyboard does not seem to have persistent memory: Cycling the usb
  connection resets to keyboard to a "rainbow" pattern mode. SteelSeries GG
  has to be running to set color patterns, indicating that the PC software does
  most of the heavy lifting.
- The keyboard has 8 colorzones from left to right. Similar to the ApexTZone
  controller, but that controller did not work for me.
- The keyboard has a "rainbow" pattern mode and "constant color" settings for
  each color zone.
- The brightness settings of the keyboard can somehow be read back by the
  SteelSeries GG Software from the keyboard. Changing the brightness with
  Mod+F11/F12 changes the slider in the SteelSeries GG software. Also, when
  changing the brightness and opening the configuration dialog, the slider
  is updated to the value set via Mod+F11/F12. I found some messages
  relating to this, but could not identify the full mechanism how to query this
  data.

## Message protocol

The HID-Messages contain a data-packet that is sent with HID-Requests to the
keyboard to change the keyboard's mode. The first byte seems to be a command-ID.
The full message-length is 64 byes (or 65 for the OpenRGB controller which
uses the first byte as deviceId selector).

Values not explicitly mentioned below are zero-bytes.

Command IDs:

- 0x21: Set color of the 8 color regions. Message format:

  0x21 0xFF R1 G1 B1 R2 G2 B2 ... [up to rgb values for zone 8]

  The 0xFF value seems to be constant - purpose unknown.

  The next sequence of bytes consist of 8 x 3 byte-RGB data for each of the
  three regions. Value range from 0 up to 255 for each color value (as you
  might expect).

- 0x23: Set overall brightness of all color channels. Seems to act as a factor
  for the individual RGB values, but can be individually adjusted by the user
  through the Mod+F11/F12 key combinations - making it interesting to maybe
  decouple overall-brightness from the overall color settings. Message format:

  0x23 [brightness]

  Valid brightness values are in the range from 0x00 (dark) to 0x10 (full
  brightness);

- 0x22: Set the keyboard to "rainbow animation" mode. Message

  Message format:

  0x22 0xFF

  As for 0x21, 0xFF seems to be constant, purpose unknown.

Other observed commands/messages of unknown purpose were:

- 0x6c 0x00 0x01 0x01
    - I am half sure that this triggers some sort of URB_INTERRUPT from USB
      device 1.2.2 reporting back some data. No idea what data though. It seems
      to be pretty 0x6c followed by only zeroes.
- 0xa3 !!! Query brightness???
- 0x90
    - I guess "get firmware version" or similar... or some other settings. Again
      a complex answer is echoed from device 1.2.2 starting with 0x90 and a
      few bytes (~8-10) of data. No idea what this is.

## Reading back the set brightness from the keyboard

I was able to _which_ messages contain the information, but not how to get them.
On windows, when adjusting the overall brightness using the Mod+F11/F12 key
combination or sometimes when using the SteelSeries GG software, the keyboard
sent a URB_INTERRUPT from the device labeled as "1.2.5" with HID-data that was
formatted as follows:

0x01 [brightness]

0x01 seems to be the data-type(?) and then brightness is a value in the range
from 0x00 (dark) to 0x10 (full brightness).

However, I was not able to capture these interrupts on my Linux machine when
pressing the brightness-buttons.

Also I was not able to find a message that tells the keyboard to report this
data to the PC. Somehow it only works on Windows. To be continued...?

*/
