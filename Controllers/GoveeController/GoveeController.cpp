/*---------------------------------------------------------*\
|  GoveeController.cpp                                      |
|                                                           |
|  Driver for Govee controller                              |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/1/2023        |
\*---------------------------------------------------------*/

#include "GoveeController.h"
#include "base64.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std::chrono_literals;

base64::byte CalculateXorChecksum(std::vector<base64::byte> packet)
{
    base64::byte checksum = 0;

    for(unsigned int i = 0; i < packet.size(); i++)
    {
        checksum ^= packet[i];
    }

    return(checksum);
}

GoveeController::GoveeController(std::string ip)
{
    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    ip_address  = ip;

    /*-----------------------------------------------------------------*\
    | Open a UDP client sending to and receiving from the Govee         |
    | Multicast IP, send port 4001 and receive port 4002                |
    \*-----------------------------------------------------------------*/
    broadcast_port.udp_client("239.255.255.250", "4001", "4002");
    broadcast_port.udp_join_multicast_group("239.255.255.250");

    /*-----------------------------------------------------------------*\
    | Start a thread to handle responses received from the Wiz device   |
    \*-----------------------------------------------------------------*/
    ReceiveThreadRun = true;
    ReceiveThread = new std::thread(&GoveeController::ReceiveThreadFunction, this);

    /*-----------------------------------------------------------------*\
    | Request device information                                        |
    \*-----------------------------------------------------------------*/
    SendScan();

    /*-----------------------------------------------------------------*\
    | Wait up to 5s for device information to be received               |
    \*-----------------------------------------------------------------*/
    for(unsigned int wait_count = 0; wait_count < 500; wait_count++)
    {
        if(ReceiveThreadRun.load() == false)
        {
            ReceiveThread->join();
            break;
        }

        std::this_thread::sleep_for(10ms);
    }

    /*-----------------------------------------------------------------*\
    | Open a UDP client sending to the Govee device IP, port 4003       |
    \*-----------------------------------------------------------------*/
    port.udp_client(ip_address.c_str(), "4003");
}

GoveeController::~GoveeController()
{
    ReceiveThreadRun = 0;
    ReceiveThread->join();
    delete ReceiveThread;
}

std::string GoveeController::GetLocation()
{
    return("IP: " + ip_address);
}

std::string GoveeController::GetSku()
{
    return(sku);
}

std::string GoveeController::GetVersion()
{
    return("BLE Hardware Version: "  + bleVersionHard  + "\r\n" +
           "BLE Software Version: "  + bleVersionSoft  + "\r\n" +
           "WiFi Hardware Version: " + wifiVersionHard + "\r\n" +
           "WiFI Software Version: " + wifiVersionSoft + "\r\n");
}

