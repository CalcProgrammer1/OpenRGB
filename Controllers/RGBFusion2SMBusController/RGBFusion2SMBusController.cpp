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
	led_data[led][RGB_FUSION_2_IDX_BRIGHTNESS]  = 0x64;	// TODO - is this *really* the max value?

	// TODO - These timing calculations are weird - need to improve
	led_data[led][RGB_FUSION_2_TIMER_1_LSB] = 0x20 * speed;
	led_data[led][RGB_FUSION_2_TIMER_1_MSB] = 0x03 * speed;
	led_data[led][RGB_FUSION_2_TIMER_2_LSB] = 0x20 * speed;
	led_data[led][RGB_FUSION_2_TIMER_2_MSB] = 0x03 * speed;

	// Here be dragons
	// Purpose of each index is not well understood
	// Existing values taken from Windows dump
	// TODO - more thorough understanding could lead to better implementation
	if (mode == RGB_FUSION_2_MODE_FLASHING) {
	    led_data[led][0xc] = 0xd0;			// ???
	    led_data[led][0xd] = 0x07;			// ???
	    led_data[led][0xe] = 0x01 * speed;		// Controls number of flashes

	    // Following four fields appear to both important, and magic
	    // No other values appear to work, but it's probably worth further investigation
	    led_data[led][RGB_FUSION_2_TIMER_1_LSB] = 0x64;
            led_data[led][RGB_FUSION_2_TIMER_1_MSB] = 0;
            led_data[led][RGB_FUSION_2_TIMER_2_LSB] = 0xc8;	// 0x64 * 2 = 0xc8 - Potentially significant?
            led_data[led][RGB_FUSION_2_TIMER_2_MSB] = 0;
	}

	if (mode == RGB_FUSION_2_MODE_COLOR_CYCLE) {
	    led_data[led][0xc] = 0xd0;			// ???
	    led_data[led][0xd] = 0x07;			// ???
	    led_data[led][0xe] = 0x07;			// ???
	}
	// End dragons. Well, probably.

	#ifdef DEBUG
	// Print out hex for debug purposes
	for (int i = 0; i < 16; i++) {
	    std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)led_data[led][i] << " ";
	}
	std::cout << std::endl;
	#endif

	/*
	 * Writes occur in 32 byte blocks
	 * Therefore, writing the second 16 bytes necessitates writing the *first* 16 bytes as well.
	 * Given that reading the existing state from the device is not yet possible,
	 * unfortunately this implies that we may overwrite existing device states if the state
	 * transition did not occur within in the same OpenRGB instance. That is to say, this current
	 * behavior is non-ideal but the best we have.
	 */
        unsigned char oca = 0x20;	// On chip address
	unsigned short chip_offset;
	if (led % 2) {
	    chip_offset = led-1;
	} else {
	    chip_offset = led;
	}
	unsigned char write_addr = oca + chip_offset;

	#ifdef DEBUG
	std::cout << "LED: " << led << "\tWrite address: " << std::hex << (int)write_addr << std::endl;
	#endif
	bus->i2c_smbus_write_block_data(0x68, write_addr, 32, led_data[led]);	// Write 32 byte blocks at a time. Matches RGBF2 dump from Windows app.

	// Protocol expects terminating sequence 0x01ff written to register 0x17
	unsigned short terminator = 0x01ff;
	unsigned char termination_addr = 0x17;
	bus->i2c_smbus_write_word_data(0x68, termination_addr, terminator);
}

