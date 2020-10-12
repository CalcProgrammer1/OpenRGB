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
{     //R0    R1
    { { 0x00, 0x01 },    //G0 B0 
      { 0x02, 0x04 }, }, //G1 B0
    { { 0x03, 0x05 }, 	 //G0 B1
      { 0x06, 0x07 }, }  //G1 B1
};

static unsigned char argb_mode_data[2][9] =
{
    { 0x05, 0x01, 0x02, 0x02, 0x03, 0x03, 0x01, 0x01, 0x01 },    //12v RGB Mode values
    { 0x0A, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 0x09 }     //5v ARGB Mode values
};

CMARGBController::CMARGBController(hid_device* dev_handle, wchar_t *_vendor, wchar_t *_device_name, char *_path)
{
    std::size_t tmp_size    = wcslen(_vendor);
    dev                     = dev_handle;
    location                = _path;

    for(std::size_t i = 0; (i < tmp_size) && (i < CM_ARGB_DEVICE_NAME_SIZE); i++)
    {
        device_name[i] = (char)_vendor[i];
    }
    for(std::size_t j = 0; (j < wcslen(_vendor)) && (tmp_size + j < CM_ARGB_DEVICE_NAME_SIZE); j++)
    {
        device_name[tmp_size + j] = (char)_device_name[j];
    }
}

CMARGBController::~CMARGBController()
{
    hid_close(dev);
}

char* CMARGBController::GetDeviceName()
{
    return device_name;
}

char* CMARGBController::GetSerial()
{
    return serial;
}

std::string CMARGBController::GetLocation()
{
    return location;
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

unsigned char CMARGBController::GetLargestColour(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char largest;
    if ( red > green )
    {
        ( red > blue ) ? largest = red : largest = blue;
    }
    else
    {
        ( green > blue ) ? largest = green : largest = blue;
    }

    return largest;
}

unsigned char CMARGBController::GetColourIndex(unsigned char red, unsigned char green, unsigned char blue)
{
    //The Coolermaster ARGB controller uses a limited colour pallette referenced by an index
    //Starting at 0x00 Random, 0x01 Red, 0x02 Green, 0x03 Blue, 0x04 Yellow, 0x05 Purple, 0x06 Cyan, 0x07 White
    //The index can be calculated by normalising the input colour, rounding those values 
    //and using them as the indicies of a 3d array containing the correct index
    unsigned char divisor   = GetLargestColour( red, green, blue);
    unsigned int r          = round( red / divisor )+1;
    unsigned int g          = round( green / divisor )+1;
    unsigned int b          = round( blue / divisor )+1;
    return argb_colour_index_data[r][g][b];
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

void CMARGBController::SendUpdate(unsigned char zone)
{
    unsigned char buffer[0x40]          = { 0x00 };
    int buffer_size                     = (sizeof(buffer) / sizeof(buffer[0]));
    bool boolARGB_header                = true;
    buffer[CM_ARGB_REPORT_BYTE]         = 0x80;
    buffer[CM_ARGB_COMMAND_BYTE]        = 0x01;
    buffer[CM_ARGB_MODE_BYTE]           = 0x01;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT );

    buffer[CM_ARGB_COMMAND_BYTE]        = (boolARGB_header) ? 0x0b : 0x04; //ARGB sends 0x0b (1011) RGB sends 0x04 (0100)
    buffer[CM_ARGB_MODE_BYTE]           = current_mode;
    buffer[CM_ARGB_ZONE_BYTE]           = zone;
    buffer[CM_ARGB_COLOUR_INDEX_BYTE]   = GetColourIndex( current_red, current_green, current_blue );
    buffer[CM_ARGB_SPEED_BYTE]          = current_speed;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT );
}
