/*---------------------------------------------------------------------*\
|  StrimerLConnectController.cpp                                        |
|                                                                       |
|  Driver for StrimerLConnect USB Controller                            |
|                                                                       |
|  Chris M (Dr_No)          03 Jul 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#include "StrimerLConnectController.h"

static uint8_t speed_data[5] =
{
    0x02, 0x01, 0x00, 0xFE, 0xFF    /* Slow to fast */
};

static uint8_t brightness_data[5] =
{
    0x08, 0x03, 0x02, 0x01, 0x00    /* 0%, 25%, 50%, 75%, 100% */
};

StrimerLConnectController::StrimerLConnectController(hid_device* dev_handle, const char* path)
{
    const uint8_t   sz  = HID_MAX_STR;
    wchar_t         tmp[sz];

    dev                 = dev_handle;
    location            = path;

    hid_get_manufacturer_string(dev, tmp, sz);
    std::wstring wName  = std::wstring(tmp);
    device_name         = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmp, sz);
    wName               = std::wstring(tmp);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));
}

StrimerLConnectController::~StrimerLConnectController()
{
    hid_close(dev);
}

std::string StrimerLConnectController::GetDeviceName()
{
    return device_name;
}

std::string StrimerLConnectController::GetSerial()
{
    const uint8_t   sz  = HID_MAX_STR;
    wchar_t         tmp[sz];

    int ret             = hid_get_serial_number_string(dev, tmp, sz);

    if (ret != 0)
    {
        LOG_DEBUG("[%s] Get HID Serial string failed", device_name.c_str());
        return("");
    }

    std::wstring w_tmp  = std::wstring(tmp);
    std::string serial  = std::string(w_tmp.begin(), w_tmp.end());

    return serial;
}

std::string StrimerLConnectController::GetLocation()
{
    return("HID: " + location);
}

void StrimerLConnectController::SendApply()
{
    uint8_t buffer[STRIMERLCONNECT_PACKET_SIZE] = { STRIMERLCONNECT_REPORT_ID, 0x2C, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00 };

    hid_write(dev, buffer, STRIMERLCONNECT_PACKET_SIZE);
}

void StrimerLConnectController::SetMode(uint8_t mode, uint8_t zone, uint8_t speed, uint8_t brightness, uint8_t direction, bool /*random_colours*/)
{
    uint8_t buffer[STRIMERLCONNECT_PACKET_SIZE] = { STRIMERLCONNECT_REPORT_ID, STRIMERLCONNECT_MODE_COMMAND, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    buffer[STRIMERLCONNECT_COMMAND_BYTE]       |= zone;

    buffer[STRIMERLCONNECT_DATA_BYTE]           = mode;
    buffer[STRIMERLCONNECT_SPEED_BYTE]          = speed_data[speed];
    buffer[STRIMERLCONNECT_DIRECTION_BYTE]      = (direction == 0) ? 1 : 0;
    buffer[STRIMERLCONNECT_BRIGHTNESS_BYTE]     = brightness_data[brightness];

    hid_write(dev, buffer, STRIMERLCONNECT_PACKET_SIZE);
}

void StrimerLConnectController::SetLedsDirect(uint8_t zone, RGBColor * led_colours, uint8_t led_count)
{
    uint8_t buffer[STRIMERLCONNECT_PACKET_SIZE] = { STRIMERLCONNECT_REPORT_ID, STRIMERLCONNECT_COLOUR_COMMAND, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    buffer[STRIMERLCONNECT_COMMAND_BYTE]       |= zone;

    for(size_t i = 0; i < led_count; i++)
    {
        uint8_t offset                          = (3 * i) + STRIMERLCONNECT_DATA_BYTE;

        buffer[offset]                          = RGBGetRValue(led_colours[i]);
        buffer[offset + 1]                      = RGBGetBValue(led_colours[i]);
        buffer[offset + 2]                      = RGBGetGValue(led_colours[i]);
    }

    hid_write(dev, buffer, STRIMERLCONNECT_PACKET_SIZE);
}
