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
};