/*-----------------------------------------*\
|  CorsairKeyboardController.cpp            |
|                                           |
|  Driver for Corsair RGB keyboard lighting |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/9/2020    |
\*-----------------------------------------*/

#include "CorsairKeyboardController.h"

#include <cstring>

static unsigned int keys[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D, 0x0E, 0x0F, 0x11, 0x12,
                              0x14, 0x15, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x24, 0x25, 0x26,
                              0x27, 0x28, 0x2A, 0x2B, 0x2C, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                              0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x44, 0x45, 0x48, 73,   74,   75,   76,   78,
                              79,   80,   81,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   96,   97,
                              98,   99,   100,  101,  102,  103,  104,  105,  108,  109,  110,  111,  112,  113,  115,
                              116,  117,  120,  121,  122,  123,  124,  126,  127,  128,  129,  132,  133,  134,  135,
                              136,  137,  139,  140,  141};

static void send_usb_msg(hid_device* dev, char * data_pkt)
{
    char usb_pkt[65];
    usb_pkt[0] = 0x00;
    for(int i = 1; i < 65; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    int bytes = hid_send_feature_report(dev, (unsigned char *)usb_pkt, 65);
    bytes++;
}

CorsairKeyboardController::CorsairKeyboardController(hid_device* dev_handle)
{
    dev = dev_handle;

    char data_pkt[64] = { 0 };

    data_pkt[0] = 0x07;
    data_pkt[1] = 0x05;
    data_pkt[2] = 0x02;
    data_pkt[3] = 0x00;
    data_pkt[4] = 0x03;

    send_usb_msg(dev, data_pkt);
}

CorsairKeyboardController::~CorsairKeyboardController()
{

}

void CorsairKeyboardController::SetLEDs(std::vector<RGBColor> colors)
{
    char data_pkt[5][64] = { 0 };
    char red_val[144];
    char grn_val[144];
    char blu_val[144];

    memset(data_pkt, 0x00, sizeof( data_pkt ));
    memset(red_val, 0x00, sizeof( red_val ));
    memset(grn_val, 0x00, sizeof( grn_val ));
    memset(blu_val, 0x00, sizeof( blu_val ));

    for(int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        RGBColor      color = colors[color_idx];
        unsigned char red   = RGBGetRValue(color);
        unsigned char grn   = RGBGetGValue(color);
        unsigned char blu   = RGBGetBValue(color);

        if( red > 7 ) red = 7;
        if( grn > 7 ) grn = 7;
        if( blu > 7 ) blu = 7;

        red = 7 - red;
        grn = 7 - grn;
        blu = 7 - blu;

        red_val[keys[color_idx]] = red;
        grn_val[keys[color_idx]] = grn;
        blu_val[keys[color_idx]] = blu;
    }

    // Perform USB control message to keyboard
    //
    // Request Type:  0x21
    // Request:       0x09
    // Value          0x0300
    // Index:         0x03
    // Size:          64

    data_pkt[0][0] = 0x7F;
    data_pkt[0][1] = 0x01;
    data_pkt[0][2] = 0x3C;

    data_pkt[1][0] = 0x7F;
    data_pkt[1][1] = 0x02;
    data_pkt[1][2] = 0x3C;

    data_pkt[2][0] = 0x7F;
    data_pkt[2][1] = 0x03;
    data_pkt[2][2] = 0x3C;

    data_pkt[3][0] = 0x7F;
    data_pkt[3][1] = 0x04;
    data_pkt[3][2] = 0x24;

    data_pkt[4][0] = 0x07;
    data_pkt[4][1] = 0x27;
    data_pkt[4][2] = 0x00;
    data_pkt[4][2] = 0x00;
    data_pkt[4][4] = 0xD8; //Number of payload bytes

    for(int i = 0; i < 60; i++)
    {
        data_pkt[0][i+4] = red_val[i*2+1] << 4 | red_val[i*2];
    }

    for(int i = 0; i < 12; i++)
    {
        data_pkt[1][i+4] = red_val[i*2+121] << 4 | red_val[i*2+120];
    }

    for(int i = 0; i < 48; i++)
    {
        data_pkt[1][i+16] = grn_val[i*2+1] << 4 | grn_val[i*2];
    }

    for(int i = 0; i < 24; i++)
    {
        data_pkt[2][i+4] = grn_val[i*2+97] << 4 | grn_val[i*2+96];
    }

    for(int i = 0; i < 36; i++)
    {
        data_pkt[2][i+28] = blu_val[i*2+1] << 4 | blu_val[i*2];
    }

    for(int i = 0; i < 36; i++)
    {
        data_pkt[3][i+4] = blu_val[i*2+73] << 4 | blu_val[i*2+72];
    }

    send_usb_msg(dev, data_pkt[0]);
    send_usb_msg(dev, data_pkt[1]);
    send_usb_msg(dev, data_pkt[2]);
    send_usb_msg(dev, data_pkt[3]);
    send_usb_msg(dev, data_pkt[4]);
}
