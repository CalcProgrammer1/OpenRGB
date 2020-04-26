/*-----------------------------------------*\
|  AuraMainboardController.cpp              |
|                                           |
|  Driver for ASUS Aura RGB USB mainboard   |
|  lighting controller                      |
|                                           |
|  Martin Hartl (inlart) 4/25/2020          |
\*-----------------------------------------*/

#include "AuraMainboardController.h"
#include <cstring>

AuraMainboardController::AuraMainboardController(hid_device* dev_handle) : AuraUSBController(dev_handle), mode(AURA_MODE_DIRECT)
{
}

AuraMainboardController::~AuraMainboardController()
{
}

void AuraMainboardController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    unsigned char   led_data[60];
    unsigned int    leds_sent = 0;

    SendEffect(channel, mode);

    while(leds_sent < num_colors)
    {
        unsigned int leds_to_send = 20;

        if((num_colors - leds_sent) < leds_to_send)
        {
            leds_to_send = num_colors - leds_sent;
        }

        for(int led_idx = 0; led_idx < leds_to_send; led_idx++)
        {
            led_data[(led_idx * 3) + 0] = RGBGetRValue(colors[led_idx + leds_sent]);
            led_data[(led_idx * 3) + 1] = RGBGetGValue(colors[led_idx + leds_sent]);
            led_data[(led_idx * 3) + 2] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        SendDirect
        (
            channel,
            leds_sent,
            leds_to_send,
            led_data
        );

        leds_sent += leds_to_send;
    }

    SendCommit();
}

void AuraMainboardController::SetMode
    (
    unsigned char mode,
    unsigned char red,
    unsigned char grn,
    unsigned char blu
    )
{
    this->mode = mode;
    unsigned char led_data[3];
    led_data[0] = red;
    led_data[1] = grn;
    led_data[2] = blu;

    for(int channel_idx = 0; channel_idx < GetChannelCount(); channel_idx++)
    {
        SendEffect
        (
            channel_idx,
            mode
        );
        SendDirect
        (
            channel_idx,
            0,
            1,
            led_data
        );
        SendCommit();
    }
}

void AuraMainboardController::SendEffect
    (
    unsigned char   channel,
    unsigned char   mode
    )
{
    unsigned char usb_buf[65];

    if(mode == AURA_MODE_DIRECT) {
        mode = AURA_MODE_STATIC;
    }
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_MAINBOARD_CONTROL_MODE_EFFECT;
    usb_buf[0x02]   = 0x00;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = channel;
    usb_buf[0x05]   = mode;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void AuraMainboardController::SendDirect
    (
    unsigned char   device,
    unsigned char   start_led,
    unsigned char   led_count,
    unsigned char*  led_data
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_MAINBOARD_CONTROL_MODE_DIRECT;
    usb_buf[0x02]   = start_led;
    usb_buf[0x03]   = 0xff;
    usb_buf[0x04]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x05], led_data, led_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void AuraMainboardController::SendCommit()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_MAINBOARD_CONTROL_MODE_COMMIT;
    usb_buf[0x02]   = 0x55;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}
