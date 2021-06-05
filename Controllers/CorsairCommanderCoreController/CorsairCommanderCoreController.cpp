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

CorsairCommanderCoreController::CorsairCommanderCoreController(hid_device* dev_handle, const char* path)
{
    dev                     = dev_handle;
    location                = path;
    keepalive_thread_run    = 1;
    controller_ready        = 0;

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
    | Packet sequence to put controller into direct mode    |
    \*-----------------------------------------------------*/
    unsigned char buffarray[][5] =
    {
        {0x08, 0x01, 0x03, 0x00, 0x02},
        {0x08, 0x0D, 0x00, 0x22, 0x00},
    };

    SendMultiPkt(buffarray, sizeof(buffarray) / sizeof(buffarray[0]), sizeof(buffarray)[0] / sizeof(buffarray[0][0]));
    SetFanMode();
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
        unsigned char   usb_buf[1025];

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Update last commit time                               |
        \*-----------------------------------------------------*/
        last_commit_time    = std::chrono::steady_clock::now();

        /*-----------------------------------------------------*\
        | Set up Commit packet                                  |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0, CORSAIR_COMMANDER_CORE_PACKET_SIZE);

        usb_buf[0]          = 0x00;
        usb_buf[1]          = 0x08;
        usb_buf[2]          = 0x06;
        usb_buf[4]          = 0xBD;
        usb_buf[5]          = 0x02;
        usb_buf[8]          = 0x12;

        hid_write(dev, usb_buf, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
    }
}


void CorsairCommanderCoreController::SendMultiPkt(unsigned char buffarray[][5], int r, int c)
{
    /*---------------------------------------------------------*\
    | Private function to send multiple packets                 |
    \*---------------------------------------------------------*/
    unsigned char* hidtemp = new unsigned char[CORSAIR_COMMANDER_CORE_PACKET_SIZE];

    for(unsigned int i = 0; i < r; i++)
    {
        memset(hidtemp, 0, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
    
        for(unsigned int j = 0; j < c; j++)
        {
            hidtemp[j+1] = buffarray[i][j];
        }

        hid_write(dev, hidtemp, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
        hid_read(dev, hidtemp, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
    }
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
        int packet_offset       = CORSAIR_COMMANDER_CORE_PREAMBLE_OFFSET;
        int led_idx             = 0;
        int channel_idx         = 0;
        unsigned char* usb_buf  = new unsigned char[CORSAIR_COMMANDER_CORE_PACKET_SIZE];

        memset(usb_buf, 0, CORSAIR_COMMANDER_CORE_PACKET_SIZE);

        /*-----------------------------------------------------*\
        | Prepare color information packet                      |
        \*-----------------------------------------------------*/
        usb_buf[0]              = 0x00;
        usb_buf[1]              = 0x08;
        usb_buf[2]              = 0x06;
        usb_buf[4]              = 0xBD;
        usb_buf[5]              = 0x02;
        usb_buf[8]              = 0x12;

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

                packet_offset = packet_offset + 3;
            }

            led_idx = led_idx + zones[zone_idx].leds_count;

            /*-------------------------------------------------*\
            | Move offset for pump zone with less than 29 LEDs  |
            \*-------------------------------------------------*/
            if(zone_idx == 0)
            {
                packet_offset = packet_offset + 3 * (29 - zones[zone_idx].leds_count);
            }

            /*-------------------------------------------------*\
            | Move offset for fans with less than 34 LEDs       |
            \*-------------------------------------------------*/
            if(zone_idx != 0)
            {
                packet_offset = packet_offset + 3 * (34 - zones[zone_idx].leds_count);
            }

            channel_idx++;
        }

        /*-----------------------------------------------------*\
        | Sending a direct mode color packet resets the timeout |
        \*-----------------------------------------------------*/
        last_commit_time = std::chrono::steady_clock::now();

        hid_write(dev, usb_buf, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
    }
}

void CorsairCommanderCoreController::SetFanMode()
{
    /*--------------------------------------------------------------------------------------------------*\
    | Force controller to 6 QL fan mode to expose maximum number of LEDs per rgb port (34 LEDs per port) |
    \*--------------------------------------------------------------------------------------------------*/

    unsigned char usb_buf[1025];

    unsigned char buffarray4[][5] =
    {
        {0x08, 0x05, 0x01, 0x01, 0x00},
        {0x08, 0x0D, 0x01, 0x1E, 0x00},
        {0x08, 0x09, 0x01, 0x00, 0x00}
    };

    SendMultiPkt(buffarray4, sizeof(buffarray4) / sizeof(buffarray4[0]), sizeof(buffarray4)[0] / sizeof(buffarray4[0][0]));

    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[0]          = 0x00;
    usb_buf[1]          = 0x08;
    usb_buf[2]          = 0x06;
    usb_buf[3]          = 0x01;
    usb_buf[4]          = 0x11;
    usb_buf[8]          = 0x0D;
    usb_buf[10]         = 0x07;
    usb_buf[11]         = 0x01;
    usb_buf[12]         = 0x08;

    for(unsigned int i = 13; i < 25; i = i + 2)
    {
        usb_buf[i]      = 0x01;
        usb_buf[i + 1]  = 0x06;
    }

    hid_write(dev, usb_buf, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
    hid_read(dev, usb_buf, CORSAIR_COMMANDER_CORE_PACKET_SIZE);

    unsigned char buffarray2[][5] =
    {
        {0x08, 0x05, 0x01, 0x01, 0x00},
        {0x08, 0x15, 0x01, 0x00, 0x00}
    };

    SendMultiPkt(buffarray2, sizeof(buffarray2) / sizeof(buffarray2[0]), sizeof(buffarray2)[0] / sizeof(buffarray2[0][0]));

    memset(usb_buf, 0, CORSAIR_COMMANDER_CORE_PACKET_SIZE);

    usb_buf[0]          = 0x00;
    usb_buf[1]          = 0x08;
    usb_buf[2]          = 0x06;
    usb_buf[4]          = 0xBD;
    usb_buf[5]          = 0x02;
    usb_buf[8]          = 0x12;

    hid_write(dev, usb_buf, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
    hid_read(dev, usb_buf, CORSAIR_COMMANDER_CORE_PACKET_SIZE);
    
    controller_ready    = 1;
}
