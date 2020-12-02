/*-------------------------------------------------------------------*\
|  TecknetController.cpp                                              |
|                                                                     |
|  Driver for Technet Devices                                         |
|                                                                     |
|  Chris M (Dr_No)          29th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "TecknetController.h"

static unsigned char tecknet_colour_mode_data[][16] =
{
    { 0x02, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00 },           // Direct
    { 0x02, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00 },           // Breathing
};

static unsigned char tecknet_speed_mode_data[][9] =
{
    { 0x00, 0x00, 0x00, 0x00 },                             // Direct
    { 0x00, 0x06, 0x03, 0x01 },                             // Breathing
};

TecknetController::TecknetController(hid_device* dev_handle, char *_path)
{
    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    dev = dev_handle;

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

    location = _path;

    current_mode        = TECKNET_MODE_DIRECT;
    current_speed       = TECKNET_SPEED_NORMAL;
    current_brightness  = TECKNET_BRIGHTNESS_HIGH;
}

TecknetController::~TecknetController()
{
    hid_close(dev);
}

std::string TecknetController::GetDeviceName()
{
    return device_name;
}

std::string TecknetController::GetSerial()
{
    return serial;
}

std::string TecknetController::GetLocation()
{
    return("HID: " + location);
}

void TecknetController::SetMode(unsigned char mode, unsigned char speed, unsigned char brightness)
{
    current_mode        = mode;
    current_speed       = speed;
    current_brightness  = brightness;

    SendUpdate();
}

void TecknetController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    //The Tecknet mouse expects inverted colours in sent packets
    current_red   = 255 - red;
    current_green = 255 - green;
    current_blue  = 255 - blue;

    SendUpdate();
}

void TecknetController::SendUpdate()
{
    unsigned char buffer[TECKNET_PACKET_LENGTH] = { 0x00 };
    int buffer_size = (sizeof(buffer) / sizeof(buffer[0]));

    for(std::size_t i = 0; i < TECKNET_COLOUR_MODE_DATA_SIZE; i++)
    {
        buffer[i] = tecknet_colour_mode_data[current_mode][i];
    }

    //Set the relevant colour info
    buffer[TECKNET_RED_BYTE]        = current_red;
    buffer[TECKNET_GREEN_BYTE]      = current_green;
    buffer[TECKNET_BLUE_BYTE]       = current_blue;
    buffer[TECKNET_BRIGHTNESS_BYTE] = current_brightness;
    buffer[TECKNET_SPEED_BYTE]      = tecknet_speed_mode_data[current_mode][current_speed];

    hid_send_feature_report(dev, buffer, buffer_size);
}

