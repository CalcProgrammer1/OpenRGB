/*-------------------------------------------------------------------*\
|  ThermaltakeRiingQuadController.cpp                                 |
|                                                                     |
|  Driver for Thermaltake Riing Quad Controller                       |
|                                                                     |
|  Chris M (Dr_No)          15th Feb 2021                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "ThermaltakeRiingQuadController.h"

#include <cstring>

ThermaltakeRiingQuadController::ThermaltakeRiingQuadController(hid_device* dev_handle, const char* path)
{
    wchar_t tmpName[HID_MAX_STR];

    dev         = dev_handle;
    location    = path;

    hid_get_manufacturer_string(dev, tmpName, HID_MAX_STR);
    std::wstring wName = std::wstring(tmpName);
    device_name = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, HID_MAX_STR);
    wName = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, HID_MAX_STR);
    wName = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

    SendInit();

    /*-----------------------------------------------------*\
    | The Riing Quad only seems to run in direct mode and   |
    | requires a packet within seconds to remain in the     |
    | set mode (similar to Corsair Node Pro). Start a thread|
    | to send a packet every TT_QUAD_KEEPALIVE seconds      |
    \*-----------------------------------------------------*/
    memset(tt_quad_buffer, 0x00, sizeof(tt_quad_buffer));
    unsigned char temp_buffer[3]    = { 0x00, 0x32, 0x52 };

    for(std::size_t zone_index = 0; zone_index < THERMALTAKE_QUAD_NUM_CHANNELS; zone_index++)
    {
        /*-------------------------------------------------*\
        | Add the constant bytes for the mode info buffer   |
        \*-------------------------------------------------*/
        memcpy(&tt_quad_buffer[zone_index][0], temp_buffer, 3);
    }

    keepalive_thread_run = 1;
    keepalive_thread = new std::thread(&ThermaltakeRiingQuadController::KeepaliveThread, this);
}

ThermaltakeRiingQuadController::~ThermaltakeRiingQuadController()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    hid_close(dev);
}

void ThermaltakeRiingQuadController::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::seconds(THERMALTAKE_QUAD_KEEPALIVE))
        {
            SendBuffer();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::string ThermaltakeRiingQuadController::GetDeviceName()
{
    return device_name;
}

std::string ThermaltakeRiingQuadController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string ThermaltakeRiingQuadController::GetSerial()
{
    return(serial);
}

void ThermaltakeRiingQuadController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    unsigned char* color_data = new unsigned char[3 * num_colors];

    for(std::size_t color = 0; color < num_colors; color++)
    {
        int color_idx = color * 3;
        color_data[color_idx + 0]   = RGBGetGValue(colors[color]);
        color_data[color_idx + 1]   = RGBGetRValue(colors[color]);
        color_data[color_idx + 2]   = RGBGetBValue(colors[color]);
    }

    tt_quad_buffer[channel][THERMALTAKE_QUAD_ZONE_BYTE]     = channel + 1;
    tt_quad_buffer[channel][THERMALTAKE_QUAD_MODE_BYTE]     = current_mode + ( current_speed & 0x03 );
    memcpy(&tt_quad_buffer[channel][THERMALTAKE_QUAD_DATA_BYTE], color_data, (num_colors * 3));
   
    hid_write(dev, tt_quad_buffer[channel], THERMALTAKE_QUAD_PACKET_SIZE);

    delete[] color_data;
}

void ThermaltakeRiingQuadController::SetMode(unsigned char mode, unsigned char speed)
{
    current_mode        = mode;
    current_speed       = speed;
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void ThermaltakeRiingQuadController::SendInit()
{
    unsigned char usb_buf[THERMALTAKE_QUAD_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Init packet                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = 0xFE;
    usb_buf[0x02]   = 0x33;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, THERMALTAKE_QUAD_PACKET_SIZE);
    hid_read_timeout(dev, usb_buf, THERMALTAKE_QUAD_PACKET_SIZE, THERMALTAKE_QUAD_INTERRUPT_TIMEOUT);
}

void ThermaltakeRiingQuadController::SendBuffer()
{
    for(std::size_t channel_index = 0; channel_index < THERMALTAKE_QUAD_NUM_CHANNELS; channel_index++)
    {
        hid_write(dev, tt_quad_buffer[channel_index], THERMALTAKE_QUAD_PACKET_SIZE);
    }

    /*-------------------------------------*\
    | Update the last commit time           |
    \*-------------------------------------*/
    last_commit_time = std::chrono::steady_clock::now();
}
