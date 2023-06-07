/*---------------------------------------------------------*\
|  CorsairCommanderCoreController.cpp                       |
|                                                           |
|  Processing Code for Corsair Commander Core               |
|                                                           |
|  Jeff P.                                                  |
\*---------------------------------------------------------*/

#include "CorsairCommanderCoreController.h"

#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std::chrono_literals;

CorsairCommanderCoreController::CorsairCommanderCoreController(hid_device* dev_handle, const char* path, int pid)
{
    dev                     = dev_handle;
    location                = path;
    keepalive_thread_run    = 1;
    controller_ready        = 0;
    packet_size             = CORSAIR_COMMANDER_CORE_PACKET_SIZE_V2;
    command_res_size        = packet_size - 4;
    this->pid               = pid;

    /*-----------------------------------------------------*\
    | Initialize controller                                 |
    \*-----------------------------------------------------*/
    InitController();

    /*-----------------------------------------------------*\
    | Start keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread = new std::thread(&CorsairCommanderCoreController::KeepaliveThread, this);
}

CorsairCommanderCoreController::~CorsairCommanderCoreController()
{
    /*-----------------------------------------------------*\
    | Close keepalive thread                                |
    \*-----------------------------------------------------*/
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    /*-----------------------------------------------------*\
    | Close HID device                                      |
    \*-----------------------------------------------------*/
    hid_close(dev);
}

void CorsairCommanderCoreController::InitController()
{
    /*-----------------------------------------------------*\
    | Get version                                           |
    \*-----------------------------------------------------*/
    unsigned char command[2] = {0x02, 0x13};
    unsigned char* res = new unsigned char[command_res_size];
    SendCommand(command, NULL, 0, res);
    version[0] = res[0];
    version[1] = res[1];
    version[2] = res[2];
    delete[] res;

    if (pid == 0x0C1C && version[0] == 1)
    {
        packet_size = CORSAIR_COMMANDER_CORE_PACKET_SIZE_V1;
        command_res_size = packet_size - 4;
    }
    else if (pid == 0x0C32)
    {
        packet_size = CORSAIR_COMMANDER_CORE_PACKET_SIZE_V3;
    }

    /*-----------------------------------------------------*\
    | Wake up device                                        |
    \*-----------------------------------------------------*/
    command[0] = 0x01;
    command[1] = 0x03;
    unsigned char cmd_data[2] = {0x00, 0x02};
    SendCommand(command, cmd_data, 2, NULL);

    SetFanMode();
}

std::string CorsairCommanderCoreController::GetFirmwareString()
{
    return "v"+std::to_string(version[0]) + "." + std::to_string(version[1]) + "." + std::to_string(version[2]);
}

std::string CorsairCommanderCoreController::GetLocationString()
{
    return("HID: " + location);
}

void CorsairCommanderCoreController::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if (controller_ready)
        {
            if((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::seconds(10))
            {
                SendCommit();
            }
        }
        std::this_thread::sleep_for(1s);
    }
}

void CorsairCommanderCoreController::SendCommit()
{
    if(!lastcolors.empty())
    {
        /*-----------------------------------------------------*\
        | If colors remain to be sent, send them                |
        \*-----------------------------------------------------*/
        SetDirectColor(lastcolors, lastzones);
    }
    else
    {
        /*-----------------------------------------------------*\
        | Update last commit time                               |
        \*-----------------------------------------------------*/
        last_commit_time    = std::chrono::steady_clock::now();

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        unsigned char command[2] = {0x01, 0x03};
        unsigned char cmd_data[2] = {0x00, 0x02};
        SendCommand(command, cmd_data, 2, NULL);
    }
}


void CorsairCommanderCoreController::SendCommand(unsigned char command[2], unsigned char data[], unsigned short int data_len, unsigned char res[])
{
    /*---------------------------------------------------------*\
    | Private function to send a command                        |
    | data_len must be <= 93 for V2 or <= 1021 for V1           |
    \*---------------------------------------------------------*/
    unsigned char* buf = new unsigned char[packet_size];

    memset(buf, 0, packet_size);
    buf[0] = 0x00;
    buf[1] = 0x08;

    memcpy(&buf[2], command, 2);
    if(data != NULL)
    {
        memcpy(&buf[4], data, data_len);
    }

    hid_write(dev, buf, packet_size);
    do
    {
        hid_read(dev, buf, packet_size);
    }
    while (buf[0] != 0x00);

    if(res != NULL)
    {
        memcpy(res, &buf[3], command_res_size);
    }

    delete[] buf;
}

