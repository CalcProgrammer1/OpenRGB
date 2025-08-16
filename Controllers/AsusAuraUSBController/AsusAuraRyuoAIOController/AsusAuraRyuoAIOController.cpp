/*---------------------------------------------------------*\
| AsusAuraRyuoAIOController.cpp                             |
|                                                           |
|   Driver for ASUS Aura Ryuo                               |
|                                                           |
|   Cooper Hall (geobot19 / Geo_bot)            08 Apr 2022 |
|   using snipets from Chris M (Dr.No)                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "AsusAuraRyuoAIOController.h"

AsusAuraRyuoAIOController::AsusAuraRyuoAIOController(hid_device* dev_handle, const char* path, std::string dev_name) : AuraUSBController(dev_handle, path, dev_name)
{
    /*-----------------------------------------------------*\
    | Add addressable devices                               |
    |    Manually adding device info for now                |
    |    TODO: Implement config table accurately            |
    \*-----------------------------------------------------*/
    device_info.push_back({0x00, 0x00, 12, 0, AuraDeviceType::FIXED});
}

AsusAuraRyuoAIOController::~AsusAuraRyuoAIOController()
{
    /*---------------------------------------------------------*\
    | HID device is closed in the base class                    |
    \*---------------------------------------------------------*/
}

std::string AsusAuraRyuoAIOController::GetLocation()
{
	return("HID: " + location);
}

void AsusAuraRyuoAIOController::SetMode(unsigned char /*channel*/, unsigned char /*mode*/, unsigned char /*red*/, unsigned char /*grn*/, unsigned char /*blu*/)
{
    /*---------------------------------------------------------*\
    | This interface is not used in this controller however is  |
    |   required by the abstract class                          |
    \*---------------------------------------------------------*/
}

void AsusAuraRyuoAIOController::SetMode(unsigned char mode, unsigned char speed, unsigned char direction, RGBColor colour)
{
	//check if update is needed
	if(!((current_mode == mode) && (ToRGBColor(current_red, current_green, current_blue) == colour) && (current_speed == speed) && (current_direction == direction)))
	{
		current_mode        = mode;
		current_speed       = speed;
		current_direction   = direction;
		current_red         = RGBGetRValue(colour);
		current_green       = RGBGetGValue(colour);
		current_blue        = RGBGetBValue(colour);
		SendUpdate();
	}
}

void AsusAuraRyuoAIOController::SetChannelLEDs(unsigned char /*channel*/, RGBColor* /*colors*/, unsigned int /*num_colors*/)
{
	/*---------------------------------------------------------*\
	| This interface is not used in this controller however is  |
	|   required by the abstract class                          |
	\*---------------------------------------------------------*/
}

void AsusAuraRyuoAIOController::SetLedsDirect(RGBColor * led_colours, uint8_t led_count)
{
	uint8_t buffer[write_packet_size]   = { 0xEC, 0x40, 0x80, 0x00, led_count };

	/*---------------------------------------------------------*\
	| Set the colour bytes in the packet                        |
	\*---------------------------------------------------------*/
	for(uint8_t index = 0; index < led_count; index++)
	{
		uint8_t offset      = (index * 3) + RED_BYTE;

		buffer[offset + 0]  = RGBGetRValue(led_colours[index]);
		buffer[offset + 1]  = RGBGetGValue(led_colours[index]);
		buffer[offset + 2]  = RGBGetBValue(led_colours[index]);
	}

	hid_write(dev, buffer, write_packet_size);
}

void AsusAuraRyuoAIOController::GetStatus()
{
	uint8_t buffer[write_packet_size]   = { 0xEC, 0x01, 0x02 };

	hid_write(dev, buffer, write_packet_size);
	hid_read_timeout(dev, buffer, read_packet_size, ASUSAURARYUOAIOCONTROLLER_TIMEOUT);

	current_red   = buffer[RED_BYTE - 1];
	current_green = buffer[GREEN_BYTE - 1];
	current_blue  = buffer[BLUE_BYTE - 1];
}

void AsusAuraRyuoAIOController::SendUpdate()
{
	uint8_t buffer[write_packet_size];

	/*-----------------------------------------------------*\
	| Zero out buffer                                       |
	\*-----------------------------------------------------*/
	memset(buffer, 0x00, write_packet_size);

	buffer[REPORTID_BYTE] = reportid;
	buffer[COMMAND_BYTE]  = modefx;
	buffer[ZONE_BYTE]     = 0;
	buffer[PROGRAM_ID_BYTE]  = program_id;
	buffer[MODE_BYTE]     = current_mode;
	buffer[RED_BYTE]      = current_red;
	buffer[GREEN_BYTE]    = current_green;
	buffer[BLUE_BYTE]     = current_blue;

	buffer[DIRECTION_BYTE]= current_direction;
	buffer[SPEED_BYTE]    = current_speed;

	hid_write(dev, buffer, write_packet_size);
}

