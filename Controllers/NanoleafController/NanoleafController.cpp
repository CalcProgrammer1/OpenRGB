/*---------------------------------------------------------*\
| NanoleafController.cpp                                    |
|                                                           |
|   Driver for Nanoleaf                                     |
|                                                           |
|   Nikita Rushmanov                            13 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "NanoleafController.h"
#include "LogManager.h"
#include "httplib.h"

long APIRequest(std::string method, std::string location, std::string URI, json* request_data = nullptr, json* response_data = nullptr)
{
    /*-------------------------------------------------------------*\
    | Append http:// to the location field to create the URL        |
    \*-------------------------------------------------------------*/
    const std::string url("http://" + location);

    /*-------------------------------------------------------------*\
    | Create httplib Client and variables to hold result            |
    \*-------------------------------------------------------------*/
    httplib::Client client(url.c_str());
    int             status  = 0;
    std::string     body    = "";

    /*-------------------------------------------------------------*\
    | Perform the appropriate call for the given method             |
    \*-------------------------------------------------------------*/
    if(method == "GET")
    {
        httplib::Result result = client.Get(URI.c_str());

        if(httplib::Error::Success == result.error())
        {
            status  = result->status;
            body    = result->body;
        }
    }
    else if(method == "PUT")
    {
        if(request_data)
        {
            httplib::Result result = client.Put(URI.c_str(), request_data->dump(), "application/json");

            if(httplib::Error::Success == result.error())
            {
                status  = result->status;
                body    = result->body;
            }
        }
        else
        {
            httplib::Result result = client.Put(URI.c_str());

            if(httplib::Error::Success == result.error())
            {
                status  = result->status;
                body    = result->body;
            }
        }
    }
    else if(method == "DELETE")
    {
        httplib::Result result = client.Delete(URI.c_str());

        if(httplib::Error::Success == result.error())
        {
            status  = result->status;
            body    = result->body;
        }
    }
    else if(method == "POST")
    {
        httplib::Result result = client.Post(URI.c_str());

        if(httplib::Error::Success == result.error())
        {
            status  = result->status;
            body    = result->body;
        }
    }

    /*-------------------------------------------------------------*\
    | If status is in the 200 range the request was successful      |
    \*-------------------------------------------------------------*/
    if((status / 100) == 2)
    {
        if(response_data)
        {
            *response_data = json::parse(body);
        }
    }
    else
    {
        LOG_DEBUG("[Nanoleaf] HTTP %i:Could not %s from %s", status, method.c_str(), url.c_str());
    }

    return status;
}

NanoleafController::NanoleafController(std::string a_address, int a_port, std::string a_auth_token)
{
    address                 = a_address;
    port                    = a_port;
    auth_token              = a_auth_token;
    location                = address + ":" + std::to_string(port);

    json data;
    if(APIRequest("GET", location, "/api/v1/"+auth_token, nullptr, &data) == 200)
    {
        name                = data["name"];
        serial              = data["serialNo"];
        manufacturer        = data["manufacturer"];
        firmware_version    = data["firmwareVersion"];
        model               = data["model"];

        brightness          = data["state"]["brightness"]["value"];
        selectedEffect      = data["effects"]["select"];

        for(json::const_iterator it = data["effects"]["effectsList"].begin(); it != data["effects"]["effectsList"].end(); ++it)
        {
            effects.push_back(it.value());
        }

        for(json::const_iterator it = data["panelLayout"]["layout"]["positionData"].begin(); it != data["panelLayout"]["layout"]["positionData"].end(); ++it)
        {
            panel_ids.push_back(it.value()["panelId"].get<int>());
        }
    }
    else
    {
        throw std::exception();
    }
}

std::string NanoleafController::Pair(std::string address, int port)
{
    const std::string location = address+":"+std::to_string(port);

    json data;
    if(APIRequest("POST", location, "/api/v1/new", nullptr, &data) == 200)
    {
        return data["auth_token"];
    }
    else
    {
        throw std::exception();
    }
}

void NanoleafController::Unpair(std::string address, int port, std::string auth_token)
{
    const std::string location = address+":"+std::to_string(port);

    /*-------------------------------------------------------------*\
    | We really don't care if this fails.                           |
    \*-------------------------------------------------------------*/
    APIRequest("DELETE", location, "/api/v1/"+auth_token, nullptr, nullptr);
}

