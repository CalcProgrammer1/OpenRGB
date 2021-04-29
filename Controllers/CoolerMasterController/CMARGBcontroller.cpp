/*-------------------------------------------------------------------*\
|  CMARGBController.cpp                                               |
|                                                                     |
|  Driver for Coolermaster ARGB USB Controller                        |
|                                                                     |
|  Chris M (Dr_No)          10th Oct 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMARGBcontroller.h"
#include <cstring>

static unsigned char argb_colour_index_data[2][2][2] =
{     //B0    B1
    { { 0x00, 0x03 },    //G0 R0
      { 0x02, 0x06 }, }, //G1 R0
    { { 0x01, 0x05 }, 	 //G0 R1
      { 0x04, 0x07 }, }  //G1 R1
};

CMARGBController::CMARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx)
{
    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    dev                     = dev_handle;
    location                = _path;
    zone_index              = _zone_idx;

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName = std::wstring(tmpName);
    device_name = std::string(wName.begin(), wName.end());

    hid_get_product_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    device_name.append(" ").append(std::string(wName.begin(), wName.end()));

    hid_get_serial_number_string(dev, tmpName, szTemp);
    wName = std::wstring(tmpName);
    serial = std::string(wName.begin(), wName.end());

    GetStatus();
}

CMARGBController::~CMARGBController()
{
    hid_close(dev);
}

void CMARGBController::GetStatus()
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = { 0x00, 0x80, 0x0B, 0x01 };
    int buffer_size                             = (sizeof(buffer) / sizeof(buffer[0]));
    int rgb_offset                              = 0;
    int zone;

    if (argb_header_data[zone_index].digital)
    {
        zone                                    = argb_header_data[zone_index].header;
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0B;
    }
    else
    {
        zone                                    = 0x00;
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0A;
        rgb_offset                              = 1;
    }

    /*---------------------------------------------------------*\
    | If this is the group then just return the first status    |
    \*---------------------------------------------------------*/
    buffer[CM_ARGB_ZONE_BYTE]                   = ( zone > 0x08 ) ? 0x01 : zone;
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

    current_mode                                = buffer[4 - rgb_offset];
    bool_random                                 = ( buffer[5 - rgb_offset] == 0x00 );
    current_speed                               = buffer[6 - rgb_offset];
    current_brightness                          = buffer[7 - rgb_offset];
    current_red                                 = buffer[8 - rgb_offset];
    current_green                               = buffer[9 - rgb_offset];
    current_blue                                = buffer[10 - rgb_offset];
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
    return("HID: " + location);
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

bool CMARGBController::GetRandomColours()
{
    return bool_random;
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
    /*---------------------------------------------------------------------------------------------------------*\
    | The Cooler Master ARGB controller V0008 uses a limited colour pallette referenced by an index             |
    | Starting at 0x00 Random, 0x01 Red, 0x02 Green, 0x03 Blue, 0x04 Yellow, 0x05 Purple, 0x06 Cyan, 0x07 White |
    | The index can be calculated by normalising the input colour, rounding those values                        |
    | and using them as the indicies of a 3d array containing the correct index                                 |
    \*---------------------------------------------------------------------------------------------------------*/
    unsigned int divisor    = GetLargestColour( red, green, blue);
    unsigned int r          = round( red / divisor );
    unsigned int g          = round( green / divisor );
    unsigned int b          = round( blue / divisor );
    unsigned char idx       = argb_colour_index_data[r][g][b];

    return idx;
}

void CMARGBController::SetLedCount(int zone, int led_count)
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = { 0x00, 0x80, 0x0D, 0x02 };
    int  buffer_size                            = (sizeof(buffer) / sizeof(buffer[0]));

    buffer[CM_ARGB_ZONE_BYTE]                   = zone;
    buffer[CM_ARGB_MODE_BYTE]                   = led_count;
    buffer[CM_ARGB_COLOUR_INDEX_BYTE]           = (0x0F - led_count > 0) ? 0x0F - led_count : 0x01;

    hid_write(dev, buffer, buffer_size);
}

void CMARGBController::SetMode(unsigned char mode, unsigned char speed, RGBColor colour, bool random_colours)
{
    bool needs_update   = !( (current_mode == mode) && (current_speed == speed) && (current_brightness == 0xFF) && (ToRGBColor(current_red, current_green, current_blue) == colour));

    if (needs_update)
    {
        current_mode        = mode;
        current_speed       = speed;
        current_brightness  = 0xFF;
        current_red         = RGBGetRValue(colour);
        current_green       = RGBGetGValue(colour);
        current_blue        = RGBGetBValue(colour);
        bool_random         = random_colours;

        SendUpdate();
    }
}

