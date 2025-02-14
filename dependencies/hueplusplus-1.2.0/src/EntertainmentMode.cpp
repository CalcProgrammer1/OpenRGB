/**
    \file EntertainmentMode.cpp
    Copyright Notice\n
    Copyright (C) 2020  Adam Honse		- developer\n
    Copyright (C) 2021  Moritz Wirger		- developer\n

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

#include "hueplusplus/EntertainmentMode.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/timing.h"

#include "hueplusplus/HueExceptionMacro.h"

namespace hueplusplus
{
constexpr uint8_t HUE_ENTERTAINMENT_HEADER_SIZE = 16;
constexpr uint8_t HUE_ENTERTAINMENT_LIGHT_SIZE = 9;

struct TLSContext
{
    mbedtls_ssl_context ssl;
    mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_timing_delay_context timer;
};

std::vector<char> hexToBytes(const std::string& hex)
{
    std::vector<char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

EntertainmentMode::EntertainmentMode(Bridge& b, Group& g)
    : bridge(&b), group(&g), tls_context(std::make_unique<TLSContext>(TLSContext {}))
{
    /*-------------------------------------------------*\
    | Signal the bridge to start streaming              |
    \*-------------------------------------------------*/
    bridge->startStreaming(std::to_string(group->getId()));

    /*-------------------------------------------------*\
    | Get the number of lights from the group           |
    \*-------------------------------------------------*/
    entertainment_num_lights = group->getLightIds().size();

    /*-------------------------------------------------*\
    | Resize Entertainment Mode message buffer          |
    \*-------------------------------------------------*/
    entertainment_msg.resize(HUE_ENTERTAINMENT_HEADER_SIZE + (entertainment_num_lights * HUE_ENTERTAINMENT_LIGHT_SIZE));

    /*-------------------------------------------------*\
    | Fill in Entertainment Mode message header         |
    \*-------------------------------------------------*/
    memcpy(&entertainment_msg[0], "HueStream", 9);
    entertainment_msg[9] = 0x01; // Version Major (1)
    entertainment_msg[10] = 0x00; // Version Minor (0)
    entertainment_msg[11] = 0x00; // Sequence ID
    entertainment_msg[12] = 0x00; // Reserved
    entertainment_msg[13] = 0x00; // Reserved
    entertainment_msg[14] = 0x00; // Color Space (RGB)
    entertainment_msg[15] = 0x00; // Reserved

    /*-------------------------------------------------*\
    | Fill in Entertainment Mode light data             |
    \*-------------------------------------------------*/
    for (unsigned int light_idx = 0; light_idx < entertainment_num_lights; light_idx++)
    {
        unsigned int msg_idx = HUE_ENTERTAINMENT_HEADER_SIZE + (light_idx * HUE_ENTERTAINMENT_LIGHT_SIZE);

        entertainment_msg[msg_idx + 0] = 0x00; // Type (Light)
        entertainment_msg[msg_idx + 1] = group->getLightIds()[light_idx] >> 8; // ID MSB
        entertainment_msg[msg_idx + 2] = group->getLightIds()[light_idx] & 0xFF; // ID LSB
        entertainment_msg[msg_idx + 3] = 0x00; // Red MSB
        entertainment_msg[msg_idx + 4] = 0x00; // Red LSB;
        entertainment_msg[msg_idx + 5] = 0x00; // Green MSB;
        entertainment_msg[msg_idx + 6] = 0x00; // Green LSB;
        entertainment_msg[msg_idx + 7] = 0x00; // Blue MSB;
        entertainment_msg[msg_idx + 8] = 0x00; // Blue LSB;
    }

    /*-------------------------------------------------*\
    | Initialize mbedtls contexts                       |
    \*-------------------------------------------------*/
    mbedtls_net_init(&tls_context->server_fd);
    mbedtls_ssl_init(&tls_context->ssl);
    mbedtls_ssl_config_init(&tls_context->conf);
    mbedtls_x509_crt_init(&tls_context->cacert);
    mbedtls_ctr_drbg_init(&tls_context->ctr_drbg);
    mbedtls_entropy_init(&tls_context->entropy);

    /*-------------------------------------------------*\
    | Seed the Deterministic Random Bit Generator (RNG) |
    \*-------------------------------------------------*/
    if (mbedtls_ctr_drbg_seed(&tls_context->ctr_drbg, mbedtls_entropy_func, &tls_context->entropy, NULL, 0) != 0)
    {
        mbedtls_entropy_free(&tls_context->entropy);
        mbedtls_ctr_drbg_free(&tls_context->ctr_drbg);
        mbedtls_x509_crt_free(&tls_context->cacert);
        mbedtls_ssl_config_free(&tls_context->conf);
        mbedtls_ssl_free(&tls_context->ssl);
        mbedtls_net_free(&tls_context->server_fd);
        throw HueException(CURRENT_FILE_INFO, "Failed to seed mbedtls RNG");
    }
}

EntertainmentMode::~EntertainmentMode()
{
    mbedtls_entropy_free(&tls_context->entropy);
    mbedtls_ctr_drbg_free(&tls_context->ctr_drbg);
    mbedtls_x509_crt_free(&tls_context->cacert);
    mbedtls_ssl_config_free(&tls_context->conf);
    mbedtls_ssl_free(&tls_context->ssl);
    mbedtls_net_free(&tls_context->server_fd);
}

