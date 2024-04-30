/*-------------------------------------------------------------------*\
|  WootingTwoKeyboardController.h                                     |
|                                                                     |
|  OpenRGB driver for Wooting RGB keyboardlighting controller         |
|      https://github.com/WootingKb/wooting-rgb-sdk                   |
|                                                                     |
|  Chris M (Dr_No)         9th July 2021                              |
\*-------------------------------------------------------------------*/

#include "WootingKeyboardController.h"

#pragma once

class WootingTwoKeyboardController : public WootingKeyboardController
{
public:
    WootingTwoKeyboardController(hid_device* dev_handle, const char *path, uint8_t wooting_type);
    ~WootingTwoKeyboardController();

    void                SendDirect(RGBColor* colors, uint8_t colour_count);

private:
    void                SendInitialize();
    bool                wooting_usb_send_buffer(RGB_PARTS part_number, uint8_t* report_buffer);
};
