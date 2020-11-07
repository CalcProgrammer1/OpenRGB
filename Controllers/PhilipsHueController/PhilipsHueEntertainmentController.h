/*---------------------------------------------------------*\
|  Definitions for Philips Hue Entertainment Mode           |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 11/6/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "Bridge.h"
#include "Group.h"

#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/debug.h"
#include "mbedtls/timing.h"

#include <string>
#include <vector>

#pragma once

#define HUE_ENTERTAINMENT_HEADER_SIZE   16
#define HUE_ENTERTAINMENT_LIGHT_SIZE    9

class PhilipsHueEntertainmentController
{
public:
    PhilipsHueEntertainmentController(hueplusplus::Bridge& bridge_ptr, hueplusplus::Group& group_ptr);
    ~PhilipsHueEntertainmentController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();
    unsigned int GetNumLEDs();

    void SetColor(RGBColor* colors);

private:
    hueplusplus::Bridge&            bridge;
    hueplusplus::Group&             group;
    std::string                     location;
    unsigned char*                  entertainment_msg;
    unsigned int                    entertainment_msg_size;
    unsigned int                    num_leds;

    mbedtls_ssl_context             ssl;
    mbedtls_net_context             server_fd;
    mbedtls_entropy_context         entropy;
    mbedtls_ctr_drbg_context        ctr_drbg;
    mbedtls_ssl_config              conf;
    mbedtls_x509_crt                cacert;
    mbedtls_timing_delay_context    timer;
};
