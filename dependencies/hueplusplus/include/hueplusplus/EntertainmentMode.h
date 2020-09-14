/**
    \file EntertainmentMode.h
    Copyright Notice\n
    Copyright (C) 2020  Adam Honse		- developer\n

    This file is part of hueplusplus.

    hueplusplus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hueplusplus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with hueplusplus.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef INCLUDE_HUEPLUSPLUS_HUE_ENTERTAINMENT_MODE_H
#define INCLUDE_HUEPLUSPLUS_HUE_ENTERTAINMENT_MODE_H

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

#define HUE_ENTERTAINMENT_HEADER_SIZE 16
#define HUE_ENTERTAINMENT_LIGHT_SIZE 9

namespace hueplusplus
{
//! \brief Class for Hue Entertainment Mode
//!
//! Provides methods to initialize and control Entertainment groups.
class EntertainmentMode
{
public:
    EntertainmentMode(Bridge& bridge, Group& group);

    bool Connect();
    bool Disconnect();
    
    bool SetColorRGB(uint8_t light_index, uint8_t red, uint8_t green, uint8_t blue);

    void Update();

protected:
    Bridge& bridge;
    Group& group;

    std::vector<uint8_t> entertainment_msg; //!< buffer containing the entertainment mode packet data
    uint8_t entertainment_num_lights; //!< number of lights in entertainment mode group

    mbedtls_ssl_context ssl;
    mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_timing_delay_context timer;
};
} // namespace hueplusplus

#endif
