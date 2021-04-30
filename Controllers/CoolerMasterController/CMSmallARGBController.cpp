/*-------------------------------------------------------------------*\
|  CMSmallARGBController.cpp                                          |
|                                                                     |
|  Driver for Coolermaster Small ARGB USB Controller                  |
|                                                                     |
|  Chris M (Dr_No)          31st Jan 2021                             |
|                                                                     |
|  Simple RGB device with 5 modes                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMSmallARGBController.h"
#include <cstring>

CMSmallARGBController::CMSmallARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx)
{
    const int szTemp = 256;
    wchar_t tmpName[szTemp];

    dev                     = dev_handle;
    location                = _path;
    zone_index              = _zone_idx;
    current_speed           = CM_SMALL_ARGB_SPEED_NORMAL;

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

CMSmallARGBController::~CMSmallARGBController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

void CMSmallARGBController::GetStatus()
{
    unsigned char buffer[CM_SMALL_ARGB_PACKET_SIZE]     = { 0x00, 0x80, 0x01, 0x01 };
    int buffer_size                                     = (sizeof(buffer) / sizeof(buffer[0]));
    int header                                          = zone_index - 1;

    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = header;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = 0x01;
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);

    memset(buffer, 0x00, buffer_size );

    buffer[CM_SMALL_ARGB_COMMAND_BYTE]                  = 0x0B;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE]                 = 0x01;
    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = 0x01;
    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);

    current_mode                                        = buffer[4];
    current_speed                                       = buffer[5];
    bool_random                                         = buffer[6] == 0x00;
    current_brightness                                  = buffer[7];
    current_red                                         = buffer[8];
    current_green                                       = buffer[9];
    current_blue                                        = buffer[10];
}

std::string CMSmallARGBController::GetDeviceName()
{
    return device_name;
}

std::string CMSmallARGBController::GetSerial()
{
    return serial;
}

std::string CMSmallARGBController::GetLocation()
{
    return("HID: " + location);
}

unsigned char CMSmallARGBController::GetZoneIndex()
{
    return zone_index;
}

unsigned char CMSmallARGBController::GetMode()
{
    return current_mode;
}

unsigned char CMSmallARGBController::GetLedRed()
{
    return current_red;
}

unsigned char CMSmallARGBController::GetLedGreen()
{
    return current_green;
}

unsigned char CMSmallARGBController::GetLedBlue()
{
    return current_blue;
}

unsigned char CMSmallARGBController::GetLedSpeed()
{
    return current_speed;
}

bool CMSmallARGBController::GetRandomColours()
{
    return bool_random;
}

void CMSmallARGBController::SetLedCount(int zone, int led_count)
{
    unsigned char buffer[CM_SMALL_ARGB_PACKET_SIZE]     = { 0x00, 0x80, 0x0D, 0x02 };
    int  buffer_size                                    = (sizeof(buffer) / sizeof(buffer[0]));

    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = zone;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = (0x0F - led_count > 0) ? 0x0F - led_count : 0x01;
    buffer[CM_SMALL_ARGB_SPEED_BYTE]                    = led_count;

    hid_write(dev, buffer, buffer_size);
}

void CMSmallARGBController::SetMode(unsigned char mode, unsigned char speed, RGBColor colour, bool random_colours)
{
    current_mode    = mode;
    current_speed   = speed;
    current_red     = RGBGetRValue(colour);
    current_green   = RGBGetGValue(colour);
    current_blue    = RGBGetBValue(colour);
    bool_random     = random_colours;

    SendUpdate();
}

void CMSmallARGBController::SetLedsDirect(RGBColor* /*led_colours*/, unsigned int /*led_count*/)
{
    // Mode not yet Tested
    /*
    const unsigned char buffer_size     = 0x41;
    unsigned char buffer[buffer_size]   = { 0x00 };
    unsigned char packet_count          = 0x00;
    std::vector<unsigned char> colours;

    //Set up the RGB triplets to send
    for(int i = 0; i < led_count; i++)
    {
        RGBColor      colour            = led_colours[i];
        unsigned char r                 = RGBGetRValue(colour);
        unsigned char g                 = RGBGetGValue(colour);
        unsigned char b                 = RGBGetBValue(colour);

        colours.push_back(r);
        colours.push_back(g);
        colours.push_back(b);
    }

    //buffer[CM_ARGB_REPORT_BYTE]       = packet_count;
    buffer[CM_SMALL_ARGB_COMMAND_BYTE]  = 0x10;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE] = 0x02;
    buffer[CM_SMALL_ARGB_ZONE_BYTE]     = small_argb_header_data[zone_index].header;
    buffer[CM_SMALL_ARGB_MODE_BYTE]     = 0x30; //30 might be the LED count??
    unsigned char buffer_idx            = CM_SMALL_ARGB_MODE_BYTE + 1;    //Start colour info from

    for (std::vector<unsigned char>::iterator it = colours.begin(); it != colours.end(); buffer_idx = 0)
    {
        //Fill the write buffer till its full or the colour buffer is empty
        buffer[CM_SMALL_ARGB_REPORT_BYTE] = packet_count;
        while (( buffer_idx < buffer_size) && ( it != colours.end() ))
        {
            buffer[buffer_idx] = *it;
            buffer_idx++;
            it++;
        }

        hid_write(dev, buffer, buffer_size);
        hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);

        //reset the write buffer
        memset(buffer, 0x00, sizeof(buffer) );
        packet_count++;
    }
    buffer[CM_SMALL_ARGB_REPORT_BYTE]         = 0x82;
    buffer[CM_SMALL_ARGB_COMMAND_BYTE]        = 0x62;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE]       = 0x00;
    buffer[CM_SMALL_ARGB_ZONE_BYTE]           = 0x73;
    buffer[CM_SMALL_ARGB_MODE_BYTE]           = 0x00;
    buffer[CM_SMALL_ARGB_SPEED_BYTE]          = 0x33;
    buffer[CM_SMALL_ARGB_COLOUR_INDEX_BYTE]   = 0x18;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);
    */
}

