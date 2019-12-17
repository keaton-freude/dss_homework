#include <iostream>
#include <exception>
#include "Application.h"
#include "HTTPSClient.h"
#include "Proxy.h"

using namespace dss;

int main(int argc, char** argv) {
    try {
        std::optional<Proxy> proxy;
        // Hard-coded CLI args parsing. Normally would use some CLI library
        if (argc == 3) {
            // Assuming the proxy host and port are set like:
            // <exe_path> <host> <port>
            std::cout << "Using Proxy settings from command line" << std::endl;
            std::cout << "Proxy Host: " << argv[1] << std::endl;
            std::cout << "Proxy Port: " << argv[2] << std::endl;

            Proxy p;
            p.proxyHost = std::string(argv[1]);
            p.proxyPort = std::stoi(std::string(argv[2]));
            proxy = p;
        } else if (argc == 1) {
            std::cout << "Not using proxy settings." << std::endl;
        } else {
            std::cout << "Incorrect command line args." << std::endl;
            std::cout << "Usage: DSSHomework.exe [[host] [port]]" << std::endl;
            std::cout << "NOTE: Both host and port are required if wanting to set proxy." << std::endl;
            std::cout << "Example: DSSHomework.exe example-proxy.com 80" << std::endl;
            return 1;
        }

        Application application(proxy);
        application.Run();
    } catch (std::exception& exception) {
        std::cerr << "Caught unhandled exception in main: " << exception.what() << std::endl;
    }
}