void GoveeController::ReceiveThreadFunction()
{
    char recv_buf[1024];

    while(ReceiveThreadRun.load())
    {
        /*-----------------------------------------------------------------*\
        | Receive up to 1024 bytes from the device with a 1s timeout        |
        \*-----------------------------------------------------------------*/
        int size = broadcast_port.udp_listen(recv_buf, 1024);

        if(size > 0)
        {
            /*-----------------------------------------------------------------*\
            | Responses are not null-terminated, so add termination             |
            \*-----------------------------------------------------------------*/
            recv_buf[size] = '\0';

            printf( "response %s \r\n", recv_buf);

            /*-----------------------------------------------------------------*\
            | Convert null-terminated response to JSON                          |
            \*-----------------------------------------------------------------*/
            json response = json::parse(recv_buf);

            /*-----------------------------------------------------------------*\
            | Check if the response contains the method name                    |
            \*-----------------------------------------------------------------*/
            if(response.contains("msg"))
            {
                /*-------------------------------------------------------------*\
                | Handle responses for scan command                             |
                | This command's response should contain a msg object           |
                | containing a data member with ip, device, sku, among others.  |
                \*-------------------------------------------------------------*/
                if(response["msg"].contains("cmd"))
                {
                    if(response["msg"]["cmd"] == "scan")
                    {
                        if(response["msg"].contains("data"))
                        {
                            if(response["msg"]["data"].contains("ip"))
                            {
                                if(response["msg"]["data"]["ip"] == ip_address)
                                {
                                    if(response["msg"]["data"].contains("sku"))
                                    {
                                        sku = response["msg"]["data"]["sku"];
                                    }

                                    if(response["msg"]["data"].contains("bleVersionHard"))
                                    {
                                        bleVersionHard = response["msg"]["data"]["bleVersionHard"];
                                    }

                                    if(response["msg"]["data"].contains("bleVersionSoft"))
                                    {
                                        bleVersionSoft = response["msg"]["data"]["bleVersionSoft"];
                                    }

                                    if(response["msg"]["data"].contains("wifiVersionHard"))
                                    {
                                        wifiVersionHard = response["msg"]["data"]["wifiVersionHard"];
                                    }

                                    if(response["msg"]["data"].contains("wifiVersionSoft"))
                                    {
                                        wifiVersionSoft = response["msg"]["data"]["wifiVersionSoft"];
                                    }

                                    ReceiveThreadRun = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void GoveeController::SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness)
{
    json command;

    command["msg"]["cmd"]                       = "colorwc";
    command["msg"]["data"]["color"]["r"]        = red;
    command["msg"]["data"]["color"]["g"]        = green;
    command["msg"]["data"]["color"]["b"]        = blue;
    command["msg"]["data"]["colorTemInKelvin"]  = "0";

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str                     = command.dump();

    port.udp_write((char *)command_str.c_str(), command_str.length() + 1);
}

void GoveeController::SendRazerData(RGBColor* colors, unsigned int size)
{
    std::vector<base64::byte> pkt = { 0xBB, 0x00, 0x00, 0xB0, 0x00, 0x00 };
    json command;

    pkt[2] = 2 + (3 * size);
    pkt[5] = size;
    pkt.resize(6 + (3 * size));

    for(std::size_t led_idx = 0; led_idx < size; led_idx++)
    {
        pkt[6 + (led_idx * 3)] = RGBGetRValue(colors[led_idx]);
        pkt[7 + (led_idx * 3)] = RGBGetGValue(colors[led_idx]);
        pkt[8 + (led_idx * 3)] = RGBGetBValue(colors[led_idx]);
    }

    pkt.push_back(CalculateXorChecksum(pkt));

    command["msg"]["cmd"]                       = "razer";
    command["msg"]["data"]["pt"]                = base64::encode(pkt);

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str                     = command.dump();

    port.udp_write((char *)command_str.c_str(), command_str.length() + 1);
}

void GoveeController::SendRazerDisable()
{
    const std::vector<base64::byte> pkt = { 0xBB, 0x00, 0x01, 0xB1, 0x00, 0x0B };
    json command;

    command["msg"]["cmd"]                       = "razer";
    command["msg"]["data"]["pt"]                = base64::encode(pkt);

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str                     = command.dump();

    port.udp_write((char *)command_str.c_str(), command_str.length() + 1);
}

void GoveeController::SendRazerEnable()
{
    const std::vector<base64::byte> pkt = { 0xBB, 0x00, 0x01, 0xB1, 0x01, 0x0A };
    json command;

    command["msg"]["cmd"]                       = "razer";
    command["msg"]["data"]["pt"]                = base64::encode(pkt);

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str                     = command.dump();

    port.udp_write((char *)command_str.c_str(), command_str.length() + 1);
}

void GoveeController::SendScan()
{
    json command;

    command["msg"]["cmd"]                       = "scan";
    command["msg"]["data"]["account_topic"]     = "GA/123456789";

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str                     = command.dump();

    broadcast_port.udp_write((char *)command_str.c_str(), command_str.length() + 1);
}
