/*------------------------------------------------------*\
|  AP2Controller.cpp                                     |
|                                                        |
|  Driver for QMK/Shine firmware for AnnePro2 keyboard   |
|                                                        |
|  Tomasz Fortuna 26/09/2021                             |
|  Sergey Gavrilov (DrZlo13) 06/06/2021                  |
\*------------------------------------------------------*/

#include "AP2Controller.h"
#include <cstring>
#include <cassert>

AP2Controller::AP2Controller(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AP2Controller::~AP2Controller()
{
    hid_close(dev);
}

std::string AP2Controller::GetDeviceLocation()
{
    return ("HID: " + location);
}

std::string AP2Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if (ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

/* All possible shine commands. Check Shine/AP-QMK code for arguments */
enum {
  /* Basic config */
  CMD_LED_ON = 0x01,
  CMD_LED_OFF = 0x02,

  CMD_LED_SET_PROFILE = 0x03,
  CMD_LED_NEXT_PROFILE = 0x04,
  CMD_LED_PREV_PROFILE = 0x05,

  CMD_LED_NEXT_INTENSITY = 0x06,
  CMD_LED_NEXT_ANIMATION_SPEED = 0x07,

  /* Masks */
  /* Override a key color, eg. capslock */
  CMD_LED_MASK_SET_KEY = 0x10,
  /* Override all keys in a row with configurable colors */
  CMD_LED_MASK_SET_ROW = 0x11,

  /* Override all keys with single color (eg. foreground color) */
  CMD_LED_MASK_SET_MONO = 0x12,

  /* Reactive / status */
  CMD_LED_GET_STATUS = 0x20,
  CMD_LED_KEY_BLINK = 0x21,
  CMD_LED_KEY_DOWN = 0x22,
  CMD_LED_KEY_UP = 0x23, /* TODO */
  CMD_LED_IAP = 0x24,
};

/* Raw HID Commands */
enum AP2RawHidCodes {
    AP2_RAW_FORWARD_SHINE = 1,
};

void AP2Controller::SendDirect(unsigned char frame_count, unsigned char * frame_data) 
{
    /* This sends HID RAW command to forward message directly to shine */
    unsigned char frame[65];
    /*
     * First byte is special and denotes message number or something like that.
     * Non-zero values will get eaten under Windows. Zero values will be eaten under Linux.
     */
    frame[0] = 0x00;
    frame[1] = AP2_RAW_FORWARD_SHINE;
    frame[2] = CMD_LED_MASK_SET_ROW;
    assert(frame_count <= 65-3);
    memcpy(frame + 3, frame_data, frame_count);
    hid_write(dev, frame, frame_count + 3);
}
