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

QMKRGBMatrixController::QMKRGBMatrixController(hid_device* dev_handle, const char* path)
{
    dev = dev_handle;
    location = path;
}

QMKRGBMatrixController::~QMKRGBMatrixController()
{
    hid_close(dev);
}

void QMKRGBMatrixController::SetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/

    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_SET_SINGLE_LED;
    usb_buf[0x02]   = led;
    usb_buf[0x03]   = red;
    usb_buf[0x04]   = green;
    usb_buf[0x05]   = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void QMKRGBMatrixController::SetLEDs(RGBColor* colors, unsigned int num_colors)
{
    unsigned int leds_sent = 0;

    while(leds_sent < num_colors)
    {
        unsigned int leds_to_send = 2;

        if((num_colors - leds_sent) < leds_to_send)
        {
            leds_to_send = num_colors - leds_sent;
        }

        unsigned char usb_buf[65];
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_RGBMATRIX_SET_LEDS;
        usb_buf[0x02] = leds_sent;
        usb_buf[0x03] = leds_to_send;

        for(unsigned int led_idx = 0; led_idx < leds_to_send; led_idx++)
        {
            usb_buf[(led_idx * 3) + 4] = RGBGetRValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 5] = RGBGetGValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 6] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        hid_write(dev, usb_buf, 65);

        leds_sent += leds_to_send;
    }
}

void QMKRGBMatrixController::SetMode(unsigned char mode)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_SET_MODE;
    usb_buf[0x02]   = QMK_RGBMATRIX_EEPROM_DONT_SAVE;
    usb_buf[0x03]   = mode;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void QMKRGBMatrixController::SetModeAndSpeed(unsigned char mode, unsigned char speed)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_SET_MODE_AND_SPEED;
    usb_buf[0x02]   = QMK_RGBMATRIX_EEPROM_DONT_SAVE;
    usb_buf[0x03]   = mode;
    usb_buf[0x04]   = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

void QMKRGBMatrixController::SetColorModeAndSpeed(hsv_t hsv_color, unsigned char mode, unsigned char speed)
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0x00;
    usb_buf[0x01]   = QMK_RGBMATRIX_SET_COLOR_MODE_AND_SPEED;
    usb_buf[0x02]   = QMK_RGBMATRIX_EEPROM_DONT_SAVE;

    // qmk hue range is between 0-255, instead of the regular 0-359, so hue needs to be converted
    unsigned int oldRange = 359;
    unsigned int newRange = 255;
    unsigned int convertedHue = (hsv_color.hue * newRange / oldRange);

    usb_buf[0x03]   = convertedHue;
    usb_buf[0x04]   = hsv_color.saturation;
    usb_buf[0x05]   = hsv_color.value;
    usb_buf[0x06]   = mode;
    usb_buf[0x07]   = speed;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);
}

std::string QMKRGBMatrixController::GetLocation()
{
    return location;
}

std::string QMKRGBMatrixController::GetVersion()
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_QMK_VERSION;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    int i = 1;
    std::string name;
    while(usb_buf[i] != 0)
    {
        name.push_back(usb_buf[i]);
        i++;
    }

    return name;
}

std::string QMKRGBMatrixController::GetName()
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_DEVICE_NAME;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    int i = 1;
    std::string name;
    while(usb_buf[i] != 0)
    {
        name.push_back(usb_buf[i]);
        i++;
    }

    return name;
}

unsigned int QMKRGBMatrixController::GetZonesCount()
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_ZONES_COUNT;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    return usb_buf[1];
}

std::string QMKRGBMatrixController::GetZoneName(int zone)
{
    unsigned char usb_buf[65];
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

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

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
    unsigned char usb_buf[65];
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

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetZoneSize(int zone)
{
    unsigned char usb_buf[65];
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

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    return usb_buf[1];
}

std::string QMKRGBMatrixController::GetLEDName(int led)
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_NAME;
    usb_buf[0x02] = led;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    int i = 1;
    std::string name;
    while(usb_buf[i] != 0)
    {
        name.push_back(usb_buf[i]);
        i++;
    }

    return name;
}

unsigned int QMKRGBMatrixController::GetLEDMatirxRows()
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_MATRIX_ROWS;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetLEDMatirxColumns()
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_MATRIX_COLUMNS;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    return usb_buf[1];
}

unsigned int QMKRGBMatrixController::GetLEDValueInMatrix(int column, int row)
{
    unsigned char usb_buf[65];
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

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    return usb_buf[1];
}

RGBColor QMKRGBMatrixController::GetLEDColor(unsigned int led)
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_LED_COLOR;
    usb_buf[0x02] = led;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

    return ToRGBColor(usb_buf[1], usb_buf[2], usb_buf[3]);
}

unsigned int QMKRGBMatrixController::GetHSV()
{
    unsigned char usb_buf[65];
    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_RGBMATRIX_GET_HSV;

    hid_write(dev, (unsigned char*)usb_buf, 65);
    hid_read_timeout(dev, (unsigned char*)usb_buf, 65, 1000);

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