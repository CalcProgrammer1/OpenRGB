/*-----------------------------------------*\
|  RGBFusion2SMBusController.cpp            |
|                                           |
|  Driver for Gigabyte Aorus RGB Fusion 2   |
|  SMBus lighting controller                |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/12/2020   |
|  Matt Harper			5/5/2020    |
\*-----------------------------------------*/

#include "RGBFusion2SMBusController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

RGBFusion2SMBusController::RGBFusion2SMBusController(i2c_smbus_interface* bus, rgb_fusion_dev_id dev)
{
    this->bus = bus;
    this->dev = dev;

    memset(led_data, 0, 10*16);

    led_count = 10;	// Protocol supports 10 'slots'
}

unsigned int RGBFusion2SMBusController::GetLEDCount()
{
	return(led_count);
}

std::string RGBFusion2SMBusController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return(return_string);
}

void RGBFusion2SMBusController::Apply()
{
    /*
     * Given that reading the existing state from the device is not yet possible,
     * unfortunately we may overwrite existing device states if the state transition did
     * not occur within in the same OpenRGB instance.
     * Current behavior is non-ideal but the best we have.
     */
    for (int i = 0; i < 5; i++) {
        #ifdef DEBUG
        std::cout << "0x" << std::hex << (int)RGB_FUSION_2_LED_START_ADDR + 2*i << "\t";
	for (int j = 0; j < 2; j++) {
	    for (int k = 0; k < 16; k++) {
                std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)led_data[2*i+j][k] << " ";
	    }
	}
	std::cout << std::endl;
        #endif

	bus->i2c_smbus_write_block_data(RGB_FUSION_2_SMBUS_ADDR,
					RGB_FUSION_2_LED_START_ADDR + 2*i,
					32,			// Writes occur in 32 byte blocks
					led_data[i*2]);
    }

    // Protocol expects terminating sequence 0x01ff written to register 0x17
    bus->i2c_smbus_write_word_data(RGB_FUSION_2_SMBUS_ADDR,
				   RGB_FUSION_2_APPLY_ADDR,
				   RGB_FUSION_2_ACTION_APPLY);
}

void RGBFusion2SMBusController::SetLEDEffect
    (
    unsigned int    led,
    int             mode,
    unsigned int    speed,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
	led_data[led][RGB_FUSION_2_IDX_MODE]        = mode;
	led_data[led][RGB_FUSION_2_IDX_RED]         = red;
	led_data[led][RGB_FUSION_2_IDX_GREEN]       = green;
	led_data[led][RGB_FUSION_2_IDX_BLUE]        = blue;
	led_data[led][RGB_FUSION_2_IDX_BRIGHTNESS]  = 0x64;

	switch (mode) {
	    case RGB_FUSION_2_MODE_PULSE:
		// Timer 1: On time
		// Timer 2: Off time
		led_data[led][RGB_FUSION_2_TIMER_1_LSB] = 0x20 * speed;
		led_data[led][RGB_FUSION_2_TIMER_1_MSB] = 0x03 * speed;
		led_data[led][RGB_FUSION_2_TIMER_2_LSB] = 0x20 * speed;
		led_data[led][RGB_FUSION_2_TIMER_2_MSB] = 0x03 * speed;
		break;

	    case RGB_FUSION_2_MODE_COLOR_CYCLE:
	        // Timer 1: Cycle time
	        led_data[led][RGB_FUSION_2_TIMER_1_LSB] = 0x00;
	        led_data[led][RGB_FUSION_2_TIMER_1_MSB] = 0x03 * speed;
	        led_data[led][RGB_FUSION_2_IDX_OPT_1]   = 0x07;		// Number of colors to cycle through. Valid range [1-7]
	        led_data[led][RGB_FUSION_2_IDX_OPT_2]   = 0x00;		// Color cycle, or color cycle and pulse. [0,1]
		break;

	    case RGB_FUSION_2_MODE_FLASHING:
	        /* Timer 1: On time
	         * Timer 2: Interval
	         * Timer 3: Cycle time */
		led_data[led][RGB_FUSION_2_TIMER_1_LSB] = 0x64;
                led_data[led][RGB_FUSION_2_TIMER_1_MSB] = 0;
                led_data[led][RGB_FUSION_2_TIMER_2_LSB] = 0xc8;
                led_data[led][RGB_FUSION_2_TIMER_2_MSB] = 0;
                led_data[led][RGB_FUSION_2_TIMER_3_LSB] = 0xd0;
                led_data[led][RGB_FUSION_2_TIMER_3_MSB] = 0x07;

	        led_data[led][RGB_FUSION_2_IDX_OPT_1] = speed;		// Controls number of flashes
		break;

	}
}

