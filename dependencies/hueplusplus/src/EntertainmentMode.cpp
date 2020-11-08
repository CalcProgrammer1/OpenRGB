/**
    \file EntertainmentMode.cpp
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

#include "hueplusplus/EntertainmentMode.h"

std::vector<char> HexToBytes(const std::string& hex)
{
    std::vector<char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

namespace hueplusplus
{
EntertainmentMode::EntertainmentMode(Bridge& bridge, Group& group):bridge(bridge),group(group)
{
    /*-------------------------------------------------*\
    | Signal the bridge to start streaming              |
    \*-------------------------------------------------*/
    bridge.StartStreaming(std::to_string(group.getId()));

    /*-------------------------------------------------*\
    | Get the number of lights from the group           |
    \*-------------------------------------------------*/
    entertainment_num_lights = group.getLightIds().size();

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
        entertainment_msg[msg_idx + 1] = group.getLightIds()[light_idx] >> 8; // ID MSB
        entertainment_msg[msg_idx + 2] = group.getLightIds()[light_idx] & 0xFF; // ID LSB
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
    mbedtls_net_init(&server_fd);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_x509_crt_init(&cacert);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    /*-------------------------------------------------*\
    | Seed the Deterministic Random Bit Generator (RNG) |
    \*-------------------------------------------------*/
    int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);

    /*-------------------------------------------------*\
    | Parse certificate                                 |
    \*-------------------------------------------------*/
    ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char*)mbedtls_test_cas_pem, mbedtls_test_cas_pem_len);
}

bool EntertainmentMode::Connect()
{
    /*-------------------------------------------------*\
    | Signal the bridge to start streaming              |
    | If successful, connect to the UDP port            |
    \*-------------------------------------------------*/
    if(bridge.StartStreaming(std::to_string(group.getId())))
    {
        /*-------------------------------------------------*\
        | Connect to the Hue bridge UDP server              |
        \*-------------------------------------------------*/
        int ret = mbedtls_net_connect(&server_fd, bridge.getBridgeIP().c_str(), "2100", MBEDTLS_NET_PROTO_UDP);

        /*-------------------------------------------------*\
        | If connecting failed, close and return false      |
        \*-------------------------------------------------*/
        if(ret != 0)
        {
            mbedtls_ssl_close_notify(&ssl);
            bridge.StopStreaming(std::to_string(group.getId()));
            return false;
        }

        /*-------------------------------------------------*\
        | Configure defaults                                |
        \*-------------------------------------------------*/
        ret = mbedtls_ssl_config_defaults(
            &conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_PRESET_DEFAULT);

        /*-------------------------------------------------*\
        | If configuring failed, close and return false     |
        \*-------------------------------------------------*/
        if(ret != 0)
        {
            mbedtls_ssl_close_notify(&ssl);
            bridge.StopStreaming(std::to_string(group.getId()));
            return false;
        }

        mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
        mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
        mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

        /*-------------------------------------------------*\
        | Convert client key to binary array                |
        \*-------------------------------------------------*/
        std::vector<char> psk_binary = HexToBytes(bridge.getClientKey());

        /*-------------------------------------------------*\
        | Configure SSL pre-shared key and identity         |
        | PSK - binary array from client key                |
        | Identity - username (ASCII)                       |
        \*-------------------------------------------------*/
        ret = mbedtls_ssl_conf_psk(&conf, (const unsigned char*)&psk_binary[0], psk_binary.size(),
            (const unsigned char*)bridge.getUsername().c_str(), bridge.getUsername().length());

        /*-------------------------------------------------*\
        | If configuring failed, close and return false     |
        \*-------------------------------------------------*/
        if(ret != 0)
        {
            mbedtls_ssl_close_notify(&ssl);
            bridge.StopStreaming(std::to_string(group.getId()));
            return false;
        }

        /*-------------------------------------------------*\
        | Set up the SSL                                    |
        \*-------------------------------------------------*/
        ret = mbedtls_ssl_setup(&ssl, &conf);

        /*-------------------------------------------------*\
        | If setup failed, close and return false           |
        \*-------------------------------------------------*/
        if(ret != 0)
        {
            mbedtls_ssl_close_notify(&ssl);
            bridge.StopStreaming(std::to_string(group.getId()));
            return false;
        }

        ret = mbedtls_ssl_set_hostname(&ssl, "localhost");

        /*-------------------------------------------------*\
        | If set hostname failed, close and return false    |
        \*-------------------------------------------------*/
        if(ret != 0)
        {
            mbedtls_ssl_close_notify(&ssl);
            bridge.StopStreaming(std::to_string(group.getId()));
            return false;
        }

        mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
        mbedtls_ssl_set_timer_cb(&ssl, &timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);

        /*-------------------------------------------------*\
        | Handshake                                         |
        \*-------------------------------------------------*/
        do
        {
            ret = mbedtls_ssl_handshake(&ssl);
        } while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

        /*-------------------------------------------------*\
        | If set hostname failed, close and return false    |
        \*-------------------------------------------------*/
        if(ret != 0)
        {
            mbedtls_ssl_close_notify(&ssl);
            bridge.StopStreaming(std::to_string(group.getId()));
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool EntertainmentMode::Disconnect()
{
    mbedtls_ssl_close_notify(&ssl);
    return bridge.StopStreaming(std::to_string(group.getId()));
}

bool EntertainmentMode::SetColorRGB(uint8_t light_index, uint8_t red, uint8_t green, uint8_t blue)
{
    if(light_index < entertainment_num_lights)
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

void EntertainmentMode::Update()
{
    int ret;
    unsigned int total = 0;

    while(total < entertainment_msg.size())
    {
        ret = mbedtls_ssl_write(&ssl, (const unsigned char*)&entertainment_msg[total], entertainment_msg.size());

        if(ret < 0)
        {
            // Return if mbedtls_ssl_write errors
            return;
        }
        else
        {
            total += ret;
        }
    }
}
} // namespace hueplusplus
