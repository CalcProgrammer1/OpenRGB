/*-----------------------------------------*\
|  WootingKeyboardController.h              |
|                                           |
|  Definitions and types for Wooting RGB    |
|  keyboard lighting controller             |
|                                           |
|  Diogo Trindade (diogotr7)    3/4/2021    |
\*-----------------------------------------*/

#include "WootingKeyboardController.h"

#pragma once

class WootingOneKeyboardController : public WootingKeyboardController
{
public:
    WootingOneKeyboardController(hid_device* dev_handle, const char *path, uint8_t wooting_type);
    ~WootingOneKeyboardController();

    void                SendDirect(RGBColor* colors, uint8_t colour_count);

private:
    void                SendInitialize();
    bool                wooting_usb_send_buffer(RGB_PARTS part_number, uint8_t* report_buffer);
};
