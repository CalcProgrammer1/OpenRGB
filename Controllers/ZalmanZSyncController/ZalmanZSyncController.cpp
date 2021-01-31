#include "ZalmanZSyncController.h"

ZalmanZSyncController::ZalmanZSyncController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

ZalmanZSyncController::~ZalmanZSyncController()
{

}

std::string ZalmanZSyncController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ZalmanZSyncController::GetSerialString()
{
    return("");
}

void ZalmanZSyncController::SetLEDs(unsigned char channel, RGBColor* colors)
{
    SendDirectFrame(channel, ZALMAN_Z_SYNC_COLOR_CHANNEL_RED,   colors);
    SendDirectFrame(channel, ZALMAN_Z_SYNC_COLOR_CHANNEL_GREEN, colors);
    SendDirectFrame(channel, ZALMAN_Z_SYNC_COLOR_CHANNEL_BLUE,  colors);

    SendApply();
}

void ZalmanZSyncController::SendApply()
{
    unsigned char usb_buf[65];

    usb_buf[0]          = 0x00;
    usb_buf[1]          = 0x33;
    usb_buf[2]          = 0xFF;

    hid_write(dev, usb_buf, 65);
}

void ZalmanZSyncController::SendDirectFrame(unsigned char channel, unsigned char color_channel, RGBColor* colors)
{
    unsigned char usb_buf[65];

    usb_buf[0]          = 0x00;
    usb_buf[1]          = 0x32;
    usb_buf[2]          = channel;
    usb_buf[3]          = 0x00;
    usb_buf[4]          = 0x18;
    usb_buf[5]          = color_channel;

    switch(color_channel)
    {
        case ZALMAN_Z_SYNC_COLOR_CHANNEL_RED:
            for(unsigned int color_idx = 0; color_idx < 24; color_idx++)
            {
                usb_buf[6 + color_idx]  = RGBGetRValue(colors[color_idx]);
            }
            break;

        case ZALMAN_Z_SYNC_COLOR_CHANNEL_GREEN:
            for(unsigned int color_idx = 0; color_idx < 24; color_idx++)
            {
                usb_buf[6 + color_idx]  = RGBGetGValue(colors[color_idx]);
            }
            break;

        case ZALMAN_Z_SYNC_COLOR_CHANNEL_BLUE:
            for(unsigned int color_idx = 0; color_idx < 24; color_idx++)
            {
                usb_buf[6 + color_idx]  = RGBGetBValue(colors[color_idx]);
            }
            break;
    }

    hid_write(dev, usb_buf, 65);
}