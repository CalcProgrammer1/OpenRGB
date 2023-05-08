/*---------------------------------------------------------*\
|  Driver for Philips Wiz                                   |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 9/15/2020        |
\*---------------------------------------------------------*/

#include "PhilipsWizController.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std::chrono_literals;

PhilipsWizController::PhilipsWizController(std::string ip, bool use_cool, bool use_warm)
{
    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location    = "IP: " + ip;

    /*-----------------------------------------------------------------*\
    | Fill in settings                                                  |
    \*-----------------------------------------------------------------*/
    use_cool_white = use_cool;
    use_warm_white = use_warm;

    /*-----------------------------------------------------------------*\
    | Open a UDP client sending to the device's IP, port 38899          |
    \*-----------------------------------------------------------------*/
    port.udp_client(ip.c_str(), "38899");

    /*-----------------------------------------------------------------*\
    | Start a thread to handle responses received from the Wiz device   |
    \*-----------------------------------------------------------------*/
    ReceiveThreadRun = 1;
    ReceiveThread = new std::thread(&PhilipsWizController::ReceiveThreadFunction, this);

    /*-----------------------------------------------------------------*\
    | Request the system config (name, firmware version, MAC address)   |
    \*-----------------------------------------------------------------*/
    RequestSystemConfig();
}

PhilipsWizController::~PhilipsWizController()
{
    ReceiveThreadRun = 0;
    ReceiveThread->join();
    delete ReceiveThread;
}

std::string PhilipsWizController::GetLocation()
{
    return(location);
}

std::string PhilipsWizController::GetName()
{
    return("Wiz");
}

std::string PhilipsWizController::GetVersion()
{
    return(module_name + " " + firmware_version);
}

std::string PhilipsWizController::GetModuleName()
{
    return(module_name);
}

std::string PhilipsWizController::GetManufacturer()
{
    return("Philips");
}

std::string PhilipsWizController::GetUniqueID()
{
    return(module_mac);
}

void PhilipsWizController::SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness)
{
    json command;

    /*-----------------------------------------------------------------*\
    | Fill in the setPilot command with RGB and brightness information. |
    | The bulb will not respond to 0, 0, 0, so if all channels are zero,|
    | set the state to off.  Otherwise, set it to on. As we're also     |
    | running direct the bulb needs to be set back to max brightness.   |
    \*-----------------------------------------------------------------*/
    command["method"]            = "setPilot";
    command["params"]["r"]       = red;
    command["params"]["g"]       = green;
    command["params"]["b"]       = blue;
    command["params"]["dimming"] = brightness;
    command["params"]["state"]   = !((red == 0) && (green == 0) && (blue == 0));

    /*-----------------------------------------------------------------*\
    | The official Wiz app also sends a warm white level with its       |
    | custom colours. Until we can figure out a way to account for it   |
    | correctly, set the cool white level to the average of RGB to      |
    | improve its apparent brightness.                                  |
    \*-----------------------------------------------------------------*/
    if(use_warm_white)
    {
        command["params"]["w"]      = (red + green + blue) / 3;
    }
    else
    {
        command["params"]["w"]      = 0;
    }

    if(use_cool_white)
    {
        command["params"]["c"]      = (red + green + blue) / 3;
    }
    else
    {
        command["params"]["c"]      = 0;
    }

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str     = command.dump();

    port.udp_write((char *)command_str.c_str(), command_str.length() + 1);
}

void PhilipsWizController::SetScene(int scene, unsigned char brightness)
{
    json command;

    /*------------------------------------------------------------*\
    | Fill in the setPilot command with Scene information.         |
    \*------------------------------------------------------------*/
    command["method"] = "setPilot";
    command["params"]["sceneId"] = scene;
    command["params"]["dimming"] = brightness;

    /*------------------------------------------------------------*\
    | Convert the JSON object to a string and write it             |
    \*------------------------------------------------------------*/
    std::string command_str = command.dump();

    port.udp_write((char*)command_str.c_str(), command_str.length() + 1);
}

void PhilipsWizController::ReceiveThreadFunction()
{
    char recv_buf[1024];

    while(ReceiveThreadRun.load())
    {
        /*-----------------------------------------------------------------*\
        | Receive up to 1024 bytes from the device with a 1s timeout        |
        \*-----------------------------------------------------------------*/
        int size = port.udp_listen_timeout(recv_buf, 1024, 1, 0);

        if(size > 0)
        {
            /*-----------------------------------------------------------------*\
            | Responses are not null-terminated, so add termination             |
            \*-----------------------------------------------------------------*/
            recv_buf[size] = '\0';

            /*-----------------------------------------------------------------*\
            | Convert null-terminated response to JSON                          |
            \*-----------------------------------------------------------------*/
            json response = json::parse(recv_buf);

            /*-----------------------------------------------------------------*\
            | Check if the response contains the method name                    |
            \*-----------------------------------------------------------------*/
            if(response.contains("method"))
            {
                /*-------------------------------------------------------------*\
                | Handle responses for getSystemConfig method                   |
                | This method's response should contain a result object         |
                | containing fwVersion, moduleName, and mac, among others.      |
                \*-------------------------------------------------------------*/
                if(response["method"] == "getSystemConfig")
                {
                    if(response.contains("result"))
                    {
                        json result = response["result"];

                        if(result.contains("fwVersion"))
                        {
                            firmware_version = result["fwVersion"];
                        }

                        if(result.contains("moduleName"))
                        {
                            module_name = result["moduleName"];
                        }

                        if(result.contains("mac"))
                        {
                            module_mac = result["mac"];
                        }
                    }
                }
            }
        }
    }
}

void PhilipsWizController::RequestSystemConfig()
{
    json command;

    /*-----------------------------------------------------------------*\
    | Fill in the getSystemConfig command                               |
    \*-----------------------------------------------------------------*/
    command["method"]           = "getSystemConfig";

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str     = command.dump();

    port.udp_write((char *)command_str.c_str(), command_str.length() + 1);

    /*-----------------------------------------------------------------*\
    | Wait up to 1s to give it time to receive and process response     |
    \*-----------------------------------------------------------------*/
    for(unsigned int wait_count = 0; wait_count < 100; wait_count++)
    {
        if(firmware_version != "")
        {
            return;
        }

        std::this_thread::sleep_for(10ms);
    }
}