bool EntertainmentMode::connect()
{
    /*-------------------------------------------------*\
    | Signal the bridge to start streaming              |
    | If successful, connect to the UDP port            |
    \*-------------------------------------------------*/
    if (bridge->startStreaming(std::to_string(group->getId())))
    {
        /*-------------------------------------------------*\
        | Connect to the Hue bridge UDP server              |
        \*-------------------------------------------------*/
        int ret = mbedtls_net_connect(
            &tls_context->server_fd, bridge->getBridgeIP().c_str(), "2100", MBEDTLS_NET_PROTO_UDP);

        /*-------------------------------------------------*\
        | If connecting failed, close and return false      |
        \*-------------------------------------------------*/
        if (ret != 0)
        {
            mbedtls_ssl_close_notify(&tls_context->ssl);
            bridge->stopStreaming(std::to_string(group->getId()));
            return false;
        }

        /*-------------------------------------------------*\
        | Configure defaults                                |
        \*-------------------------------------------------*/
        ret = mbedtls_ssl_config_defaults(
            &tls_context->conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_PRESET_DEFAULT);

        /*-------------------------------------------------*\
        | If configuring failed, close and return false     |
        \*-------------------------------------------------*/
        if (ret != 0)
        {
            mbedtls_ssl_close_notify(&tls_context->ssl);
            bridge->stopStreaming(std::to_string(group->getId()));
            return false;
        }

        mbedtls_ssl_conf_authmode(&tls_context->conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
        mbedtls_ssl_conf_ca_chain(&tls_context->conf, &tls_context->cacert, NULL);
        mbedtls_ssl_conf_rng(&tls_context->conf, mbedtls_ctr_drbg_random, &tls_context->ctr_drbg);

        /*-------------------------------------------------*\
        | Convert client key to binary array                |
        \*-------------------------------------------------*/
        std::vector<char> psk_binary = hexToBytes(bridge->getClientKey());

        /*-------------------------------------------------*\
        | Configure SSL pre-shared key and identity         |
        | PSK - binary array from client key                |
        | Identity - username (ASCII)                       |
        \*-------------------------------------------------*/
        ret = mbedtls_ssl_conf_psk(&tls_context->conf, (const unsigned char*)&psk_binary[0], psk_binary.size(),
            (const unsigned char*)bridge->getUsername().c_str(), bridge->getUsername().length());

        /*-------------------------------------------------*\
        | If configuring failed, close and return false     |
        \*-------------------------------------------------*/
        if (ret != 0)
        {
            mbedtls_ssl_close_notify(&tls_context->ssl);
            bridge->stopStreaming(std::to_string(group->getId()));
            return false;
        }

        /*-------------------------------------------------*\
        | Set up the SSL                                    |
        \*-------------------------------------------------*/
        ret = mbedtls_ssl_setup(&tls_context->ssl, &tls_context->conf);

        /*-------------------------------------------------*\
        | If setup failed, close and return false           |
        \*-------------------------------------------------*/
        if (ret != 0)
        {
            mbedtls_ssl_close_notify(&tls_context->ssl);
            bridge->stopStreaming(std::to_string(group->getId()));
            return false;
        }

        ret = mbedtls_ssl_set_hostname(&tls_context->ssl, "localhost");

        /*-------------------------------------------------*\
        | If set hostname failed, close and return false    |
        \*-------------------------------------------------*/
        if (ret != 0)
        {
            mbedtls_ssl_close_notify(&tls_context->ssl);
            bridge->stopStreaming(std::to_string(group->getId()));
            return false;
        }

        mbedtls_ssl_set_bio(
            &tls_context->ssl, &tls_context->server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
        mbedtls_ssl_set_timer_cb(
            &tls_context->ssl, &tls_context->timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);

        /*-------------------------------------------------*\
        | Handshake                                         |
        \*-------------------------------------------------*/
        do
        {
            ret = mbedtls_ssl_handshake(&tls_context->ssl);
        } while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

        /*-------------------------------------------------*\
        | If set hostname failed, close and return false    |
        \*-------------------------------------------------*/
        if (ret != 0)
        {
            mbedtls_ssl_close_notify(&tls_context->ssl);
            bridge->stopStreaming(std::to_string(group->getId()));
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool EntertainmentMode::disconnect()
{
    mbedtls_ssl_close_notify(&tls_context->ssl);
    return bridge->stopStreaming(std::to_string(group->getId()));
}

bool EntertainmentMode::setColorRGB(uint8_t light_index, uint8_t red, uint8_t green, uint8_t blue)
{
    if (light_index < entertainment_num_lights)
    {
        unsigned int msg_idx = HUE_ENTERTAINMENT_HEADER_SIZE + (light_index * HUE_ENTERTAINMENT_LIGHT_SIZE);

        entertainment_msg[msg_idx + 3] = red; // Red MSB
        entertainment_msg[msg_idx + 4] = red; // Red LSB;
        entertainment_msg[msg_idx + 5] = green; // Green MSB;
        entertainment_msg[msg_idx + 6] = green; // Green LSB;
        entertainment_msg[msg_idx + 7] = blue; // Blue MSB;
        entertainment_msg[msg_idx + 8] = blue; // Blue LSB;

        return true;
    }
    else
    {
        return false;
    }
}

bool EntertainmentMode::update()
{
    int ret;
    unsigned int total = 0;

    while (total < entertainment_msg.size())
    {
        ret = mbedtls_ssl_write(
            &tls_context->ssl, (const unsigned char*)&entertainment_msg[total], entertainment_msg.size());

        if (ret < 0)
        {
            // Return if mbedtls_ssl_write errors
            return false;
        }
        else
        {
            total += ret;
        }
    }
    return true;
}
} // namespace hueplusplus
