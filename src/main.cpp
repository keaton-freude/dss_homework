#include <iostream>
#include <exception>
#include "Application.h"
#include "HTTPSClient.h"

using namespace dss;

int main(int argc, char** argv) {
    try {
        Application application;
        application.Run();
    } catch (std::exception& exception) {
        std::cerr << "Caught unhandled exception in main: " << exception.what() << std::endl;
    }
}