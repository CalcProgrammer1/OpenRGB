/*---------------------------------------------------------------------*\
|  BloodyMouseController.cpp                                            |
|                                                                       |
|  Driver for BloodyMouse USB Controller                                |
|                                                                       |
|  Chris M (Dr_No)          30 Jun 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#include "BloodyMouseController.h"

BloodyMouseController::BloodyMouseController(hid_device* dev_handle, const char* path, uint16_t product_id)
{
    dev                 = dev_handle;
    location            = path;
    pid                 = product_id;

    InitDevice();
}

BloodyMouseController::~BloodyMouseController()
{
    hid_close(dev);
}

uint16_t BloodyMouseController::GetPid()
{
    return pid;
}

std::string BloodyMouseController::GetSerial()
{
    const uint8_t   sz  = HID_MAX_STR;
    wchar_t         tmp[sz];

    int ret             = hid_get_serial_number_string(dev, tmp, sz);

    if (ret != 0)
    {
        LOG_DEBUG("[BloodyMouse] Get HID Serial string failed");
        return("");
    }

    std::wstring w_tmp  = std::wstring(tmp);
    std::string serial  = std::string(w_tmp.begin(), w_tmp.end());

    return serial;
}

std::string BloodyMouseController::GetLocation()
{
    return("HID: " + location);
}

void  BloodyMouseController::InitDevice()
{
    uint8_t buffer[BLOODYMOUSE_WRITE_PACKET_SIZE]   = { 0x07, 0x03, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };

    hid_send_feature_report(dev, buffer, BLOODYMOUSE_WRITE_PACKET_SIZE);

    buffer[BLOODYMOUSE_MODE_BYTE]                   = 0;
    buffer[BLOODYMOUSE_DATA_BYTE]                   = 1;

    hid_send_feature_report(dev, buffer, BLOODYMOUSE_WRITE_PACKET_SIZE);
}

void  BloodyMouseController::SetLedsDirect(std::vector<RGBColor> colors)
{
    uint8_t buffer[BLOODYMOUSE_WRITE_PACKET_SIZE]   = { 0x07, 0x03, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00 };

    for(uint8_t i = 0; i < colors.size(); i++)
    {
        uint8_t offset                              = 3 * (colors[i] >> 24) + BLOODYMOUSE_DATA_BYTE;

        buffer[offset]                              = RGBGetRValue(colors[i]);
        buffer[offset + 1]                          = RGBGetGValue(colors[i]);
        buffer[offset + 2]                          = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, buffer, BLOODYMOUSE_WRITE_PACKET_SIZE);
}
