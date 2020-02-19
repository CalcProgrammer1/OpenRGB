/*-----------------------------------------*\
|  AuraAddressableController.cpp            |
|                                           |
|  Driver for ASUS Aura RGB Addressable     |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/18/2020   |
\*-----------------------------------------*/

#include "AuraAddressableController.h"
#include <cstring>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

AuraAddressableController::AuraAddressableController(hid_device* dev_handle)
{
    dev = dev_handle;

    channel_leds[0] = 40;
}

AuraAddressableController::~AuraAddressableController()
{

}

void AuraAddressableController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char   led_data[60];
    unsigned int    leds_sent = 0;

    SendDirectBegin();

    while(leds_sent < colors.size())
    {
        unsigned int leds_to_send = 20;

        if((colors.size() - leds_sent) < leds_to_send)
        {
            leds_to_send = colors.size() - leds_sent;
        }

        for(int led_idx = 0; leds_to_send < 20; led_idx++)
        {
            led_data[(led_idx * 3) + 0] = RGBGetRValue(colors[led_idx]);
            led_data[(led_idx * 3) + 1] = RGBGetGValue(colors[led_idx]);
            led_data[(led_idx * 3) + 2] = RGBGetBValue(colors[led_idx]);
        }

        SendDirect
        (
            0,
            leds_sent,
            leds_to_send,
            led_data
        );
    }

    SendDirectApply();
}

void AuraAddressableController::SetMode
    (
    unsigned char mode,
    unsigned char red,
    unsigned char grn,
    unsigned char blu
    )
{
    SendEffect
    (
        mode,
        red,
        grn,
        blu
    );
}

void AuraAddressableController::SendEffect
    (
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
    usb_buf[0x01]   = AURA_CONTROL_MODE_EFFECT;
    usb_buf[0x02]   = 0x00;
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
    hid_send_feature_report(dev, usb_buf, 64);
}

void AuraAddressableController::SendDirect
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
    usb_buf[0x01]   = AURA_CONTROL_MODE_DIRECT;
    usb_buf[0x02]   = device;
    usb_buf[0x03]   = start_led;
    usb_buf[0x04]   = led_count;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x05], led_data, led_count * 3);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 64);
}

void AuraAddressableController::SendDirectBegin()
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
    usb_buf[0x01]   = AURA_CONTROL_MODE_EFFECT;
    usb_buf[0x04]   = 0xFF;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 64);
}

void AuraAddressableController::SendDirectApply()
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
    usb_buf[0x02]   = 0x80;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 64);
}
