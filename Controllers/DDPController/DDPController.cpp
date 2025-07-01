/*---------------------------------------------------------*\
| DDPController.cpp                                         |
|                                                           |
|   Driver for DDP protocol devices                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "DDPController.h"
#include "LogManager.h"
#include <cstring>
#include <algorithm>

DDPController::DDPController(const std::vector<DDPDevice>& device_list)
{
    devices         = device_list;
    unique_endpoints = NULL;
    num_endpoints   = 0;
    sequence_number = 0;
    keepalive_time_ms = 1000;
    keepalive_thread_run = false;
    
    InitializeNetPorts();
    
    if(!devices.empty())
    {
        keepalive_thread_run = true;
        keepalive_thread = std::thread(&DDPController::KeepaliveThreadFunction, this);
    }
}

DDPController::~DDPController()
{
    keepalive_thread_run = false;
    if(keepalive_thread.joinable())
    {
        keepalive_thread.join();
    }
    
    CloseNetPorts();
    if(unique_endpoints != NULL)
    {
        delete[] unique_endpoints;
    }
}

bool DDPController::InitializeNetPorts()
{
    if(devices.empty())
    {
        return true;
    }
    
    num_endpoints = 0;
    
    for(unsigned int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
    {
        bool found = false;
        for(unsigned int ep_idx = 0; ep_idx < num_endpoints; ep_idx++)
        {
            if(strcmp(unique_endpoints[ep_idx].ip, devices[dev_idx].ip.c_str()) == 0 && 
               unique_endpoints[ep_idx].port == devices[dev_idx].port)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            num_endpoints++;
        }
    }
    
    unique_endpoints = new DDPEndpoint[num_endpoints];
    unsigned int endpoint_count = 0;
    
    for(unsigned int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
    {
        bool found = false;
        for(unsigned int ep_idx = 0; ep_idx < endpoint_count; ep_idx++)
        {
            if(strcmp(unique_endpoints[ep_idx].ip, devices[dev_idx].ip.c_str()) == 0 && 
               unique_endpoints[ep_idx].port == devices[dev_idx].port)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            strncpy(unique_endpoints[endpoint_count].ip, devices[dev_idx].ip.c_str(), 15);
            unique_endpoints[endpoint_count].ip[15] = '\0';
            unique_endpoints[endpoint_count].port = devices[dev_idx].port;
            endpoint_count++;
        }
    }
    
    for(unsigned int ep_idx = 0; ep_idx < num_endpoints; ep_idx++)
    {
        net_port* port = new net_port();
        char port_str[16];
        snprintf(port_str, 16, "%d", unique_endpoints[ep_idx].port);
        
        if(port->udp_client(unique_endpoints[ep_idx].ip, port_str))
        {
            udp_ports.push_back(port);
        }
        else
        {
            udp_ports.push_back(NULL);
        }
    }
    
    return true;
}

void DDPController::CloseNetPorts()
{
    for(unsigned int port_idx = 0; port_idx < udp_ports.size(); port_idx++)
    {
        if(udp_ports[port_idx] != NULL)
        {
            delete udp_ports[port_idx];
        }
    }
    udp_ports.clear();
}

int DDPController::GetPortIndex(const DDPDevice& device)
{
    for(unsigned int ep_idx = 0; ep_idx < num_endpoints; ep_idx++)
    {
        if(strcmp(unique_endpoints[ep_idx].ip, device.ip.c_str()) == 0 && 
           unique_endpoints[ep_idx].port == device.port)
        {
            return (int)ep_idx;
        }
    }
    return -1;
}

void DDPController::UpdateLEDs(const std::vector<unsigned int>& colors)
{
    if(udp_ports.empty()) return;

    {
        std::lock_guard<std::mutex> lock(last_update_mutex);
        last_colors = colors;
        last_update_time = std::chrono::steady_clock::now();
    }

    unsigned int color_index = 0;
    
    for(unsigned int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
    {
        if(color_index >= colors.size()) break;
            
        unsigned int bytes_per_pixel = 3;
        unsigned int total_bytes = devices[dev_idx].num_leds * bytes_per_pixel;
        std::vector<unsigned char> device_data(total_bytes);
        
        for(unsigned int led_idx = 0; led_idx < devices[dev_idx].num_leds && (color_index + led_idx) < colors.size(); led_idx++)
        {
            unsigned int color = colors[color_index + led_idx];
            unsigned char r = color & 0xFF;
            unsigned char g = (color >> 8) & 0xFF;
            unsigned char b = (color >> 16) & 0xFF;
            unsigned int pixel_offset = led_idx * bytes_per_pixel;
            
            device_data[pixel_offset + 0] = r;
            device_data[pixel_offset + 1] = g;
            device_data[pixel_offset + 2] = b;
        }
        
        unsigned int max_data_per_packet = DDP_MAX_DATA_SIZE;
        unsigned int bytes_sent = 0;
        
        while(bytes_sent < total_bytes)
        {
            unsigned int chunk_size = (max_data_per_packet < (total_bytes - bytes_sent)) ? max_data_per_packet : (total_bytes - bytes_sent);
            
            if(!SendDDPPacket(devices[dev_idx], device_data.data() + bytes_sent, (unsigned short)chunk_size, bytes_sent))
                break;
            
            bytes_sent += chunk_size;
        }
        
        color_index += devices[dev_idx].num_leds;
    }
    
    sequence_number++;
}

bool DDPController::SendDDPPacket(const DDPDevice& device, const unsigned char* data, unsigned short length, unsigned int offset)
{
    int port_index = GetPortIndex(device);
    if(port_index < 0 || port_index >= (int)udp_ports.size())
    {
        return false;
    }
    
    if(udp_ports[port_index] == NULL)
    {
        net_port* port = new net_port();
        char port_str[16];
        snprintf(port_str, 16, "%d", unique_endpoints[port_index].port);
        
        if(port->udp_client(unique_endpoints[port_index].ip, port_str))
        {
            udp_ports[port_index] = port;
        }
        else
        {
            delete port;
            return false;
        }
    }
    
    std::vector<unsigned char> packet(DDP_HEADER_SIZE + length);
    ddp_header* header = (ddp_header*)packet.data();
    
    header->flags = DDP_FLAG_VER_1 | DDP_FLAG_PUSH;
    header->sequence = sequence_number & 0x0F;
    header->data_type = 1;
    header->dest_id = 1;
    header->data_offset = htonl(offset);
    header->data_length = htons(length);
    
    memcpy(packet.data() + DDP_HEADER_SIZE, data, length);
    
    int bytes_sent = udp_ports[port_index]->udp_write((char*)packet.data(), (int)packet.size());
    
    return bytes_sent == (int)packet.size();
}

void DDPController::SetKeepaliveTime(unsigned int time_ms)
{
    keepalive_time_ms = time_ms;
}

void DDPController::KeepaliveThreadFunction()
{
    while(keepalive_thread_run)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        if(keepalive_time_ms == 0)
            continue;
            
        std::vector<unsigned int> colors_to_send;
        bool should_send = false;
        
        {
            std::lock_guard<std::mutex> lock(last_update_mutex);
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            long long time_since_update = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time).count();
            
            if(time_since_update >= keepalive_time_ms && !last_colors.empty())
            {
                colors_to_send = last_colors;
                should_send = true;
                last_update_time = now;
            }
        }
        
        if(should_send)
        {
            unsigned int color_index = 0;
            
            for(unsigned int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
            {
                if(color_index >= colors_to_send.size()) break;
                    
                unsigned int bytes_per_pixel = 3;
                unsigned int total_bytes = devices[dev_idx].num_leds * bytes_per_pixel;
                std::vector<unsigned char> device_data(total_bytes);
                
                for(unsigned int led_idx = 0; led_idx < devices[dev_idx].num_leds && (color_index + led_idx) < colors_to_send.size(); led_idx++)
                {
                    unsigned int color = colors_to_send[color_index + led_idx];
                    unsigned char r = color & 0xFF;
                    unsigned char g = (color >> 8) & 0xFF;
                    unsigned char b = (color >> 16) & 0xFF;
                    unsigned int pixel_offset = led_idx * bytes_per_pixel;
                    
                    device_data[pixel_offset + 0] = r;
                    device_data[pixel_offset + 1] = g;
                    device_data[pixel_offset + 2] = b;
                }
                
                unsigned int max_data_per_packet = DDP_MAX_DATA_SIZE;
                unsigned int bytes_sent = 0;
                
                while(bytes_sent < total_bytes)
                {
                    unsigned int chunk_size = (max_data_per_packet < (total_bytes - bytes_sent)) ? max_data_per_packet : (total_bytes - bytes_sent);
                    
                    if(!SendDDPPacket(devices[dev_idx], device_data.data() + bytes_sent, (unsigned short)chunk_size, bytes_sent))
                        break;
                    
                    bytes_sent += chunk_size;
                }
                
                color_index += devices[dev_idx].num_leds;
            }
        }
    }
}
