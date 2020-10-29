/*-------------------------------------------------------------------*\
|  CMARGBController.cpp                                               |
|                                                                     |
|  Driver for Coolermaster ARGB USB Controller                        |
|                                                                     |
|  Chris M (Dr_No)          10th Oct 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMARGBcontroller.h"

static unsigned char argb_colour_index_data[2][2][2] =
{     //B0    B1
    { { 0x00, 0x03 },    //G0 R0
      { 0x02, 0x06 }, }, //G1 R0
    { { 0x01, 0x05 }, 	 //G0 R1
      { 0x04, 0x07 }, }  //G1 R1
};

static unsigned char argb_mode_data[2][9] =
{
    { 0x05, 0x01, 0x02, 0x02, 0x03, 0x03, 0x01, 0x01, 0x01 },    //12v RGB Mode values
    { 0x0A, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 0x09 }     //5v ARGB Mode values
};

CMARGBController::CMARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx)
{
    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    dev                     = dev_handle;
    location                = _path;
    zone_index              = _zone_idx;
    current_speed           = CM_ARGB_SPEED_NORMAL;

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());
}

CMARGBController::~CMARGBController()
{
    hid_close(dev);
}

std::string CMARGBController::GetDeviceName()
{
    return device_name;
}

std::string CMARGBController::GetSerial()
{
    return serial;
}

std::string CMARGBController::GetLocation()
{
    return location;
}

unsigned char CMARGBController::GetZoneIndex()
{
    return zone_index;
}

unsigned char CMARGBController::GetMode()
{
    return current_mode;
}

unsigned char CMARGBController::GetLedRed()
{
    return current_red;
}

unsigned char CMARGBController::GetLedGreen()
{
    return current_green;
}

unsigned char CMARGBController::GetLedBlue()
{
    return current_blue;
}

unsigned char CMARGBController::GetLedSpeed()
{
    return current_speed;
}

unsigned int CMARGBController::GetLargestColour(unsigned int red, unsigned int green, unsigned int blue)
{
    unsigned int largest;
    if ( red > green )
    {
        ( red > blue ) ? largest = red : largest = blue;
    }
    else
    {
        ( green > blue ) ? largest = green : largest = blue;
    }

    return (largest == 0) ? 1 : largest;
}

unsigned char CMARGBController::GetColourIndex(unsigned char red, unsigned char green, unsigned char blue)
{
    //The Coolermaster ARGB controller uses a limited colour pallette referenced by an index
    //Starting at 0x00 Random, 0x01 Red, 0x02 Green, 0x03 Blue, 0x04 Yellow, 0x05 Purple, 0x06 Cyan, 0x07 White
    //The index can be calculated by normalising the input colour, rounding those values 
    //and using them as the indicies of a 3d array containing the correct index
    unsigned int divisor    = GetLargestColour( red, green, blue);
    unsigned int r          = round( red / divisor );
    unsigned int g          = round( green / divisor );
    unsigned int b          = round( blue / divisor );
    unsigned char idx       = argb_colour_index_data[r][g][b];
    return idx;
}

void CMARGBController::SetMode(unsigned char mode, unsigned char speed)
{
    current_mode  = mode;
    current_speed = speed;

    SendUpdate();
}

void CMARGBController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    current_red   = red;
    current_green = green;
    current_blue  = blue;

    SendUpdate();
}

void CMARGBController::SendUpdate()
{
    unsigned char buffer[0x40]          = { 0x00 };
    int buffer_size                     = (sizeof(buffer) / sizeof(buffer[0]));
    bool boolARGB_header                = true;
    bool boolPassthru                   = ( current_mode == CM_ARGB_MODE_PASSTHRU );
    buffer[CM_ARGB_REPORT_BYTE]         = 0x80;
    buffer[CM_ARGB_COMMAND_BYTE]        = 0x01;
    buffer[CM_ARGB_FUNCTION_BYTE]       = boolPassthru ? 0x02 : 0x01;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

    if ( boolARGB_header & !boolPassthru )
    {
        buffer[CM_ARGB_COMMAND_BYTE]        = 0x0b; //ARGB sends 0x0b (1011) RGB sends 0x04 (0100)
        buffer[CM_ARGB_FUNCTION_BYTE]       = (false) ? 0x01 : 0x02; //This controls custom mode TODO
        buffer[CM_ARGB_ZONE_BYTE]           = argb_header_data[zone_index].header;
        buffer[CM_ARGB_MODE_BYTE]           = argb_mode_data[1][current_mode];
        buffer[CM_ARGB_COLOUR_INDEX_BYTE]   = GetColourIndex( current_red, current_green, current_blue );
        buffer[CM_ARGB_SPEED_BYTE]          = current_speed;

        hid_write(dev, buffer, buffer_size);
        hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);
    }
    else
    {
        //Todo: 12V Analogue Header
    }

}
