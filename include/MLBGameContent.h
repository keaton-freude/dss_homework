#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <json.hpp>

using json = nlohmann::json;

/**
 *  Provides a type definition for the MLB Game Content available through the
 *  MLB Stats API 
 */

namespace dss
{

// The API returns a huge amount of data and fields, but we will only be targeting
// the subset we need

struct MLBGamePhotoCut {
    std::string aspectRatio;
    uint32_t width;
    uint32_t height;
    std::string src;
};

struct MLBGamePhoto {
    std::unordered_map<std::string, MLBGamePhotoCut> cuts;
};

struct MLBGameHome {
    MLBGamePhoto photo;
    std::string headline;
};

struct MLBGameRecap {
    MLBGameHome home;
};

struct MLBGameEditorial {
    MLBGameRecap recap;
};

struct MLBGameContent {
    MLBGameEditorial editorial;
};

struct MLBGame {
    MLBGameContent content;
};

struct MLBGameContentDate {
    std::vector<MLBGame> games;
};

struct MLBStats {
    uint32_t totalGames;
    std::vector<MLBGameContentDate> dates;
};

// The order of the from_json functions _matters_
// Why?? ADL??
// Order from most-nested to least (matching structs above)

inline void from_json(const json &j, MLBGamePhotoCut &cut) {
    j.at("aspectRatio").get_to(cut.aspectRatio);
    j.at("width").get_to(cut.width);
    j.at("height").get_to(cut.height);
    j.at("src").get_to(cut.src);
}

inline void from_json(const json &j, MLBGamePhoto &photo) {
    j.at("cuts").get_to(photo.cuts);
}

inline void from_json(const json &j, MLBGameHome &home) {
    j.at("photo").get_to(home.photo);
    j.at("headline").get_to(home.headline);
}

inline void from_json(const json &j, MLBGameRecap &recap) {
    j.at("home").get_to(recap.home);
}

inline void from_json(const json &j, MLBGameEditorial &editorial) {
    j.at("recap").get_to(editorial.recap);
}

inline void from_json(const json &j, MLBGameContent &gameContent) {
    j.at("editorial").get_to(gameContent.editorial);
}

inline void from_json(const json &j, MLBGame &game) {
    j.at("content").get_to(game.content);
}

inline void from_json(const json &j, MLBGameContentDate &date) {
    j.at("games").get_to(date.games);
}

inline void from_json(const json &j, MLBStats &stats) {
    j.at("totalGames").get_to(stats.totalGames);
    j.at("dates").get_to(stats.dates);
}


}