void CMARGBController::SetLedsDirect(RGBColor *led_colours, unsigned int led_count)
{
    const unsigned char buffer_size     = CM_ARGB_PACKET_SIZE;
    unsigned char buffer[buffer_size]   = { 0x00, 0x10, 0x02 };
    unsigned char packet_count          = 0;
    std::vector<unsigned char> colours;

    /*---------------------------------------------*\
    | Set up the RGB triplets to send               |
    \*---------------------------------------------*/
    for(unsigned int i = 0; i < led_count; i++)
    {
        RGBColor      colour            = led_colours[i];

        colours.push_back( RGBGetRValue(colour) );
        colours.push_back( RGBGetGValue(colour) );
        colours.push_back( RGBGetBValue(colour) );
    }

    buffer[CM_ARGB_ZONE_BYTE]           = argb_header_data[zone_index].header;
    buffer[CM_ARGB_MODE_BYTE]           = led_count;
    unsigned char buffer_idx            = CM_ARGB_MODE_BYTE + 1;

    for(std::vector<unsigned char>::iterator it = colours.begin(); it != colours.end(); buffer_idx = 0)
    {
        /*-----------------------------------------------------------------*\
        | Fill the write buffer till its full or the colour buffer is empty |
        \*-----------------------------------------------------------------*/
        buffer[CM_ARGB_REPORT_BYTE] = packet_count;
        while (( buffer_idx < buffer_size) && ( it != colours.end() ))
        {
            buffer[buffer_idx] = *it;
            buffer_idx++;
            it++;
        }

        hid_write(dev, buffer, buffer_size);
        hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

        /*-----------------------------------------------------------------*\
        | Reset the write buffer                                            |
        \*-----------------------------------------------------------------*/
        memset(buffer, 0x00, buffer_size );
        packet_count++;
    }

    buffer[CM_ARGB_REPORT_BYTE]         = 0x82;
    /*buffer[CM_ARGB_COMMAND_BYTE]        = 0x62;
    buffer[CM_ARGB_FUNCTION_BYTE]       = 0x00;
    buffer[CM_ARGB_ZONE_BYTE]           = 0x73;
    buffer[CM_ARGB_MODE_BYTE]           = 0x00;
    buffer[CM_ARGB_COLOUR_INDEX_BYTE]   = 0x33;
    buffer[CM_ARGB_SPEED_BYTE]          = 0x1B;*/

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);
}

void CMARGBController::SendUpdate()
{
    unsigned char buffer[CM_ARGB_PACKET_SIZE]   = { 0x00 };
    int  buffer_size                            = (sizeof(buffer) / sizeof(buffer[0]));
    bool boolARGB_header                        = argb_header_data[zone_index].digital;
    bool boolPassthru                           = ( current_mode == CM_ARGB_MODE_PASSTHRU ) || ( current_mode == CM_RGB_MODE_PASSTHRU );
    bool boolDirect                             = ( current_mode == CM_ARGB_MODE_DIRECT );
    unsigned char function                      = boolPassthru ? (boolARGB_header ? 0x02 : 0x04) : (boolARGB_header ? 0x01 : 0x03);
    buffer[CM_ARGB_REPORT_BYTE]                 = 0x80;
    buffer[CM_ARGB_COMMAND_BYTE]                = 0x01;
    buffer[CM_ARGB_FUNCTION_BYTE]               = boolDirect ? 0x01 : function;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);

    /*-----------------------------------------------------------------*\
    | Direct mode is now set up and no other mode packet is required    |
    \*-----------------------------------------------------------------*/
    if(boolDirect)
    {
        return;
    }

    if(boolARGB_header)
    {
        buffer[CM_ARGB_COMMAND_BYTE]            = 0x0b;                                     //ARGB sends 0x0b (1011) RGB sends 0x04 (0100)
        buffer[CM_ARGB_FUNCTION_BYTE]           = (false) ? 0x01 : 0x02;                    //This controls direct mode TODO
        buffer[CM_ARGB_ZONE_BYTE]               = argb_header_data[zone_index].header;
        buffer[CM_ARGB_MODE_BYTE]               = current_mode;
        buffer[CM_ARGB_COLOUR_INDEX_BYTE]       = bool_random ? 0x00 : 0x10;
        buffer[CM_ARGB_SPEED_BYTE]              = current_speed;
        buffer[CM_ARGB_BRIGHTNESS_BYTE]         = current_brightness;
        buffer[CM_ARGB_RED_BYTE]                = current_red;
        buffer[CM_ARGB_GREEN_BYTE]              = current_green;
        buffer[CM_ARGB_BLUE_BYTE]               = current_blue;

        hid_write(dev, buffer, buffer_size);
        hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);
    }
    else
    {
        buffer[CM_ARGB_COMMAND_BYTE]                        = boolPassthru ? 0x01 : 0x04;   //ARGB sends 0x0b (1011) RGB sends 0x04 (0100)
        buffer[CM_ARGB_MODE_BYTE + CM_RGB_OFFSET]           = current_mode;
        buffer[CM_ARGB_COLOUR_INDEX_BYTE + CM_RGB_OFFSET]   = bool_random ? 0x00 : 0x10;
        buffer[CM_ARGB_SPEED_BYTE + CM_RGB_OFFSET]          = current_speed;
        buffer[CM_ARGB_BRIGHTNESS_BYTE + CM_RGB_OFFSET]     = current_brightness;
        buffer[CM_ARGB_RED_BYTE + CM_RGB_OFFSET]            = current_red;
        buffer[CM_ARGB_GREEN_BYTE + CM_RGB_OFFSET]          = current_green;
        buffer[CM_ARGB_BLUE_BYTE + CM_RGB_OFFSET]           = current_blue;

        hid_write(dev, buffer, buffer_size);
        hid_read_timeout(dev, buffer, buffer_size, CM_ARGB_INTERRUPT_TIMEOUT);
    }
}
