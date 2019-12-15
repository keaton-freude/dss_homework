#include <iostream>
#include <exception>
#include "Application.h"


using namespace dss;

int main() {
    try {
        Application application;
        application.Run();
    } catch (std::exception& exception) {
        std::cerr << "Caught unhandled exception in main: " << exception.what() << std::endl;
    }
}