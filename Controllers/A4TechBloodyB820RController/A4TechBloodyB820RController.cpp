/*-------------------------------------------------------------------*\
|  DarkProjectKeyboardController.cpp                                  |
|                                                                     |
|  Driver for DarkProjectKeyboard USB Controller                      |
|                                                                     |
|  Chris M (DrNo)          8 Apr 2022                                 |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "LogManager.h"
#include "A4TechBloodyB820RController.h"
#include <iostream>
#include <sstream>
#include <string>
#include "fstream"

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

//static uint8_t packet_map1[104] =
//        {
///*00        ESC     F1      F2      F3      F4      F5      F6      F7      F8      F9      F10     F11     F12  */
//                6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//
///*00        PS      SL      PB  */
//                19, 20, 21,
//
//
///*10        `       1       2       3       4       5       6       7       8       9       0       -       =  */
//                22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
//
///*10        BCK     INS     HM      PU      NUM     [N /]   [N *]   [N -]  */
//                35, 36, 37, 38, 39, 40, 41, 42,
//
//
//
///*20        TAB     Q       W       E       R       T       Y       U       I       O       P       [       ]  */
//                43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
//
///*20        \       DEL     END     PD      [N 7]   [N 8]   [N 9]   [N +]  */
//                56, 57, 58, 59, 60, 61, 62, 63
//        };
//
//static uint16_t packet_map2[58] =
//        {
//
///*30        LCAPS   A       S       D       F       G       H       J       K       L       ;       '       BRK  */
//            6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//
///*30        [N 4]   [N 5]   [N 6]  */
//            19, 20, 21,
//
//
//
//
///*40        LSHIFT  Z       X       C       V       B       N       M       ,       .       /       RSHIFT  */
//            22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
//
///*40        UP      [N 1]   [N 2]   [N 3]   [N BRK]  */
//            34, 35, 36, 37, 38,
//
//
//
//
///*50        LCTRL   LWIN    LALT    SPC     RALT    RFN     RCNTXT  RCTRL   LEFT    DOWN    RIGHT   [N 0]   [N .]  */
//            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
//
///* Missing Indexes 9, 22, 28, 40, 46, 70, 74, 75, 82, 86, 87, 92, 98, 99, 101 */
//        };




A4TechBloodyB820RController::A4TechBloodyB820RController(hid_device* dev_handle, const char* path)
{
    dev                 = dev_handle;
    location            = path;

    InitDevice();
}

A4TechBloodyB820RController::~A4TechBloodyB820RController()
{
    hid_close(dev);
}

std::string A4TechBloodyB820RController::GetDeviceName()
{
    const int szTemp    = HID_MAX_STR;
    wchar_t tmpName[szTemp];

    hid_get_manufacturer_string(dev, tmpName, szTemp);
    std::wstring wName  = std::wstring(tmpName);
    std::string name    = std::string(wName.begin(), wName.end());

    return name;
}

std::string A4TechBloodyB820RController::GetSerial()
{
    const int szTemp    = HID_MAX_STR;
    wchar_t   tmpName[szTemp];

    hid_get_serial_number_string(dev, tmpName, szTemp);
    std::wstring wName  = std::wstring(tmpName);
    std::string serial  = std::string(wName.begin(), wName.end());

    return serial;
}

std::string A4TechBloodyB820RController::GetLocation()
{
    return("HID: " + location);
}

std::string hexArrayToString(const uint8_t* arr, size_t size) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < size; ++i) {
        ss << std::setw(2) << static_cast<int>(arr[i]);
//        ss << std::hex << static_cast<int>(arr[i]);
    }
    return ss.str();
}

void  A4TechBloodyB820RController::InitDevice()
{
    uint8_t buffer[BLOODY_B820R_PACKET_SIZE]   = { 0x07, 0x03, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);
//    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);

    buffer[BLOODY_B820R_MODE_BYTE]                   = 0;
    buffer[BLOODY_B820R_DATA_BYTE]                   = 1;

    hid_send_feature_report(dev, buffer, BLOODY_B820R_PACKET_SIZE);
}

