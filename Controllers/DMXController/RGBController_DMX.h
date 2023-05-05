/*-----------------------------------------*\
|  RGBController_DMX.h                      |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  DMX interface                            |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/30/2023   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "serial_port.h"
#include <chrono>
#include <thread>

struct DMXDevice
{
    std::string name;
    std::string port;
    unsigned int keepalive_time;
    unsigned int red_channel;
    unsigned int green_channel;
    unsigned int blue_channel;
    unsigned int brightness_channel;
};

class RGBController_DMX : public RGBController
{
public:
    RGBController_DMX(std::vector<DMXDevice> device_list);
    ~RGBController_DMX();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
	std::vector<DMXDevice> 	                            devices;
    serial_port *                                       port;
    std::thread *                                       keepalive_thread;
    std::atomic<bool>                                   keepalive_thread_run;
    std::chrono::milliseconds                           keepalive_delay;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
