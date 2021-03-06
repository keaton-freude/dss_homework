#pragma once

#include <list>
#include <thread>
#include <functional>
#include <optional>
#include <string>

#include "HTTPSClient.h"
#include "Proxy.h"


namespace dss
{

/**
 *  Provides an abstraction over fetching MLB stats,
 *  extracting required information and downloading image data
 * 
 *  Users can subscribe to data as it comes in
 * 
 *  The work for any given batch request is spread across 1 thread
 *  per IO request. We expect to spend the vast majority of time
 *  waiting for IO to finish
 * 
 */
class MLBStatsFetcher {
private:
    // NOTE: We are omitting the date parameter, it will be added on when constructing
    // the entire api endpoint
    const std::string MLB_STATS_URI = 
        "https://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions";

    std::list<std::thread> _threads;
    std::list<std::function<void(std::string, std::string, std::vector<unsigned char>&&)>> _observers;

    HTTPSClient _client;
public:
    MLBStatsFetcher(std::optional<Proxy> proxyHost);
    ~MLBStatsFetcher();

    // Not a great way to handle dates..
    void StartForDate(const std::string &date);

    void AddObserver(std::function<void(std::string, std::string, std::vector<unsigned char>&&)> observer);
};

}