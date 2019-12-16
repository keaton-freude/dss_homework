#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "json.hpp"
#include "Url.h"

#include "MLBStatsFetcher.h"
#include "MLBGameContent.h"

using namespace dss;
using json = nlohmann::json;

// Still not sure if we should be joining all of our threads here
// or if we should just detach them. Issue is that if the user press <ESC>
// before the threads have finished, it will appear to hang until the responses
// come in. If one has connection issues, it might hang for quite some time!
// Detaching threads when we create them in StartForDate seems like the wrong idea,
// maybe a better method is to have our threads be cancellable..
MLBStatsFetcher::~MLBStatsFetcher() {
    for(auto& t: _threads) {
        t.join();
    }
}

void MLBStatsFetcher::StartForDate(const std::string &date) {
    // General idea is:
    // Create a thread, which will then pull the game data for the given date
    // From there, spawn a thread for each image download action
    // After an image is complete, and other data is collected, hit each observable
    // with the data

    _threads.push_back(std::thread([this, date](){
        httplib::Client client(this->MLB_STATS_HOST.c_str());
        // example date: 2018-06-10
        const std::string endpoint = MLB_STATS_GAME_ENDPOINT_PREFIX + "&date=" + date;
        auto res = client.Get(endpoint.c_str());
        if (res && res->status == 200) {
            // good to go
            std::cout << "Got response and its good!" << std::endl;

            // Decode the body into a JSON object
            auto parsed = nlohmann::json::parse(res->body);
            auto data = parsed.get<MLBStats>();
            std::cout << "Got " << data.totalGames << " total games of data" << std::endl;
            for(int i = 0; i < data.totalGames; ++i) {

                // Create a thread to handle downloading images and alerting observers
                this->_threads.push_back(std::thread([this, data, i]() {
                    // Download a picture, for now just assume a 16:9 ratio.. probably use just 1280x720 as it'll look fine
                    // because it'll be pretty downscaled
                    // Take the image source and break it out into the host and endpoint
                    auto url = Url(data.dates[0].games[i].content.editorial.recap.home.photo.cuts.at("1280x720").src);

                    httplib::SSLClient imageClient(url.Host().c_str());
                    
                    httplib::Headers headers = {
                        { "Content-Type", "application/octet-stream"}
                    };


                    auto imageResponse = imageClient.Get(url.Path().c_str(), headers);
                    
                    if (imageResponse && imageResponse->status == 200) {
                        auto &game = data.dates[0].games[i];
                        std::vector<unsigned char> textureData(imageResponse->body.begin(), imageResponse->body.end());

                        for(const auto& observer: this->_observers) {
                            observer(game.content.editorial.recap.home.headline, std::move(textureData));
                        }
                    } else {
                        // Probably give up here too?
                        // TODO
                        std::abort();
                    }
                }));
            }
        } else {
            // Can't really continue if we fail to even get the stats
            std::cerr << "Could not retrieve stats. If response was successful, status code is next line" << std::endl;
            if (res) {
                std::cerr << "Status Code: " << res->status << std::endl;
            }
            std::abort();
        }
    }));
}

void MLBStatsFetcher::AddObserver(std::function<void(std::string, std::vector<unsigned char>&&)> observer) {
    _observers.push_back(observer);
}