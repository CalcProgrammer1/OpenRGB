/*-----------------------------------------*\
|  RGBController.h                          |
|                                           |
|  Definitions and types for generic RGB    |
|  lighting controller interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/2/2019    |
\*-----------------------------------------*/

#pragma once

#include <vector>
#include <string>

typedef unsigned int RGBColor;

#define RGBGetRValue(rgb)   (rgb & 0x000000FF)
#define RGBGetGValue(rgb)   ((rgb >> 8) & 0x000000FF)
#define RGBGetBValue(rgb)   ((rgb >> 16) & 0x000000FF)

#define ToRGBColor(r, g, b) ((b << 16) | (g << 8) | (r))

typedef struct
{
    std::string         name;   /* LED name         */
} led;

typedef struct
{
    std::string         name;   /* Mode name        */
} mode;

typedef int zone_type;

enum
{
    ZONE_TYPE_SINGLE,
    ZONE_TYPE_LINEAR,
    ZONE_TYPE_MATRIX
};

typedef struct
{
    std::string         name;   /* Zone name        */
	zone_type           type;   /* Zone type        */
    std::vector<std::vector<int>>
                        map;    /* LED index map    */
} zone;

class RGBController
{
public:
    std::string         name;   /* controller name  */
    std::vector<led>    leds;   /* LEDs             */
    std::vector<zone>   zones;  /* Zones            */
    std::vector<mode>   modes;  /* Modes            */

    virtual int  GetMode()         = 0;
    virtual void SetMode(int mode) = 0;
    virtual void SetAllLEDs(RGBColor color) = 0;
	virtual void SetAllZoneLEDs(int zone, RGBColor color) = 0;
	virtual void SetLED(int led, RGBColor color) = 0;
};