void CorsairCommanderCoreController::WriteData(unsigned char endpoint[2], unsigned char data_type[2], unsigned char data[], unsigned short int data_len)
{
    /*---------------------------------------------------------*\
    | Private function to write data to an endpoint             |
    \*---------------------------------------------------------*/

    /*---------------------------------------------------------*\
    | Open endpoint                                             |
    \*---------------------------------------------------------*/
    unsigned char command[2] = {0x0d, 0x00};
    SendCommand(command, endpoint, 2, NULL);


    /*---------------------------------------------------------*\
    | Write data                                                |
    \*---------------------------------------------------------*/
    unsigned short int data_start_index = 0;
    while(data_start_index < data_len)
    {
        if(data_start_index == 0)
        {
            /*---------------------------------------------------------*\
            | First packet                                              |
            \*---------------------------------------------------------*/
            int packet_data_len = packet_size - 10;
            if(data_len < packet_data_len)
            {
                packet_data_len = data_len;
            }
            unsigned char* buf = new unsigned char[packet_data_len+6];
            unsigned short int real_len = data_len+2;
            /*---------------------------------------------------------*\
            | Convert length to little endian                           |
            \*---------------------------------------------------------*/
            buf[0] = (unsigned char) real_len & 0xFF;
            buf[1] = (unsigned char) (real_len >> 8) & 0xFF;
            buf[2] = 0x00;
            buf[3] = 0x00;
            memcpy(&buf[4], data_type, 2);
            memcpy(&buf[6], data, packet_data_len);

            command[0] = 0x06;
            command[1] = 0x00;
            SendCommand(command, buf, packet_data_len+6, NULL);
            delete[] buf;
            data_start_index += packet_data_len;
        }
        else
        {
            /*-----------------------------------------------------------------------------------------------------*\
            | The rest of the packets                                                                               |
            | This command is not in v1 but it should never be reached as all data should fit in the first packet   |
            \*-----------------------------------------------------------------------------------------------------*/
            int packet_data_len = packet_size - 4;
            if(data_len-data_start_index < packet_data_len)
            {
                packet_data_len = data_len-data_start_index;
            }

            command[0] = 0x07;
            command[1] = 0x00;
            SendCommand(command, &data[data_start_index], packet_data_len, NULL);
            data_start_index += packet_data_len;
        }
    }

    /*---------------------------------------------------------*\
    | Close endpoint                                            |
    \*---------------------------------------------------------*/
    command[0] = 0x05;
    command[1] = 0x01;
    SendCommand(command, NULL, 0, NULL);
}

void CorsairCommanderCoreController::SetDirectColor
    (
        std::vector<RGBColor> colors,
        std::vector<zone> zones
    )
{
    if(controller_ready == 1 && ((std::chrono::steady_clock::now() - last_commit_time) > std::chrono::milliseconds(33)))
    {
        lastcolors              = colors;
        lastzones               = zones;
        int packet_offset       = 0;
        int led_idx             = 0;
        int channel_idx         = 0;
        unsigned char* usb_buf  = new unsigned char[CORSAIR_COMMANDER_CORE_RGB_DATA_LENGTH];

        memset(usb_buf, 0, CORSAIR_COMMANDER_CORE_RGB_DATA_LENGTH);

        for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            /*-------------------------------------------------*\
            | Add led colors                                    |
            \*-------------------------------------------------*/
            for(unsigned int i = led_idx; i < led_idx + zones[zone_idx].leds_count; i++)
            {
                usb_buf[packet_offset]   = RGBGetRValue(colors[i]);
                usb_buf[packet_offset+1] = RGBGetGValue(colors[i]);
                usb_buf[packet_offset+2] = RGBGetBValue(colors[i]);

                packet_offset += 3;
            }

            led_idx = led_idx + zones[zone_idx].leds_count;

            /*-------------------------------------------------*\
            | Move offset for pump zone with less than 29 LEDs  |
            \*-------------------------------------------------*/
            if(zone_idx == 0)
            {
                packet_offset += 3 * (29 - zones[zone_idx].leds_count);
            }
            else
            {
            /*-------------------------------------------------*\
            | Move offset for fans with less than 34 LEDs       |
            \*-------------------------------------------------*/
                packet_offset += 3 * (34 - zones[zone_idx].leds_count);
            }

            channel_idx++;
        }

        /*-----------------------------------------------------*\
        | Sending a direct mode color packet resets the timeout |
        \*-----------------------------------------------------*/
        last_commit_time = std::chrono::steady_clock::now();

        unsigned char endpoint[2] = {0x22, 0x00};
        unsigned char data_type[2] = {0x12, 0x00};
        WriteData(endpoint, data_type, usb_buf, packet_offset);

        delete[] usb_buf;
    }
}

void CorsairCommanderCoreController::SetFanMode()
{
    /*--------------------------------------------------------------------------------------------------*\
    | Force controller to 6 QL fan mode to expose maximum number of LEDs per rgb port (34 LEDs per port) |
    \*--------------------------------------------------------------------------------------------------*/

    unsigned char endpoint[2]  = {0x1E, 0x00};
    unsigned char data_type[2] = {0x0D, 0x00};
    unsigned char buf[15];

    /*-----------------------------------------------------*\
    | Set AIO mode                                          |
    \*-----------------------------------------------------*/
    buf[0]         = 0x07;
    buf[1]         = 0x01;
    buf[2]         = 0x08;

    /*-----------------------------------------------------*\
    | SET fan modes                                         |
    \*-----------------------------------------------------*/
    for(unsigned int i = 3; i < 15; i = i + 2)
    {
        buf[i]      = 0x01;
        buf[i + 1]  = 0x06;
    }

    WriteData(endpoint, data_type, buf, 15);
    
    controller_ready    = 1;
}
