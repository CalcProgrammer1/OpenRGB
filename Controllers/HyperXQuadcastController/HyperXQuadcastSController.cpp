/*-----------------------------------------*\
|  HyperXQuadcastSController.cpp            |
|                                           |
|  Implementation for the HyperX            |
|  Quadcast S RGB microphone                |
|                                           |
|  Matt Silva (thesilvanator) 2022          |
\*-----------------------------------------*/
#include "HyperXQuadcastSController.h"

HyperXQuadcastSController::HyperXQuadcastSController(hidapi_wrapper hid_wrapper, hid_device* dev_handle, std::string path)
{
    wrapper     = hid_wrapper;
    dev         = dev_handle;
    location    = path;

    wchar_t serial_string[128];
    int ret = wrapper.hid_get_serial_num_string(dev, serial_string, 128);

    if(ret != 0)
    {
        serial_number = "";
    }
    else
    {
        std::wstring return_wstring = serial_string;
        serial_number = std::string(return_wstring.begin(), return_wstring.end());
    }
}

HyperXQuadcastSController::~HyperXQuadcastSController()
{
    if(dev)
    {
        wrapper.hid_close(dev);
    }
}

std::string HyperXQuadcastSController::GetDeviceLocation()
{
    return location;
}

std::string HyperXQuadcastSController::GetSerialString()
{
    return serial_number;
}


void HyperXQuadcastSController::SaveColors(std::vector<RGBColor> colors, unsigned int num_frames)
{
    using namespace std::chrono_literals;

    int res;
    unsigned int num_color_packets        = 0;
    unsigned int frame                    = 0;
    unsigned char color[HXQS_PACKET_SIZE] = {0};
    unsigned char empty[HXQS_PACKET_SIZE] = {0};

    num_color_packets = num_frames/8;
    if(num_frames % 8)
    {
        num_color_packets++;
    }

    lock.lock();

    /*---------------------------------------------------------*\
    | Start Save Transaction                                    |
    |   0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x53, (uint8_t)num_color_packets, 0);

    while(frame < num_frames)
    {
        memset(color, 0, HXQS_PACKET_SIZE);

        unsigned int i = 0;
        while(i < 8 && frame < num_frames)
        {
            int index           = HXQS_FRAME_SIZE * i;
            RGBColor top        = colors[frame*2];
            RGBColor bot        = colors[frame*2 + 1];

            color[index + 1]    = 0x81;
            color[index + 2]    = RGBGetRValue(top);
            color[index + 3]    = RGBGetGValue(top);
            color[index + 4]    = RGBGetBValue(top);
            color[index + 5]    = 0x81;
            color[index + 6]    = RGBGetRValue(bot);
            color[index + 7]    = RGBGetGValue(bot);
            color[index + 8]    = RGBGetBValue(bot);

            i++;
            frame++;
        }

        std::this_thread::sleep_for(15ms);
        res = wrapper.hid_send_feature_report(dev,color,HXQS_PACKET_SIZE);
    }

    /*---------------------------------------------------------*\
    | Post Save Transaction                                     |
    |   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x02, 0, 0);

    /*---------------------------------------------------------*\
    | Stop Save Transaction                                     |
    |   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x23, 1, 0);

    SendEOT((uint8_t)num_frames);

    /*---------------------------------------------------------*\
    | Post Save Transaction                                     |
    |   0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00          |
    \*---------------------------------------------------------*/
    SendToRegister(0x02, 0, 0);

    lock.unlock();

    // likes to have one temporary direct packet after the save for some reason
    SendDirect(colors);
}

void HyperXQuadcastSController::SendDirect(std::vector<RGBColor> colors)
{
    using namespace std::chrono_literals;

    if(colors.size() != 2)
    {
        LOG_ERROR("[HyperX Quadcast S] Unable to send direct packet, incorrect size: %d", colors.size());
        return;
    }

    int res;
    RGBColor c1 = colors[0];
    RGBColor c2 = colors[1];
    uint8_t buffer[HXQS_PACKET_SIZE];

    // colour packet
    memset(buffer,0,HXQS_PACKET_SIZE);

    //buffer[0x00]     = 0;               //buffer requires 0 at index 0 as device does not use ReportIDs
    buffer[0x01]    = 0x81;
    buffer[0x02]    = RGBGetRValue(c1);
    buffer[0x03]    = RGBGetGValue(c1);
    buffer[0x04]    = RGBGetBValue(c1);
    buffer[0x05]    = 0x81;
    buffer[0x06]    = RGBGetRValue(c2);
    buffer[0x07]    = RGBGetGValue(c2);
    buffer[0x08]    = RGBGetBValue(c2);

    lock.lock();

    res = wrapper.hid_send_feature_report(dev, buffer, HXQS_PACKET_SIZE);
    std::this_thread::sleep_for(15ms);

    SendToRegister(0xF2, 0, 1);

    lock.unlock();
}

void HyperXQuadcastSController::SendEOT(uint8_t frame_count)
{
    using namespace std::chrono_literals;
    uint8_t buffer[HXQS_PACKET_SIZE];

    memset(buffer,0,HXQS_PACKET_SIZE);

    //buffer[0x00]     = 0;           //buffer requires 0 at index 0 as device does not use ReportIDs
    buffer[0x01]    = 0x08;
    buffer[0x3C]    = 0x28;
    buffer[0x3D]    = frame_count;
    buffer[0x3E]    = 0x00;
    buffer[0x3F]    = 0xAA;
    buffer[0x40]    = 0x55;

    int result      = wrapper.hid_send_feature_report(dev,buffer,HXQS_PACKET_SIZE);
    LOG_DEBUG("[HyperX Quadcast S] SendEOT with frame count %02X wrote %d bytes", frame_count, result);
    std::this_thread::sleep_for(15ms);
}

void HyperXQuadcastSController::SendToRegister(uint8_t reg, uint8_t param1, uint8_t param2)
{
    using namespace std::chrono_literals;
    uint8_t buffer[HXQS_PACKET_SIZE];

    memset(buffer,0,HXQS_PACKET_SIZE);

    //buffer[0x00]     = 0;           //buffer requires 0 at index 0 as device does not use ReportIDs
    buffer[0x01]    = 0x04;
    buffer[0x02]    = reg;         // 0xF2 Apply, 0x53 Save
    buffer[0x08]    = param1;
    buffer[0x09]    = param2;

    int result      = wrapper.hid_send_feature_report(dev, buffer, HXQS_PACKET_SIZE);
    if(result < 0)
    {
        LOG_ERROR("[HyperX Quadcast S] SendToRegister failed: %ls", wrapper.hid_error(dev));
    }
    else
    {
        LOG_DEBUG("[HyperX Quadcast S] SendToRegister %02X with P1 %02X P2 %02X wrote %d bytes", reg, param1, param2, result);
    }
    std::this_thread::sleep_for(15ms);
}
