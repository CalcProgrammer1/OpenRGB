/*-----------------------------------------*\
|  NanoleafController.cpp                   |
|                                           |
|  API Interface for Nanoleaf devices       |
|                                           |
|  Nikita Rushmanov 01/13/2022              |
\*-----------------------------------------*/

#include "NanoleafController.h"
#include "LogManager.h"
#include <curl/curl.h>

std::size_t WriteMemoryCallback(const char* in, std::size_t size, std::size_t num, std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

long APIRequest(std::string method, std::string location, std::string URI, json* request_data = nullptr, json* response_data = nullptr)
{
    const std::string url("http://"+location+URI);

    CURL* curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    if(request_data)
    {
        // LOG_DEBUG("[Nanoleaf] Sending data: %s", request_data->dump().c_str());
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, request_data->dump().c_str());
    }

    // Response information.
    long httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /*---------------------------------------------------------*\
    | Hook up data container (will be passed as the last        |
    | parameter to the callback handling function). Can be any  |
    | pointer type, since it will internally be passed as a     |
    | void pointer.                                             |
    \*---------------------------------------------------------*/
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode/100 == 2)
    {
        if(response_data)
        {
            *response_data = json::parse(*httpData.get());
        }
    }
    else
    {
        LOG_DEBUG("[Nanoleaf] HTTP %i:Could not %s from %s", httpCode, method, url);
    }

    return httpCode;
}

NanoleafController::NanoleafController(std::string a_address, int a_port, std::string a_auth_token)
{
    address = a_address;
    port = a_port;
    auth_token = a_auth_token;
    location = address+":"+std::to_string(port);

    json data;
    if(APIRequest("GET", location, "/api/v1/"+auth_token, nullptr, &data) == 200)
    {
        name = data["name"];
        serial = data["serialNo"];
        manufacturer = data["manufacturer"];
        firmware_version = data["firmwareVersion"];
        model = data["model"];

        brightness = data["state"]["brightness"]["value"];
        selectedEffect = data["effects"]["select"];

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

    // We really don't care if this fails.
    APIRequest("DELETE", location, "/api/v1/"+auth_token, nullptr, nullptr);
}

void NanoleafController::UpdateLEDs(std::vector<RGBColor>& colors)
{
    // Requires StartExternalControl() to have been called prior.

    if(model == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        uint8_t size = panel_ids.size();

        uint8_t* message = (uint8_t*)malloc(size*7+6+1);

        message[0] = (uint8_t)size;

        for (int i = 0; i < size; i++)
        {
            message[7*i+0+1] = (uint8_t)panel_ids[i];
            message[7*i+1+1] = (uint8_t)1;
            message[7*i+2+1] = (uint8_t)RGBGetRValue(colors[i]);
            message[7*i+3+1] = (uint8_t)RGBGetGValue(colors[i]);
            message[7*i+4+1] = (uint8_t)RGBGetBValue(colors[i]);
            message[7*i+5+1] = (uint8_t)0;
            message[7*i+6+1] = (uint8_t)0;
        }

        external_control_socket.udp_write(reinterpret_cast<char*>(message), size*7+6+1);
    }
    else if(model == NANOLEAF_CANVAS_MODEL)
    {
        // Insert V2 protocol implementation here.
    }
}

void NanoleafController::StartExternalControl()
{
    json request;
    request["write"]["command"] = "display";
    request["write"]["animType"] = "extControl";

    if(model == NANOLEAF_LIGHT_PANELS_MODEL)
    {
        request["write"]["extControlVersion"] = "v1";
    }
    else if(model == NANOLEAF_CANVAS_MODEL)
    {
        request["write"]["extControlVersion"] = "v2";
    }

    json response;
    if(APIRequest("PUT", location, "/api/v1/"+auth_token+"/effects", &request, &response)/100 == 2)
    {
        external_control_socket.udp_client(response["streamControlIpAddr"].get<std::string>().c_str(), std::to_string(response["streamControlPort"].get<int>()).c_str());

        selectedEffect = NANOLEAF_DIRECT_MODE_EFFECT_NAME;
    }
}

void NanoleafController::SelectEffect(std::string effect_name)
{
    json request;
    request["select"] = effect_name;
    if(APIRequest("PUT", location, "/api/v1/"+auth_token+"/effects", &request)/100 == 2)
    {
        selectedEffect = effect_name;
    }
}

void NanoleafController::SetBrightness(int a_brightness)
{
    json request;
    request["brightness"]["value"] = a_brightness;
    if(APIRequest("PUT", location, "/api/v1/"+auth_token+"/state", &request)/100 == 2)
    {
        brightness = a_brightness;
    }
}