void CMSmallARGBController::SendUpdate()
{
    unsigned char buffer[CM_SMALL_ARGB_PACKET_SIZE]     = { 0x00 };
    int  buffer_size                                    = (sizeof(buffer) / sizeof(buffer[0]));
    bool boolPassthru                                   = ( current_mode == CM_SMALL_ARGB_MODE_PASSTHRU );
    bool boolDirect                                     = ( current_mode == CM_SMALL_ARGB_MODE_DIRECT );
    unsigned char function                              = boolPassthru ? 0x02 : 0x01;
    buffer[CM_SMALL_ARGB_REPORT_BYTE]                   = 0x80;
    buffer[CM_SMALL_ARGB_COMMAND_BYTE]                  = boolDirect   ? 0x10 : 0x01;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE]                 = boolDirect   ? 0x01 : function;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = boolPassthru ? 0x00 : 0x02;

    hid_write(dev, buffer, buffer_size);

    buffer[CM_SMALL_ARGB_COMMAND_BYTE]                  = 0x0b;
    buffer[CM_SMALL_ARGB_FUNCTION_BYTE]                 = (false) ? 0x01 : 0x02; //This controls custom mode TODO
    buffer[CM_SMALL_ARGB_ZONE_BYTE]                     = small_argb_header_data[zone_index].header;
    buffer[CM_SMALL_ARGB_MODE_BYTE]                     = current_mode;
    buffer[CM_SMALL_ARGB_SPEED_BYTE]                    = current_speed;
    buffer[CM_SMALL_ARGB_COLOUR_INDEX_BYTE]             = (bool_random) ? 0x00 : 0x10; //This looks to still be the colour index and controls random colours
    buffer[CM_SMALL_ARGB_BRIGHTNESS_BYTE]               = 0xFF;
    buffer[CM_SMALL_ARGB_RED_BYTE]                      = current_red;
    buffer[CM_SMALL_ARGB_GREEN_BYTE]                    = current_green;
    buffer[CM_SMALL_ARGB_BLUE_BYTE]                     = current_blue;

    hid_write(dev, buffer, buffer_size);
    hid_read_timeout(dev, buffer, buffer_size, CM_SMALL_ARGB_INTERRUPT_TIMEOUT);
}
