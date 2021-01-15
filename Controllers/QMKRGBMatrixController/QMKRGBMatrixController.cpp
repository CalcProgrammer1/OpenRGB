/*-------------------------------------------------------------------*\
|  QMKRGBMatrixController.cpp                                         |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper         10th Octobber 2020                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "QMKRGBMatrixController.h"
#include <cstring>

QMKRGBMatrixController::QMKRGBMatrixController
    (
    hid_device* dev_handle, 
    const char* path, 
    unsigned short vendor_id
    )
{
    dev = dev_handle;
    location = path;

    unsigned int massdrop_vid = 0x04D8;
    // ATSAM (MD) boards will crash eventually if this is higher than 2
    leds_per_update = vendor_id == massdrop_vid ? 2 : 9;
}

QMKRGBMatrixController::~QMKRGBMatrixController()
{
    hid_close(dev);
}

std::string QMKRGBMatrixController::GetLocation()
{
    return location;
}

void QMKRGBMatrixController::DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[33];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_DIRECT_MODE_SET_SINGLE_LED;
    usb_buf[0x02]   = led;
    usb_buf[0x03]   = red;
    usb_buf[0x04]   = green;
    usb_buf[0x05]   = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 33);
    hid_read(dev, usb_buf, 33);
}

void QMKRGBMatrixController::DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int leds_count)
{
    unsigned int leds_sent = 0;

    while(leds_sent < leds_count)
    {
        if((leds_count - leds_sent) < leds_per_update)
        {
            leds_per_update = leds_count - leds_sent;
        }

        unsigned char usb_buf[33];
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_RGBMATRIX_DIRECT_MODE_SET_LEDS;
        usb_buf[0x02] = leds_sent;
        usb_buf[0x03] = leds_per_update;

        for(unsigned int led_idx = 0; led_idx < leds_per_update; led_idx++)
        {
            usb_buf[(led_idx * 3) + 4] = RGBGetRValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 5] = RGBGetGValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 6] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        hid_write(dev, usb_buf, 33);

        leds_sent += leds_per_update;
    }
}

RGBColor QMKRGBMatrixController::DirectModeGetLEDColor(unsigned int led)
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_DIRECT_MODE_GET_LED_COLOR;
    usb_buf[0x02] = led;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return ToRGBColor(usb_buf[1], usb_buf[2], usb_buf[3]);
}

void QMKRGBMatrixController::QMKModeSetModeAndSpeed(unsigned char mode, unsigned char speed)
{
    unsigned char usb_buf[33];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_QMK_MODE_SET_MODE_AND_SPEED;
    usb_buf[0x02]   = mode;
    usb_buf[0x03]   = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 33);
    hid_read(dev, usb_buf, 33);
}

void QMKRGBMatrixController::QMKModeSetColorModeAndSpeed(hsv_t hsv_color, unsigned char mode, unsigned char speed)
{
    unsigned char usb_buf[33];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_QMK_MODE_SET_COLOR_MODE_AND_SPEED;
    // qmk hue range is between 0-255, instead of the regular 0-359, so hue needs to be converted
    usb_buf[0x02]   = hsv_color.hue * 255 / 359;
    usb_buf[0x03]   = hsv_color.saturation;
    usb_buf[0x04]   = hsv_color.value;
    usb_buf[0x05]   = mode;
    usb_buf[0x06]   = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 33);
    hid_read(dev, usb_buf, 33);
}

unsigned int QMKRGBMatrixController::QMKModeGetColor()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_QMK_MODE_GET_COLOR;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    // qmk hue range is between 0-255, instead of the regular 0-359, so hue needs to be converted
    unsigned int oldRange = 255;
    unsigned int newRange = 359;
    unsigned int convertedHue = (usb_buf[1] * newRange / oldRange);

    hsv_t hsv;
    hsv.hue = convertedHue;
    hsv.saturation = usb_buf[2];
    hsv.value = usb_buf[3];

    return hsv2rgb(&hsv);
}

unsigned int QMKRGBMatrixController::GetProtocolVersion()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_PROTOCOL_VERSION;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

std::string QMKRGBMatrixController::GetDeviceName()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_DEVICE_NAME;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    int i = 1;
    std::string name;
    while(usb_buf[i] != 0)
    {
        name.push_back(usb_buf[i]);
        i++;
    }

    return name;
}

std::string QMKRGBMatrixController::GetDeviceVendor()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_DEVICE_VENDOR;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    int i = 1;
    std::string vendor;
    while(usb_buf[i] != 0)
    {
        vendor.push_back(usb_buf[i]);
        i++;
    }

    return vendor;
}

bool QMKRGBMatrixController::GetEnabledModes(int mode)
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ENABLED_MODES;
    usb_buf[0x02] = mode;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1] == QMK_RGBMATRIX_SUCCESS ? true : false;
}

unsigned int QMKRGBMatrixController::GetActiveMode()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ACTIVE_MODE;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetZonesCount()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ZONES_COUNT;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

std::string QMKRGBMatrixController::GetZoneName(int zone)
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ZONE_NAME;
    usb_buf[0x02] = zone;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    int i = 1;
    std::string name;
    while(usb_buf[i] != 0)
    {
        name.push_back(usb_buf[i]);
        i++;
    }

    return name;
}

unsigned int QMKRGBMatrixController::GetZoneType(int zone)
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ZONE_TYPE;
    usb_buf[0x02] = zone;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetZoneSize(int zone)
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ZONE_SIZE;
    usb_buf[0x02] = zone;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetLEDMatirxRows()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_MATRIX_ROWS;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetLEDMatirxColumns()
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_MATRIX_COLUMNS;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetLEDValueInMatrix(int column, int row)
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_VALUE_IN_MATRIX;
    usb_buf[0x02] = column;
    usb_buf[0x03] = row;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return usb_buf[1];
}

std::string QMKRGBMatrixController::GetLEDName(int led_column, int led_row)
{
    unsigned char usb_buf[33];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_NAME;
    usb_buf[0x02] = led_column;
    usb_buf[0x03] = led_row;

    hid_write(dev, (unsigned char*)usb_buf, 33);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 33, 1000);

    return QMKKeycodeToKeyNameMap[usb_buf[1]];
}
