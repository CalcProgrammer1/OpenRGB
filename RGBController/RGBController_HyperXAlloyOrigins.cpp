/*-----------------------------------------*\
|  RGBController_HyperXAlloyOrigins.cpp     |
|                                           |
|  Generic RGB Interface for HyperX Alloy   |
|  Origins RGB Keyboard                     |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/11/2020   |
\*-----------------------------------------*/

#include "RGBController_HyperXAlloyOrigins.h"

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

using namespace std::chrono_literals;

THREAD keepalive_thread(void *param)
{
    RGBController_HyperXAlloyOrigins* controller = static_cast<RGBController_HyperXAlloyOrigins*>(param);
    controller->KeepaliveThread();
    THREADRETURN
}

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static unsigned int matrix_map[6][23] =
    { {   0,  NA,  12,  18,  24,  30,  NA,  42,  48,  54,  60,  NA,  66,  72,  78,  84,  90,  96, 102,  NA,  NA,  NA,  NA },
      {   1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  NA,  67,  73,  85,  NA,  91,  97, 103,  NA,  NA,  NA,  NA },
      {   2,  NA,   8,  14,  20,  26,  NA,  32,  38,  44,  50,  56,  62,  68,  74,  86,  92,  98, 104,  NA,  NA,  NA,  NA },
      {   3,  NA,   9,  15,  21,  27,  NA,  33,  39,  45,  51,  57,  63,  69,  87,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA },
      {   4,  NA,  16,  22,  28,  34,  NA,  40,  NA,  46,  52,  58,  64,  70,  82,  NA,  NA, 100,  NA,  NA,  NA,  NA,  NA },
      {   5,  11,  17,  NA,  NA,  NA,  NA,  41,  NA,  NA,  NA,  NA,  65,  77,  83,  89,  95, 101, 107,  NA,  NA,  NA,  NA } };

RGBController_HyperXAlloyOrigins::RGBController_HyperXAlloyOrigins(HyperXAlloyOriginsController* hyperx_ptr)
{
    hyperx = hyperx_ptr;

    name        = "HyperX Alloy Origins Keyboard Device";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "HyperX Alloy Origins Keyboard Device";

    mode Custom;
    Custom.name       = "Custom";
    Custom.value      = 0xFFFF;
    Custom.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Custom.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Custom);

    SetupZones();

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

RGBController_HyperXAlloyOrigins::~RGBController_HyperXAlloyOrigins()
{

}

void RGBController_HyperXAlloyOrigins::SetupZones()
{
    zone new_zone;

    new_zone.name               = "Keyboard";
    new_zone.type               = ZONE_TYPE_MATRIX;
    new_zone.leds_min           = 155;
    new_zone.leds_max           = 155;
    new_zone.leds_count         = 155;
    new_zone.matrix_map         = new matrix_map_type;
    new_zone.matrix_map->height = 6;
    new_zone.matrix_map->width  = 23;
    new_zone.matrix_map->map    = (unsigned int *)&matrix_map;
    
    zones.push_back(new_zone);

    for(int led_idx = 0; led_idx < 155; led_idx++)
    {
        led new_led;

        new_led.name = "Keyboard LED ";
        new_led.name.append(std::to_string(led_idx));

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_HyperXAlloyOrigins::ResizeZone(int zone, int new_size)
{
    
}
    
void RGBController_HyperXAlloyOrigins::DeviceUpdateLEDs()
{
    hyperx->SetLEDsDirect(colors);
}

void RGBController_HyperXAlloyOrigins::UpdateZoneLEDs(int zone)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins::UpdateSingleLED(int led)
{
    DeviceUpdateLEDs();
}

void RGBController_HyperXAlloyOrigins::SetCustomMode()
{

}

void RGBController_HyperXAlloyOrigins::UpdateMode()
{

}

void RGBController_HyperXAlloyOrigins::KeepaliveThread()
{
    while(1)
    {
        if(active_mode == 0)
        {
            if((std::chrono::steady_clock::now() - last_update_time) > std::chrono::milliseconds(50))
            {
                UpdateLEDs();
            }
        }
        std::this_thread::sleep_for(10ms);;
    }
}