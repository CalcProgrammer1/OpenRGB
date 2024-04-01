/*-------------------------------------------------------------------*\
|  BloodyB820RController.cpp                                          |
|                                                                     |
|  Driver for A4Tech Bloody B820R Keyboard Controller                 |
|                                                                     |
|  Mohammed Julfikar Ali Mahbub (o-julfikar)          01 Apr 2024     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "LogManager.h"
#include "BloodyB820RController.h"

static uint8_t map_layout1[58][2] = {
        {6},
        {7},
        {8},
        {9},
        {10},
        {11},
        {12},
        {13},
        {14},
        {15},
        {16},
        {17},
        {18},
        {19},
        {20},
        {21},
        {22},
        {23},
        {24},
        {25},
        {26},
        {27},
        {28},
        {29},
        {30},
        {31},
        {32},
        {33},
        {34},
        {35},
        {36},
        {37},
        {38},
        {39},
        {40},
        {41},
        {42},
        {43},
        {44},
        {45},
        {46},
        {47},
        {48},
        {49},
        {50},
        {51},
        {52},
        {53},
        {54},
        {55},
        {56},
        {57},
        {58},
        {59},
        {60},
        {61},
        {62},
        {63}
};

static uint8_t map_layout2[58][2] = {
        {6},
        {7},
        {8},
        {9},
        {10},
        {11},
        {12},
        {13},
        {14},
        {15},
        {16},
        {17},
        {18},
        {19},
        {20},
        {21},
        {22, 55},
        {23},
        {24},
        {25},
        {26},
        {27},
        {28},
        {29},
        {30},
        {31},
        {32},
        {33},
        {34},
        {35},
        {36},
        {37},
        {38},
        {39},
        {40},
        {41},
        {42},
        {43},
        {44},
        {45},
        {46},
        {47},
        {48},
        {49},
        {50},
        {51},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {}
};


BloodyB820RController::BloodyB820RController(hid_device* dev_handle, const char* path)
{
    dev                 = dev_handle;
    location            = path;

    InitDevice();
}

BloodyB820RController::~BloodyB820RController()
{
    hid_close(dev);
}

std::string BloodyB820RController::GetDeviceName()
{
    const int szTemp    = HID_MAX_STR;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName  = std::wstring(tmpName);
    std::string name    = std::string(wName.begin(), wName.end());

    return name;
}

std::string BloodyB820RController::GetSerial()
{
    const int szTemp    = HID_MAX_STR;
    wchar_t   tmpName[szTemp];

    hid_get_serial_number_string(dev, tmpName, szTemp);
    std::wstring wName  = std::wstring(tmpName);
    std::string serial  = std::string(wName.begin(), wName.end());

    return serial;
}

std::string BloodyB820RController::GetLocation()
{
    return("HID: " + location);
}

void  BloodyB820RController::InitDevice()
{
    uint8_t buffer[BLOODY_B820R_PACKET_SIZE]   = { 0x07, 0x03, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);

    buffer[BLOODY_B820R_MODE_BYTE]                   = 0;
    buffer[BLOODY_B820R_DATA_BYTE]                   = 1;

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);
}

void  BloodyB820RController::ReleaseDevice()
{
    uint8_t buffer[BLOODY_B820R_PACKET_SIZE]   = { 0x07, 0x03, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);

    buffer[BLOODY_B820R_MODE_BYTE]                   = 0;

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);
}

void BloodyB820RController::SetMode(
        int             mode,
        int             speed,
        unsigned char   brightness,
        unsigned char   dir,
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue) {


    switch (mode) {
        case BLOODY_B820R_MODE_DIRECT:
        case BLOODY_B820R_MODE_INIT:
            InitDevice();
            break;
        case BLOODY_B820R_MODE_RELEASE:
            ReleaseDevice();
            break;
        default:
            break;
    }

}

void BloodyB820RController::SetLEDDirect(std::vector<RGBColor> colors)
{

    uint8_t R1Buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x07, 0x00, 0x00 };
    uint8_t R2Buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x08, 0x00, 0x00 };
    uint8_t G1Buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x09, 0x00, 0x00 };
    uint8_t G2Buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x0a, 0x00, 0x00 };
    uint8_t B1Buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x0b, 0x00, 0x00 };
    uint8_t B2Buffer[BLOODY_B820R_PACKET_SIZE] = { 0x07, 0x03, 0x06, 0x0c, 0x00, 0x00 };

    /*-----------------------------------------------------------------*\
    | Set up Direct packet                                              |
    |   packet_map is the index of the Key from full_matrix_map and     |
    |   the value is the position in the direct packet buffer           |
    \*-----------------------------------------------------------------*/


    for(size_t i = 0; i < colors.size(); i++)
    {
        RGBColor key                                            = colors[i];


        size_t layout1_length = sizeof(map_layout1) / sizeof(map_layout1[0]);

        if (i < layout1_length) {
            for(auto j : map_layout1[i]) {
                if (j != 0) {
                    R1Buffer[j] = RGBGetRValue(key);
                    G1Buffer[j] = RGBGetGValue(key);
                    B1Buffer[j] = RGBGetBValue(key);
                }
            }
        } else {
            for(auto j : map_layout2[i - layout1_length]) {
                if (j != 0) {
                    R2Buffer[j] = RGBGetRValue(key);
                    G2Buffer[j] = RGBGetGValue(key);
                    B2Buffer[j] = RGBGetBValue(key);
                }
            }
        }
    }

    hid_send_feature_report(dev, R1Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, R2Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, G1Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, G2Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, B1Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, B2Buffer, BLOODY_B820R_PACKET_SIZE);
}