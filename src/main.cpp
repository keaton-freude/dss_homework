#include <iostream>
#include "httplib.h"
#include "json.hpp"

int main() {
    std::cout << "Starting!" << std::endl;
    httplib::Client client("statsapi.mlb.com");
    
    auto res = client.Get("/api/v1/schedule?hydrate=game%28content%28editorial%28recap%29%29%29,decisions&date=2019-10-01&sportId=1");

    if (res && res->status == 200) {
        std::cout << "Success!" << std::endl;
        std::cout << res->body << std::endl;
    } else {
        std::cout << "Failure!" << std::endl;
    }
}