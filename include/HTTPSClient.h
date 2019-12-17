#pragma once

#include <vector>
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

    // @uriString doesn't take into account queryParams
    // @queryParams are key-value pairs, like: {"key", "value"}
    std::string Get(const std::string &uriString, std::vector<std::pair<std::string, std::string>> queryParams);

    // Version where we don't care about query params
    std::string Get(const std::string &uriString);
};

}