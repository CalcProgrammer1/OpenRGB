/*-----------------------------------------*\
|  AsusAuraAddressableController.cpp        |
|                                           |
|  Driver for ASUS Aura RGB Addressable     |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2020   |
\*-----------------------------------------*/

#include "AsusAuraAddressableController.h"
#include <cstring>

AuraAddressableController::AuraAddressableController(hid_device* dev_handle, const char* path) : AuraUSBController(dev_handle, path)
{
    /*-----------------------------------------------------*\
    | Add addressable devices                               |
    \*-----------------------------------------------------*/
    for(int i = 0; i < config_table[0x02]; ++i)
    {
        device_info.push_back({0x01, (unsigned char)i, 0x01, 0, AuraDeviceType::ADDRESSABLE});
    }
}

AuraAddressableController::~AuraAddressableController()
{

}

void AuraAddressableController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    unsigned char   led_data[60];
    unsigned int    leds_sent = 0;

    while(leds_sent < num_colors)
    {
        unsigned int leds_to_send = 20;

        if((num_colors - leds_sent) < leds_to_send)
        {
            leds_to_send = num_colors - leds_sent;
        }

        for(unsigned int led_idx = 0; led_idx < leds_to_send; led_idx++)
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

    SendDirectApply(channel);
}

void AuraAddressableController::SetMode
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
    )
{
    SendEffect
    (
        channel,
        mode,
        red,
        grn,
        blu
    );
}

void AuraAddressableController::SendEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu
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
    usb_buf[0x01]   = AURA_ADDRESSABLE_CONTROL_MODE_EFFECT;
    usb_buf[0x02]   = channel;
    usb_buf[0x03]   = 0x00;
    usb_buf[0x04]   = mode;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    usb_buf[0x05]   = red;
    usb_buf[0x06]   = grn;
    usb_buf[0x07]   = blu;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void AuraAddressableController::SendDirectApply
    (
    unsigned char   channel
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
    usb_buf[0x01]   = AURA_CONTROL_MODE_DIRECT;
    usb_buf[0x02]   = 0x80 | channel;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}
