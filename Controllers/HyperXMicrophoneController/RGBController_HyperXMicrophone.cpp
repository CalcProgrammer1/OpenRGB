/*---------------------------------------------------------*\
| RGBController_HyperXMicrophone.cpp                        |
|                                                           |
|   RGBController for HyperX microphone                     |
|                                                           |
|   Matt Silva (thesilvanator)                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/**------------------------------------------------------------------*\
    @name HyperX Quadcast S
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectHyperXMicrophoneControllers
    @comment The HyperX Quadcast S has a manufacturer issue
    with the interface it uses (0) for controlling its RGB.
    HID requires that any HID interface have at least one
    Interrupt IN endpoint; however, the HXQS does not,
    even though its interface reports itelf as hid and
    responds to hid requests. As such Linux doesn't bind
    to the usbhid driver and it goes undetected by
    hidapi-hidraw. Windows does detect it as hid and hidapi
    finds and interacts with it just fine. To work around
    the Linux issue, hidapi-libusb is loaded dynamically
    using dlopen/dlsym as hidapi using a libusb backend is
    able to find the device and interact with it. This
    requires that you have support for dlopen/dlsym on your
    Linux platform as well as hidapi-libusb (and libusb)
    libraries installed in the standard dynamic library
    path.

    The controller for this device has a wrapper for hidapi
    functions so that the controller can be the same across
    all platforms, but call the correct underlying functions
    that are defined in the detector under an #ifdef
    for that platform.

    Additionally, hidapi-libusb has an error that causes
    hid_close() to hang on this device, see:
    https://github.com/libusb/hidapi/issues/456
    This will be fixed on newer versions of hidapi-libusb,
    but until then, OpenRGB will hang/crash if you try to
    rescan devices once a HXQS has been detected during
    program session.
\*-------------------------------------------------------------------*/

#include "RGBController_HyperXMicrophone.h"
#include <LogManager.h>

using namespace std::chrono_literals;

RGBController_HyperXMicrophone::RGBController_HyperXMicrophone(HyperXMicrophoneController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetNameString();
    vendor                  = "HyperX";
    type                    = DEVICE_TYPE_MICROPHONE;
    description             = "HyperX Microphone Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode Direct;
    Direct.name             = "Direct";
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();

    keepalive_thread_run    = 1;
    keepalive_thread        = new std::thread(&RGBController_HyperXMicrophone::KeepaliveThread, this);
};

RGBController_HyperXMicrophone::~RGBController_HyperXMicrophone()
{
    keepalive_thread_run = 0;
    keepalive_thread->join();
    delete keepalive_thread;

    delete controller;
}

void RGBController_HyperXMicrophone::SetupZones()
{
    led Top;
    Top.name   = "Top";
    Top.value  = 0;

    led Bot;
    Bot.name   = "Bottom";
    Bot.value  = 1;

    leds.push_back(Top);
    leds.push_back(Bot);

    zone Mic;
    Mic.name       = "Microphone";
    Mic.type       = ZONE_TYPE_SINGLE;
    Mic.leds_min   = 2;
    Mic.leds_max   = 2;
    Mic.leds_count = 2;
    Mic.matrix_map = nullptr;

    zones.push_back(Mic);

    SetupColors();
}

void RGBController_HyperXMicrophone::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_HyperXMicrophone::DeviceUpdateLEDs()
{
    last_update_time = std::chrono::steady_clock::now();
    controller->SendDirect(colors);
}
void RGBController_HyperXMicrophone::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}
void RGBController_HyperXMicrophone::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}
void RGBController_HyperXMicrophone::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXMicrophone::DeviceSaveMode()
{
    LOG_DEBUG("[%s] Saving current direct colors to device", name.c_str());
    controller->SaveColors(colors, 1);
}

void RGBController_HyperXMicrophone::KeepaliveThread()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
        {
            UpdateLEDs();
        }
        std::this_thread::sleep_for(15ms);
    }
}
