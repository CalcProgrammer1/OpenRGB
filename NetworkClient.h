#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#pragma once

class NetworkClient
{
public:
    NetworkClient(std::vector<RGBController *>& control);

    void        ConnectionThread();
    void        ListenThread();

protected:
    std::vector<RGBController *>& controllers;

private:
    net_port    port;
};