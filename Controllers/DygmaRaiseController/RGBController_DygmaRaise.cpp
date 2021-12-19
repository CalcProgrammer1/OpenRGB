/*-----------------------------------------*\
|  RGBController_DygmaRaise.cpp             |
|                                           |
|  RGB Interface DygmaRaise keyboard        |
|                                           |
|  Timo Schlegel (@eispalast) 12/12/2021    |
\*-----------------------------------------*/

#include "RGBController_DygmaRaise.h"

#define NA              0xFFFFFFFF
#define LED_REAL_COUNT  ((6*14)+(12*14))
#define LED_COUNT       (LED_REAL_COUNT - 121)

static unsigned int kb_matrix_map_ISO[6][14] =
    { {  0,  1,  2,  3,  4,  5,  6, 39, 38, 37, 36, 35, 34, 33 },
      {  7,  8,  9, 10, 11, 12, 47, 46, 45, 44, 43, 42, 41, 40 },
      { 13, 14, 15, 16, 17, 18, 54, 53, 52, 51, 50, 49, 48, NA },
      { 19, 20, 21, 22, 23, 24, 25, 60, 59, 58, 57, 56, 55, NA }, 
      { 26, 27, 28, NA, 29, 30, NA, 66, 65, NA, 64, 63, 62, 61 }, 
      { NA, NA, NA, NA, 31, 32, NA, 68, 67, NA, NA, NA, NA, NA } };

static unsigned int underglow_matrix[11][14] =
    { { 2, 3 , 4, 5  , 6 , 7,  NA, 38, 37, 36, 35, 34, 33, 32}, 
      { 1, NA, NA, NA, NA, 8,  NA, 39, NA, NA, NA, NA, NA, 31  }, 
      { 0, NA, NA, NA, NA, 9,  NA, 40, NA, NA, NA, NA, NA, 30  }, 
      { 29, NA, NA, NA, NA, 10,NA, 41, NA, NA, NA, NA, NA, 61  }, 
      { 28, NA, NA, NA, NA, 11,NA, 42, NA, NA, NA, NA, NA, 60  }, 
      { 27, NA, NA, NA, NA, 12,NA, 43, NA, NA, NA, NA, NA, 59  }, 
      { 26, NA, NA, NA, NA, 13,NA, 44, NA, NA, NA, NA, NA, 58  }, 
      { 25, NA, NA, NA, NA, 14,NA, 45, NA, NA, NA, NA, NA, 57  }, 
      { 24, NA, NA, NA, NA, 15,NA, 46, NA, NA, NA, NA, NA, 56  }, 
      { 23, NA, NA, NA, NA, 16,NA, 47, NA, NA, NA, NA, NA, 55  }, 
      { 22, 21, 20, 19, 18, 17,NA, 48,49,50,51,52,53, 54, }};


static const char* zone_names[] =
{
    "Keyboard",
    "Underglow",
    "Neuron",
};

static zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
    ZONE_TYPE_MATRIX,
    ZONE_TYPE_SINGLE,
};

static const unsigned int zone_sizes[] =
{
    69,
    62,
    1,
};

static const char* led_names[] =
{
    "Key: Escape",
    "Key: 1",                           
    "Key: 2",                      
    "Key: 3",                      
    "Key: 4",                      
    "Key: 5",                      
    "Key: 6",                      
    "Key: Tab",                    
    "Key: Q",                      
    "Key: W",                      
    "Key: E",                      
    "Key: R",                      
    "Key: T",                      
    "Key: Caps Lock",              
    "Key: A",                      
    "Key: S",                      
    "Key: D",                      
    "Key: F",                      
    "Key: G",                      
    "Key: Left Shift",             
    "Key: \\ (ISO)",               
    "Key: Z",                      
    "Key: X",                      
    "Key: C",                      
    "Key: V",                      
    "Key: B",                      
    "Key: Left Control",           
    "Key: Left Windows",           
    "Key: Left Alt",               
    "Key: T1",                     
    "Key: T2",                     
    "Key: T3",                     
    "Key: T4",                     
    "Key: Backspace",              
    "Key: =",                      
    "Key: -",                      
    "Key: 0",                      
    "Key: 9",                      
    "Key: 8",                      
    "Key: 7",                      
    "Key: Enter",                  
    "Key: ]",                      
    "Key: [",                      
    "Key: P",                      
    "Key: O",                      
    "Key: I",                      
    "Key: U",                      
    "Key: Y",                      
    "Key: #",                      
    "Key: '",                      
    "Key: ;",                      
    "Key: L",                      
    "Key: K",                      
    "Key: J",                      
    "Key: H",                      
    "Key: Right Shift",            
    "Key: /",                      
    "Key: .",                      
    "Key: ,",                      
    "Key: M",                      
    "Key: N",                      
    "Key: Right Control",          
    "Key: Right Windows",          
    "Key: Right Fn",               
    "Key: Right Alt",              
    "Key: T6",                     
    "Key: T5",                     
    "Key: T8",                     
    "Key: T7",             
};

RGBController_DygmaRaise::RGBController_DygmaRaise(DygmaRaiseController* controller_ptr) 
{
    controller      = controller_ptr;

    name            = "Raise";
    vendor          = "Dygma";
    type            = DEVICE_TYPE_KEYBOARD;
    description     = "Dygma Raise Split Mech KB";
    location        = controller->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_DygmaRaise::~RGBController_DygmaRaise() 
{
    delete controller;
}

void RGBController_DygmaRaise::SetupZones() 
{
    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(size_t i=0; i<3; i++)
    {
        zone new_zone;
        new_zone.name                   = zone_names[i];
        new_zone.type                   = zone_types[i];
        new_zone.leds_min               = zone_sizes[i];
        new_zone.leds_max               = zone_sizes[i];
        new_zone.leds_count             = zone_sizes[i];

        if(i==0)
        {
            new_zone.matrix_map             = new matrix_map_type;
            new_zone.matrix_map->height     = 6;
            new_zone.matrix_map->width      = 14;
            new_zone.matrix_map->map        = (unsigned int *)&kb_matrix_map_ISO;
        }
        else if(i==1)
        {
            new_zone.matrix_map             = new matrix_map_type;
            new_zone.matrix_map->height     = 11;
            new_zone.matrix_map->width      = 14;
            new_zone.matrix_map->map        = (unsigned int *)&underglow_matrix;
        }
        
        zones.push_back(new_zone);
    }
    
    /*---------------------------------------------------------*\
    | Set up keyboard LEDs                                      |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < zone_sizes[0]; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx];
        leds.push_back(new_led);
    }

    /*---------------------------------------------------------*\
    | Set up underglow LEDs                                     |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < zone_sizes[1]; led_idx++)
    {
        led new_led;
        new_led.name  = "Underglow";
        leds.push_back(new_led);
    }

    /*---------------------------------------------------------*\
    | Set up Neuron LED                                         |
    \*---------------------------------------------------------*/
    led new_led;
    new_led.name = "Neuron";
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_DygmaRaise::ResizeZone(int /*zone*/, int /*new_size*/) 
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}


void RGBController_DygmaRaise::DeviceUpdateLEDs() 
{
    controller->SendDirect(colors,leds.size());
}

void RGBController_DygmaRaise::UpdateZoneLEDs(int /*zone*/) 
{
    DeviceUpdateLEDs();
}

void RGBController_DygmaRaise::UpdateSingleLED(int /*led*/) 
{
    DeviceUpdateLEDs();
}

void RGBController_DygmaRaise::SetCustomMode() 
{
    active_mode = 0;
}

void RGBController_DygmaRaise::DeviceUpdateMode() 
{
    
}
