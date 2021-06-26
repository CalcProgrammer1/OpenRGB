/*------------------------------------------*\
|  HyperXAlloyOriginsCoreController.cpp      |
|                                            |
|  Driver for HyperX Alloy Origins Core      |
|  RGB Keyboard lighting controller          |
|                                            |
|  Volodymyr Nazarchuk (Vavooon) 4/28/2021   |
|  mike white (kamaaina)          6/9/2021   |
\*------------------------------------------*/

#include "HyperXAlloyOriginsCoreController.h"
#include <cstring>

// Skip these indices in the color output
static unsigned int skip_idx[] = {6, 7, 14, 15, 22, 23, 30, 31, 38, 39, 44, 46, 47, 54, 55, 58, 60, 61, 62, 63, 70, 71, 78, 79, 86, 87, 94, 95, 101, 102, 103, 109, 110, 111, 118, 119};

HyperXAlloyOriginsCoreController::HyperXAlloyOriginsCoreController(hid_device* dev_handle, hid_device_info* dev_info)
{
    dev              = dev_handle;
    location         = dev_info->path;

    /*-----------------------------------------------------*\
    | Get the firmware version from the device info         |
    \*-----------------------------------------------------*/
    char fw_version_buf[8];
    memset(fw_version_buf, '\0', sizeof(fw_version_buf));

    unsigned short version = dev_info->release_number;
    sprintf(fw_version_buf, "%.2X.%.2X", (version & 0xFF00) >> 8, version & 0x00FF);

    firmware_version = fw_version_buf;
    isDimming = false;
    cur_color = color0 = color1 = 0;
    memset(&hsv, 0, sizeof(hsv_t));
    column = speed = direction = 0;
    brightness_lower_bound = brightness_upper_bound = 0;
    for (int i=0; i<87; i++)
       colors_.push_back(0);
    memset(buf, 0x00, sizeof(buf));
    color_end = false;
    iteration = 0;
    data = nullptr;
}

HyperXAlloyOriginsCoreController::~HyperXAlloyOriginsCoreController()
{
    hid_close(dev);
}

std::string HyperXAlloyOriginsCoreController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXAlloyOriginsCoreController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string HyperXAlloyOriginsCoreController::GetFirmwareVersion()
{
    return(firmware_version);
}

void HyperXAlloyOriginsCoreController::SetLEDs(std::vector<RGBColor> colors, unsigned char mode)
{
    if (mode == HYPERX_AOC_MODE_SWIPE)
    {
        if (iteration < (HYPERX_AOC_SPEED_MAX - speed))
        {
            iteration++;
            return;
        }

        if (direction == MODE_DIRECTION_LEFT)
        {
            if (column == 19)
            {
               column = 0;
               color_end = !color_end;
            }
        }
        else
        {
            if (column < 0)
            {
               column = 18;
               color_end = !color_end;
            }
        }
        cur_color = color_end ? color1 : color0;

        unsigned int index;
        for (int i=0; i<6; i++)
        {
            index = data[i][column];
            if (index != 0xFFFFFFFF)
               colors_[index] = cur_color;
        }
        direction == MODE_DIRECTION_LEFT ? column++ : column--;
        colors = colors_;
        iteration = 0;
    }
    else
    {
       memset(buf, 0x00, sizeof(buf));
    }

    for(unsigned int skip_cnt = 0; skip_cnt < (sizeof(skip_idx) / sizeof(skip_idx[0])); skip_cnt++)
    {
        colors.insert(colors.begin() + skip_idx[skip_cnt], 0x00000000);
    }

    if (mode == HYPERX_AOC_MODE_BREATHING)
    {
        if (isDimming)
        {
           hsv.value -= (4 * speed);
           if (hsv.value <= brightness_lower_bound)
           {
               hsv.value = 1;
               isDimming = false;
           }
        }
        else
        {
           hsv.value += (4 * speed);
           if (hsv.value >= brightness_upper_bound)
           {
               hsv.value = 255;
               isDimming = true;
           }
        }
        color0 = hsv2rgb(&hsv);
    }

    int offset = 0;
    int rowPos = 0;

    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        if (color_idx > 0 && color_idx % 16 == 0)
        {
            offset += 48;
            rowPos = 0;
        }

        
        if (mode == HYPERX_AOC_MODE_BREATHING)
        {
            buf[rowPos + offset]      = RGBGetGValue(color0);
            buf[rowPos + offset + 16] = RGBGetRValue(color0);
            buf[rowPos + offset + 32] = RGBGetBValue(color0);
        }
        else
        {
            buf[rowPos + offset]      = RGBGetGValue(colors[color_idx]);
            buf[rowPos + offset + 16] = RGBGetRValue(colors[color_idx]);
            buf[rowPos + offset + 32] = RGBGetBValue(colors[color_idx]);
        }

        rowPos++;
    }

    unsigned int sentBytes   = 0;
    unsigned int bytesToSend = sizeof(buf);
    unsigned int payloadSize = 60;
    unsigned int seq         = 0;

    while(sentBytes < bytesToSend)
    {
        if (bytesToSend - sentBytes < payloadSize)
        {
            payloadSize = bytesToSend - sentBytes;
        }

        unsigned char packet[65];
        memset(packet, 0x00, sizeof(packet));

        packet[1] = 0xA2;
        packet[2] = seq++;
        packet[4] = payloadSize;

        memcpy(&packet[5], &buf[sentBytes], payloadSize);
        hid_write(dev, packet, 65);

        sentBytes += payloadSize;
    }
}

void HyperXAlloyOriginsCoreController::SetMode(unsigned char mode_value, unsigned char direction, unsigned char speed, std::vector<RGBColor> colors, matrix_map_type* matrix_map)
{
   memset(buf, 0, sizeof(buf));
   switch (mode_value)
   {
      case HYPERX_AOC_MODE_DIRECT:
         break;
      case HYPERX_AOC_MODE_BREATHING:
         color0 = colors[0];
         rgb2hsv(color0, &hsv);
         this->speed = speed;
         brightness_lower_bound = 255 % (4 * speed);
         brightness_upper_bound = 255 - brightness_lower_bound;
         break;
      case HYPERX_AOC_MODE_SWIPE:
         this->speed = speed;
         color0 = colors[0];
         cur_color = color0;
         color1 = colors[1];
         this->direction = direction;
         column = 0;
         data = (unsigned int (*)[19])matrix_map->map;
         break;
   }
}