void A4TechBloodyB820RController::SetLedsDirect(std::vector<RGBColor> colors)
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


//    std::ofstream outputFile("example.txt");
//    if (outputFile.is_open()) {
//        outputFile << colors.size();
//        outputFile.close();
//    }

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
//        uint16_t offset                                         = packet_map1[i];

//        uint8_t[2] offset1 = map_layout1[i];

//        for(auto j : map_layout1[i]) {
//            R1Buffer[j] = RGBGetRValue(key);
//            G1Buffer[j] = RGBGetGValue(key);
//            B1Buffer[j] = RGBGetBValue(key);
//        }

//        uint8_t[2] offset2 = map_layout2[i];

//        for(auto j : map_layout2[i]) {
//            R2Buffer[j] = RGBGetRValue(key);
//            G2Buffer[j] = RGBGetGValue(key);
//            B2Buffer[j] = RGBGetBValue(key);
//        }


//        if (i < colors.size() / 2) {
//            R1Buffer[offset] = RGBGetRValue(key);
//            G1Buffer[offset] = RGBGetGValue(key);
//            B1Buffer[offset] = RGBGetBValue(key);
//        } else {
//            R2Buffer[offset] = RGBGetRValue(key);
//            G2Buffer[offset] = RGBGetGValue(key);
//            B2Buffer[offset] = RGBGetBValue(key);
//        }

//        R1Buffer[offset] = RGBGetRValue(key);
//        R2Buffer[offset] = RGBGetRValue(key);
//        G1Buffer[offset] = RGBGetGValue(key);
//        G2Buffer[offset] = RGBGetGValue(key);
//        B1Buffer[offset] = RGBGetBValue(key);
//        B2Buffer[offset] = RGBGetBValue(key);

//        RGbuffer[DARKPROJECTKEYBOARD_RED_BLUE_BYTE + offset]    = RGBGetRValue(key);
//        RGbuffer[DARKPROJECTKEYBOARD_GREEN_BYTE + offset]       = RGBGetGValue(key);
//        BAbuffer[DARKPROJECTKEYBOARD_RED_BLUE_BYTE + offset]    = RGBGetBValue(key);
    }



    std::ofstream outputFile("example.txt");
    if (outputFile.is_open()) {
        outputFile << colors.size();
        outputFile << "\n";
        outputFile << hexArrayToString(R1Buffer, BLOODY_B820R_PACKET_SIZE);
        outputFile << "\n";
        outputFile << hexArrayToString(R2Buffer, BLOODY_B820R_PACKET_SIZE);
        outputFile << "\n";
        outputFile << hexArrayToString(G1Buffer, BLOODY_B820R_PACKET_SIZE);
        outputFile << "\n";
        outputFile << hexArrayToString(G2Buffer, BLOODY_B820R_PACKET_SIZE);
        outputFile << "\n";
        outputFile << hexArrayToString(B1Buffer, BLOODY_B820R_PACKET_SIZE);
        outputFile << "\n";
        outputFile << hexArrayToString(B2Buffer, BLOODY_B820R_PACKET_SIZE);
        outputFile << "\n";
        outputFile << "Hello world14";
        outputFile.close();
    }


    hid_send_feature_report(dev, R1Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, R2Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, G1Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, G2Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, B1Buffer, BLOODY_B820R_PACKET_SIZE);
    hid_send_feature_report(dev, B2Buffer, BLOODY_B820R_PACKET_SIZE);


//    hid_write(dev, R1Buffer, BLOODY_B820R_PACKET_SIZE);
//    hid_write(dev, R2Buffer, BLOODY_B820R_PACKET_SIZE);
//    hid_write(dev, G1Buffer, BLOODY_B820R_PACKET_SIZE);
//    hid_write(dev, G2Buffer, BLOODY_B820R_PACKET_SIZE);
//    hid_write(dev, B1Buffer, BLOODY_B820R_PACKET_SIZE);
//    hid_write(dev, B2Buffer, BLOODY_B820R_PACKET_SIZE);
}