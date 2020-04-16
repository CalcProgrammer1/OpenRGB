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

    GetFirmwareVersion();
    GetConfigTable();
}

AuraAddressableController::~AuraAddressableController()
{

}

unsigned int AuraAddressableController::GetChannelCount()
{
    return( 1 );
}

std::string AuraAddressableController::GetDeviceName()
{
    return(device_name);
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

    SendDirectApply(channel);
}

void AuraAddressableController::SetMode
    (
    unsigned char mode,
    unsigned char red,
    unsigned char grn,
    unsigned char blu
    )
{
    for(int channel_idx = 0; channel_idx < GetChannelCount(); channel_idx++)
    {
        SendEffect
        (
            channel_idx,
            mode,
            red,
            grn,
            blu
        );
    }
}

void AuraAddressableController::GetConfigTable()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_REQUEST_CONFIG_TABLE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Copy the firmware string if the reply ID is correct   |
    \*-----------------------------------------------------*/
    if(usb_buf[1] == 0x30)
    {
        memcpy(config_table, &usb_buf[4], 60);

        for(int i = 0; i < 60; i+=6)
        {
            printf("%02X %02X %02X %02X %02X %02X\r\n", config_table[i + 0],
                                                        config_table[i + 1],
                                                        config_table[i + 2],
                                                        config_table[i + 3],
                                                        config_table[i + 4],
                                                        config_table[i + 5]);
        }
    }
}

void AuraAddressableController::GetFirmwareVersion()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up firmware version request packet                |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_REQUEST_FIRMWARE_VERSION;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Copy the firmware string if the reply ID is correct   |
    \*-----------------------------------------------------*/
    if(usb_buf[1] == 0x02)
    {
        memcpy(device_name, &usb_buf[2], 16);
    }
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
    usb_buf[0x01]   = AURA_CONTROL_MODE_EFFECT;
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
