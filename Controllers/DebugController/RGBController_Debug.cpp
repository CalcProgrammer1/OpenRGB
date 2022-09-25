#include "RGBController_Debug.h"

#include <algorithm>
#include <cstring>

/**------------------------------------------------------------------*\
    @name Debug
    @category Unknown
    @type I2C
    @save :x:
    @direct :x:
    @effects :x:
    @detectors DetectDebugControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_Debug::RGBController_Debug()
{

}

void RGBController_Debug::ResizeZone(int index, int new_size)
{
    //Make sure that it isn't out of bounds (negative numbers)
    if(new_size < int(zones[index].leds_min))
    {
         new_size = zones[index].leds_min;
    }

    // Same thing as the above line except for over 100
    if(new_size > int(zones[index].leds_max))
    {
        new_size = zones[index].leds_max;
    }

    // Store the previous amount of LEDs
    int old_size = zones[index].leds_count;

    // Set the LED count in the zone to the new ammount
    zones[index].leds_count = new_size;

    // Set the new ammount of LEDs for to the new size
    size_t old_leds_size = leds.size();

    // Add the new ammount of LEDs to the old ammount
    size_t new_leds_size = leds.size() - old_size + new_size;

    leds.resize(std::max(old_leds_size, new_leds_size));

    memmove((void *)(&leds[zones[index].start_idx] + old_leds_size), (const void *)(&leds[zones[index].start_idx] + new_leds_size), (old_leds_size - zones[index].start_idx - old_size) * sizeof(led));

    leds.resize(new_leds_size);

    for(int i = 0; i < new_size; ++i)
    {
        leds[zones[index].start_idx + i].name = "Linear LED " + std::to_string(i);
    }

    SetupColors();
}
