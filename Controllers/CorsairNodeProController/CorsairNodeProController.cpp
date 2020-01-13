/*---------------------------------------------------------*\
|  Processing Code for Corsair Lighting Node Pro            |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/12/2020        |
\*---------------------------------------------------------*/

#include "CorsairNodeProController.h"

#include <fstream>
#include <iostream>
#include <string>

//Include thread libraries for Windows or Linux
#ifdef WIN32
#include <process.h>
#else
#include "pthread.h"
#include "unistd.h"
#endif

//Thread functions have different types in Windows and Linux
#ifdef WIN32
#define THREAD static void
#define THREADRETURN
#else
#define THREAD static void*
#define THREADRETURN return(NULL);
#endif

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

THREAD keepalive_thread(void *param)
{
    CorsairNodeProController* corsair = static_cast<CorsairNodeProController*>(param);
    corsair->KeepaliveThread();
    THREADRETURN
}

CorsairNodeProController::CorsairNodeProController(libusb_device_handle* dev_handle)
{
    dev = dev_handle;
    channel_leds[0] = 60;
    channel_leds[1] = 60;

    /*-----------------------------------------------------*\
    | The Corsair Lighting Node Pro requires a packet within|
    | 20 seconds of sending the lighting change in order    |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
#ifdef WIN32
    _beginthread(keepalive_thread, 0, this);
#else
    pthread_t thread;
    pthread_create(&thread, NULL, &keepalive_thread, this);
#endif
}

CorsairNodeProController::~CorsairNodeProController()
{
}

void CorsairNodeProController::SendKeepalive()
{
    unsigned char usb_apply[] =
    {
        0x33, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    int             actual;

    /*-----------------------------------------------------*\
    | Send apply packet                                     |
    \*-----------------------------------------------------*/
    libusb_interrupt_transfer(dev, 0x01, usb_apply, 64, &actual, 0);
}

void CorsairNodeProController::KeepaliveThread()
{
    while(1)
    {
        SendKeepalive();

        Sleep(5000);
    }
}

void CorsairNodeProController::SetChannelLEDs(unsigned int channel, std::vector<RGBColor> colors)
{
    unsigned char usb_start[] =
    {
        0x38, 0x00, 0x02, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    unsigned char usb_data[] =
    {
        0x32, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    unsigned char usb_apply[] =
    {
        0x33, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    int             actual;

    /*-----------------------------------------------------*\
    | Set channel in USB packets                            |
    \*-----------------------------------------------------*/
    usb_data[0x01]   = channel;
    usb_start[0x01]  = channel;

    /*-----------------------------------------------------*\
    | Send start packet                                     |
    \*-----------------------------------------------------*/
    libusb_interrupt_transfer(dev, 0x01, usb_start, 64, &actual, 0);

    /*-----------------------------------------------------*\
    | Send red channel packet 1                             |
    \*-----------------------------------------------------*/
    std::size_t pkt_max = 50;

    if(pkt_max > colors.size())
    {
        pkt_max = colors.size();
    }

    usb_data[0x02]  = 0;
    usb_data[0x03]  = pkt_max;
    usb_data[0x04]  = 0x00; //red

    for (std::size_t idx = 0; idx < pkt_max; idx++)
    {
        RGBColor color = colors[idx];
        usb_data[idx + 5] = RGBGetRValue(color);
    }

    libusb_interrupt_transfer(dev, 0x01, usb_data, 64, &actual, 0);

    /*-----------------------------------------------------*\
    | Send red channel packet 2 if necessary                |
    \*-----------------------------------------------------*/
    for(int idx = 4; idx < 64; idx++)
    {
        usb_data[idx] = 0;
    }

    pkt_max         = 0;

    if (colors.size() > 50)
    {
        pkt_max = colors.size() - 50;
    }

    if(pkt_max > 0)
    {
        usb_data[0x02]  = 50;
        usb_data[0x03]  = pkt_max;
        usb_data[0x04]  = 0x00; //red

        for (std::size_t idx = 0; idx < pkt_max; idx++)
        {
            RGBColor color = colors[idx + 50];
            usb_data[idx + 5] = RGBGetRValue(color);
        }

        libusb_interrupt_transfer(dev, 0x01, usb_data, 64, &actual, 0);
    }
    
    /*-----------------------------------------------------*\
    | Send green channel packet 1                           |
    \*-----------------------------------------------------*/
    pkt_max = 50;

    if(pkt_max > colors.size())
    {
        pkt_max = colors.size();
    }

    usb_data[0x02]  = 0;
    usb_data[0x03]  = pkt_max;
    usb_data[0x04]  = 0x01; //green

    for (std::size_t idx = 0; idx < pkt_max; idx++)
    {
        RGBColor color = colors[idx];
        usb_data[idx + 5] = RGBGetGValue(color);
    }

    libusb_interrupt_transfer(dev, 0x01, usb_data, 64, &actual, 0);

    /*-----------------------------------------------------*\
    | Send green channel packet 2 if necessary              |
    \*-----------------------------------------------------*/
    for(int idx = 4; idx < 64; idx++)
    {
        usb_data[idx] = 0;
    }

    pkt_max         = 0;

    if (colors.size() > 50)
    {
        pkt_max = colors.size() - 50;
    }

    if(pkt_max > 0)
    {
        usb_data[0x02]  = 50;
        usb_data[0x03]  = pkt_max;
        usb_data[0x04]  = 0x01; //green

        for (std::size_t idx = 0; idx < pkt_max; idx++)
        {
            RGBColor color = colors[idx + 50];
            usb_data[idx + 5] = RGBGetGValue(color);
        }

        libusb_interrupt_transfer(dev, 0x01, usb_data, 64, &actual, 0);
    }

    /*-----------------------------------------------------*\
    | Send blue channel packet 1                            |
    \*-----------------------------------------------------*/
    pkt_max = 50;

    if(pkt_max > colors.size())
    {
        pkt_max = colors.size();
    }

    usb_data[0x02]  = 0;
    usb_data[0x03]  = pkt_max;
    usb_data[0x04]  = 0x02; //blue

    for (std::size_t idx = 0; idx < pkt_max; idx++)
    {
        RGBColor color = colors[idx];
        usb_data[idx + 5] = RGBGetBValue(color);
    }

    libusb_interrupt_transfer(dev, 0x01, usb_data, 64, &actual, 0);

    /*-----------------------------------------------------*\
    | Send blue channel packet 2 if necessary               |
    \*-----------------------------------------------------*/
    for(int idx = 4; idx < 64; idx++)
    {
        usb_data[idx] = 0;
    }

    pkt_max         = 0;

    if (colors.size() > 50)
    {
        pkt_max = colors.size() - 50;
    }

    if(pkt_max > 0)
    {
        usb_data[0x02]  = 50;
        usb_data[0x03]  = pkt_max;
        usb_data[0x04]  = 0x02; //blue

        for (std::size_t idx = 0; idx < pkt_max; idx++)
        {
            RGBColor color = colors[idx + 50];
            usb_data[idx + 5] = RGBGetBValue(color);
        }

        libusb_interrupt_transfer(dev, 0x01, usb_data, 64, &actual, 0);
    }

    /*-----------------------------------------------------*\
    | Send apply packet                                     |
    \*-----------------------------------------------------*/
    libusb_interrupt_transfer(dev, 0x01, usb_apply, 64, &actual, 0);
}
