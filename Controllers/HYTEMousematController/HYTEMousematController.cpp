/*---------------------------------------------------------*\
|  HYTEMousematController.cpp                               |
|                                                           |
|  Driver for HYTE CNVS RGB mousemat controller             |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 7/18/2023        |
\*---------------------------------------------------------*/

#include "HYTEMousematController.h"

HYTEMousematController::HYTEMousematController(char* port)
{
    port_name = port;

    /*-----------------------------------------------------*\
    | Open the port                                         |
    | Baud rate doesn't matter for ACM device               |
    \*-----------------------------------------------------*/
    serialport = new serial_port(port_name.c_str(), 2000000);

    FirmwareAnimationControl(false);
}

HYTEMousematController::~HYTEMousematController()
{

}

std::string HYTEMousematController::GetLocation()
{
    return(port_name);
}

void HYTEMousematController::FirmwareAnimationControl(bool enabled)
{
    unsigned char serial_buf[4];

    memset(serial_buf, 0, sizeof(serial_buf));

    serial_buf[0]   = 0xFF;
    serial_buf[1]   = 0xDC;
    serial_buf[2]   = 0x05;
    serial_buf[3]   = enabled;

    serialport->serial_write((char *)serial_buf, sizeof(serial_buf));
}

void HYTEMousematController::StreamingCommand(RGBColor* colors)
{
    unsigned char serial_buf[157];

    memset(serial_buf, 0, sizeof(serial_buf));

    serial_buf[0]   = 0xFF;
    serial_buf[1]   = 0xEE;
    serial_buf[2]   = 0x02;
    serial_buf[3]   = 0x01;
    serial_buf[4]   = 0x00;
    serial_buf[5]   = 0x32;
    serial_buf[6]   = 0x00;

    for(unsigned int color_idx = 0; color_idx < 50; color_idx++)
    {
        serial_buf[7 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
        serial_buf[8 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        serial_buf[9 + (color_idx * 3)] = RGBGetBValue(colors[color_idx]);
    }

    serialport->serial_write((char *)serial_buf, sizeof(serial_buf));
    serialport->serial_flush_tx();
}
