#include <iostream>

#include "json.hpp"

#include "MLBStatsFetcher.h"
#include "MLBGameContent.h"

using namespace dss;
using json = nlohmann::json;

MLBStatsFetcher::MLBStatsFetcher(std::optional<Proxy> proxyHost) 
    :   _client(proxyHost)
{
}

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
        // example date: 2018-06-10
        try {
            auto res = _client.Get(MLB_STATS_GAME_ENDPOINT_PREFIX, {
                {"sportId", "1"},
                {"date", date}
            });

            std::cout << "Got response and its good!" << std::endl;

            // Decode the body into a JSON object
            auto parsed = nlohmann::json::parse(res);
            auto data = parsed.get<MLBStats>();
            std::cout << "Got " << data.totalGames << " total games of data" << std::endl;
            for(uint32_t i = 0; i < data.totalGames; ++i) {

                // Create a thread to handle downloading images and alerting observers
                this->_threads.push_back(std::thread([this, data, i]() {
                    // Download a picture, for now just assume a 16:9 ratio.. probably use just 1280x720 as it'll look fine
                    // because it'll be pretty downscaled
                    // Take the image source and break it out into the host and endpoint
                    auto url = data.dates[0].games[i].content.editorial.recap.home.photo.cuts.at("1280x720").src;

                    try {
                        auto imageResponse = this->_client.Get(url);
                        
                        auto &game = data.dates[0].games[i];
                        std::vector<unsigned char> textureData(imageResponse.begin(), imageResponse.end());

                        for(const auto& observer: this->_observers) {
                            observer(
                                game.content.editorial.recap.home.headline,
                                game.content.editorial.recap.home.blurb,
                                std::move(textureData));
                        }
                    } catch (std::exception &innerEx) {
                        std::cerr << "Failed to download image data: " << innerEx.what() << std::endl;
                        throw;
                    }
                }));
            }
        } catch (std::exception& ex) {
            // HTTPS Request failed, best we can do is print a message and re-throw
            std::cerr << "HTTPS Request failed. Message: " << ex.what() << std::endl;
            throw;
        }
    }));
}

void MLBStatsFetcher::AddObserver(std::function<void(std::string, std::string, std::vector<unsigned char>&&)> observer) {
    _observers.push_back(observer);
}