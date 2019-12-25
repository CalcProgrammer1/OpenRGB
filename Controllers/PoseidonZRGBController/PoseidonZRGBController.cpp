/*-----------------------------------------*\
|  PoseidonZRGBController.cpp               |
|                                           |
|  Driver for Thermaltake Poseidon Z RGB    |
|  Keyboard lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "PoseidonZRGBController.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

static unsigned int keys[] = {8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21,
                              22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35,
                              36, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                              50, 51, 52, 54, 55, 56, 57, 58, 59, 60, 62, 63, 64,
                              65, 66, 67, 68, 70, 71, 72, 73, 74, 75, 76, 78, 79,
                              80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
                              93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105,
                              106, 108, 109, 111, 112, 114, 115, 117, 118, 119, 120,
                              124, 128, 129 };

PoseidonZRGBController::PoseidonZRGBController(hid_device* dev_handle)
{
    dev = dev_handle;
}

PoseidonZRGBController::~PoseidonZRGBController()
{

}

char* PoseidonZRGBController::GetDeviceName()
{
    return device_name;
}

void PoseidonZRGBController::SetLEDsDirect(std::vector<RGBColor) colors)
{
    unsigned char red_grn_buf[264];
    unsigned char blu_buf[264];

    for(int i = 0; i < 264; i++)
    {
        red_grn_buf[i] = 0x00;
        blu_buf[i]     = 0x00;
    }

    red_grn_buf[0] = POSEIDONZ_START;
    red_grn_buf[1] = POSEIDONZ_LED_CMD;
    red_grn_buf[2] = POSEIDONZ_PROFILE;
    red_grn_buf[3] = POSEIDONZ_RED_GRN_CH;
    red_grn_buf[4] = 0x00;
    red_grn_buf[5] = 0x00;
    red_grn_buf[6] = 0x00;
    red_grn_buf[7] = 0x00;

    blu_buf[0] = POSEIDONZ_START;
    blu_buf[1] = POSEIDONZ_LED_CMD;
    blu_buf[2] = POSEIDONZ_PROFILE;
    blu_buf[3] = POSEIDONZ_BLU_CH;
    blu_buf[4] = 0x00;
    blu_buf[5] = 0x00;
    blu_buf[6] = 0x00;
    blu_buf[7] = 0x00;

    for(int i = 0; i < 104; i++)
    {
        red_grn_buf[keys[i]      ] = RGBGetRValue(colors[i]);
        red_grn_buf[keys[i] + 128] = RGBGetGValue(colors[i]);
        blu_buf[    keys[i]      ] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, red_grn_buf, 264);
    
    Sleep(1);

    hid_send_feature_report(dev, blu_buf, 264);

}

void PoseidonZRGBController::SetLEDs(std::vector<RGBColor> colors)
{
    unsigned char buf[264] = {0};

    //Send Red Packet
    for(int i = 0; i < 264; i++)
    {
        buf[i] = 0xFF;
    }

    buf[0] = 0x07;
    buf[1] = 0x09;
    buf[2] = 0x01;
    buf[3] = 0x01;

    for(int i = 0; i < 104; i++)
    {
        buf[keys[i]] = RGBGetRValue(colors[i]);
    }

    hid_send_feature_report(dev, buf, 264);

    Sleep(10);
    
    //Send Green Packet
    for(int i = 0; i < 264; i++)
    {
        buf[i] = 0x00;
    }

    buf[0] = 0x07;
    buf[1] = 0x09;
    buf[2] = 0x01;
    buf[3] = 0x02;

    buf[8] = 0xFF;

    for(int i = 0; i < 104; i++)
    {
        buf[keys[i]] = RGBGetGValue(colors[i]);
    }

    hid_send_feature_report(dev, buf, 264);

    Sleep(10);
    
    //Send Blue Packet
    for(int i = 0; i < 264; i++)
    {
        buf[i] = 0x11;
    }

    buf[0] = 0x07;
    buf[1] = 0x09;
    buf[2] = 0x01;
    buf[3] = 0x03;

    for(int i = 0; i < 104; i++)
    {
       buf[keys[i]] = RGBGetBValue(colors[i]);
    }

    hid_send_feature_report(dev, buf, 264);

    Sleep(10);
    
    //Send Update Packet
    for(int i = 0; i < 264; i++)
    {
        buf[i] = 0x00;
    }

    buf[0] = 0x07;
    buf[1] = 0x02;
    buf[2] = 0x01;
    buf[8] = 0x01;  //Preset to save
    buf[13] = 0x04; //Brightness?
    buf[16] = 0x08;
    buf[22] = 0x03;

    hid_send_feature_report(dev, buf, 264);
}
