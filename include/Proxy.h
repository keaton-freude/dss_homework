#pragma once

#include <string>

namespace dss
{

// Encapsulate basic Proxy Settings
struct Proxy {
    std::string proxyHost;
    uint16_t proxyPort;
};
    
}