void NanoleafController::UpdateLEDs(std::vector<RGBColor>& colors)
{
    /*-------------------------------------------------------------*\
    | Requires StartExternalControl() to have been called prior.    |
    \*-------------------------------------------------------------*/

    if(model == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        /*---------------------------------------------------------*\
        | Protocol V1 - https://forum.nanoleaf.me/docs              |
        |                                                           |
        | Size      Description                                     |
        | --------------------------------------------------------- |
        | 1         nPanels         Number of panels                |
        |                                                           |
        | 1         panelId         ID of panel                     |
        | 1         nFrames         Number of frames (always 1)     |
        | 1         R               Red channel                     |
        | 1         G               Green channel                   |
        | 1         B               Blue channel                    |
        | 1         W               White channel (ignored)         |
        | 1         transitionTime  Transition time (x 100ms)       |
        \*---------------------------------------------------------*/
        std::size_t size    = panel_ids.size();

        uint8_t* message    = (uint8_t*)malloc((size * 7) + 1);

        message[0]          = (uint8_t)size;                                /* nPanels          */

        for(unsigned int i = 0; i < size; i++)
        {
            message[(7 * i) + 0 + 1] = (uint8_t)panel_ids[i];               /* panelId          */
            message[(7 * i) + 1 + 1] = (uint8_t)1;                          /* nFrames          */
            message[(7 * i) + 2 + 1] = (uint8_t)RGBGetRValue(colors[i]);    /* R                */
            message[(7 * i) + 3 + 1] = (uint8_t)RGBGetGValue(colors[i]);    /* G                */
            message[(7 * i) + 4 + 1] = (uint8_t)RGBGetBValue(colors[i]);    /* B                */
            message[(7 * i) + 5 + 1] = (uint8_t)0;                          /* W                */
            message[(7 * i) + 6 + 1] = (uint8_t)0;                          /* transitionTime   */
        }

        external_control_socket.udp_write((char*)message, ((int)size * 7) + 1);

        free(message);
    }
    else if((model == NANOLEAF_CANVAS_MODEL)
         || (model == NANOLEAF_SHAPES_MODEL))
    {
        /*---------------------------------------------------------*\
        | Protocol V2 - https://forum.nanoleaf.me/docs              |
        |                                                           |
        | Size      Description                                     |
        | --------------------------------------------------------- |
        | 2         nPanels         Number of panels                |
        |                                                           |
        | 2         panelId         ID of panel                     |
        | 1         R               Red channel                     |
        | 1         G               Green channel                   |
        | 1         B               Blue channel                    |
        | 1         W               White channel (ignored)         |
        | 2         transitionTime  Transition time (x 100ms)       |
        \*---------------------------------------------------------*/
        std::size_t size    = panel_ids.size();

        uint8_t* message    = (uint8_t*)malloc((size * 8) + 2);

        message[0]          = (uint8_t)(size >> 8);                         /* nPanels H        */
        message[1]          = (uint8_t)(size & 0xFF);                       /* nPanels L        */

        for(unsigned int i = 0; i < size; i++)
        {
            message[(8 * i) + 0 + 2] = (uint8_t)(panel_ids[i] >> 8);        /* panelId H        */
            message[(8 * i) + 1 + 2] = (uint8_t)(panel_ids[i] & 0xFF);      /* panelId L        */
            message[(8 * i) + 2 + 2] = (uint8_t)RGBGetRValue(colors[i]);    /* R                */
            message[(8 * i) + 3 + 2] = (uint8_t)RGBGetGValue(colors[i]);    /* G                */
            message[(8 * i) + 4 + 2] = (uint8_t)RGBGetBValue(colors[i]);    /* B                */
            message[(8 * i) + 5 + 2] = (uint8_t)0;                          /* W                */
            message[(8 * i) + 6 + 2] = (uint8_t)0;                          /* transitionTime H */
            message[(8 * i) + 7 + 2] = (uint8_t)0;                          /* transitionTime L */
        }

        external_control_socket.udp_write((char *)message, ((int)size * 8) + 2);

        free(message);
    }
}

void NanoleafController::StartExternalControl()
{
    json request;
    request["write"]["command"]     = "display";
    request["write"]["animType"]    = "extControl";

    /*-------------------------------------------------------------*\
    | Determine whether to use v1 or v2 extControl protocol based   |
    | on model string                                               |
    \*-------------------------------------------------------------*/
    if(model == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        /*---------------------------------------------------------*\
        | Protocol v1 returns IP and port for UDP communication     |
        \*---------------------------------------------------------*/
        request["write"]["extControlVersion"] = "v1";

        json response;
        if((APIRequest("PUT", location, "/api/v1/"+auth_token+"/effects", &request, &response) / 100) == 2)
        {
            external_control_socket.udp_client(response["streamControlIpAddr"].get<std::string>().c_str(), std::to_string(response["streamControlPort"].get<int>()).c_str());

            selectedEffect = NANOLEAF_DIRECT_MODE_EFFECT_NAME;
        }
    }
    else if((model == NANOLEAF_CANVAS_MODEL)
         || (model == NANOLEAF_SHAPES_MODEL))
    {
        /*---------------------------------------------------------*\
        | Protocol v2 does not return anything, use device IP and   |
        | port 60222                                                |
        \*---------------------------------------------------------*/
        request["write"]["extControlVersion"] = "v2";

        if((APIRequest("PUT", location, "/api/v1/"+auth_token+"/effects", &request) / 100) == 2)
        {
            external_control_socket.udp_client(address.c_str(), "60222");

            selectedEffect = NANOLEAF_DIRECT_MODE_EFFECT_NAME;
        }
    }
}

void NanoleafController::SelectEffect(std::string effect_name)
{
    json request;
    request["select"] = effect_name;

    if((APIRequest("PUT", location, "/api/v1/"+auth_token+"/effects", &request) / 100) == 2)
    {
        selectedEffect = effect_name;
    }
}

void NanoleafController::SetBrightness(int a_brightness)
{
    json request;
    request["brightness"]["value"] = a_brightness;

    if((APIRequest("PUT", location, "/api/v1/"+auth_token+"/state", &request) / 100) == 2)
    {
        brightness = a_brightness;
    }
}

std::string NanoleafController::GetAuthToken()
{
    return auth_token;
};

std::string NanoleafController::GetName()
{
    return name;
};

std::string NanoleafController::GetSerial()
{
    return serial;
};

std::string NanoleafController::GetManufacturer()
{
    return manufacturer;
};

std::string NanoleafController::GetFirmwareVersion()
{
    return firmware_version;
};

std::string NanoleafController::GetModel()
{
    return model;
};

std::vector<std::string>& NanoleafController::GetEffects()
{
    return effects;
};

std::vector<int>& NanoleafController::GetPanelIds()
{
    return panel_ids;
};

std::string NanoleafController::GetSelectedEffect()
{
    return selectedEffect;
};

int NanoleafController::GetBrightness()
{
    return brightness;
};
