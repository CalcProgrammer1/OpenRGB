/*-----------------------------------------*\
|  RGBController_E131.h                     |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  E1.31 Streaming ACN interface            |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/18/2019  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "E131Controller.h"

typedef unsigned int e131_rgb_order;

enum
	{
	E131_RGB_ORDER_RGB,
	E131_RGB_ORDER_RBG,
	E131_RGB_ORDER_GRB,
	E131_RGB_ORDER_GBR,
	E131_RGB_ORDER_BRG,
	E131_RGB_ORDER_BGR
	};

struct E131Device
{
	std::string name;
	unsigned int num_leds;
	unsigned int start_universe;
	unsigned int start_channel;
	e131_rgb_order rgb_order;
	zone_type type;
}

class RGBController_E131 : public RGBController
{
public:
    RGBController_E131();
    int GetMode();
    void SetMode(int mode);
    void SetCustomMode();
    void SetAllLEDs(RGBColor color);
    void SetAllZoneLEDs(int zone, RGBColor color);
    void SetLED(int led, RGBColor color);
    void UpdateLEDs();

private:
};