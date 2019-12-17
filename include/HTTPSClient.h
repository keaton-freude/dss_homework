#pragma once

#include <string>

namespace dss
{

/**
 *  Wrap up the Poco HTTPS Client 
 */
class HTTPSClient {
private:
public:
    HTTPSClient();
    ~HTTPSClient();

    std::string Get(const std::string &uriString);
};

}