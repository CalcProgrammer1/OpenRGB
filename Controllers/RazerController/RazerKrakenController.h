/*-----------------------------------------*\
|  RazerKrakenController.h                  |
|                                           |
|  Definitions and types for Razer Kraken   |
|  devices                                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/28/2021   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

/*---------------------------------------------------------*\
| Struct packing macro for GCC and MSVC                     |
\*---------------------------------------------------------*/
#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

union razer_kraken_effect_byte
{
    unsigned char value;

    struct razer_kraken_effect_byte_bits
    {
        unsigned char on_off_static :1;
        unsigned char single_colour_breathing :1;
        unsigned char spectrum_cycling :1;
        unsigned char sync :1;
        unsigned char two_colour_breathing :1;
        unsigned char three_colour_breathing :1;
    } bits;
};

/*---------------------------------------------------------*\
| Razer Kraken Report Types (taken from OpenRazer)          |
\*---------------------------------------------------------*/
PACK(typedef struct razer_kraken_request_report
{
    unsigned char report_id;
    unsigned char destination;
    unsigned char length;
    unsigned char addr_h;
    unsigned char addr_l;
    unsigned char arguments[32];
});

PACK(typedef struct razer_kraken_response_report
{
    unsigned char report_id;
    unsigned char arguments[36];
});

class RazerKrakenController
{
public:
    RazerKrakenController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~RazerKrakenController();

    unsigned int            GetDeviceIndex();
    device_type             GetDeviceType();
    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetName();
    std::string             GetSerialString();

    void                    SetModeCustom(unsigned char red, unsigned char grn, unsigned char blu);
    void                    SetModeOff();
    void                    SetModeSpectrumCycle();
    void                    SetModeStatic(unsigned char red, unsigned char grn, unsigned char blu);

private:
    hid_device*             dev;
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             location;
    std::string             name;
    device_type             type;

    /*---------------------------------------------------------*\
    | Kraken LED/Mode Addresses                                 |
    \*---------------------------------------------------------*/
    unsigned short          breathing_address[3];
    unsigned short          custom_address;
    unsigned short          led_mode_address;

    /*---------------------------------------------------------*\
    | Index of device in Razer device list                      |
    \*---------------------------------------------------------*/
    unsigned int            device_index;

    /*---------------------------------------------------------*\
    | HID report index for request and response                 |
    \*---------------------------------------------------------*/
    unsigned char           report_index;
    unsigned char           response_index;

    /*---------------------------------------------------------*\
    | Private functions based on OpenRazer                      |
    \*---------------------------------------------------------*/
    razer_kraken_request_report razer_kraken_create_report(unsigned char report_id, unsigned char destination, unsigned char length, unsigned short address);

    razer_kraken_request_report razer_create_mode_spectrum_cycle_kraken_report(unsigned char variable_storage, unsigned char led_id);

    std::string                 razer_get_firmware();
    std::string                 razer_get_serial();
    
    void                        razer_set_mode_breathing();
    void                        razer_set_mode_custom(unsigned char red, unsigned char grn, unsigned char blu);
    void                        razer_set_mode_none();
    void                        razer_set_mode_spectrum_cycle();
    void                        razer_set_mode_static(unsigned char red, unsigned char grn, unsigned char blu);

    int                         razer_usb_receive(razer_kraken_response_report* report);
    int                         razer_usb_send(razer_kraken_request_report* report);


};
