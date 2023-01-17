/*-----------------------------------------*\
|  HyperXQuadcastSController.h              |
|                                           |
|  Implementation for the HyperX            |
|  Quadcast S RGB microphone                |
|                                           |
|  Matt Silva (thesilvanator) 2022          |
\*-----------------------------------------*/
#pragma once

#include <string>
#include <hidapi/hidapi.h>
#include "LogManager.h"
#include "RGBController.h"

#define HXQS_PACKET_SIZE 64 + 1
#define HXQS_FRAME_SIZE  8

// wrapper typedefs
typedef int (*HXQS_Report_Send_t)(hid_device*, const unsigned char*, size_t);
typedef int (*HXQS_Report_Get_t)(hid_device*, unsigned char*, size_t);
typedef int (*HXQS_Get_Serial_t)(hid_device*, wchar_t*, size_t);
typedef hid_device* (*HXQS_hid_open_path_t)(const char*);
typedef hid_device_info* (*HXQS_hid_enumerate_t)       (unsigned short, unsigned short);
typedef void             (*HXQS_hid_free_enumeration_t)(hid_device_info*);
typedef void (*HXQS_hid_close_t)(hid_device*);
typedef const wchar_t*   (*HXQS_hid_error_t)           (hid_device*);

/*----------------------------------------------------*\
|  See comment at top of HyperXQuadcastSDetect.cpp for |
|  details about the hidapi wrapper for this device    |
\*----------------------------------------------------*/
struct HXQS_HIDAPI_WRAPPER {
    void*                       dyn_handle;
    HXQS_Report_Send_t          send_feature_report;
    HXQS_Report_Get_t           get_feature_report;
    HXQS_Get_Serial_t           get_serial_num_string;
    HXQS_hid_open_path_t        open_path;
    HXQS_hid_enumerate_t        enumerate;
    HXQS_hid_free_enumeration_t free_enumeration;
    HXQS_hid_close_t            close;
    HXQS_hid_error_t            error;
};

class HyperXQuadcastSController
{
public:
    HyperXQuadcastSController(hid_device* dev, HXQS_HIDAPI_WRAPPER wrapper, std::string path);
    ~HyperXQuadcastSController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect(std::vector<RGBColor> color_data);
    void SaveColors(std::vector<RGBColor> colors, unsigned int num_frames);

private:
    hid_device*         dev;
    std::string         location;
    std::string         serial_number;
    std::mutex          lock;
    HXQS_HIDAPI_WRAPPER hidapi_wrapper;

    void                SendEOT(uint8_t frame_count);
    void                SendToRegister(uint8_t reg, uint8_t param1, uint8_t param2);
};
