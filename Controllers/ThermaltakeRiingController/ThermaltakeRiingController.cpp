/*-----------------------------------------*\
|  ThermaltakeRiingController.cpp           |
|                                           |
|  Definitions and types for Thermaltake    |
|  Riing Plus lighting controller           |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/7/2020    |
\*-----------------------------------------*/

#include "ThermaltakeRiingController.h"

#include <cstring>

ThermaltakeRiingController::ThermaltakeRiingController(libusb_device_handle* dev_handle)
{
    dev = dev_handle;

    SendInit();

    channel_leds[0] = 9;
    channel_leds[1] = 9;
    channel_leds[2] = 9;
    channel_leds[4] = 0;
    channel_leds[4] = 0;
}

ThermaltakeRiingController::~ThermaltakeRiingController()
{

}

void ThermaltakeRiingController::SetChannelLEDs(unsigned char channel, std::vector<RGBColor> colors)
{
    unsigned char* color_data = new unsigned char[colors.size()];

    for(int color = 0; color < colors.size(); color++)
    {
        int color_idx = color * 3;
        color_data[color_idx + 0] = RGBGetGValue(colors[color]);
        color_data[color_idx + 1] = RGBGetRValue(colors[color]);
        color_data[color_idx + 2] = RGBGetBValue(colors[color]);
    }

    SendRGB(channel, THERMALTAKE_MODE_PER_LED, THERMALTAKE_SPEED_FAST, colors.size(), color_data);

    delete[] color_data;
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void ThermaltakeRiingController::SendInit()
{
    unsigned char usb_buf[64];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Init packet                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xFE;
    usb_buf[0x01]   = 0x33;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_interrupt_transfer(dev, 0x01, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x81, usb_buf, 64, &actual, 0);
}

void ThermaltakeRiingController::SendRGB
    (
        unsigned char       port,
        unsigned char       mode,
        unsigned char       speed,
        unsigned char       num_colors,
        unsigned char*      color_data
    )
{
    unsigned char usb_buf[64];
    int           actual;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up RGB packet                                     |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x32;
    usb_buf[0x01]   = 0x52;
    usb_buf[0x02]   = port;
    usb_buf[0x03]   = mode + speed;

    /*-----------------------------------------------------*\
    | Copy in GRB color data                                |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x04], color_data, (num_colors * 3));

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    libusb_interrupt_transfer(dev, 0x01, usb_buf, 64, &actual, 0);
    libusb_interrupt_transfer(dev, 0x81, usb_buf, 64, &actual, 0);
}
