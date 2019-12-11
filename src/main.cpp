#include <iostream>
#include "httplib.h"

int main() {
    std::cout << "Starting!" << std::endl;

    httplib::Client client("jsonplaceholder.typicode.com");

    auto res = client.Get("/todos/1");

    if (res && res->status == 200) {
        std::cout << "Success!" << std::endl;
    } else {
        std::cout << "Failure!" << std::endl;
    }
}