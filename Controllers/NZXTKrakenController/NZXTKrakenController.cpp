/*---------------------------------------------------------*\
|  Driver for NZXT Kraken                                   |
|                                                           |
|  Martin Hartl (inlart), 04/04/2020                        |
\*---------------------------------------------------------*/

#include "NZXTKrakenController.h"

#include <string>
#include <sstream>
#include <cstring>

const int NZXT_KRAKEN_READ_ENDPOINT = 0x81;
const int NZXT_KRAKEN_WRITE_ENDPOINT = 0x01;

static void SetColor(const std::vector<RGBColor>& colors, unsigned char* color_data)
{
    for (std::size_t idx = 0; idx < colors.size(); idx++)
    {
        int pixel_idx = idx * 3;
        RGBColor color = colors[idx];
        color_data[pixel_idx + 0x00] = RGBGetRValue(color);
        color_data[pixel_idx + 0x01] = RGBGetGValue(color);
        color_data[pixel_idx + 0x02] = RGBGetBValue(color);
    }
}

static RGBColor ToLogoColor(RGBColor rgb)
{
    return ToRGBColor(RGBGetGValue(rgb), RGBGetRValue(rgb), RGBGetBValue(rgb));
}

NZXTKrakenController::NZXTKrakenController(libusb_device_handle* dev_handle)
{
    dev = dev_handle;

    /*-----------------------------------------------------*\
    | Get the firmware version                              |
    \*-----------------------------------------------------*/
    UpdateStatus();
}

NZXTKrakenController::~NZXTKrakenController()
{

}

std::string NZXTKrakenController::GetFirmwareVersion()
{
    return firmware_version;
}

void NZXTKrakenController::UpdateStatus()
{
    int actual;
    unsigned char usb_buf[64];
    memset(usb_buf, 0, sizeof(usb_buf));

    libusb_interrupt_transfer(dev, NZXT_KRAKEN_READ_ENDPOINT, usb_buf, sizeof(usb_buf), &actual, 0);

    /*-----------------------------------------------------*\
    | Extract cooler information                            |
    \*-----------------------------------------------------*/
    liquid_temperature = usb_buf[0x1] + (usb_buf[0x2] * 0.1);
    fan_speed = usb_buf[0x3] << 8 | usb_buf[0x4];
    pump_speed = usb_buf[0x5] << 8 | usb_buf[0x6];

    /*-----------------------------------------------------*\
    | Extract firmware version                              |
    \*-----------------------------------------------------*/
    int major = usb_buf[0xb];
    int minor = usb_buf[0xc] << 8 | usb_buf[0xd];
    int patch = usb_buf[0xe];
    std::stringstream ss;
    ss << major << '.' << minor << '.' << patch;
    firmware_version = ss.str();
}

void NZXTKrakenController::UpdateEffect
    (
    NZXTKrakenChannel_t channel,
    unsigned char mode,
    bool direction,
    unsigned char speed,
    int seq,
    std::vector<RGBColor> colors
    )
{
    unsigned char color_data[9 * 3];
    memset(color_data, 0, sizeof(color_data));

    if(!colors.empty() && channel != NZXT_KRAKEN_CHANNEL_RING)
    {
        colors[0] = ToLogoColor(colors[0]);
    }

    SetColor(colors, color_data);
    SendEffect(channel, mode, direction, color_data, speed, false, seq);
}

void NZXTKrakenController::SendEffect
    (
    unsigned char   channel,
    unsigned char   mode,
    bool            direction,
    unsigned char*  color_data,
    unsigned char   speed /* = 0x02 */,
    bool            movement /* = false */,
    int             cis /* = 0 */,
    int             size /* = 0 */
    )
{
    int actual;
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set effect mode                                       |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x02;
    usb_buf[0x01]   = 0x4c;

    /*-----------------------------------------------------*\
    | Set effect channel, movement and direction            |
    \*-----------------------------------------------------*/
    usb_buf[0x02]   = channel;
    usb_buf[0x02]   |= movement ? ( 1 << 3 ) : 0;
    usb_buf[0x02]   |= direction ? ( 1 << 4 ) : 0;

    /*-----------------------------------------------------*\
    | Set mode                                              |
    \*-----------------------------------------------------*/
    usb_buf[0x03]   = mode;

    /*-----------------------------------------------------*\
    | Set effect speed, size and color in set               |
    \*-----------------------------------------------------*/
    usb_buf[0x04]   = speed;
    usb_buf[0x04]   |= size << 3;
    usb_buf[0x04]   |= cis << 5;

    /*-----------------------------------------------------*\
    | Copy color data bytes                                 |
    \*-----------------------------------------------------*/
    if(color_data)
    {
        memcpy(usb_buf + 0x05, color_data, 9 * 3);
    }

    /*-----------------------------------------------------*\
    | Send effect                                           |
    \*-----------------------------------------------------*/
    libusb_interrupt_transfer
    (
        dev,
        NZXT_KRAKEN_WRITE_ENDPOINT,
        usb_buf,
        sizeof(usb_buf),
        &actual,
        0
    );
}
